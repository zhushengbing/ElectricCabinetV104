#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "config.h"

#define I2C_CRR           1
#define I2C_ERR           0 

void I2C_Delay(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAck(uint8_t ack);
uint8_t I2C_WaitAck(void);
uint8_t I2C_SendByte(uint8_t data);
uint8_t I2C_ReceiveByte(void);

#endif
