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
#include "SystemManager.h"
#include "SystemManager_Cfg.h"
#include "Mcu_if.h"

/* entry functions */
static MYBOOL SystemMgr_BootEntryHandler(void);
static MYBOOL SystemMgr_StandbyEntryHandler(void);
static MYBOOL SystemMgr_NormalEntryHandler(void);
static MYBOOL SystemMgr_EOLEntryHandler(void);

/* active functions */
static void SystemMgr_BootActiveHandler(void);
static void SystemMgr_StandbyActiveHandler(void);
static void SystemMgr_NormalActiveHandler(void);
static void SystemMgr_EOLActiveHandler(void);

/* exit functions */
static MYBOOL SystemMgr_StandbyExitHandler(void);
static MYBOOL SystemMgr_NormalExitHandler(void);
static MYBOOL SystemMgr_EOLExitHandler(void);

//static void SystemMgr_PrepareSleepHandler(void);
//static void SystemMgr_GotoSleepHandler(void);
static void SystemMgr_StatrupHandler(void);
static void SystemMgr_ChangeToNextState(E_SYS_STATE state);
static void SystemMgr_ChangeState(E_SYS_STATE state);
static void SystemMgr_ChangeWorkingState(E_WORKING_STATE state);
static void SystemMgr_EntryHandler(E_SYS_STATE state);
static void SystemMgr_ActiveHandler(E_SYS_STATE state);
static void SystemMgr_ExitHandler(E_SYS_STATE state);


static E_SYS_STATE seSysCurrState = E_SYS_STATE_STANDBY;//current system state
static E_SYS_STATE seSysNextState = E_SYS_STATE_STANDBY;//next system state
static E_WORKING_STATE seSysWorkState = E_WORKING_STATE_ENTRY;//working state in the current state
//static U16 suhwSysSleepCnt = 0u;//Count value associated with sleep
static U16 suhwSysStartupCnt = 0u;//Used to wait for hardware stability
//static MYBOOL smbSysSleepLock = FALSE;//Lock sleep process, wakeup signal loss timeout
static MYBOOL smbStartupFlag = TRUE;//Check if the first startup,only be used once

/* entry function tables */
static const SYS_ENTRY_TASK_FUNC scfSysEntryFuncTables[E_SYS_STATE_MAX] = {
	SystemMgr_StandbyEntryHandler,//standby
	SystemMgr_NormalEntryHandler,//running
	SystemMgr_EOLEntryHandler,//eol
	SystemMgr_BootEntryHandler,//prepare to bootloader
};

/* active function tables */
static const SYS_ACTIVE_TASK_FUNC scfSysActiveFuncTables[E_SYS_STATE_MAX] = {
	SystemMgr_StandbyActiveHandler,//standby
	SystemMgr_NormalActiveHandler,//running
	SystemMgr_EOLActiveHandler,//eol
	SystemMgr_BootActiveHandler,//prepare to bootloader

};

/* exit function tables */
static const SYS_EXIT_TASK_FUNC scfSysExitFuncTables[E_SYS_STATE_MAX] = {
	SystemMgr_StandbyExitHandler,//standby
	SystemMgr_NormalExitHandler,//running
	SystemMgr_EOLExitHandler,//eol
	NULL,//prepare to bootloader
};

/***********************************************************************
* Name          : SystemMgr_Init
* Description   : Initialize the task scheduler
* Parameter     : None
* Returns       : None
***********************************************************************/
void SystemMgr_Init(void)
{
	//MODULES_INDEX_T ltIndex = 0U;

	/*register signal and hal device */
	CS_SIG_RegisterSignal();
    RegisterHalDev();
	
	EepServ_Init();//initial EepData
	DiagServ_Init();//initial diagnostic
	TraceServ_Init();//initial trace

	/* initial all app modules in register */
	/*for(ltIndex = 0U;ltIndex < ARRAY_LENGTH(gcfSysInitialHandlerTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysInitialHandlerTables[ltIndex]);
    }*/	
}

/***********************************************************************
* Name          : SystemMgr_BootActiveHandler
* Description   : Normal handler in the boot mode
* Parameter     : None
* Returns       : None
***********************************************************************/
static void SystemMgr_BootActiveHandler(void)
{
	(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_SOFT_RESET,NULL);//execute soft reset
	while(1);//if software can not use,waiting for watchdog reset
}

/***********************************************************************
* Name          : SystemMgr_BootEntryHandler
* Description   : Entry handler in the prepare to boot mode
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_BootEntryHandler(void)
{
	//MODULES_INDEX_T ltIndex = 0u;

	/* execute functions */
	/*for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysBootEntryTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysBootEntryTables[ltIndex]);
    }*/

    return (TRUE);
}

