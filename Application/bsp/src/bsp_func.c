#include "bsp_init.h"
#include "bsp_dev.h"
#include "bsp_func.h"
#include "bsp_eeprom.h"

#define DIFF_MAX  5

static uint8_t Speed_Table[3] = {1, 2, 3};  

extern uint16_t Bsp_DWIN_BCD_Data(uint8_t data);

uint8_t active_puthot[2], active_gethot[6];

static uint8_t Next_GetHot_Status = 0xFF, Pre_GetHot_Status = 0xFF;

void Bsp_Function_PutHot_Speed(uint8_t diff_t, uint8_t *speed)
{
  if( (Device_Params_Info.RT_Water + diff_t) <= Device_Params_Info.EEPROM_Data.TT_Water )
  {
    if(Device_Params_Info.PutHotSpeed != *speed)
    {
      if(Device_Params_Info.Debunce.cPutHotSpeed > 5)
        Device_Params_Info.PutHotSpeed = *speed;
      else
        Device_Params_Info.Debunce.cPutHotSpeed++;
    }
  }
  else
    Device_Params_Info.Debunce.cPutHotSpeed = 0;
}
/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_Func_PutHot_Process(void)
{
  static uint8_t prePutHot = 0;
  
  if(prePutHot != Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit)
  {
    prePutHot = Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit;
    Device_Params_Info.PutHotActive = 1;
  }
  
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit == 0 || Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit)
	{
    Device_Params_Info.PutHotActive = 0;
		Device_Params_Info.PutHotSpeed = 0;
		return;
	}
  
  if(Device_Params_Info.PutHotActive == 1)
  {
    uint8_t *speed = NULL;
    
    if(Device_Params_Info.RT_Air <= 350)
      speed = Speed_Table+2;
    else if(Device_Params_Info.RT_Air <= 500)
      speed = Speed_Table+1;
    else
      speed = Speed_Table;
    Bsp_Function_PutHot_Speed(DIFF_MAX, speed);
  }
  else
  {
    if( (Device_Params_Info.RT_Water+DIFF_MAX) < Device_Params_Info.EEPROM_Data.TT_Water )
    {
      Device_Params_Info.PutHotActive = 1;
    }
  }
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_Func_GetHot_Process(void)
{
	uint16_t max_air;
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit == 0)
	{
		Device_Params_Info.GetHotSpeed = 0;
		return;
	}
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit)
	{
		Device_Params_Info.GetHotSpeed = 0;
		return;
	}
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 2)
		max_air = Device_Params_Info.EEPROM_Data.MT_Air;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 1)
		max_air = 550;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 0)
		max_air = 350;
	switch(Device_Params_Info.GetHotSpeed )
	{
		case 0:
		{
			if( (Device_Params_Info.RT_Air + 10) <= max_air)
				Device_Params_Info.GetHotSpeed = 1;
		}break;
		case 1:
		{
			if( Device_Params_Info.RT_Air >= max_air)
				Device_Params_Info.GetHotSpeed = 0;
		}break;
		default:break;
	} 
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_MainFunc_Process(void)
{
	switch(Device_Params_Info.GetHotSpeed)
	{
		case 0:
			HAL_GPIO_WritePin(HEATERROD1_GPIO_Port, HEATERROD1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(HEATERROD2_GPIO_Port, HEATERROD2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(HEATERROD3_GPIO_Port, HEATERROD3_Pin, GPIO_PIN_RESET);
			break;
		case 1:
		case 2:
			HAL_GPIO_WritePin(HEATERROD1_GPIO_Port, HEATERROD1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(HEATERROD2_GPIO_Port, HEATERROD2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(HEATERROD3_GPIO_Port, HEATERROD3_Pin, GPIO_PIN_SET);
			break;
		default:break;
	}
	
	switch(Device_Params_Info.PutHotSpeed)
	{
		case 0:
			FAN_MID_GPIO_Port->ODR &= ~FAN_MID_Pin;
			FAN_HIGH_GPIO_Port->ODR &= ~FAN_HIGH_Pin;
			FAN_LOW_GPIO_Port->ODR &= ~FAN_LOW_Pin;
			break;
		case 1:
			FAN_MID_GPIO_Port->ODR &= ~FAN_MID_Pin;
			FAN_HIGH_GPIO_Port->ODR &= ~FAN_HIGH_Pin;
			FAN_LOW_GPIO_Port->ODR |= FAN_LOW_Pin;	
			break;
		case 2:
			FAN_LOW_GPIO_Port->ODR &= ~FAN_LOW_Pin;
			FAN_HIGH_GPIO_Port->ODR &= ~FAN_HIGH_Pin;
			FAN_MID_GPIO_Port->ODR |= FAN_MID_Pin;
			break;
		case 3:
			FAN_LOW_GPIO_Port->ODR &= ~FAN_LOW_Pin;
			FAN_MID_GPIO_Port->ODR &= ~FAN_MID_Pin;
			FAN_HIGH_GPIO_Port->ODR |= FAN_HIGH_Pin;
			break;
		default:break;
	}
	
	if( Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit || Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit)
		MOTOR_WATER_GPIO_Port->ODR |= MOTOR_WATER_Pin;
	else
		MOTOR_WATER_GPIO_Port->ODR &= ~MOTOR_WATER_Pin;
	
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit)
		MOTOR_AIR_GPIO_Port->ODR |= MOTOR_AIR_Pin;
	else
		MOTOR_AIR_GPIO_Port->ODR &= ~MOTOR_AIR_Pin;
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_Func_GetRealTime(void)
{
	Bsp_DWIN_GetRealTime();
}

void Bsp_Func_PutHotTimer_ON(void)
{
  uint8_t hour, minute, week;
  struct Timing_Event_Tags* pTimer_ON = &Device_Params_Info.EEPROM_Data.PutHotTiming_ON;
  struct Timing_Event_Tags* pTimer_OFF = &Device_Params_Info.EEPROM_Data.PutHotTiming_OFF;
  if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit == 0)return;
  if(pTimer_ON->Hour == pTimer_OFF->Hour &&\
     pTimer_ON->Minute == pTimer_OFF->Minute)return;
  if(Device_Params_Info.RTCInfo.Week == 0)
    week = 6;
  else
    week = Device_Params_Info.RTCInfo.Week - 1;
  if( ((Device_Params_Info.EEPROM_Data.Timing_Week >> week ) & 0x01) == 0 )return;
  hour = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Hour);
  minute = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Minute);
  if( hour != pTimer_ON->Hour){active_puthot[0] = 1;return;}
	if( minute != pTimer_ON->Minute){active_puthot[0] = 1;return;}
  if(active_puthot[0] != 0)
  {
    active_puthot[0] = 0;
    Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit = 1;
  }
}

void Bsp_Func_PutHotTimer_OFF(void)
{
  uint8_t hour, minute;
  struct Timing_Event_Tags* pTimer_ON = &Device_Params_Info.EEPROM_Data.PutHotTiming_ON;
  struct Timing_Event_Tags* pTimer_OFF = &Device_Params_Info.EEPROM_Data.PutHotTiming_OFF;
  if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit == 0)return;
  if(pTimer_ON->Hour == pTimer_OFF->Hour &&\
     pTimer_ON->Minute == pTimer_OFF->Minute)return;
  hour = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Hour);
  minute = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Minute);
  if( hour != pTimer_OFF->Hour){active_puthot[1] = 1;return;}
	if( minute != pTimer_OFF->Minute){active_puthot[1] = 1;return;}
  if(active_puthot[1] != 0)
  {
    Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit = 0;
    active_puthot[1] = 0;
  }
}

