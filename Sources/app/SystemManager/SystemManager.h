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

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H
#include "SystemManager_if.h"
#include "Global_Definition.h"

#define SYSTEM_START_UP_FILTER_TIME         (10)          	//unit by 10ms

#define SYSTEM_PREPARE_SLEEP_COUNT          (500)         	//unit by 10ms
#define SYSTEM_CONFIRM_SLEEP_COUNT          (100)         	//unit by 10ms
#define SYSTEM_ENTER_EOL_COUNT				(50)			//unit by 10ms
typedef unsigned char           			SYS_ID_T;
typedef unsigned char           			MODULES_INDEX_T;

extern void SystemMgr_CyclicContainer_10ms(void);


#endif

