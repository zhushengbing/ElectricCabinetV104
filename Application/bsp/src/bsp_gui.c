#include "bsp_dev.h"

void Bsp_GUI_Update(void)
{
	static uint16_t u16DWIN_Icon = 0;
	static uint16_t u16RT_OutWater = 0;
	static uint16_t u16RT_InWater = 0;
	static uint16_t u16RT_Air = 0;
	static uint16_t u16RT_TT = 0;
	
	DWIN_Params_Info.DWIN_Icon.Bit.AddKey_Bit = 1;
	DWIN_Params_Info.DWIN_Icon.Bit.DecKey_Bit = 1;
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit || Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit)
		DWIN_Params_Info.DWIN_Icon.Bit.TimmingKey_Bit = 1;
	else
		DWIN_Params_Info.DWIN_Icon.Bit.TimmingKey_Bit = 0;
	DWIN_Params_Info.DWIN_Icon.Bit.GetHotTiming_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotTiming_Bit;
	DWIN_Params_Info.DWIN_Icon.Bit.PutHotTiming_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHotTiming_Bit;
	DWIN_Params_Info.DWIN_Icon.Bit.PowerKey_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit;
	DWIN_Params_Info.DWIN_Icon.Bit.PutHotKey_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit;
	DWIN_Params_Info.DWIN_Icon.Bit.WaterAlarm_Bit = Device_Params_Info.AlarmPutHot;
	DWIN_Params_Info.DWIN_Icon.Bit.AirAlarm_Bit = Device_Params_Info.AlarmGetHot;
	if(Device_Params_Info.AlarmGetHot == 0 )
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotPicture_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit;
	else
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotPicture_Bit = 0;
	
	if(Device_Params_Info.AlarmPutHot == 0 && Device_Params_Info.PutHotActive)
		DWIN_Params_Info.DWIN_Icon.Bit.PutHotPicture_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit;
	else
		DWIN_Params_Info.DWIN_Icon.Bit.PutHotPicture_Bit = 0;
	
	DWIN_Params_Info.DWIN_Icon.All &= ~0x0380;
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 0)
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotLevel1Picture_Bit = 1;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 1)
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotLevel2Picture_Bit = 1;
	else if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit == 2)
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotLevel3Picture_Bit = 1;
	
	if(u16DWIN_Icon != DWIN_Params_Info.DWIN_Icon.All)
	{
		u16DWIN_Icon = DWIN_Params_Info.DWIN_Icon.All;
		Bsp_DWIN_WriteVariable(ICON_DEVICE_STATUS,&u16DWIN_Icon,2,1);
	}
	
	if(u16RT_OutWater != Device_Params_Info.RT_OutWater)
	{
		u16RT_OutWater = Device_Params_Info.RT_OutWater;
		Bsp_DWIN_WriteVariable(NUM_RTOUTWATER,&u16RT_OutWater,2,1);
	}
	
	if(u16RT_InWater != Device_Params_Info.RT_InWater)
	{
		u16RT_InWater = Device_Params_Info.RT_InWater;
		Bsp_DWIN_WriteVariable(NUM_RTINWATER,&u16RT_InWater,2,1);
	}
	
	if(u16RT_Air != Device_Params_Info.RT_Air)
	{
		u16RT_Air = Device_Params_Info.RT_Air;
		Bsp_DWIN_WriteVariable(NUM_RTAIR,&u16RT_Air,2,1);
	}
	
	if(u16RT_TT != Device_Params_Info.EEPROM_Data.TT_Water)
	{
		u16RT_TT = Device_Params_Info.EEPROM_Data.TT_Water;
		Bsp_DWIN_WriteVariable(NUM_TTWATER,&u16RT_TT,2,1);
	}
}