/***********************************************************************
* Name          : SystemMgr_PrepareSleepHandler
* Description   : Goto sleep handler
* Parameter     : None
* Returns       : None
***********************************************************************/
#if 0
static void SystemMgr_PrepareSleepHandler(void)
{
	if(smbSysSleepLock == FALSE)
	{
	    if(suhwSysSleepCnt >= SYSTEM_PREPARE_SLEEP_COUNT)
	    {
	    	smbSysSleepLock = TRUE;
	    	suhwSysSleepCnt = 0u;
			/* save ifc */
			DiagServ_SaveIfcToFlash();
	    }
		else
		{
	    	suhwSysSleepCnt++;
		}
	}
	else    //lock current state,then wait for SYSTEM_CONFIRM_SLEEP_COUNT time to goto sleep mode
	{
		SystemMgr_GotoSleepHandler();
	}
}
#endif
/***********************************************************************
* Name          : SystemMgr_GotoSleepHandler
* Description   : Goto sleep handler
* Parameter     : None
* Returns       : None
***********************************************************************/
#if 0
static void SystemMgr_GotoSleepHandler(void)
{
    if(suhwSysSleepCnt >= SYSTEM_CONFIRM_SLEEP_COUNT)//waiting to save ifc
    {
        suhwSysSleepCnt = 0u;
        PRINT_IMPORTANT_MSG("system goto sleep");
		DelayMs(5);
		/* go to sleep */
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEEP_SLEEP,NULL);
    }
	else
	{
    	suhwSysSleepCnt++;
	}

}
#endif



/***********************************************************************
* Name          : SystemMgr_StatrupHandler
* Description   : Startup handler
* Parameter     : None
* Returns       : None
***********************************************************************/
static void SystemMgr_StatrupHandler(void)
{
	E_HAL_STATE leHalState = E_HAL_STATE_FAIL;

	leHalState = HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_CHECK_VOLT_DISCHARGE,NULL);//check power discharge state for devices
	if((leHalState == E_HAL_STATE_OK) || (suhwSysStartupCnt > SYSTEM_START_UP_FILTER_TIME))
	{
		smbStartupFlag = FALSE;//startup completed
	}
	else
	{
		suhwSysStartupCnt++;//waiting
	}
}


/***********************************************************************
* Name          : SystemMgr_StandbyActiveHandler
* Description   : Normal handler in standby mode
* Parameter     : None
* Returns       : None
***********************************************************************/
static void SystemMgr_StandbyActiveHandler(void)
{
	E_BAT_STATE leBatteryState = E_BAT_STATE_LOW;
    MYBOOL lmbWakeupState = FALSE;
	U8 lubSysReqChange = SYS_CMD_REQUEST_TO_APP;
	//MODULES_INDEX_T ltIndex = 0u;
	
	CS_SIG_SyncReceive(SIG_BOOL_WAKEUP_IS_ACTIVE,&lmbWakeupState);
	if(FALSE == lmbWakeupState) 	  //wakeup signal active
	{
		//SystemMgr_PrepareSleepHandler();
	}
	else
	{
		//smbSysSleepLock = FALSE;
        //suhwSysSleepCnt = 0u;
		if(smbStartupFlag == TRUE)
		{
			SystemMgr_StatrupHandler();
		}
		else
		{
			CS_SIG_SyncReceive(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
			CS_SIG_SyncReceive(SIG_ENUM_BATTERY_STATE,&leBatteryState);
			if(E_BAT_STATE_NORMAL == leBatteryState)
			{
				if(lubSysReqChange == SYS_CMD_REQUEST_TO_BOOT)
				{
					SystemMgr_ChangeToNextState(E_SYS_STATE_PREPARE_TO_BOOT);
				}
				else
				{
					SystemMgr_ChangeToNextState(E_SYS_STATE_NORMAL);
				}
			}
			else
			{
				/* execute functions */
				/*for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysStandbyActiveTables);ltIndex++)
				{
					EXECUTE_FUNC_POINTER(gcfSysStandbyActiveTables[ltIndex]);
				}*/
			}
		}
	}
}


/***********************************************************************
* Name          : SystemMgr_StandbyEntryHandler
* Description   : Entry handler in the standby mode
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_StandbyEntryHandler(void)
{
	//MODULES_INDEX_T ltIndex = 0u;
	/* execute functions */
	/*for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysStandbyEntryTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysStandbyEntryTables[ltIndex]);
    }*/

    return (TRUE);
}

/***********************************************************************
* Name          : SystemMgr_StandbyExitHandler
* Description   : Exit handler in the standby mode
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_StandbyExitHandler(void)
{
	MODULES_INDEX_T ltIndex = 0u;

	/* execute functions */
	for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysStandbyExitTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysStandbyExitTables[ltIndex]);
    }
	
	return (TRUE);
}


