#ifndef __BSP_INIT_H__

#include "config.h"

extern SPI_HandleTypeDef HAL_K1_SPI;
extern SPI_HandleTypeDef HAL_K3_SPI;
extern TIM_HandleTypeDef HAL_BaseTime_TIM;
extern UART_HandleTypeDef HAL_DWIN_USART;
extern UART_HandleTypeDef HAL_WIFI_USART;
extern UART_HandleTypeDef HAL_RS485_USART;

void Bsp_Peripherals_Init(void);
void Bsp_USART_Send(UART_HandleTypeDef *huart ,uint8_t *pdata, uint16_t len);

#endif
