#define PD_Controller 0x0001
#define PI_Controller 0x0002
#define Enable_Motor 0x0004
#define PI_Steering 0x0008
#define Car_mode 0x0010
#define acc_calibation_mode 0x0020
#define gyro_calibation_mode 0x0040
#define Manual_Motor_control 0x0080
#define Motor_control_mode 0x0100
#define position_Controller 0x0200

#define dalta_gab_position 50
#define dalta_gab_angle 2

#define degree2raw 10.0f
#define cm2raw 58.6f
#define raw2cm 0.0017f
#define raw2cmps 0.853f
#define cmps2raw 1.172f

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "kb_balancing_plugin.h"

kb_balancing_plugin::kb_balancing_plugin()
{
  polling_ms = kb_balancing_plugin_POLLING_MS;
}

void kb_balancing_plugin::init(void)
{
  state = s_detect;
  initialized = true;
}

int kb_balancing_plugin::prop_count(void)
{
  // not supported
  return 0;
}

bool kb_balancing_plugin::prop_name(int index, char *name)
{
  // not supported
  return false;
}

bool kb_balancing_plugin::prop_unit(int index, char *unit)
{
  // not supported
  return false;
}

bool kb_balancing_plugin::prop_attr(int index, char *attr)
{
  // not supported
  return false;
}

bool kb_balancing_plugin::prop_read(int index, char *value)
{
  // not supported
  return false;
}

bool kb_balancing_plugin::prop_write(int index, char *value)
{
  // not supported
  return false;
}

void kb_balancing_plugin::process(Driver *drv)
{
  //  char buffer[64];
  //  sprintf(buffer, "%lldms\n", (esp_timer_get_time() / 1000));
  //  uart_write_bytes(UART_NUM_0, (const char*)buffer, strlen(buffer));
}

char *kb_balancing_plugin::random()
{
  return "nat-random";
}

void kb_balancing_plugin::position_control_mode(uint8_t dir, float val, float speed, float gab)
{
  uint32_t R_wheel_position_start, L_wheel_position_start, Robot_Target_Angle;
  int16_t reset_speed_count = 0, reset_turn_count = 0;
  float auto_speed = 0;
  float auto_trun = 0;
  int32_t cal_gab = 0;
  uint8_t moving_mode = 0;

  speed = abs(speed);
  gab = abs(gab);

  if (dir == 1 || dir == 3)
    val = -val;

  if (dir > 1)
    moving_mode = 1;

  if ((robot_mode & position_Controller) == position_Controller)
  {

    switch (moving_mode)
    {
    case 0: //go Forword/Backword

      if (gab == 0)
        gab = dalta_gab_position;

      get_wheelposition(0);
      L_wheel_position_start = _kb_bbposition_motor[0] + (val * cm2raw);
      R_wheel_position_start = _kb_bbposition_motor[1] + (val * cm2raw);

      cal_gab = (L_wheel_position_start - _kb_bbposition_motor[0]) + (R_wheel_position_start - _kb_bbposition_motor[1]);

      vTaskDelay(50 / portTICK_RATE_MS);

      while (!(abs(cal_gab) <= (gab * cm2raw)))
      {
        get_wheelposition(0);
        cal_gab = ((L_wheel_position_start + R_wheel_position_start) - (_kb_bbposition_motor[0] + _kb_bbposition_motor[1]));

        /////////////////////////////////////////////////////////////////

        float step_go = 0.5;
        float max_go_speed = speed * cmps2raw;

        if (cal_gab > 0)
        {

          if (abs(auto_speed) < max_go_speed)
            auto_speed += step_go;
          if (reset_speed_count < 2)
            reset_speed_count++;
        }
        else
        {

          if (abs(auto_speed) < max_go_speed)
            auto_speed -= step_go;
          if (reset_speed_count > -2)
            reset_speed_count--;
        }

        if (reset_speed_count == 0)
          auto_speed = 0;

        set_ch2(auto_speed);

        ///////////////////////////////////////////////////////////////
        vTaskDelay(50 / portTICK_RATE_MS);
      }
      break;

    case 1: //trun L/R

      if (gab == 0)
        gab = dalta_gab_angle * degree2raw;

      get_angle(0);
      Robot_Target_Angle = _kb_bbacc[2] + (val * degree2raw);

      int32_t cal_gab = (Robot_Target_Angle - _kb_bbacc[2]);

      vTaskDelay(50 / portTICK_RATE_MS);
      while (!(abs(cal_gab) <= (gab * degree2raw)))
      {
        get_angle(0);
        cal_gab = (Robot_Target_Angle - _kb_bbacc[2]);
        ///////////////////////////////////////////////////////////////

        float step_turn = 10;
        float max_turn_speed = speed * degree2raw;

        if (Robot_Target_Angle < _kb_bbacc[2])
        {
          if (abs(auto_trun) < max_turn_speed)
            auto_trun += step_turn;
          if (reset_turn_count < 2)
            reset_turn_count++;
        }
        else
        {
          if (abs(auto_trun) < max_turn_speed)
            auto_trun -= step_turn;
          if (reset_turn_count > -2)
            reset_turn_count--;
        }

        if (reset_turn_count == 0)
          auto_trun = 0;

        set_ch2(auto_trun);

        //////////////////////////////////////////////////////////

        vTaskDelay(50 / portTICK_RATE_MS);
      }
      break;
    }
  }
}