/***********************************************************************
* Name          : SystemMgr_NormalActiveHandler
* Description   : active handler in the normal mode
* Parameter     : None
* Returns       : None
***********************************************************************/
static void SystemMgr_NormalActiveHandler(void)
{

	E_BAT_STATE leBatteryState = E_BAT_STATE_LOW;
    MYBOOL lmbWakeupState = FALSE;
	MYBOOL lmbAdcDetectState = FALSE;
    MODULES_INDEX_T ltIndex = 0u;
	U8 lubSysReqChange = SYS_CMD_REQUEST_TO_APP;
	
	CS_SIG_SyncReceive(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
    CS_SIG_SyncReceive(SIG_BOOL_WAKEUP_IS_ACTIVE,&lmbWakeupState);
	CS_SIG_SyncReceive(SIG_ENUM_BATTERY_STATE,&leBatteryState);
	CS_SIG_SyncReceive(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);
	
    if((lmbWakeupState == FALSE) || (leBatteryState == E_BAT_STATE_LOW) || (leBatteryState == E_BAT_STATE_HIGH)\
		|| (lubSysReqChange == SYS_CMD_REQUEST_TO_BOOT) || (lmbAdcDetectState == TRUE))
    {
    	SystemMgr_ChangeToNextState(E_SYS_STATE_STANDBY);
    }
	else if(lubSysReqChange == SYS_CMD_REQUEST_TO_EOL)
	{
		SystemMgr_ChangeToNextState(E_SYS_STATE_EOL);
	}
    else
    {
		/* execute functions */
        for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysNormalActiveTables);ltIndex++)
        {
            EXECUTE_FUNC_POINTER(gcfSysNormalActiveTables[ltIndex]);
        }
    }
}


/***********************************************************************
* Name          : SystemMgr_NormalEntryHandler
* Description   : Entry handle in the running state
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_NormalEntryHandler(void)
{
	E_DEVCTRL_SM_STATE leRequestPMState = E_DEVCTRL_SM_STATE_POWERON;//power on
	MYBOOL lmbRet = FALSE;
	MODULES_INDEX_T ltIndex = 0u;
	
	/* execute functions */
	for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysNormalEntryTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysNormalEntryTables[ltIndex]);
    }
	
	//execute power-on sequence
	if(HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQUEST_CHANGE_PM_STATE,&leRequestPMState) == E_HAL_STATE_OK)
	{
		
		lmbRet = TRUE;
	}
	else
	{
		/* waiting...*/
	}

    return (lmbRet);
}

/***********************************************************************
* Name          : SystemMgr_NormalExitHandler
* Description   : Exit handler in the normal mode
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_NormalExitHandler(void)
{
	E_DEVCTRL_SM_STATE leRequestPMState = E_DEVCTRL_SM_STATE_SHUTDOWN;
	MYBOOL lmbRet = FALSE;
	MODULES_INDEX_T ltIndex = 0u;

	/* execute functions */
	for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysNormalExitTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysNormalExitTables[ltIndex]);
    }
	
	if(seSysNextState == E_SYS_STATE_STANDBY)
	{
		/*execute power-down sequence */
		if(HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQUEST_CHANGE_PM_STATE,&leRequestPMState) == E_HAL_STATE_OK)
		{
			lmbRet = TRUE;
		}
		else
		{
			/* waiting...*/
		}
	}
	else
	{
		lmbRet = TRUE;
	}
	return (lmbRet);
}

/***********************************************************************
* Name          : SystemMgr_NormalExitHandler
* Description   : Normal handler in the eol mode
* Parameter     : None
* Returns       : None
***********************************************************************/
static void SystemMgr_EOLActiveHandler(void)
{
    MODULES_INDEX_T ltIndex = 0u;
	U8 lubSysReqChange = SYS_CMD_REQUEST_TO_APP;
	
	CS_SIG_SyncReceive(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
	if(lubSysReqChange == SYS_CMD_REQUEST_TO_EOL)
	{
	    for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysEOLActiveTables);ltIndex++)
	    {
	        EXECUTE_FUNC_POINTER(gcfSysEOLActiveTables[ltIndex]);
	    }
	}
	else
	{
		SystemMgr_ChangeToNextState(E_SYS_STATE_NORMAL);
	}
}

/***********************************************************************
* Name          : SystemMgr_EOLEntryHandler
* Description   : Entry process in the state of EOL
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_EOLEntryHandler(void)
{

	MODULES_INDEX_T ltIndex = 0u;
	
	/* execute functions */
	for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysEOLEntryTables);ltIndex++)
	{
	    EXECUTE_FUNC_POINTER(gcfSysEOLEntryTables[ltIndex]);
	}

    return (TRUE);
}

