#ifndef __BSP_DWIN_H__
#define __BSP_DWIN_H__

#include "config.h"

#define ManagerCode           (char*)"WFTEC"
#define DWIN_BUFFER_SIZE      50

#define Reg_Version           0x00
#define Reg_LED_SET           0x01
#define Reg_BZ_TIME           0x02
#define Reg_Page_ID           0x03
#define Reg_LED_STA           0x1E
#define Reg_CLOCK             0x20

#define Var_KeyDevice         (uint16_t)0x0000
#define Var_KeyPassWord       (uint16_t)0x0001
#define Var_KeyWeekSet        (uint16_t)0x0002

#define Var_StrRealTime_Year  (uint16_t)0x0060 
#define Var_StrRealTime_Month (uint16_t)0x0061 
#define Var_StrRealTime_Day   (uint16_t)0x0062 
#define Var_StrRealTime_Hour  (uint16_t)0x0063 
#define Var_StrRealTime_Min   (uint16_t)0x0064 
#define Var_StrRealTime_Sec   (uint16_t)0x0065 

#define Key_GetHot             (uint8_t)0x01
#define Key_PutHot            (uint8_t)0x02
#define Key_Add               (uint8_t)0x04
#define Key_Dec               (uint8_t)0x08
#define Key_Timming           (uint8_t)0x10
#define Key_TimmingEnable     (uint8_t)0x20
#define Key_ManagerEnter      (uint16_t)0x400
#define Key_FactoryReset      (uint16_t)0x800
#define Key_GetHotLevel       (uint16_t)0x1000



#define KEY_PAGE0                    (uint16_t)0x0000
	#define KEY_PAGE0_GETHOT         (uint16_t)0x0000
	#define KEY_PAGE0_PUTHOT         (uint16_t)0x0001
	#define KEY_PAGE0_ADD            (uint16_t)0x0002
	#define KEY_PAGE0_DEC            (uint16_t)0x0003
	#define KEY_PAGE0_TIMING         (uint16_t)0x0004
	#define KEY_PAGE0_MANAGER        (uint16_t)0x0005
	#define KEY_PAGE0_GETHOTLEVEL    (uint16_t)0x0006
	
#define KEY_PAGE1                    (uint16_t)0x0001
	#define KEY_PAGE1_GETBACK        (uint16_t)0x0000
	#define KEY_PAGE1_RETURN         (uint16_t)0x0001
	
#define KEY_PAGE2                    (uint16_t)0x0002
	#define KEY_PAGE2_RETURN         (uint16_t)0x0000
	#define KEY_PAGE2_MAXWATER_UP    (uint16_t)0x0001
	#define KEY_PAGE2_MAXWATER_DOWN  (uint16_t)0x0002
	#define KEY_PAGE2_MAXAIR_UP      (uint16_t)0x0003
	#define KEY_PAGE2_MAXAIR_DOWN    (uint16_t)0x0004
	#define KEY_PAGE2_DEFAULT        (uint16_t)0x0005
	
#define KEY_PAGE3                    (uint16_t)0x0003
	#define KEY_PAGE3_GETBACK        (uint16_t)0x0000
	#define KEY_PAGE3_PUTHOT_TIMING  (uint16_t)0x0001
	#define KEY_PAGE3_GETHOT_TIMING  (uint16_t)0x0002
	#define KEY_PAGE3_RTTIME_SET     (uint16_t)0x0003
	#define KEY_PAGE3_ON_HOUR_UP     (uint16_t)0x0004
	#define KEY_PAGE3_ON_HOUR_DOWN   (uint16_t)0x0005
	#define KEY_PAGE3_ON_MIN_UP      (uint16_t)0x0006
	#define KEY_PAGE3_ON_MIN_DOWN    (uint16_t)0x0007
	#define KEY_PAGE3_OFF_HOUR_UP    (uint16_t)0x0008
	#define KEY_PAGE3_OFF_HOUR_DOWN  (uint16_t)0x0009
	#define KEY_PAGE3_OFF_MIN_UP     (uint16_t)0x000A
	#define KEY_PAGE3_OFF_MIN_DOWN   (uint16_t)0x000B
//	#define KEY_PAGE3_TIMING_SAVE    (uint16_t)0x000C
	#define KEY_PAGE3_WEEK1          (uint16_t)0x000D
	#define KEY_PAGE3_WEEK2          (uint16_t)0x000E
	#define KEY_PAGE3_WEEK3          (uint16_t)0x000F
	#define KEY_PAGE3_WEEK4          (uint16_t)0x0010
	#define KEY_PAGE3_WEEK5          (uint16_t)0x0011
	#define KEY_PAGE3_WEEK6          (uint16_t)0x0012
	#define KEY_PAGE3_WEEK7          (uint16_t)0x0013
	#define KEY_PAGE3_WEEK_SAVE      (uint16_t)0x0014
	#define KEY_PAGE3_TIMER_ENABLE   (uint16_t)0x0015
  #define KEY_PAGE3_TIM1_ENABLE    (uint16_t)0x0016
  #define KEY_PAGE3_TIM2_ENABLE    (uint16_t)0x0017
  #define KEY_PAGE3_TIM3_ENABLE    (uint16_t)0x0018
  #define KEY_PAGE3_TIM1_SAVE      (uint16_t)0x0019
  #define KEY_PAGE3_TIM2_SAVE      (uint16_t)0x001A
  #define KEY_PAGE3_TIM3_SAVE      (uint16_t)0x001B

