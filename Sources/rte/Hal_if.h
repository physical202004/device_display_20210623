/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
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
 ----------------------------------------------------------------------
* 0.1    Jun 26,2016                        Initial version
 ----------------------------------------------------------------------
* 0.2    Jan 4,2017       ShangQiuju   update HAL_DevGetPowerStatus,
                                                        HALBlock_DevGetPowerStatus
*------------------------------------------------------------------------
* 0.3   Jan 24,2017  ZhangJie  Fix Coverity warnings.
*------------------------------------------------------------------------
* 0.4	Dec 19,2018		Huang,Haoxiong	improve HAL structure
*=====================================================================*/
#ifndef HAL_IF_H
#define HAL_IF_H

#include "Define.h"

/**** Definition of constants ****/


typedef enum{
	E_HAL_STATE_OK = 0,						//OK
	E_HAL_STATE_FAIL,						//Operation failed
	E_HAL_STATE_BUSY,						//Device is occupied
	E_HAL_STATE_FAULT,						//Fault is detected
	E_HAL_STATE_RANGE,						//Parameter out of range
	E_HAL_STATE_TIMEOUT,					//Requested is timeout
	E_HAL_STATE_INVALID_PARA,				//Incorrect parameter provided
	E_HAL_STATE_INACTIVE,					//Device in the inactive mode
	E_HAL_STATE_WAITING,					//The device is processing, please waiting
	E_HAL_STATE_NO_FUNC,					//The function for device isn't existed
}E_HAL_STATE;

typedef  void (* V_hal_Init)(void);
typedef  void (* V_hal_DeInit)(void);
typedef  MYBOOL  (* V_hal_open)(void);
typedef  MYBOOL (* V_hal_close)(void);
typedef  E_HAL_STATE (* V_hal_read)(U8* buffer, U16 Len);
typedef  E_HAL_STATE (* V_hal_write)(U8* buffer, U16 Len);
typedef  E_HAL_STATE (* V_hal_iocontrol)(U16 type, void* buffer);

typedef  void (* V_hal_block_Init)(void);
typedef  void (* V_hal_block_DeInit)(void);
typedef  MYBOOL  (* V_hal_block_open)(void);
typedef  MYBOOL  (* V_hal_block_close)(void);
typedef  E_HAL_STATE (* V_hal_block_read)(U32 address,U8* buffer, U16 Len);
typedef  E_HAL_STATE (* V_hal_block_write)(U32 address,U8* buffer, U16 Len);
typedef  E_HAL_STATE (* V_hal_block_iocontrol)(U16 type, void* buffer);

typedef struct{
    V_hal_Init              hal_Init;
    V_hal_DeInit            hal_DeInit;
    V_hal_open              hal_open;
    V_hal_close             hal_close;
    V_hal_read              hal_read;
    V_hal_write             hal_write;
    V_hal_iocontrol         hal_iocontrol;
}cdev;

typedef struct{
    V_hal_block_Init        hal_block_Init;
    V_hal_block_DeInit      hal_block_DeInit;
    V_hal_block_open        hal_block_open;
    V_hal_block_close       hal_block_close;
    V_hal_block_read        hal_block_read;
    V_hal_block_write       hal_block_write;
    V_hal_block_iocontrol   hal_block_iocontrol;
}bdev;

/**** Definition of macros ****/

/* define device controller id */
#define DEVICE_CONTROLLER_ID					DEVICE_CONTROLLER

/* define all devices id */
#define COMM_DEVICE_ID							COMM_DRIVER_ID
#define BACKLIGHT_DEVICE_ID 					LP8860_DRIVER_ID
#define FOG_DEVICE_ID							FOG_DRIVER_ID
#define LVDS_DEVICE_ID							DS90UB948_DRIVER_ID
#define TP_DEVICE_ID							SYNAPTICS_TP_DRIVER_ID

#define HAL_PASS_1
#include "Hal_Def.h"

#define HAL_PASS_2
#include "Hal_Def.h"

extern void RegisterHalDev(void);
/* Macro definitions*/
extern void HALChar_Init(U8 DeviceId);
extern void HALChar_DeInit(U8 DeviceId);
extern MYBOOL HALChar_Open(U8 DeviceId);
extern MYBOOL HALChar_Close(U8 DeviceId);
extern E_HAL_STATE HALChar_Read(U8 DeviceId, U8* buffer, U16 Len);
extern E_HAL_STATE HALChar_Write(U8 DeviceId, U8* buffer, U16 Len);
extern E_HAL_STATE HALChar_IoControl(U8 DeviceId, U16 type, void* buffer);

extern void HALBlock_Init(U8 DeviceId);
extern void HALBlock_DeInit(U8 DeviceId);
extern MYBOOL HALBlock_Open(U8 DeviceId);
extern MYBOOL HALBlock_Close(U8 DeviceId);
extern E_HAL_STATE HALBlock_Read(U8 DeviceId, U32 address,U8* buffer, U16 Len);
extern E_HAL_STATE HALBlock_Write(U8 DeviceId, U32 address,U8* buffer, U16 Len);
extern E_HAL_STATE HALBlock_IoControl(U8 DeviceId, U16 type, void* buffer);

/**** Declaration of constants ****/
/**** Declaration of variables ****/

/**** Declaration of functions ****/



#endif /*HAL_IF_H*/

/****************************** END OF FILE ***************************/

