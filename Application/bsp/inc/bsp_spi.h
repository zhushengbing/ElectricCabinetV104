#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "config.h"

uint8_t Bsp_SPI_Read_Byte(void);
void Bsp_SPI_Read(uint8_t* pdata, uint8_t size);

#endif