float kb_balancing_plugin::read_sensor(uint8_t d1, uint8_t d2)
{
  float tmp = 0;

  switch (d1)
  {
  case 0:
    tmp = get_angle(d2);
    break;
  case 1:
    tmp = get_gyro(d2);
    break;
  case 2:
    tmp = get_acc(d2);
    break;
  }

  return tmp;
}

float kb_balancing_plugin::read_wheel(uint8_t d1, uint8_t d2)
{
  float tmp = 0;

  switch (d1)
  {
  case 0:
    tmp = get_wheelposition(d2) * raw2cm;
    break;
  case 1:
    tmp = get_wheelspeed(d2) * raw2cmps;
    break;
  case 2:
    tmp = get_speed(d2);
    break;
  }

  return tmp;
}

void kb_balancing_plugin::write_command(uint8_t d1, float d2)
{
  switch (d1)
  {
  case 0:
    set_ch1(d2);
    break;
  case 1:
    set_ch2(d2);
    break;
  }
}

void kb_balancing_plugin::set_pid_gain(uint8_t d1, float d2)
{
  for (int re_sent = 0; re_sent < 3; re_sent++)
  {
    switch (d1)
    {
    case 0:
      set_kp_pitch(d2);
      break;
    case 1:
      set_kd_pitch(d2);
      break;
    case 2:
      set_kp_yaw(d2);
      break;
    case 3:
      set_ki_yaw(d2);
      break;
    case 4:
      set_kp_speed(d2);
      break;
    case 5:
      set_ki_speed(d2);
      break;
    }
    vTaskDelay(1);
  }
}

void kb_balancing_plugin::set_limit(uint8_t d1, float d2)
{
  for (int re_sent = 0; re_sent < 2; re_sent++)
  {
    switch (d1)
    {
    case 0:
      set_max_pitch(d2);
      break;
    case 1:
      set_max_speed(d2);
      break;
    }
    vTaskDelay(1);
  }
}

