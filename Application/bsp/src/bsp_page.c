#include "bsp_page.h"
#include "bsp_dev.h"
#include "bsp_init.h"
#include "string.h"

extern void Bsp_DWIN_WriteVariable(uint16_t varID, void *data, uint8_t len, uint8_t swap);
extern void Bsp_DWIN_ReadVariable(uint16_t varID, uint8_t len);
extern void Bsp_DWIN_SelectPage(uint8_t pageID);
extern uint16_t Bsp_DWIN_BCD_Data(uint8_t data);

uint8_t rtc_set[13] = { 0x5A, 0xA5, 0x0A, 0x80, 0x1F, 0X5A, 0, 0, 0, 0x00, 0, 0, 0};

static uint8_t Page0Key_Process(uint16_t keyValue)
{
	uint8_t isupdate = 0;
	switch(keyValue)
	{
		case KEY_PAGE0_GETHOT:
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = !Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit;
      if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit == 0 &&\
         Device_Params_Info.Special_Mode.HoldingTime > 0)
      {
        Device_Params_Info.Special_Mode.HoldingTime = -1;
        Device_Params_Info.Special_Mode.Enable = 0;
      }
			isupdate = 1;
			break;
		case KEY_PAGE0_PUTHOT:
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit = !Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit;
			isupdate = 1;
			break;
		case KEY_PAGE0_ADD:
			if(Device_Params_Info.EEPROM_Data.TT_Water < Device_Params_Info.EEPROM_Data.MT_Water)
			{
				Device_Params_Info.EEPROM_Data.TT_Water ++;
        Device_Params_Info.EEPROM_Update = 1;
			}
			isupdate = 1;
			break;
		case KEY_PAGE0_DEC:
			if(Device_Params_Info.EEPROM_Data.TT_Water > 0)
			{
				Device_Params_Info.EEPROM_Data.TT_Water --;
        Device_Params_Info.EEPROM_Update = 1;
			}
			isupdate = 1;
			break;
		case KEY_PAGE0_TIMING:
		{
			uint16_t pGetHotTimer[12] = {
				Device_Params_Info.EEPROM_Data.GetHotTiming_ON[0].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_ON[0].Minute,
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[0].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[0].Minute,
        Device_Params_Info.EEPROM_Data.GetHotTiming_ON[1].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_ON[1].Minute,
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[1].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[1].Minute,
        Device_Params_Info.EEPROM_Data.GetHotTiming_ON[2].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_ON[2].Minute,
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[2].Hour, 
				Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[2].Minute
			};
			uint16_t pPutHotTimer[4] = {
				Device_Params_Info.EEPROM_Data.PutHotTiming_ON.Hour, 
				Device_Params_Info.EEPROM_Data.PutHotTiming_ON.Minute,
				Device_Params_Info.EEPROM_Data.PutHotTiming_OFF.Hour, 
				Device_Params_Info.EEPROM_Data.PutHotTiming_OFF.Minute
			};
			Bsp_DWIN_WriteVariable(NUM_GETHOT_ONTIMING_HOUR1, pGetHotTimer, 24, 1);
			Bsp_DWIN_WriteVariable(NUM_PUTHOT_ONTIMING_HOUR, pPutHotTimer, 8, 1);
      Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK, &Device_Params_Info.EEPROM_Data.Timing_Week, 2, 1);
			Bsp_DWIN_SelectPage(PAGE3);
		}break;
		case KEY_PAGE0_GETHOTLEVEL:
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit++;
			if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit > 2)
			{
				Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit = 0;
			}
			isupdate = 1;
			break;
		case KEY_PAGE0_MANAGER:
			Bsp_DWIN_SelectPage(PAGE1);
			break;
		default:break;
	}
	return isupdate;
}

