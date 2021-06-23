/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : EOL manager
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/11/02
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2020/11/02  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef EOL_MANAGER_H
#define EOL_MANAGER_H
#include "EolManager_if.h"

#define COMM_EOL_SERIAL_NUM_LENGTH				(8U)			//length of serial number
#define COMM_EOL_IFC_DATA_LENGTH				(8U)			//length of IFC data length
#define COMM_EOL_DEVICE_DATA_BYTE				(7U)			//position of data section first byte

typedef enum{
	E_EOL_DEVICE_TYPE_PRODUCT = 0,
	E_EOL_DEVICE_TYPE_DISPLAY,
    E_EOL_DEVICE_TYPE_SLIDER,
    E_EOL_DEVICE_TYPE_TP,
    E_EOL_DEVICE_TYPE_TFT,
    E_EOL_DEVICE_TYPE_BACKLIGHT,
    E_EOL_DEVICE_TYPE_KEY,
}E_EOL_DEVICE_TYPE;//type of eol command


enum{
    E_EOL_PRODUCT_CMD_READ_EXT_VER = 0,        	//read external version
    E_EOL_PRODUCT_CMD_READ_INT_VER,            	//read internal version
    E_EOL_PRODUCT_CMD_WRITE_HW_VER,            	//write hardware version
    E_EOL_PRODUCT_CMD_READ_HW_VER,             	//read hardware version
    E_EOL_PRODUCT_CMD_READ_CHECKSUM,           	//read checksum
	E_EOL_PRODUCT_CMD_READ_ADC_VALUE,			//read adc vlaue	
	E_EOL_PRODUCT_CMD_QUICK_ENTER_SLEEP,		//quick enter sleep
	E_EOL_PRODUCT_CMD_SET_AGING_TIME,			//set aging time
	E_EOL_PRODUCT_CMD_READ_AGING_TIME, 			//read aging time
	E_EOL_PRODUCT_CMD_START_AGING, 				//start aging
	E_EOL_PRODUCT_CMD_STOP_AGING,				//stop aging
	E_EOL_PRODUCT_CMD_READ_AGING_STATE,			//read aging state
	E_EOL_PRODUCT_CMD_GET_IFC_TOTAL_QUANTITY,	//read ifc quantity
	E_EOL_PRODUCT_CMD_READ_IFC,					//read all ifc information
	E_EOL_PRODUCT_CMD_CLEAR_ALL_IFC,			//clear all ifc information
	E_EOL_PRODUCT_CMD_WRITE_SERIAL_NUMBER, 		//write serial-number
	E_EOL_PRODUCT_CMD_READ_SERIAL_NUMBER,		//read serial-number
	E_EOL_PRODUCT_CMD_WRITE_HW_INT_VER,			//write hardware internal version
	E_EOL_PRODUCT_CMD_READ_HW_INT_VER, 			//read hardware internal version
}E_EOL_PRODUCT_CMD;//type of product command

enum{
	E_EOL_DISPLAY_CMD_WRITE_VCOM = 0,          	//write vcom
    E_EOL_DISPLAY_CMD_READ_VCOM, 				//read vcom
    E_EOL_DISPLAY_CMD_WTITE_BRIGHTNESS,     	//write backlight brightness
    E_EOL_DISPLAY_CMD_READ_BRIGHTNESS,      	//read backlight brightness
    E_EOL_DISPLAY_CMD_READ_BL_NTC_VALUE,       	//read NTC value
	E_EOL_DISPLAY_CMD_WRITE_BL_RATIO,			//write brightness ratio		
	E_EOL_DISPLAY_CMD_READ_BL_RATIO,			//read brightness ratio 
}E_EOL_DISPLAY_CMD;//type of display command


#endif

