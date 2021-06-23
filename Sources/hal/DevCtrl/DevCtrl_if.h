/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : device controller
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/03/08  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef DEVCTRL_IF_H
#define DEVCTRL_IF_H
#include "DevCtrl_Io.h"
#include "DevCtrl_Adc.h"
#include "DevCtrl_Sm.h"
#include "Comm_Driver_if.h"
#include "FOG_Driver_if.h"
#include "LP8860_Driver_if.h"
#include "DS90UB948_Driver_if.h"
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#include "SynapticsTP_Driver_if.h"
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif
#include "Hal_if.h"

#define DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"

#define DEV_INVALID_VALUE              			(0xFFU)		//invalid value

/*************** macro hardware function ***************/
//#define USE_WAKEUP_SIGNAL
#define DC_DC_CONTROL
#define USE_CAPTURE_FR_HU_FUNC
#define USE_ADC_INTERRUPT_DETECT
/*************** macro hardware function ***************/

/*************** touch information ***************/
#define DEV_TP_NUM_MAX							(5U)		//touch number
#define DEV_TP_ACTION_PRESS						(0xC0U)
#define DEV_TP_ACTION_MOVE						(0x90U)
#define DEV_TP_ACTION_RELEASE					(0xA0U)
#define DEV_TP_DATA_SIZE						(6U)

typedef enum
{
	E_DEV_TP_TEST_NONE = 0,
	E_DEV_TP_OPEN_TEST,
	E_DEV_TP_SHORT_TEST,
	E_DEV_TP_WAIT_TEST_RESULT,
	E_DEV_TP_SELF_TEST_COMPLETE
}E_TP_CAT_MODE_TYPE;

typedef enum
{
	E_DEV_TP_UPGRADE_IDLE = 0,
	E_DEV_TP_UPGRADE_PREPARE,
	E_DEV_TP_UPGRADE_PREPARE_DONE,
	E_DEV_TP_UPGRADE_RAWDATA,
	E_DEV_TP_UPGRADE_RAWDATA_DONE,
	E_DEV_TP_UPGRADE_VERIFY_CHECKSUM,
	E_DEV_TP_UPGRADE_VERIFY_CHECKSUM_DONE
}E_TP_UPGRADE_STATE;
	
typedef enum{
	E_TP_UPGRADE_REPLY_IDLE = 0,
	E_TP_UPGRADE_REPLY_PREPARE_DONE,
	E_TP_UPGRADE_REPLY_ROWDATA_DONE,
	E_TP_UPGRADE_REPLY_UPGRADE_SUCCESS,
	E_TP_UPGRADE_REPLY_PROCESSING,
	E_TP_UPGRADE_REPLY_REQUEST_DATA,
	E_TP_UPGRADE_REPLY_FAILED
}E_TP_UPGRADE_REPLY;

typedef struct{
	U8  recvDataLen ;							//Upgrade recv data lenth
	MYBOOL recvDataFlag;						//if recv data from APP,flag is TRUE
	U8* recvData;								//Upgrade recv data buffer
	E_TP_UPGRADE_STATE TPUpgradestate;			//Upgrade mode state
	E_TP_UPGRADE_REPLY ProcessReply;			//relay current state to APP
}S_DEV_TPUPGRADE_INFO;

typedef union
{
	struct
	{	
		U8 id;
		U8 action;
		U8 xposH;
		U8 xposL;
		U8 yposH;
		U8 yposL;
	};
	U8 data[DEV_TP_DATA_SIZE];
}U_DEV_TP_DATA;

typedef struct
{
	U8 reservedBits : 6;
	U8 intUpdate : 1;
	U8 extUpdate : 1;
	U_DEV_TP_DATA touch;
}S_DEV_TP_INFO;
/*
enum{
	E_DEV_TP_INFO_ID = 0,
	E_DEV_TP_INFO_ACTION,
	E_DEV_TP_INFO_X_H,
	E_DEV_TP_INFO_X_L,
	E_DEV_TP_INFO_Y_H,
	E_DEV_TP_INFO_Y_L,
	E_DEV_TP_INFO_MAX,
};*/

/************** touch information ***************/

/*************** backlight information ***************/
#define DEV_BL_BRN_MIN					(0U)//minimum brightness
#define DEV_BL_BRN_MAX					(0xFFFFU)//maximum brightness
typedef unsigned short DEV_BRN_16BIT_T;
typedef unsigned char DEV_TEMPERATURE_8BIT_T;

#define NTC_TEMPERATURE_INVALID_STATE	(0xFFU)
/*************** backlight information ***************/


/* print message with device name */
#define PRINT_DEV_MSG(_name,_msg)               \
	TraceServ_SendDStrMsg(_name,_msg, E_TRACE_EVENT_GENERAL);

/* print message with device id */
#define PRINT_DEV_ID_MSG(_id,_msg)              \
	TraceServ_SendStrWithHexMsg(_msg,(U8 *)&_id,1u,E_TRACE_EVENT_GENERAL);

/* print error message with device name */
#define IS_DEV_ERR(_condition,_name,_msg,_ret)  \
do{                                             \
    if(_condition)                              \
    {                                           \
        PRINT_DEV_MSG(_name,_msg);              \
        _ret;                                   \
    }                                           \
    else										\
    {											\
		/* do nothing... */						\
    }											\
}while(0)

typedef void (* CYCLIC_TASK_10MS)(void);

typedef char DEV_NAME_T;
typedef unsigned char DEV_NUM_T;

typedef enum{

    E_DEVCTRL_STATE_CLOSE = 0,
	E_DEVCTRL_STATE_CLOSING,
	E_DEVCTRL_STATE_OPEN,
	E_DEVCTRL_STATE_OPENING,
}E_DEVCTRL_STATE;

typedef enum{
	E_DEVCTRL_POWER_REQ_NONE = 0,
	E_DEVCTRL_POWER_REQ_ON,
	E_DEVCTRL_POWER_REQ_OFF,
	//E_DEVCTRL_POWER_REQ_RESTART,
	E_DEVCTRL_POWER_REQ_RELEASE,
}E_DEVCTRL_POWER_REQ;

typedef enum{
	E_DEVCTRL_FAULT_STATE_NONE = 0,							//Normal,no fault happen
	E_DEVCTRL_FAULT_STATE_MARK,								//It set from fault pin
	E_DEVCTRL_FAULT_STATE_POWER,							//It set from power supply voltage
	E_DEVCTRL_FAULT_STATE_FATAL,							//Unrecoverable fault happen
}E_DEVCTRL_FAULT_STATE;

typedef struct dev_info
{
    const DEV_NAME_T            *name;						//Device name
    const DEV_ID_T              id;							//Device id
	E_DEVCTRL_FAULT_STATE		faultState;					//Device error status
	E_DEVCTRL_STATE             devState;					//Device status
    E_DEVCTRL_POWER_REQ			powerReq;					//request to powering on/off
	const S_DEV_POWER_CTRL      *powerCtrl;					//Register the functions that the device needs to perform when power is switched
    const CYCLIC_TASK_10MS      running10msTask;			//Task in running status
    struct dev_info             *next;						//Point to the address of the next device
}S_DEV_INFO;


extern MYBOOL DevCtrl_Register_Dev(S_DEV_INFO *dev);
extern MYBOOL DevCtrl_Unregister_Dev(S_DEV_INFO *dev);
extern void DevCtrl_CycleTask10ms(void);
extern void DevCtrl_UpdateAllDevsState(void);

/* header file must be placed here */
#include "TraceServ_if.h"


#endif


