#include "bsp_init.h"
#include "bsp_dev.h"
#include "bsp_page.h"
#include "stdio.h"
#include "string.h"

#include "cmsis_os.h"

struct DWIN_Params_Tags DWIN_Params_Info;

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
 *************************************************/
void DWIN_Delay(void)
{
	uint16_t u16Num = 1000;
	while(u16Num--);
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
uint16_t Bsp_DWIN_SWAP_Data(uint16_t data)
{
	
	return ((data>>8)&0xff)|(data<<8);
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
uint16_t Bsp_DWIN_BCD_Data(uint8_t data)
{
	
	return ((data>>4)*10) + (data&0x0F);
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
uint16_t base_bit[4] = {1000, 100, 10, 1};
uint16_t Bsp_DWIN_CHAR_TO_INT(uint8_t data, uint8_t bit, uint8_t maxbit)
{
	if(maxbit > 3)
		return  0;
	return data*base_bit[bit];
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_WriteRegs(uint8_t regID, uint16_t data)
{
	uint8_t cmd[7] = {0x5A, 0xA5, 0x04, 0x80, regID, data>>8, data};
	Bsp_USART_Send(&HAL_DWIN_USART, cmd, 7);
  DWIN_Delay();
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_WriteVariable(uint16_t varID, void *data, uint8_t len, uint8_t swap)
{
	uint8_t DWIN_Buffer[len];
	uint8_t i;
	uint8_t cmd[50] = {0x5A, 0xA5, 0x00, 0x82, varID>>8, varID};
	uint32_t pdata = (uint32_t)data;
	if(swap)
	{
		for(i=0; i<(len>>1); i++)
		{
			*(uint16_t*)(DWIN_Buffer + (i<<1)) = Bsp_DWIN_SWAP_Data(*(uint16_t*)(pdata + (i<<1)));
		}
	}
	else
	{
		for(i=0; i<len; i++)
		{
			DWIN_Buffer[i] = *(uint8_t*)(pdata++);
		}
	}
	for(i=0; i<len; i++)
	{
		cmd[6+i] = DWIN_Buffer[i];
	}
	cmd[2] = 3 + len;
	Bsp_USART_Send(&HAL_DWIN_USART, cmd, len+6);
	DWIN_Delay();
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_ReadVariable(uint16_t varID, uint8_t len)
{
	uint8_t cmd[7] = {0x5A, 0xA5, 0x04, 0x83, varID>>8, varID, len};
	Bsp_USART_Send(&HAL_DWIN_USART, cmd, 8);
	DWIN_Delay();
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_SelectPage(uint8_t pageID)
{
	Bsp_DWIN_WriteRegs(Reg_Page_ID, pageID);
	Device_Params_Info.Page_Info.PageID = pageID;
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_GetRealTime(void)
{
	uint8_t cmd[6] = {0x5A, 0xA5, 0x03, 0x81, 0x20, 0x07};
	Bsp_USART_Send(&HAL_DWIN_USART, cmd, 6);
  DWIN_Delay();
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_DWIN_Rev_Process(void)
{
	if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Header != 0xA55A)return;
	
	if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Opcode == 0x81)//读取到的数据是寄存器数据
	{
		//实时时间读取
		if(DWIN_Params_Info.DWIN_Rev.Buffer.RegBit.Addr == Reg_CLOCK)
		{
			Device_Params_Info.RTCInfo.Year = DWIN_Params_Info.DWIN_Rev.Buffer.Data[6];
			Device_Params_Info.RTCInfo.Month = DWIN_Params_Info.DWIN_Rev.Buffer.Data[7];
			Device_Params_Info.RTCInfo.Day = DWIN_Params_Info.DWIN_Rev.Buffer.Data[8];
			Device_Params_Info.RTCInfo.Week = DWIN_Params_Info.DWIN_Rev.Buffer.Data[9];
			Device_Params_Info.RTCInfo.Hour = DWIN_Params_Info.DWIN_Rev.Buffer.Data[10];
			Device_Params_Info.RTCInfo.Minute = DWIN_Params_Info.DWIN_Rev.Buffer.Data[11];
			Device_Params_Info.RTCInfo.Second = DWIN_Params_Info.DWIN_Rev.Buffer.Data[12];
		}
	}
	else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Opcode == 0x83)//读取到的数据是变量数据
	{
		//按键操作
		if( DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr <=  Bsp_DWIN_SWAP_Data(KEY_PAGE11))
		{
			uint16_t key_value;
			uint16_t page_id;
			page_id = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr);
			key_value = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
      if(page_id < 12)
        PageKey_Process[page_id](key_value);
		}
		else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(STR_MANAGER_PWD))
		{
			uint8_t len;
			uint8_t i;
			len = DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.DataLen;
			memset(Device_Params_Info.PassWord, 0, 16);
			for(i=0;i<=len;i++)
			{
				Device_Params_Info.PassWord[i] = DWIN_Params_Info.DWIN_Rev.Buffer.Data[7+i];
			}
			PageKey_Process[PAGE1](KEY_PAGE1_RETURN);
		}
		else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(NUM_MAXAIR))
		{
			Device_Params_Info.EEPROM_Data.MT_Air = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
			Device_Params_Info.EEPROM_Data.MT_Water = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData1);
			Bsp_DWIN_SelectPage(PAGE0);
		}
		else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(NUM_GETHOT_ONTIMING_HOUR1))
		{
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[0].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[0].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData1);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[0].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData2);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[0].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData3);
		}
    else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(NUM_GETHOT_ONTIMING_HOUR2))
		{
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[1].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[1].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData1);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[1].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData2);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[1].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData3);
		}
    else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(NUM_GETHOT_ONTIMING_HOUR3))
		{
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[2].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
			Device_Params_Info.EEPROM_Data.GetHotTiming_ON[2].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData1);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[2].Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData2);
			Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[2].Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData3);
		}
		else if(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.Addr ==  Bsp_DWIN_SWAP_Data(NUM_PUTHOT_ONTIMING_HOUR))
		{
			Device_Params_Info.EEPROM_Data.PutHotTiming_ON.Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData);
			Device_Params_Info.EEPROM_Data.PutHotTiming_ON.Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData1);
			Device_Params_Info.EEPROM_Data.PutHotTiming_OFF.Hour = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData2);
			Device_Params_Info.EEPROM_Data.PutHotTiming_OFF.Minute = Bsp_DWIN_SWAP_Data(DWIN_Params_Info.DWIN_Rev.Buffer.VarBit.VarData3);
		}
	}
}
