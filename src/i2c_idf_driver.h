#ifndef __MA_I2C_IDF_DRIVER__
#define __MA_I2C_IDF_DRIVER__

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#define CONFIG_I2C_MASTER_PORT_NUM 1
#define CONFIG_I2C_MASTER_SDA GPIO_NUM_4
#define CONFIG_I2C_MASTER_SCL GPIO_NUM_5

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 400000                             /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL I2C_MASTER_ACK                /*!< I2C ack value */
#define NACK_VAL I2C_MASTER_NACK               /*!< I2C nack value */

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t ESP_SLAVE_ADDR, uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t ESP_SLAVE_ADDR, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(i2c_port_t i2c_port)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_port, &conf);
    return i2c_driver_install(i2c_port, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

// /**
//  * @brief i2c slave initialization
//  */
// static esp_err_t i2c_slave_init(i2c_port_t i2c_port, uint8_t ESP_SLAVE_ADDR)
// {
//     i2c_config_t conf_slave;
//     conf_slave.sda_io_num = I2C_SLAVE_SDA_IO;
//     conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
//     conf_slave.scl_io_num = I2C_SLAVE_SCL_IO;
//     conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
//     conf_slave.mode = I2C_MODE_SLAVE;
//     conf_slave.slave.addr_10bit_en = 0;
//     conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
//     i2c_param_config(i2c_port, &conf_slave);
//     return i2c_driver_install(i2c_port, conf_slave.mode,
//                               I2C_SLAVE_RX_BUF_LEN,
//                               I2C_SLAVE_TX_BUF_LEN, 0);
// }

// /**
//  * @brief test function to show buffer
//  */
// static void disp_buf(uint8_t *buf, int len)
// {
//     int i;
//     for (i = 0; i < len; i++)
//     {
//         printf("%02x ", buf[i]);
//         if ((i + 1) % 16 == 0)
//         {
//             printf("\n");
//         }
//     }
//     printf("\n");
// }

#endif