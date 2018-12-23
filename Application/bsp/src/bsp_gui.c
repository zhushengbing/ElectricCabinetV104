#include "bsp_dev.h"

void Bsp_GUI_Update(void)
{
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
	DWIN_Params_Info.DWIN_Icon.Bit.WaterAlarm_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit;
	DWIN_Params_Info.DWIN_Icon.Bit.AirAlarm_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit;
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmGetHot_Bit == 0 )
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotPicture_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHot_Bit;
	else
		DWIN_Params_Info.DWIN_Icon.Bit.GetHotPicture_Bit = 0;
	
	if(Device_Params_Info.EEPROM_Data.Device_Status.Bit.AlarmPutHot_Bit == 0 && Device_Params_Info.PutHotActive)
		DWIN_Params_Info.DWIN_Icon.Bit.PutHotPicture_Bit = Device_Params_Info.EEPROM_Data.Device_Status.Bit.PutHot_Bit;
	else
		DWIN_Params_Info.DWIN_Icon.Bit.PutHotPicture_Bit = 0;
	
	DWIN_Params_Info.DWIN_Icon.Bit.GetHotLevelPicture_Bit = 1 << Device_Params_Info.EEPROM_Data.Device_Status.Bit.GetHotLevel_Bit;
	
	Bsp_DWIN_WriteVariable(ICON_DEVICE_STATUS,&DWIN_Params_Info.DWIN_Icon.All,2,1);
	Bsp_DWIN_WriteVariable(NUM_RTWATER,&Device_Params_Info.RT_Water,2,1);
	Bsp_DWIN_WriteVariable(NUM_RTAIR,&Device_Params_Info.RT_Air,2,1);
	Bsp_DWIN_WriteVariable(NUM_TTWATER,&Device_Params_Info.EEPROM_Data.TT_Water,2,1);
}