static uint8_t Page1Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	switch(keyValue)
	{
		case KEY_PAGE1_GETBACK:
			Bsp_DWIN_SelectPage(PAGE0);
			break;
		case KEY_PAGE1_RETURN:
		{
			int isok = -1;
			isok = strcmp(ManagerCode, Device_Params_Info.PassWord);
			if(isok)
			{
				Bsp_DWIN_SelectPage(PAGE7);
			}
			else
			{
				Bsp_DWIN_WriteVariable(NUM_MAXWATER,&Device_Params_Info.EEPROM_Data.MT_Water,2,1);
				Bsp_DWIN_WriteVariable(NUM_MAXAIR,&Device_Params_Info.EEPROM_Data.MT_Air,2,1);
				Bsp_DWIN_SelectPage(PAGE2);
			}
		}break;
		default:break;
	}
	return is_update;
}

static uint8_t Page2Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	switch(keyValue)
	{
		case KEY_PAGE2_MAXWATER_UP:
			
			break;
		case KEY_PAGE2_MAXWATER_DOWN:
			break;
		case KEY_PAGE2_MAXAIR_UP:
			break;
		case KEY_PAGE2_MAXAIR_DOWN:
			break;
		case KEY_PAGE2_DEFAULT:
			Device_Params_Info.EEPROM_Data.MT_Water = 65;
			Device_Params_Info.EEPROM_Data.MT_Air = 650;
			Bsp_DWIN_WriteVariable(NUM_MAXAIR,&Device_Params_Info.EEPROM_Data.MT_Air,2,1);
			Bsp_DWIN_WriteVariable(NUM_MAXWATER,&Device_Params_Info.EEPROM_Data.MT_Water,2,1);
			break;
		case KEY_PAGE2_RETURN:
			Bsp_DWIN_ReadVariable(NUM_MAXAIR, 2);
			break;
		default:break;
	}
	return is_update;
}

static uint8_t Page3Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	switch(keyValue)
	{
		case KEY_PAGE3_GETBACK:
			Bsp_DWIN_SelectPage(PAGE0);
			break;
		case KEY_PAGE3_TIM1_SAVE:
			/*读取定时数据并保存*/
			Bsp_DWIN_ReadVariable(NUM_GETHOT_ONTIMING_HOUR1, 4);
      Device_Params_Info.EEPROM_Data.Timing_Week = Device_Params_Info.PreTiming_Week;
			Bsp_DWIN_SelectPage(PAGE10);
			break;
    case KEY_PAGE3_TIM2_SAVE:
			/*读取定时数据并保存*/
			Bsp_DWIN_ReadVariable(NUM_GETHOT_ONTIMING_HOUR2, 4);
      Device_Params_Info.EEPROM_Data.Timing_Week = Device_Params_Info.PreTiming_Week;
			Bsp_DWIN_SelectPage(PAGE14);
			break;
    case KEY_PAGE3_TIM3_SAVE:
			/*读取定时数据并保存*/
			Bsp_DWIN_ReadVariable(NUM_GETHOT_ONTIMING_HOUR3, 4);
      Device_Params_Info.EEPROM_Data.Timing_Week = Device_Params_Info.PreTiming_Week;
			Bsp_DWIN_SelectPage(PAGE15);
			break;
		case KEY_PAGE3_GETHOT_TIMING:
			Bsp_DWIN_SelectPage(PAGE4);
			break;
		case KEY_PAGE3_PUTHOT_TIMING:
			Bsp_DWIN_SelectPage(PAGE3);
			break;
		case KEY_PAGE3_RTTIME_SET:
		{
			uint8_t i;
			uint16_t pDWINdata = 0;
			uint8_t data_bit = 0;
			uint16_t data = 0;
			uint8_t *pdata = &Device_Params_Info.RTCInfo.Year;
			for(i=0;i<12;i++)
			{
				data_bit = ((i+1)%2)<<2;
				pDWINdata = NUM_TEMP_TIME_YEAR1 + i;
				data = (pdata[i/2] >> data_bit) & 0x0f;
				Bsp_DWIN_WriteVariable(pDWINdata, &data, 2, 1);
			}
			Device_Params_Info.Page_Info.Cursor = 0;
			Bsp_DWIN_SelectPage(PAGE5);
		}break;
		case KEY_PAGE3_WEEK1:
			Device_Params_Info.PreTiming_Week ^= 0x01;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK2:
			Device_Params_Info.PreTiming_Week ^= 0x02;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK3:
			Device_Params_Info.PreTiming_Week ^= 0x04;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK4:
			Device_Params_Info.PreTiming_Week ^= 0x08;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK5:
			Device_Params_Info.PreTiming_Week ^= 0x10;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK6:
			Device_Params_Info.PreTiming_Week ^= 0x20;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK7:
			Device_Params_Info.PreTiming_Week ^= 0x40;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE3_WEEK_SAVE:
			Device_Params_Info.EEPROM_Data.Timing_Week = Device_Params_Info.PreTiming_Week;
			Device_Params_Info.EEPROM_Update = 1;
			Bsp_DWIN_SelectPage(PAGE10);
			break;
		case KEY_PAGE3_TIMER_ENABLE:
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit = !Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit;
      Device_Params_Info.EEPROM_Update = 1;
      is_update = 1;
			break;
    case KEY_PAGE3_TIM1_ENABLE:
			Device_Params_Info.PreTiming_Week ^= 0x80;
      Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
    case KEY_PAGE3_TIM2_ENABLE:
      Device_Params_Info.PreTiming_Week ^= 0x100;
      Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
    case KEY_PAGE3_TIM3_ENABLE:
      Device_Params_Info.PreTiming_Week ^= 0x200;
      Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		default:break;
	}
	return is_update;
}

