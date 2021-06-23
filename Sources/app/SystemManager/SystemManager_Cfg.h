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


#ifndef SYSTEM_MANAGER_CFG_H
#define SYSTEM_MANAGER_CFG_H
#include "SystemManager.h"
#include "CommServ_if.h"
#include "DisplayManager_if.h"
#include "EolManager_if.h"
#include "AgingManager_if.h"

/* assuming we got a config table, we go through it until we see the delimiter */
/* register initial state callback function */
/*const APP_ACTIVE_TASK_FUNC gcfSysInitialHandlerTables[] = {
	NULL,
};*/

/* Register the function to execute before entering the bootloader mode */
/*const APP_ENTRY_TASK_FUNC gcfSysBootEntryTables[] = {
	NULL,
};*/

/* Register the function to execute before entering the standby state */
/*const APP_ENTRY_TASK_FUNC gcfSysStandbyEntryTables[] = {
	NULL,
};*/

/* Register a function executed in the active state of standby mode */
/*const APP_ACTIVE_TASK_FUNC gcfSysStandbyActiveTables[] = {
    NULL,
};*/

/* Register a function executed before exit the standby mode */
const APP_EXIT_TASK_FUNC gcfSysStandbyExitTables[] = {
    DisplayMgr_StandbyExitHandler,
};

/* Register the function to execute before entering the normal mode */
const APP_ENTRY_TASK_FUNC gcfSysNormalEntryTables[] = {
	AgingMgr_NormalEntryHandler,
	DisplayMgr_NormalEntryHandler,
	CommServ_NormalEntryHandler,
};

/* Register a function executed in the active state of normal mode */
const APP_ACTIVE_TASK_FUNC gcfSysNormalActiveTables[] = {
	AgingMgr_NormalActiveHandler,
    CommServ_NormalActiveHandler,
	DisplayMgr_NormalActiveHandler,
};

/* Register a function executed before exit the normal mode */
const APP_EXIT_TASK_FUNC gcfSysNormalExitTables[] = {
    DisplayMgr_NormalExitHandler,
};


/* Register the function to execute before entering the EOL mode */
const APP_ENTRY_TASK_FUNC gcfSysEOLEntryTables[] = {
	CommServ_EOLEntryHandler,
	DisplayMgr_EOLEntryHandler,
};

/* Register the function executed in the active state of EOL mode */
const APP_ACTIVE_TASK_FUNC gcfSysEOLActiveTables[] = {
    CommServ_EOLActiveHandler,
	EOLMgr_EOLActiveHandler,
	DisplayMgr_EOLActiveHandler,
};

/* Register a function executed before exit the EOL mode */
const APP_EXIT_TASK_FUNC gcfSysEOLExitTables[] = {
    NULL,
};

#endif