#define KEY_PAGE4                    (uint16_t)0x0004
	#define KEY_PAGE4_GETBACK        (uint16_t)0x0000
	#define KEY_PAGE4_PUTHOT_TIMING  (uint16_t)0x0001
	#define KEY_PAGE4_GETHOT_TIMING  (uint16_t)0x0002
	#define KEY_PAGE4_RTTIME_SET     (uint16_t)0x0003
	#define KEY_PAGE4_ON_HOUR_UP     (uint16_t)0x0004
	#define KEY_PAGE4_ON_HOUR_DOWN   (uint16_t)0x0005
	#define KEY_PAGE4_ON_MIN_UP      (uint16_t)0x0006
	#define KEY_PAGE4_ON_MIN_DOWN    (uint16_t)0x0007
	#define KEY_PAGE4_OFF_HOUR_UP    (uint16_t)0x0008
	#define KEY_PAGE4_OFF_HOUR_DOWN  (uint16_t)0x0009
	#define KEY_PAGE4_OFF_MIN_UP     (uint16_t)0x000A
	#define KEY_PAGE4_OFF_MIN_DOWN   (uint16_t)0x000B
	#define KEY_PAGE4_TIMING_SAVE    (uint16_t)0x000C
	#define KEY_PAGE4_WEEK1          (uint16_t)0x000D
	#define KEY_PAGE4_WEEK2          (uint16_t)0x000E
	#define KEY_PAGE4_WEEK3          (uint16_t)0x000F
	#define KEY_PAGE4_WEEK4          (uint16_t)0x0010
	#define KEY_PAGE4_WEEK5          (uint16_t)0x0011
	#define KEY_PAGE4_WEEK6          (uint16_t)0x0012
	#define KEY_PAGE4_WEEK7          (uint16_t)0x0013
	#define KEY_PAGE4_WEEK_SAVE      (uint16_t)0x0014
	#define KEY_PAGE4_TIMER_ENABLE   (uint16_t)0x0015

#define KEY_PAGE5                    (uint16_t)0x0005
	#define KEY_PAGE5_KEYPAD_0       (uint16_t)0x0000
	#define KEY_PAGE5_KEYPAD_1       (uint16_t)0x0001
	#define KEY_PAGE5_KEYPAD_2       (uint16_t)0x0002
	#define KEY_PAGE5_KEYPAD_3       (uint16_t)0x0003
	#define KEY_PAGE5_KEYPAD_4       (uint16_t)0x0004
	#define KEY_PAGE5_KEYPAD_5       (uint16_t)0x0005
	#define KEY_PAGE5_KEYPAD_6       (uint16_t)0x0006
	#define KEY_PAGE5_KEYPAD_7       (uint16_t)0x0007
	#define KEY_PAGE5_KEYPAD_8       (uint16_t)0x0008
	#define KEY_PAGE5_KEYPAD_9       (uint16_t)0x0009
	#define KEY_PAGE5_GETBACK        (uint16_t)0x000A
	#define KEY_PAGE5_TIMEING_SET    (uint16_t)0x000B
	#define KEY_PAGE5_KEYPAD_DEC     (uint16_t)0x000C
	#define KEY_PAGE5_KEYPAD_RETURN  (uint16_t)0x000D

#define KEY_PAGE6                    (uint16_t)0x0006

#define KEY_PAGE7                    (uint16_t)0x0007
  #define KEY_PAGE7_RETURN        (uint16_t)0x0000

#define KEY_PAGE8                    (uint16_t)0x0008

#define KEY_PAGE9                    (uint16_t)0x0009

#define KEY_PAGE10                   (uint16_t)0x000A
	#define KEY_PAGE10_RETURN       (uint16_t)0x0000
	
#define KEY_PAGE11                   (uint16_t)0x000B
	#define KEY_PAGE11_RETURN       (uint16_t)0x0000

#define NUM_RTWATER                  (uint16_t)0x0020
#define NUM_RTAIR                    (uint16_t)0x0021
#define NUM_TTWATER                  (uint16_t)0x0022
#define NUM_MAXAIR                   (uint16_t)0x0023
#define NUM_MAXWATER                 (uint16_t)0x0024