static uint8_t Page4Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	switch(keyValue)
	{
		case KEY_PAGE4_GETBACK:
			Bsp_DWIN_SelectPage(PAGE0);
			break;
		case KEY_PAGE4_TIMING_SAVE:
			/*读取定时数据并保存*/
			Bsp_DWIN_ReadVariable(NUM_PUTHOT_ONTIMING_HOUR, 4);
			Bsp_DWIN_SelectPage(PAGE11);
			break;
		case KEY_PAGE4_GETHOT_TIMING:
			Bsp_DWIN_SelectPage(PAGE4);
			break;
		case KEY_PAGE4_PUTHOT_TIMING:
			Bsp_DWIN_SelectPage(PAGE3);
			break;
		case KEY_PAGE4_RTTIME_SET:
			Bsp_DWIN_SelectPage(PAGE5);
			break;
		case KEY_PAGE4_WEEK1:
			Device_Params_Info.PreTiming_Week ^= 0x01;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK2:
			Device_Params_Info.PreTiming_Week ^= 0x02;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK3:
			Device_Params_Info.PreTiming_Week ^= 0x04;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK4:
			Device_Params_Info.PreTiming_Week ^= 0x08;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK5:
			Device_Params_Info.PreTiming_Week ^= 0x10;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK6:
			Device_Params_Info.PreTiming_Week ^= 0x20;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK7:
			Device_Params_Info.PreTiming_Week ^= 0x40;
			Bsp_DWIN_WriteVariable(ICON_TIMING_WEEK,&Device_Params_Info.PreTiming_Week,2,1);
			break;
		case KEY_PAGE4_WEEK_SAVE:
			Device_Params_Info.EEPROM_Data.Timing_Week = Device_Params_Info.PreTiming_Week;
			Device_Params_Info.EEPROM_Update = 1;
			Bsp_DWIN_SelectPage(PAGE11);
			break;
		case KEY_PAGE4_TIMER_ENABLE:
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit = !Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit;
      Device_Params_Info.EEPROM_Update = 1;
      is_update = 1;
			break;
		default:break;
	}
	return is_update;
}

