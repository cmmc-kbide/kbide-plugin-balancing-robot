#ifndef __kb_balancing_plugin_H__
#define __kb_balancing_plugin_H__

#include "driver.h"
#include "device.h"
#include "i2c_idf_driver.h"

#define true 1
#define false 0

#define kb_balancing_plugin_POLLING_MS 20

class kb_balancing_plugin : public Device
{
  private:
    enum
    {
        s_detect
    } state;
    TickType_t tickcnt, polling_tickcnt;

    int8_t _kb_bbareRobotStanding;
    int16_t _kb_bbtorque_motor[2];
    int16_t _kb_bbpwm_motor[2];
    int16_t _kb_bbangle[3];
    int16_t _kb_bbgyro[3];
    int16_t _kb_bbacc[3];
    int16_t _kb_bboffset[6];
    int32_t _kb_bbposition_motor[2];
    int16_t _kb_bbspeed_motor[2];
    int16_t _kb_bbrobot_speed;
    uint8_t _kb_bb_cmd = 1;
    float tmpf;

    uint16_t robot_mode;

  public:
    // constructor
    kb_balancing_plugin();
    // override
    void init(void);
    void process(Driver *drv);
    int prop_count(void);
    bool prop_name(int index, char *name);
    bool prop_unit(int index, char *unit);
    bool prop_attr(int index, char *attr);
    bool prop_read(int index, char *value);
    bool prop_write(int index, char *value);
    // method
    char *random();

    //main
    void position_control_mode(uint8_t dir, float val, float speed, float gab);
    float read_sensor(uint8_t d1, uint8_t d2);
    float read_wheel(uint8_t d1, uint8_t d2);
    void write_command(uint8_t d1, float d2);
    void set_pid_gain(uint8_t d1, float d2);
    void set_limit(uint8_t d1, float d2);
    void cal_sensor(void);
    void set_mode(int d1, int d2, int d3, int d4, int d5, int d6, int d7);

    //fn
    void set_reboot(void);
    void set_sensoroffset(int gx, int gy, int gz, int ax, int ay, int az);
    uint8_t get_RobotStand(void);
    float get_gyro(int axis);
    float get_acc(int axis);
    float get_angle(int axis);
    float get_speed(int axis);
    int16_t get_offset(int axis);
    float get_wheelspeed(int axis);
    float get_robotspeed(void);
    int32_t get_wheelposition(int axis);
    void set_mode_int(uint16_t var);
    void set_ch1(float var);
    void set_ch2(float var);
    void set_kp_speed(float var);
    void set_ki_speed(float var);
    void set_kp_pitch(float var);
    void set_kd_pitch(float var);
    void set_kp_yaw(float var);
    void set_ki_yaw(float var);
    void set_max_pitch(float var);
    void set_max_speed(float var);
};

#endif