#define NUM_GETHOT_ONTIMING_HOUR1    (uint16_t)0x0030
#define NUM_GETHOT_ONTIMING_MINUTE1  (uint16_t)0x0031
#define NUM_GETHOT_OFFTIMING_HOUR1   (uint16_t)0x0032
#define NUM_GETHOT_OFFTIMING_MINUTE1 (uint16_t)0x0033
#define NUM_GETHOT_ONTIMING_HOUR2    (uint16_t)0x0034
#define NUM_GETHOT_ONTIMING_MINUTE2  (uint16_t)0x0035
#define NUM_GETHOT_OFFTIMING_HOUR2   (uint16_t)0x0036
#define NUM_GETHOT_OFFTIMING_MINUTE2 (uint16_t)0x0037
#define NUM_GETHOT_ONTIMING_HOUR3    (uint16_t)0x0038
#define NUM_GETHOT_ONTIMING_MINUTE3  (uint16_t)0x0039
#define NUM_GETHOT_OFFTIMING_HOUR3   (uint16_t)0x003a
#define NUM_GETHOT_OFFTIMING_MINUTE3 (uint16_t)0x003b

#define NUM_PUTHOT_ONTIMING_HOUR     (uint16_t)0x0040
#define NUM_PUTHOT_ONTIMING_MINUTE   (uint16_t)0x0041
#define NUM_PUTHOT_OFFTIMING_HOUR    (uint16_t)0x0042    
#define NUM_PUTHOT_OFFTIMING_MINUTE  (uint16_t)0x0043

#define STR_MANAGER_PWD              (uint16_t)0x0050

#define NUM_TEMP_TIME_YEAR1          (uint16_t)0x0060
#define NUM_TEMP_TIME_YEAR2          (uint16_t)0x0061
#define NUM_TEMP_TIME_MONTH1         (uint16_t)0x0062
#define NUM_TEMP_TIME_MONTH2         (uint16_t)0x0063
#define NUM_TEMP_TIME_DAY1           (uint16_t)0x0064
#define NUM_TEMP_TIME_DAY2           (uint16_t)0x0065
#define NUM_TEMP_TIME_HOUR1          (uint16_t)0x0066
#define NUM_TEMP_TIME_HOUR2          (uint16_t)0x0067
#define NUM_TEMP_TIME_MINUTE1        (uint16_t)0x0068
#define NUM_TEMP_TIME_MINUTE2        (uint16_t)0x0069
#define NUM_TEMP_TIME_SECOND1        (uint16_t)0x006A
#define NUM_TEMP_TIME_SECOND2        (uint16_t)0x006B
#define NUM_TEMP_TIME_CURSOR         (uint16_t)0x006C

#define ICON_DEVICE_STATUS           (uint16_t)0x0010
#define ICON_TIMING_WEEK             (uint16_t)0x0011

#pragma pack(1)

union DWIN_Icon_Status_Tags
{
	struct
	{
		uint8_t   PowerKey_Bit           :1;
		uint8_t   PutHotKey_Bit          :1;
		uint8_t   AddKey_Bit             :1;
		uint8_t   DecKey_Bit             :1;
		uint8_t   TimmingKey_Bit         :1;
		uint8_t   GetHotPicture_Bit      :1;
		uint8_t   PutHotPicture_Bit      :1;
		uint8_t   GetHotLevelPicture_Bit :3;
		uint8_t   WaterAlarm_Bit         :1;
		uint8_t   AirAlarm_Bit           :1;
		uint8_t   GetHotTiming_Bit       :1;
		uint8_t   PutHotTiming_Bit       :1;
	}Bit;
	uint16_t All;
};

union Rev_Buf_Tags
{
	struct
	{
		uint16_t Header;
		uint8_t  Len;
		uint8_t  Opcode;
		uint16_t Addr;
		uint8_t  DataLen;
		uint16_t VarData;
		uint16_t VarData1;
		uint16_t VarData2;
		uint16_t VarData3;
	}VarBit;
	struct
	{
		uint16_t Header;
		uint8_t  Len;
		uint8_t  Opcode;
		uint8_t  Addr;
		uint8_t  DataLen;
	}RegBit;
	uint8_t  Data[DWIN_BUFFER_SIZE];
};

#pragma pack()

struct DWIN_Rev_Tag
{
	union Rev_Buf_Tags Buffer;
	uint8_t  LifeTime;
	uint8_t  Count;
};

struct DWIN_Params_Tags
{
	struct DWIN_Rev_Tag         DWIN_Rev;
	union DWIN_Icon_Status_Tags DWIN_Icon;
};

extern struct DWIN_Params_Tags DWIN_Params_Info;

void Bsp_DWIN_Rev_Process(void);
void Bsp_DWIN_DGUI_Process(void);
void Bsp_DWIN_GetRealTime(void);
void Bsp_DWIN_WriteVariable(uint16_t varID, void *data, uint8_t len, uint8_t swap);

#endif