struct Timing_Event_Tags *GetHot_Timer_Table[3][2] = 
{
  &Device_Params_Info.EEPROM_Data.GetHotTiming_ON[0], &Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[0],
  &Device_Params_Info.EEPROM_Data.GetHotTiming_ON[1], &Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[1],
  &Device_Params_Info.EEPROM_Data.GetHotTiming_ON[2], &Device_Params_Info.EEPROM_Data.GetHotTiming_OFF[2],
};

void Bsp_Func_GetHotTimer_ON(uint8_t timer)
{
  uint8_t hour, minute, week;
  uint8_t active_index = timer<<1;
  struct Timing_Event_Tags* pTimer_ON = GetHot_Timer_Table[timer][0];
  struct Timing_Event_Tags* pTimer_OFF = GetHot_Timer_Table[timer][1];
  if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit == 0)return;
  if(((Device_Params_Info.EEPROM_Data.Timing_Week>>7)&(0x01<<timer)) == 0)return;
  if(pTimer_ON->Hour == pTimer_OFF->Hour &&\
     pTimer_ON->Minute == pTimer_OFF->Minute)return;
  if(Device_Params_Info.RTCInfo.Week == 0)
    week = 6;
  else
    week = Device_Params_Info.RTCInfo.Week - 1;
  if( ((Device_Params_Info.EEPROM_Data.Timing_Week >> week ) & 0x01) == 0 )return;
  hour = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Hour);
  minute = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Minute);
  if( hour != pTimer_ON->Hour){active_gethot[active_index] = 1;return;}
	if( minute != pTimer_ON->Minute){active_gethot[active_index] = 1;return;}
  if(active_gethot[active_index] != 0)
  {
    active_gethot[active_index] = 0;
    Next_GetHot_Status = 1;
//    Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = 1;
  }
}

void Bsp_Func_GetHotTimer_OFF(uint8_t timer)
{
  uint8_t hour, minute;
  uint8_t active_index = 1 + (timer<<1);
  struct Timing_Event_Tags* pTimer_ON = GetHot_Timer_Table[timer][0];
  struct Timing_Event_Tags* pTimer_OFF = GetHot_Timer_Table[timer][1];
  if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit == 0)return;
  if(((Device_Params_Info.EEPROM_Data.Timing_Week>>7)&(0x01<<timer)) == 0)return;
  if(pTimer_ON->Hour == pTimer_OFF->Hour &&\
     pTimer_ON->Minute == pTimer_OFF->Minute)return;
