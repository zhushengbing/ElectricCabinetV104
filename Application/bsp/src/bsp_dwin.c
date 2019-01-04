#include "bsp_init.h"
#include "bsp_dev.h"
#include "bsp_page.h"
#include "stdio.h"
#include "string.h"

#include "cmsis_os.h"

const uint8_t CRC16_TABH[256] =
{
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
	0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
	0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
	0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
	0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
	0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
} ;

const uint8_t CRC16_TABL[256] =
{
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05,
	0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA,
	0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA,
	0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15,
	0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0,
	0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35,
	0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
	0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
	0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
	0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64,
	0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE,
	0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
	0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
	0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99,
	0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E,
	0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
	0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40,
};

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
uint16_t Get_CRC16Value(uint8_t* pdata, uint16_t len)
{
	uint8_t index, crch, crcl;
	uint16_t i;
	
	crch = crcl = 0xFF;
	for(i=0; i<len; i++)
	{
		index = crch ^ pdata[i];
		crch = crcl ^ CRC16_TABH[index];
		crcl = CRC16_TABL[index];
	}
	return ((uint16_t)crch<<8 | crcl);
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
	uint8_t DWIN_Buffer[len+1];
	uint8_t i;
	uint8_t cmd[50] = {0x5A, 0xA5, 0x00, 0x82, varID>>8, varID};
	
	if(swap)
	{
		for(i=0; i<len; i++)
		{
			if(i%2 == 0)
				DWIN_Buffer[i] = ((uint8_t*)data)[i+1];
			else
				DWIN_Buffer[i] = ((uint8_t*)data)[i-1];
		}
	}
	else
	{
		for(i=0; i<len; i++)
		{
			DWIN_Buffer[i] = ((uint8_t*)data)[i];
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
	uint8_t cmd[9] = {0x5A, 0xA5, 0x04, 0x83, varID>>8, varID, len};
	
	Bsp_USART_Send(&HAL_DWIN_USART, cmd, 7);
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
	uint8_t cmd[8] = {0x5A, 0xA5, 0x03, 0x81, 0x20, 0x07};

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