/***********************************************************************
* Name          : SystemMgr_EOLExitHandler
* Description   : Exit handler in the EOL mode
* Parameter     : None
* Returns       : TRUE:complete,FALSE:There are still tasks that have not been completed
***********************************************************************/
static MYBOOL SystemMgr_EOLExitHandler(void)
{
	MODULES_INDEX_T ltIndex = 0u;

	/* execute functions */
	for(ltIndex = 0u;ltIndex < ARRAY_LENGTH(gcfSysEOLExitTables);ltIndex++)
    {
        EXECUTE_FUNC_POINTER(gcfSysEOLExitTables[ltIndex]);
    }
	return (TRUE);
}

/***********************************************************************
* Name          : SystemMgr_ChangeToNextState
* Description   : Change to Next State
* Parameter     : state:next system state
* Returns       : None
***********************************************************************/
static void SystemMgr_ChangeToNextState(E_SYS_STATE state)
{
    if((state >= E_SYS_STATE_MAX) || (state == seSysNextState))    //invalid
    {
        /*do nothing...*/
    }
	else
	{
    	seSysNextState = state;
	}
}


/***********************************************************************
* Name          : SystemMgr_ChangeState
* Description   : Change to new state
* Parameter     : state:set state
* Returns       : None
***********************************************************************/
static void SystemMgr_ChangeState(E_SYS_STATE state)
{
    if(state >= E_SYS_STATE_MAX)
    {
        /*do nothing...*/
    }
	else
	{
    	seSysCurrState = state;
		SystemMgr_ChangeWorkingState(E_WORKING_STATE_ENTRY);
	}
}

/***********************************************************************
* Name          : SystemMgr_ChangeWorkingState
* Description   : Change the working status of the system
* Parameter     : state:working state
* Returns       : None
***********************************************************************/
static void SystemMgr_ChangeWorkingState(E_WORKING_STATE state)
{
	seSysWorkState = state;
	TraceServ_SendSysStateMsg(seSysCurrState,seSysWorkState);
}


/***********************************************************************
* Name          : SystemMgr_EntryHandler
* Description   : Execute entry function
* Parameter     : state:system state
* Returns       : None
***********************************************************************/
static void SystemMgr_EntryHandler(E_SYS_STATE state)
{
    if(NULL != scfSysEntryFuncTables[state])
    {
        if(scfSysEntryFuncTables[state]() == TRUE)
        {
            SystemMgr_ChangeWorkingState(E_WORKING_STATE_ACTIVE);
        }
		else
		{
        	/* waiting... */
    	}
    }
    else
    {
        SystemMgr_ChangeWorkingState(E_WORKING_STATE_ACTIVE);
    }

}

/***********************************************************************
* Name          : SystemMgr_ActiveHandler
* Description   : Execute working function
* Parameter     : state:system state
* Returns       : None
***********************************************************************/
static void SystemMgr_ActiveHandler(E_SYS_STATE state)
{
    if(NULL != scfSysActiveFuncTables[state])
    {
        scfSysActiveFuncTables[state]();
    }
	else
	{
    	/*do nothing...*/
	}
	
	if(seSysCurrState != seSysNextState)
    {
		SystemMgr_ChangeWorkingState(E_WORKING_STATE_EXIT);
    }
	else
	{
    	/*do nothing...*/
	}
}

/***********************************************************************
* Name          : SystemMgr_ExitHandler
* Description   : Execute exit function
* Parameter     : state:system state
* Returns       : None
***********************************************************************/
static void SystemMgr_ExitHandler(E_SYS_STATE state)
{
    if(NULL != scfSysExitFuncTables[state])
    {
        if(scfSysExitFuncTables[state]() == TRUE)
        {
        	SystemMgr_ChangeState(seSysNextState);
        }
		else
		{
			/* waiting... */
		}
    }
    else
    {
        SystemMgr_ChangeState(seSysNextState);
    }
}


/***********************************************************************
* Name          : SystemMgr_UpdateState
* Description   : Update system state to Trace
* Parameter     : None
* Returns       : None
***********************************************************************/
void SystemMgr_UpdateState(void)
{	
	TraceServ_SendSysStateMsg(seSysCurrState,seSysWorkState);
}

/***********************************************************************
* Name          : SystemMgr_CycleTask10ms
* Description   : 10ms cyclic Task handler
* Parameter     : None
* Returns       : None
***********************************************************************/
void SystemMgr_CycleTask10ms(void)
{
	
    SystemMgr_CyclicContainer_10ms();
	
    switch (seSysWorkState)
    {
        case E_WORKING_STATE_ENTRY:
            SystemMgr_EntryHandler(seSysCurrState);
            break;
        case E_WORKING_STATE_ACTIVE:
            SystemMgr_ActiveHandler(seSysCurrState);
            break;
		case E_WORKING_STATE_EXIT:
            SystemMgr_ExitHandler(seSysCurrState);
            break;
        default:
			seSysWorkState = E_WORKING_STATE_ENTRY;
            break;
    }

}


