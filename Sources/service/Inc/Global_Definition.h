/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department :  ADS SW HZ                                            *
*   AUTHOR     :                                                       *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    Aug 08,2019   Huang Haoxiong    Initial version
*=====================================================================*/
#ifndef GLOBAL_DEFINITION_H
#define GLOBAL_DEFINITION_H
#include "Define.h"
#include "Tools.h"
#include "ver.h"
#include "SystemManager_if.h"
#include "Signal_if.h"
#include "DevCtrl_if.h"
#include "EepServ_if.h"
#include "CommServ_if.h"
#include "DiagServ_if.h"

/* -----------------------------------------------------------------------------
    Global Definition of macro
------------------------------------------------------------------------------*/
/*function macro switch */
//#define TOUCH_FUNCTION				//touch function switch
//#define BIST_FUNCTION				//bist function switch
//#define GRADIENT_FUNCTION			//gradient function switch
/*function macro switch */


#define SYS_CMD_REQUEST_TO_APP					0U
#define SYS_CMD_REQUEST_TO_EOL					1U
#define SYS_CMD_REQUEST_TO_BOOT					2U

#define SYS_ONE_CYCLE_TIME						(1)//10ms
#define SYS_CYCLE_500MS_TIME					(SYS_ONE_CYCLE_TIME * 50)//500ms
#define SYS_CYCLE_1S_TIME						(SYS_ONE_CYCLE_TIME * 100)//1s,unit by 10ms
#define SYS_CYCLE_2S_TIME						(SYS_ONE_CYCLE_TIME * 200)//2s,unit by 10ms
#define SYS_CYCLE_5S_TIME						(SYS_ONE_CYCLE_TIME * 500)//5s,unit by 10ms
#define SYS_CYCLE_10S_TIME						(SYS_ONE_CYCLE_TIME * 1000)//10s,unit by 10ms

/* information */
#define PRODUCTION_INFO							(0x01U)//desay
/* resolution */
#define HORIZONTAL_RESOLUTION					(1920U)
#define VERTICAL_RESOLUTION						(720U)
#define RESOLUTION_1920_720_SIZE				(0x01U)
#define RESOLUTION_1280_720_SIZE				(0x02U)
#define RESOLUTION_1920_1080_SIZE				(0x03U)
/* display size */
#define DISPLAY_8_INCH							(0x01U)
#define DISPLAY_10_25_INCH						(0x02U)
#define DISPLAY_10_4_INCH						(0x03U)
#define DISPLAY_12_3_INCH						(0x04U)
#define DISPLAY_DOUBLE_10_25_INCH				(0x05U)
#define DISPLAY_DOUBLE_12_3_INCH				(0x06U)
/* LVDS signal format */
#define LVDS_VESA_FORMAT						(0x01U)
#define LVDS_JEIDA_FORMAT						(0x02U)
/* display type */
#define CENTRAL_CONTROL_SCREEN					(0x01U)
#define DASHBOARD_SCREEN						(0x02U)
#define AIR_CONDITIONING_SCREEN					(0x03U)
#define HANDRAIL_SCREEN							(0x04U)
#define REAR_PILLOW_SCREEN						(0x05U)


/* brightness ratio limit */
#define BRIGHTNESS_RATIO_MAX  					(100U)
#define BRIGHTNESS_RATIO_MIN           			(70U)
#define BRIGHTNESS_RATIO_TYPE           		(90U)


//aging state
enum{
    E_AGING_STATE_STOP = 0,
    E_AGING_STATE_RUNNING,
    E_AGING_STATE_FINISHED,
    E_AGING_STATE_FINISHED_BY_CMD,
};
/* aging infomation */


typedef enum{
	E_BAT_STATE_LOW = 0,
	E_BAT_STATE_NORMAL,
	E_BAT_STATE_HIGH,
}E_BAT_STATE;//battery state

typedef enum{
	E_TEMPERATURE_STATE_NORMAL = 0,
	E_TEMPERATURE_STATE_CLOSE_HIGH,
	E_TEMPERATURE_STATE_HIGH,
}E_TEMPERATURE_STATE;//temperature state

/* check null pointer  */
#define CHECK_POINTER(_err_msg,_ptr,lbRet)						\
ISERR(_err_msg,(_ptr == NULL),lbRet)


/* Check whether the function is a null pointer, then the execution is not empty,otherwise set _ret to TRUE */
#define CHECK_FUNC_POINTER(_func,_ret)  						\
do{                                     						\
    if(_func == NULL){_ret = TRUE;}     						\
    else{_ret = _func();}               						\
}while(0)

/* Check whether the function is a null pointer, then the execution is not empty */
#define EXECUTE_FUNC_POINTER(_func)     						\
do{ 															\
	if(_func != NULL)											\
		{_func();}												\
}while(0)

#define ARRAY_LENGTH(Array)  (sizeof(Array)/sizeof((Array)[0]))//length of array


/* macro user eeprom address,use for 0~55 byte.
 * 56~255 use for IFC function,detail can see diagServ files.
 * eeprom minimum unit:2byte 
 */
#define EEP_ACCESS_KEY1_ADDR			0	//2byte
#define EEP_ACCESS_KEY2_ADDR			2	//2byte
#define EEP_CHECKSUM_ADDR				4	//2byte
#define EEP_MAIN_HW_ADDR				6	//2byte
#define EEP_INTERNAL_HW_ADDR			8	//2byte
#define EEP_FOG_VCOM_ADDR				10	//4byte
#define EEP_AGING_TIME_ADDR				14	//2byte
#define EEP_AGING_STATE_ADDR			16	//2byte
#define EEP_AGING_ENTRY_FLAG_ADDR		18	//2byte
#define EEP_SERIAL_NUMBER_ADDR			20	//8byte
#define EEP_BL_CONFIG_FLAG_ADDR			28	//2byte
#define EEP_BRIGHTNESS_RATIO_ADDR		30	//2byte

#endif  // GLOBAL_DEFINITION_H