static uint8_t Page5Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	uint8_t *pdata = NULL;
	uint16_t pDWINdata = 0;
	uint8_t data_bit = 0;
	uint16_t data = 0;
	uint16_t cursor = 0;
	
	if( keyValue == KEY_PAGE5_KEYPAD_DEC)
	{
			if(Device_Params_Info.Page_Info.Cursor)
				Device_Params_Info.Page_Info.Cursor--;
			else
				Device_Params_Info.Page_Info.Cursor = 11;
	}
	else if( keyValue == KEY_PAGE5_KEYPAD_RETURN)
	{
		struct RTC_Info_Tags *pTime = &Device_Params_Info.RTCInfo;
		rtc_set[6] = pTime->Year;
		rtc_set[7] = pTime->Month;
		rtc_set[8] = pTime->Day;
		rtc_set[10] = pTime->Hour;
		rtc_set[11] = pTime->Minute;
		rtc_set[12] = pTime->Second;
		Bsp_USART_Send(&HAL_DWIN_USART, rtc_set, 13);
	}
	if(Device_Params_Info.Page_Info.Cursor >= 12)return is_update;
	
	pdata = &Device_Params_Info.RTCInfo.Year + Device_Params_Info.Page_Info.Cursor/2;
	data_bit = ((Device_Params_Info.Page_Info.Cursor+1)%2)<<2;
	pDWINdata = NUM_TEMP_TIME_YEAR1 + Device_Params_Info.Page_Info.Cursor;
	switch(keyValue)
	{
		case KEY_PAGE5_GETBACK:
		{
			Bsp_DWIN_SelectPage(PAGE0);
		}break;
		case KEY_PAGE5_TIMEING_SET:
		{
			Bsp_DWIN_SelectPage(PAGE3);
		}break;
		case KEY_PAGE5_KEYPAD_0:
		case KEY_PAGE5_KEYPAD_1:
		case KEY_PAGE5_KEYPAD_2:
		case KEY_PAGE5_KEYPAD_3:
		case KEY_PAGE5_KEYPAD_4:
		case KEY_PAGE5_KEYPAD_5:
		case KEY_PAGE5_KEYPAD_6:
		case KEY_PAGE5_KEYPAD_7:
		case KEY_PAGE5_KEYPAD_8:
		case KEY_PAGE5_KEYPAD_9:
		{
      *pdata &= ~(uint8_t)(0xF << data_bit);
			*pdata |= (uint8_t)(keyValue << data_bit);
			Device_Params_Info.Page_Info.Cursor++;
			data = (*pdata >> data_bit) & 0x0f;
			Bsp_DWIN_WriteVariable(pDWINdata, &data, 2, 1);
		}break;
		default:break;
	}
	cursor = 1 << Device_Params_Info.Page_Info.Cursor;
	Bsp_DWIN_WriteVariable(NUM_TEMP_TIME_CURSOR, &cursor, 2, 1);
	return is_update;
}

static uint8_t Page6Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	return is_update;
}

static uint8_t Page7Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	if(keyValue == KEY_PAGE7_RETURN)
	{
		memset(Device_Params_Info.PassWord, 0, 16);
		Bsp_DWIN_SelectPage(PAGE1);
		is_update = 1;
	}
	return is_update;
}

static uint8_t Page8Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	return is_update;
}

static uint8_t Page9Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	return is_update;
}

static uint8_t Page10Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	if(keyValue == KEY_PAGE10_RETURN)
	{
		Bsp_DWIN_SelectPage(PAGE3);
	}
	return is_update;
}

static uint8_t Page11Key_Process(uint16_t keyValue)
{
	uint8_t is_update = 0;
	if(keyValue == KEY_PAGE11_RETURN)
	{
		Bsp_DWIN_SelectPage(PAGE4);
	}
	return is_update;
}


uint8_t (*PageKey_Process[12])(uint16_t) = 
{
	Page0Key_Process,
	Page1Key_Process,
	Page2Key_Process,
	Page3Key_Process,
	Page4Key_Process,
	Page5Key_Process,
	Page6Key_Process,
	Page7Key_Process,
	Page8Key_Process,
	Page9Key_Process,
	Page10Key_Process,
	Page11Key_Process,
};