//  if(Device_Params_Info.RTCInfo.Week == 0)
//    week = 6;
//  else
//    week = Device_Params_Info.RTCInfo.Week - 1;
//  if( ((Device_Params_Info.EEPROM_Data.Timing_Week >> week ) & 0x01) == 0 )return;
  hour = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Hour);
  minute = Bsp_DWIN_BCD_Data(Device_Params_Info.RTCInfo.Minute);
  if( hour != pTimer_OFF->Hour){active_gethot[active_index] = 1;return;}
	if( minute != pTimer_OFF->Minute){active_gethot[active_index] = 1;return;}
  if(active_gethot[active_index] != 0)
  {
    active_gethot[active_index] = 0;
    Next_GetHot_Status = 0;
//    Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = 0;
  }
}

void Bsp_Func_Timer(void)
{
  uint8_t i;
  Bsp_Func_PutHotTimer_ON();
  Bsp_Func_PutHotTimer_OFF();
  for(i=0;i<3;i++)
  {
    Bsp_Func_GetHotTimer_ON(i);
    Bsp_Func_GetHotTimer_OFF(i);
  }
  if(Device_Params_Info.Special_Mode.HoldingTime <= 0)
  {
    if(Next_GetHot_Status != 0xFF)
    {
      Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = Next_GetHot_Status;
      Pre_GetHot_Status = Next_GetHot_Status;
      Next_GetHot_Status = 0xFF;
    }
    else if(Device_Params_Info.Special_Mode.HoldingTime == 0)
    {
      Device_Params_Info.Special_Mode.HoldingTime = -1;
      Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = Pre_GetHot_Status;
    }
  }
}

/*************************************************
 * @author: mark.zhu
 * @param:
 * @function:
*************************************************/
void Bsp_Func_Alarm_Process(void)
{
	uint16_t max_air;
	/*放热保护检测*/
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit == 0)
	{
		if( Device_Params_Info.RT_Water > Device_Params_Info.EEPROM_Data.TT_Water )
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit = 1;
	}
	else
	{
		if( Device_Params_Info.RT_Water < Device_Params_Info.EEPROM_Data.TT_Water )
    {
      if(Device_Params_Info.Debunce.cPutHotAlarm < 5)
        Device_Params_Info.Debunce.cPutHotAlarm ++;
      else
        Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit = 0;
    }
    else
      Device_Params_Info.Debunce.cPutHotAlarm = 0;
	}
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit == 0)
		 Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit = HAL_GPIO_ReadPin(ALARM_WATER_GPIO_Port,ALARM_WATER_Pin);
	
	//蓄热保护检测
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 2)
		max_air = Device_Params_Info.EEPROM_Data.MT_Air;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 1)
		max_air = 550;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 0)
		max_air = 350;
	
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit == 0)
	{
		if(Device_Params_Info.RT_Air > max_air)
			Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit = 1;
	}
	else
	{
		if( Device_Params_Info.RT_Air < max_air )
    {
      if(Device_Params_Info.Debunce.cGetHotAlarm < 5)
        Device_Params_Info.Debunce.cGetHotAlarm ++;
      else
        Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit = 0;
    }
    else
      Device_Params_Info.Debunce.cGetHotAlarm = 0;
	}
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit == 0)
		 Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit = HAL_GPIO_ReadPin(ALARM_HEAT_GPIO_Port,ALARM_HEAT_Pin);

  if(Device_Params_Info.Special_Mode.Enable)
  {
    if(Device_Params_Info.RT_Air <= SPECIAL_TEMP)
    {
      if(Device_Params_Info.Special_Mode.Debunce < 10)
        Device_Params_Info.Special_Mode.Debunce ++;
      else if(Device_Params_Info.Special_Mode.HoldingTime <= 0)
      {
        Device_Params_Info.Special_Mode.HoldingTime = 3600;
        Pre_GetHot_Status = Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit;
      }
    }
    else
    {
      Device_Params_Info.Special_Mode.Debunce = 0;
    }
  }
  else 
  {
    if(Device_Params_Info.RT_Air >= 2*SPECIAL_TEMP)
    {
      if(Device_Params_Info.Special_Mode.Debunce < 10)
          Device_Params_Info.Special_Mode.Debunce ++;
      else
      {
        Device_Params_Info.Special_Mode.Debunce = 0;
        Device_Params_Info.Special_Mode.Enable = 1;
      }
    }
    else
    {
      Device_Params_Info.Special_Mode.Debunce = 0;
    }
  }
}

void Bsp_Func_Special_Mode(void)
{
  if(Device_Params_Info.Special_Mode.HoldingTime > 0)
  {
    Device_Params_Info.Special_Mode.HoldingTime--;
    Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit = 1;
  }
}

void Bsp_Func_EEPROM_Update(void)
{
  if(Device_Params_Info.EEPROM_Update == 0)return;
  Device_Params_Info.EEPROM_Update = 0;
  EEPROM_Write((uint8_t*)&Device_Params_Info.EEPROM_Data.EEPROM_Key, 0, sizeof(struct EEPROM_Data_Tags));
}
