/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : System manager
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/04
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2019/09/04  Huang Haoxiong      Initial
*************************************************************************************************************/


#ifndef SYSTEM_MANAGER_IF_H
#define SYSTEM_MANAGER_IF_H
#include "Define.h"

typedef MYBOOL (*const SYS_ENTRY_TASK_FUNC)(void);
typedef void (*const SYS_ACTIVE_TASK_FUNC)(void);
typedef MYBOOL (*const SYS_EXIT_TASK_FUNC)(void);


typedef void (*const APP_ENTRY_TASK_FUNC)(void);
typedef void (*const APP_ACTIVE_TASK_FUNC)(void);
typedef void (*const APP_EXIT_TASK_FUNC)(void);

typedef MYBOOL (*const APP_ENTER_SLEEP_TASK_FUNC)(void);




typedef enum{
    E_WORKING_STATE_ENTRY = 0,
    E_WORKING_STATE_ACTIVE,
    E_WORKING_STATE_EXIT,
}E_WORKING_STATE;//system working state

typedef enum{
    E_SYS_STATE_STANDBY = 0,
    E_SYS_STATE_NORMAL,
    E_SYS_STATE_EOL,
    E_SYS_STATE_PREPARE_TO_BOOT,
    E_SYS_STATE_MAX,
}E_SYS_STATE;//system state


extern void SystemMgr_Init(void);
extern void SystemMgr_CycleTask10ms(void);
extern void SystemMgr_UpdateState(void);
#endif


