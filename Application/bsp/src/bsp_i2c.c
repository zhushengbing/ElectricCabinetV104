#include "bsp_i2c.h"

#define I2C_RETRY_COUNT   3 

#define I2C_SDA_OUT     //RTC_SDA_GPIO_Port->MODER |= 0x10000000
#define I2C_SDA_IN      //RTC_SDA_GPIO_Port->MODER &= 0xCFFFFFFF

#define I2C_SDA_Set(x)  {(x>0)?(EEPROM_SDA_GPIO_Port->ODR |= EEPROM_SDA_Pin):(EEPROM_SDA_GPIO_Port->ODR &= ~EEPROM_SDA_Pin);}
#define I2C_SDA_State   EEPROM_SDA_GPIO_Port->IDR &= EEPROM_SDA_Pin

#define I2C_SCL_Set(x)  {(x>0)?(EEPROM_SCL_GPIO_Port->ODR |= EEPROM_SCL_Pin):(EEPROM_SCL_GPIO_Port->ODR &= ~EEPROM_SCL_Pin);}  

void I2C_Delay(void)  
{
//	HAL_Delay(1);
  uint8_t delay = 100;
  while(delay--);
}

void I2C_Start(void)
{
	I2C_SDA_Set(1);
	I2C_SCL_Set(1);
	I2C_Delay();
	I2C_SDA_Set(0);
	I2C_Delay();
	I2C_SCL_Set(0);
	I2C_Delay();
}

void I2C_Stop(void)  
{  
 I2C_SDA_Set(0);   
 I2C_Delay();    
 I2C_SCL_Set(1);   
 I2C_Delay();      
 I2C_SDA_Set(1);  
 I2C_Delay();      
}

void I2C_SendAck(uint8_t ack)  
{  
  I2C_SDA_Set(ack); 
  I2C_Delay();  
  I2C_SCL_Set(1);  
  I2C_Delay();  
  I2C_SCL_Set(0);   
  I2C_Delay();        
} 

uint8_t I2C_SendByte(uint8_t data)  
{  
	uint8_t i; 
	uint8_t ack;
	for(i=0;i<8;i++){ 
		if(data&0x80){			
			I2C_SDA_Set(1);
		}else{
			I2C_SDA_Set(0); 
		}			 
		I2C_Delay();   
		I2C_SCL_Set(1);  
		I2C_Delay();
		I2C_SCL_Set(0);
    data<<=1; 
	}  
	I2C_SDA_Set(1);   
	I2C_SDA_IN;
	I2C_Delay();
	I2C_SCL_Set(1);	
	I2C_Delay();
	if(I2C_SDA_State){
		ack = I2C_ERR;		
	}else{
		ack = I2C_CRR; 
	}
	I2C_SDA_OUT;
	I2C_SCL_Set(0); 
	I2C_Delay();
	return ack;
} 

uint8_t I2C_ReceiveByte(void)  
{  
	uint8_t i;
	uint8_t recv_data = 0;
  I2C_SDA_Set(1);
	I2C_SDA_IN;
	I2C_Delay();
	for(i=0;i<8;i++)  
	{  
    recv_data <<= 1;
		I2C_SCL_Set(1);
		I2C_Delay();
		if(I2C_SDA_State){
			recv_data |= 0x01;
		} 

		I2C_SCL_Set(0);
		I2C_Delay();
	}  
	I2C_SDA_OUT;
	return recv_data;  
}