void kb_balancing_plugin::cal_sensor(void)
{
  robot_mode = (acc_calibation_mode | gyro_calibation_mode | Manual_Motor_control);
  set_mode_int(robot_mode);
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void kb_balancing_plugin::set_mode(int d1, int d2, int d3, int d4, int d5, int d6, int d7)
{

  if (d1)
    robot_mode |= Manual_Motor_control;
  if (d2)
    robot_mode |= Motor_control_mode;
  if (d3)
    robot_mode |= PD_Controller;
  if (d4)
    robot_mode |= PI_Controller;
  if (d5)
    robot_mode |= PI_Steering;
  if (d6)
    robot_mode |= Car_mode;
  if (d7)
    robot_mode |= position_Controller;

  for (int re_sent = 0; re_sent < 3; re_sent++)
  {
    set_mode_int(robot_mode);
    vTaskDelay(1);
  }
}

void kb_balancing_plugin::set_reboot(void)
{
  _kb_bb_cmd = 1;
  vTaskDelay(500 / portTICK_PERIOD_MS);
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

void kb_balancing_plugin::set_sensoroffset(int gx, int gy, int gz, int ax, int ay, int az)
{
  uint8_t datasize = 12;

  _kb_bboffset[0] = gx;
  _kb_bboffset[1] = gy;
  _kb_bboffset[2] = gz;
  _kb_bboffset[3] = ax;
  _kb_bboffset[4] = ay;
  _kb_bboffset[5] = az;

  uint8_t tmp[12] = {0};
  tmp[0] = (uint16_t)_kb_bboffset[0];
  tmp[1] = (uint16_t)_kb_bboffset[0] >> 8;
  tmp[2] = (uint16_t)_kb_bboffset[1];
  tmp[3] = (uint16_t)_kb_bboffset[1] >> 8;
  tmp[0] = (uint16_t)_kb_bboffset[2];
  tmp[1] = (uint16_t)_kb_bboffset[2] >> 8;
  tmp[2] = (uint16_t)_kb_bboffset[3];
  tmp[3] = (uint16_t)_kb_bboffset[3] >> 8;
  tmp[0] = (uint16_t)_kb_bboffset[4];
  tmp[1] = (uint16_t)_kb_bboffset[4] >> 8;
  tmp[2] = (uint16_t)_kb_bboffset[5];
  tmp[3] = (uint16_t)_kb_bboffset[5] >> 8;

  _kb_bb_cmd = 4;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

uint8_t kb_balancing_plugin::get_RobotStand(void)
{
  uint8_t datasize = 1;
  uint8_t tmp = 0;

  _kb_bb_cmd = 5;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)&tmp, datasize);

  return tmp;
}

float kb_balancing_plugin::get_gyro(int axis)
{
  uint8_t datasize = 6;
  uint8_t tmp[6] = {0};

  _kb_bb_cmd = 6;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbgyro[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bbgyro[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;
  _kb_bbgyro[2] = (uint16_t)tmp[4] | (uint16_t)tmp[5] << 8;

  switch (axis)
  {
  case 0:
    tmpf = (float)_kb_bbgyro[0] * 0.1f;
    break;
  case 1:
    tmpf = (float)_kb_bbgyro[1] * 0.1f;
    break;
  case 2:
    tmpf = (float)_kb_bbgyro[2] * 0.1f;
    break;
  }
  return tmpf;
}

float kb_balancing_plugin::get_acc(int axis)
{
  uint8_t datasize = 6;
  uint8_t tmp[6] = {0};

  _kb_bb_cmd = 7;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbacc[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bbacc[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;
  _kb_bbacc[2] = (uint16_t)tmp[4] | (uint16_t)tmp[5] << 8;

  switch (axis)
  {
  case 0:
    tmpf = (float)_kb_bbacc[0] * 0.1f;
    break;
  case 1:
    tmpf = (float)_kb_bbacc[1] * 0.1f;
    break;
  case 2:
    tmpf = (float)_kb_bbacc[2] * 0.1f;
    break;
  }
  return tmpf;
}

float kb_balancing_plugin::get_angle(int axis)
{
  uint8_t datasize = 6;
  uint8_t tmp[6] = {0};

  _kb_bb_cmd = 8;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbangle[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bbangle[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;
  _kb_bbangle[2] = (uint16_t)tmp[4] | (uint16_t)tmp[5] << 8;

  switch (axis)
  {
  case 0:
    tmpf = (float)_kb_bbangle[0] * 0.1f;
    break;
  case 1:
    tmpf = (float)_kb_bbangle[1] * 0.1f;
    break;
  case 2:
    tmpf = (float)_kb_bbangle[2] * 0.1f;
    break;
  }
  return tmpf;
}

float kb_balancing_plugin::get_speed(int axis)
{
  uint8_t datasize = 4;
  uint8_t tmp[4] = {0};

  _kb_bb_cmd = 9;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbpwm_motor[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bbpwm_motor[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;

  if (axis)
  {
    tmpf = (float)_kb_bbpwm_motor[0] * 0.1f;
  }
  else
  {
    tmpf = (float)_kb_bbpwm_motor[1] * 0.1f;
  }
  return tmpf;
}

int16_t kb_balancing_plugin::get_offset(int axis)
{
  uint8_t datasize = 12;
  uint8_t tmp[12] = {0};

  _kb_bb_cmd = 11;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bboffset[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bboffset[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;
  _kb_bboffset[2] = (uint16_t)tmp[4] | (uint16_t)tmp[5] << 8;
  _kb_bboffset[3] = (uint16_t)tmp[6] | (uint16_t)tmp[7] << 8;
  _kb_bboffset[4] = (uint16_t)tmp[8] | (uint16_t)tmp[9] << 8;
  _kb_bboffset[5] = (uint16_t)tmp[10] | (uint16_t)tmp[11] << 8;

  return _kb_bboffset[axis];
}

float kb_balancing_plugin::get_wheelspeed(int axis)
{
  uint8_t datasize = 4;
  uint8_t tmp[4] = {0};

  _kb_bb_cmd = 12;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbspeed_motor[0] = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;
  _kb_bbspeed_motor[1] = (uint16_t)tmp[2] | (uint16_t)tmp[3] << 8;

  if (axis)
  {
    tmpf = (float)_kb_bbspeed_motor[0] * 0.1f;
  }
  else
  {
    tmpf = (float)_kb_bbspeed_motor[1] * 0.1f;
  }

  return tmpf;
}

float kb_balancing_plugin::get_robotspeed(void)
{
  uint8_t datasize = 2;
  uint8_t tmp[2] = {0};

  _kb_bb_cmd = 13;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbrobot_speed = (uint16_t)tmp[0] | (uint16_t)tmp[1] << 8;

  float tmpf = (float)_kb_bbrobot_speed * 0.01f;

  return tmpf;
}

int32_t kb_balancing_plugin::get_wheelposition(int axis)
{
  uint8_t datasize = 8;
  uint8_t tmp[8] = {0};

  _kb_bb_cmd = 14;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_read_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);

  _kb_bbposition_motor[0] = (uint32_t)tmp[0] | (uint32_t)tmp[1] << 8 | (uint32_t)tmp[2] << 16 | (uint32_t)tmp[3] << 24;
  _kb_bbposition_motor[1] = (uint32_t)tmp[4] | (uint32_t)tmp[5] << 8 | (uint32_t)tmp[6] << 16 | (uint32_t)tmp[7] << 24;

  if (axis)
  {
    tmpf = (float)_kb_bbspeed_motor[0];
  }
  else
  {
    tmpf = (float)_kb_bbspeed_motor[1];
  }

  return tmpf;
}

void kb_balancing_plugin::set_mode_int(uint16_t var)
{
  uint8_t datasize = 2;

  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)var;
  tmp[1] = (uint16_t)var >> 8;

  _kb_bb_cmd = 15;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_ch1(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 18;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}
void kb_balancing_plugin::set_ch2(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 19;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_kp_speed(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 1.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 20;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_ki_speed(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 1.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 21;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_kp_pitch(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 22;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_kd_pitch(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 23;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_kp_yaw(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 24;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_ki_yaw(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 25;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_max_pitch(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 26;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}

void kb_balancing_plugin::set_max_speed(float var)
{
  uint8_t datasize = 2;
  int16_t tmp_var = var * 10.0f;
  uint8_t tmp[2] = {0};
  tmp[0] = (uint16_t)tmp_var;
  tmp[1] = (uint16_t)tmp_var >> 8;

  _kb_bb_cmd = 27;
  i2c_master_write_slave(I2C_NUM_1, 0x69, &_kb_bb_cmd, 1);
  // vTaskDelay(1);
  i2c_master_write_slave(I2C_NUM_1, 0x69, (uint8_t *)tmp, datasize);
}
