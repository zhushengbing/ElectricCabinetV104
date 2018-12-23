#ifndef __BSP_GUI_H__
#define __BSP_GUI_H__

#include "config.h"

struct PageGUI_Tags
{
	uint8_t Cursor;
	uint8_t PageID;
};

void Bsp_GUI_Update(void);
#endif
