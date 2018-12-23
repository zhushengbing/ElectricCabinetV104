#include "bsp_init.h"
#include "bsp_spi.h"

typedef enum { CS = 0, SCK, MISO, MOSI}SPI_GPIO_Typedef;
#define  SPI_Mode0  0  //CPOL = 0, CPHA = 0,
#define  SPI_Mode1  1  //CPOL = 0, CPHA = 1,
#define  SPI_Mode2  2  //CPOL = 1, CPHA = 0,
#define  SPI_Mode3  3  //CPOL = 1, CPHA = 1,

//定义SPI接口
#define SPI_MODE  SPI_Mode0
GPIO_TypeDef *SPI_Port[4] = 
{
  K2_CS_GPIO_Port,//CS
  K2_SCK_GPIO_Port,//SCK
  K2_MISO_GPIO_Port,//MISO
  NULL,//MOSI
};

uint16_t SPI_Pin[4] = 
{
  K2_CS_Pin,//CS
  K2_SCK_Pin,//SCK
  K2_MISO_Pin,//MISO
  NULL,//MOSI
};

void SPI_SetPin(SPI_GPIO_Typedef spi, uint8_t level)
{
  if(level)
    SPI_Port[spi]->ODR |= SPI_Pin[spi];
  else
    SPI_Port[spi]->ODR &= ~SPI_Pin[spi];
}

uint8_t SPI_GetPin(SPI_GPIO_Typedef spi)
{
  if(SPI_Port[spi]->IDR & SPI_Pin[spi])
    return 1;
  else
    return 0;
}

uint8_t Bsp_SPI_Read_Byte(void)
{
  uint16_t data = 0;
  uint8_t i;
  SPI_SetPin(CS, 0);
  #if SPI_MODE == SPI_Mode0
  SPI_SetPin(SCK, 0); 
  for(i=0;i<8;i++)
  {
    SPI_SetPin(SCK, 1); 
    data <<= 1;
    if(SPI_GetPin(MISO)) 
      data |= 0x01;
    SPI_SetPin(SCK, 0); 
  }
  #elif SPI_MODE == SPI_Mode1
  SPI_SetPin(SCK, 0);
  for(i=0;i<8;i++)
  {
    SPI_SetPin(SCK, 1);
    SPI_SetPin(SCK, 0); 
    data <<= 1;
    if(SPI_GetPin(MISO))
      data |= 0x01;
  }
  #elif SPI_MODE == SPI_Mode2
  SPI_SetPin(SCK, 1); 
  for(i=0;i<8;i++)
  {
    SPI_SetPin(SCK, 0);
    data <<= 1;
    if(SPI_GetPin(MISO)) 
      data |= 0x01;
    SPI_SetPin(SCK, 1);
  }
  #elif SPI_MODE == SPI_Mode3
  SPI_SetPin(SCK, 1); 
  for(i=0;i<8;i++)
  {
    SPI_SetPin(SCK, 0);
    SPI_SetPin(SCK, 1);
    data <<= 1;
    if(SPI_GetPin(MISO)) 
      data |= 0x01;
  }
  #endif
  SPI_SetPin(CS, 1);
  return data;
}

void Bsp_SPI_Read(uint8_t* pdata, uint8_t size)
{
  uint8_t i;
  for(i=0; i<size; i++)
  {
    *pdata++ = Bsp_SPI_Read_Byte();
  }
}
