#include "bsp_eeprom.h"
#include "bsp_i2c.h"

static   void EEPROM_ReadBuffer(uint8_t* pBuffer,uint16_t ReadAddress,uint8_t NumbyteToWrite);
static   void EEPROM_WriteBuffer(uint8_t* pBuffer,uint16_t WriteAddress,uint8_t NumByteToWrite);
static   uint8_t *EEPROM_ReadPage(uint8_t *,uint8_t,uint8_t);
static   uint8_t *EEPROM_WritePage(uint8_t *,uint16_t,uint8_t);

uint8_t EEPROM_Write(uint8_t* pBuffer,uint16_t WriteAddress,uint8_t NumbyteToWrite)
{
  uint8_t TempBuffer,Temp2Buffer;
  if((WriteAddress+NumbyteToWrite) > EE_SIZE) 
    {
      return 0;
    }
  else
    {
      EEPROM_WriteBuffer(pBuffer,WriteAddress,NumbyteToWrite);
      EEPROM_ReadBuffer(&TempBuffer,WriteAddress+NumbyteToWrite-1,1);
      Temp2Buffer=*(pBuffer+NumbyteToWrite-1);
      if(TempBuffer==Temp2Buffer)
        return 1;
      else
        return 0;
    }
}

uint8_t EEPROM_Read(uint8_t* pBuffer,uint16_t ReadAddress,uint8_t NumbyteToWrite) 
{
  if((ReadAddress+NumbyteToWrite) > EE_SIZE) 
    return 0;
  else
  {
    EEPROM_ReadBuffer(pBuffer,ReadAddress,NumbyteToWrite);
      return 1;
  }
}


/**
***************************************************
 *@Function     :EEPROM_ReadBuffer()
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 ReadAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/  
void EEPROM_ReadBuffer(uint8_t* pBuffer,uint16_t ReadAddress,uint8_t NumbyteToWrite)
{
  EEPROM_ReadPage(pBuffer,ReadAddress,NumbyteToWrite); 
}



/**
***************************************************
 *@Function     :EEPROM_WriteBuffer()
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 WriteAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
void EEPROM_WriteBuffer(uint8_t* pBuffer,uint16_t WriteAddress,uint8_t NumByteToWrite)
{
  uint8_t NumOfPage=0,NumOfSingle=0; //,count=0
  uint16_t Part=0;//
  
  /*******判断起始地址是否跨页片区******/
  Part=WriteAddress/PAGE_SIZE;
  if(Part!=0)
    Part=PAGE_SIZE*(Part+1)-WriteAddress;      
  else 
    Part=PAGE_SIZE-WriteAddress;
  
  /******/
  if(Part >= NumByteToWrite)
    EEPROM_WritePage(pBuffer,WriteAddress,NumByteToWrite);
  else
  {
    NumOfPage = (NumByteToWrite-Part)/PAGE_SIZE;  
    NumOfSingle = (NumByteToWrite-Part)%PAGE_SIZE;   
    pBuffer = EEPROM_WritePage(pBuffer,WriteAddress,Part);
    
    NumByteToWrite -= Part;
    WriteAddress += Part;
    while(NumOfPage--)
    {     
      pBuffer = EEPROM_WritePage(pBuffer,WriteAddress,PAGE_SIZE);
      WriteAddress += PAGE_SIZE;
    }   
    if(NumOfSingle!=0)
      EEPROM_WritePage(pBuffer,WriteAddress,NumOfSingle);
  }
}



/**
***************************************************
 *@Function     :EEPROM_WritePage(uint8_t *pBuffer,uint8_t PartAddr,uint8_t WriteAddr,uint8_t NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 PartAddr:
                 WriteAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
uint8_t* EEPROM_WritePage(uint8_t *pBuffer,uint16_t WriteAddr,uint8_t NumByteToWrite)
{
  /*send Start condition,test on EV5 and clear it*/
  I2C_Start();
  if(I2C_CRR == (I2C_SendByte(WriteAddress_EEPROM)))  //发送写命令并检查应答位
  {
    /******writeAddress for 8bit or 16 bit**********/
  #if (EEPROM<32)
   /**send the EEPROM's internal address,test on EV8 and clear it**/
    I2C_SendByte((uint8_t)WriteAddr);
  #else
    I2C_SendByte((uint8_t)((WriteAddr&0xFF00)>>8));
    I2C_SendByte((uint8_t)(WriteAddr&0x00FF)); 
  #endif 
    /* DMA control */
    /*send the bufferData and buffer size*/
    while(NumByteToWrite--)
    {
      I2C_SendByte(*pBuffer);
      pBuffer++;
    }
    
    /*send STOP condition*/
    I2C_Stop();
    return pBuffer;
  }
  return NULL;
}


/**
*************************************************
 *@Function     : EEPROM_ReadPage(uint8_t *pBuffer,uint8_t PartAddr,uint8_t WriteAddr,uint8_t NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :
 *@Brief        :
***************************************************/
uint8_t* EEPROM_ReadPage(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite)
{
  /*send Start condition,test on EV5 and clear it*/
  I2C_Start();
  /*send Part Address for write,test on EV6 and clear it*/
  if(I2C_CRR == (I2C_SendByte(WriteAddress_EEPROM)))  //发送写命令并检查应答位
  {

    /*send the EEPROM's internal address,test on EV8 and clear it
    I2C_SendByte(I2C1,WriteAddr);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));  */
     /******writeAddress for 8bit or 16 bit**********/
  #if (EEPROM<32)
   /**send the EEPROM's internal address,test on EV8 and clear it**/
    I2C_SendByte((uint8_t)WriteAddr);
  #else
    I2C_SendByte((uint8_t)((WriteAddr&0xFF00)>>8));
    I2C_SendByte((uint8_t)(WriteAddr&0x00FF)); 
  #endif
    /* DMA control */
    /*send Start condition,test on EV5 and clear it*/
    I2C_Start();
    /*send Part Address for write,test on EV6 and clear it*/
    I2C_SendByte(ReadAddress_EEPROM);
    while(NumByteToWrite--)
    {
      *pBuffer=I2C_ReceiveByte();
      pBuffer++;
      if(NumByteToWrite == 0)
        I2C_SendAck(1);  //发送非应答信号
      else
        I2C_SendAck(0);  //发送应答信号
    }
    I2C_Stop();	
   return pBuffer;
  }
  return NULL;
}

