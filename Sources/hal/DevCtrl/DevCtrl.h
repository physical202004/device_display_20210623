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

#ifndef DEVCTRL_H
#define DEVCTRL_H
#include "DevCtrl_if.h"
#include "DevCtrl_Tree.h"
#include "DevCtrl_Sm.h"
#include "Timer_Adapt_if.h"
#include "Exti_Adapt_if.h"
#include "Mcu_if.h"

#ifdef USE_CAPTURE_FR_HU_FUNC
#include "Capture_Adapt_if.h"
#endif

typedef struct
{
    DEV_ID_T id;
    DEV_POWER_SQUENCE_T index;
}S_POWER_SEQ_INFO;

/* power manager */
#define DEV_POWER_PASS_1
#include "DevCtrl_Def.h"

/* ntc */
#define DEV_NTC_PASS_1
#include "DevCtrl_Ntc_Def.h"

#define DEV_NTC_PASS_2
#include "DevCtrl_Ntc_Def.h"

#define DEV_NTC_PASS_3
#include "DevCtrl_Ntc_Def.h"

#define DEV_NTC_SHORT_VALUE							(100)			//open threshold value
#define DEV_NTC_OPEN_VALUE							(4000)			//short threshold value
#define DEV_NTC_RECORD_OPENSHORT_TIME				(3000)			//30s,unit by 10ms
#define DEV_VOLTAGE_DISCHARGE_THRESHOLD				(559)			//discharge thershold value

#define DEV_CONFIRM_SLEEP_TIME						(500)			//5s,unit by 10ms

#define DEV_WAKEUP_INACTIVE							(LOW_LEVEL)		//wakeup inactive

extern int DevCtrl_CmpId(const void *id,S_DEV_INFO *dev2);
extern int DevCtrl_CmpName(const void *name,S_DEV_INFO *dev2);
extern void DevCtrl_SM_ExecPowerOffHandler(S_DEV_INFO *dev);
extern void DevCtrl_SM_ExecPowerOnHandler(S_DEV_INFO *dev);
extern void DevCtrl_SM_UpdateDevState(S_DEV_INFO *dev,E_DEVCTRL_STATE state);
extern MYBOOL DevCtrl_SM_DevIsAllowPowerOn(S_DEV_INFO* dev);
extern void DevCtrl_Hardware_TaskEntry_10ms(void);
extern MYBOOL DevCtrl_DetectAllPowerVoltage(void);
extern void DevCtrl_ControlDcDcEnable(U8 level);
#endif


