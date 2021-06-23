/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : device power control
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

#ifndef DEVCTRL_SM_H
#define DEVCTRL_SM_H
#include "Define.h"

typedef MYBOOL (* SEQUENCE_ACTION_FUNC)(void);

typedef unsigned char DEV_POWER_SQUENCE_T;

typedef unsigned char DEV_ID_T;

typedef struct
{
    SEQUENCE_ACTION_FUNC powerOnFunc;
    SEQUENCE_ACTION_FUNC shutDownFunc;
	SEQUENCE_ACTION_FUNC lowPowerFunc;
}S_DEV_POWER_CTRL;

typedef enum{
    E_DEVCTRL_SM_STATE_POWERON = 0,
    E_DEVCTRL_SM_STATE_SHUTDOWN,
    E_DEVCTRL_SM_STATE_MAX,
}E_DEVCTRL_SM_STATE;

#define PM_SEQUENCE_TIMEOUT_IN_STAGE				1000	//timeout in power sequence stage,unit by 10ms

#define BAT_AD_THRESHOLD_LOW_TO_NORMAL				1055	//7.5V
#define BAT_AD_THRESHOLD_NORMAL_TO_LOW         		906		//6.5V
#define BAT_AD_THRESHOLD_NORMAL_TO_HIGH             2830	//19.5V
#define BAT_AD_THRESHOLD_HIGH_TO_NORMAL             2681	//18.5V

extern void DevCtrl_SM_Init(void);
extern void DevCtrl_SM_BatteryStateMachine(void);
extern void DevCtrl_SM_TaskEntry_10ms(void);
extern MYBOOL DevCtrl_SM_RequestUpdateState(E_DEVCTRL_SM_STATE nextState);
extern E_DEVCTRL_SM_STATE DevCtrl_SM_GetCurrState(void);
extern E_DEVCTRL_SM_STATE DevCtrl_SM_GetNextState(void);


#endif


