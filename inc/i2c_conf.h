#ifndef I2C_CONF_H_
#define I2C_CONF_H_

void Setup_I2C(void);
int get_current(void);
int get_voltage(void);

I2C_HandleTypeDef    I2cHandle;
static const uint8_t slave_address = 0x40;
uint8_t cmdBuff[1];
uint8_t sendBuff[2];       // save data sent to i2c slave
uint8_t receiveBuff[2];

#endif /* I2C_CONF_H_ */
