#ifndef __BSP_DEV_H__
#define __BSP_DEV_H__

#include "config.h"
#include "bsp_dwin.h"
#include "bsp_gui.h"

struct RTC_Info_Tags
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
	uint8_t Week;
};

union Device_Status_Tag
{
	struct
	{
		uint8_t GetHot_Bit            :1;
		uint8_t PutHot_Bit            :1;
		uint8_t GetHotTiming_Bit      :1;
		uint8_t PutHotTiming_Bit      :1;
		uint8_t AlarmPutHot_Bit       :1;
		uint8_t AlarmGetHot_Bit       :1;
		uint8_t GetHotLevel_Bit       :2;
	}Bit;
	uint16_t All;
};

#pragma pack(1)

struct Timing_Event_Tags
{
	uint8_t  Hour;
	uint8_t  Minute;
};

struct EEPROM_Data_Tags
{
		uint32_t                  EEPROM_Key;
    union Device_Status_Tag   Device_Status;
		uint16_t                  TT_Water; //目标水温 
		uint16_t                  MT_Water; //最高水温
		uint16_t                  MT_Air;   //柜内最高温度
		struct Timing_Event_Tags  GetHotTiming_ON[3];
		struct Timing_Event_Tags  GetHotTiming_OFF[3];
    struct Timing_Event_Tags  PutHotTiming_ON;
    struct Timing_Event_Tags  PutHotTiming_OFF;
		uint16_t 					        Timing_Week;
};

struct Debounce_Count_Tags
{
  uint8_t cGetHotAlarm;
	uint8_t cPutHotAlarm;
	uint8_t cPutHotSpeed;
};

struct Special_Mode_Tags
{
  uint8_t  Debunce;
  uint8_t  Enable;
  int16_t  HoldingTime;
};

struct Dev_Params_Tags
{
	struct EEPROM_Data_Tags    EEPROM_Data;
	uint8_t                    EEPROM_Update;
	char                       PassWord[16];
	struct PageGUI_Tags        Page_Info;
	struct RTC_Info_Tags       RTCInfo;
	uint16_t                   RT_Water; //水温
	uint16_t                   RT_Air;   //柜内空气温度
	uint16_t                   PreTiming_Week;
  uint8_t                    PutHotActive;
	uint8_t                    PutHotSpeed;
	uint8_t                    GetHotSpeed;
  struct Debounce_Count_Tags Debunce;
  struct Special_Mode_Tags   Special_Mode;
};

#pragma pack()

extern struct Dev_Params_Tags Device_Params_Info;

#endif
