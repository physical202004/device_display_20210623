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

#include "DevCtrl.h"

static void DevCtrl_SM_SubShutDownTaskEntry(void);
static void DevCtrl_SM_SubPowerOnTaskEntry(void);
static E_BAT_STATE DevCtrl_SM_LowVoltHandler(DEV_ADC_16BIT_T batteryVolt);
static E_BAT_STATE DevCtrl_SM_NormalVoltHandler(DEV_ADC_16BIT_T batteryVolt);
static E_BAT_STATE DevCtrl_SM_HighVoltHandler(DEV_ADC_16BIT_T batteryVolt);
static MYBOOL DevCtrl_SM_ChangeStateIsAllow(void);
static DEV_POWER_SQUENCE_T DevCtrl_SM_GetDevicePowerOnStage(DEV_ID_T id);
static DEV_POWER_SQUENCE_T DevCtrl_SM_GetDevicePowerOffStage(DEV_ID_T id);
static MYBOOL DevCtrl_SM_IsDevicePowerOffTail(void);
static MYBOOL DevCtrl_SM_IsDevicePowerOnTail(void);
static MYBOOL DevCtrl_SM_PowerOffStageDevCheck(S_DEV_INFO *dev);
static MYBOOL DevCtrl_SM_PowerOffStageIsComplete(void);
static MYBOOL DevCtrl_SM_PowerOnStageIsComplete(void);
static void DevCtrl_SM_ChangetoNewState(E_DEVCTRL_SM_STATE newState);
static void DevCtrl_SM_ClearDevAttri(S_DEV_INFO *dev);

/* variable */

static const DEV_POWER_SQUENCE_T stPowerOnSize = ARRAY_LENGTH(gsDevPowerOnSeq);		//size of power-on sequence
static const DEV_POWER_SQUENCE_T stPowerOffSize = ARRAY_LENGTH(gsDevPowerOffSeq);	//size of power-down sequence

/* power management state */
static E_DEVCTRL_SM_STATE seDevCtrlSmNextState = E_DEVCTRL_SM_STATE_SHUTDOWN;
static E_DEVCTRL_SM_STATE seDevCtrlSmCurrState = E_DEVCTRL_SM_STATE_SHUTDOWN;
static U16 suhwSmSequenceTimeoutCnt = 0;											//timeout counter in power sequence
static U16 suhwBatteryDiagCnt = 0;													//record the diagnostic count value of abnormal battery status
static DEV_POWER_SQUENCE_T stPowerOnTail = 0u;										//The last index value of the power-on sequence
static DEV_POWER_SQUENCE_T stPowerOffTail = 0u;										//The last index value of the power-down sequence

/***********************************************************************
* Name          : DevCtrl_SM_Init
* Description   : Initialize power manager of device
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_SM_Init(void)
{
	DEV_POWER_SQUENCE_T i = 0u;

	/* Calculate the largest index value in the array */
	for(i = 0u,stPowerOnTail = gsDevPowerOnSeq[0].index;i < stPowerOnSize;i++)
	{
		if(gsDevPowerOnSeq[i].index > stPowerOnTail)
		{
		    stPowerOnTail = gsDevPowerOnSeq[i].index;
		}
		else
		{
			/* do nothing... */
		}
	}
	
	for(i = 0u,stPowerOffTail = gsDevPowerOffSeq[0].index;i < stPowerOffSize;i++)
	{
		if(gsDevPowerOffSeq[i].index > stPowerOffTail)
		{
		    stPowerOffTail = gsDevPowerOffSeq[i].index;
		}
		else
		{
			/* do nothing... */
		}
	}
	
}

/***********************************************************************
* Name          : DevCtrl_SM_LowVoltHandler
* Description   : Processing handler in low voltage state
* Parameter     : None
* Returns       : None
***********************************************************************/
static E_BAT_STATE DevCtrl_SM_LowVoltHandler(DEV_ADC_16BIT_T batteryVolt)
{
	E_BAT_STATE leBatState = E_BAT_STATE_LOW;
	if(batteryVolt > BAT_AD_THRESHOLD_NORMAL_TO_HIGH)
	{
		leBatState = E_BAT_STATE_HIGH;					//switch to high voltage
		suhwBatteryDiagCnt = 0;//clear counter
	}
	else if(batteryVolt > BAT_AD_THRESHOLD_LOW_TO_NORMAL)
	{
		leBatState = E_BAT_STATE_NORMAL;				//switch to normal voltage
		suhwBatteryDiagCnt = 0;//clear counter
	}
	else
	{
		/* low */
		/*if(suhwBatteryDiagCnt < DIAGSERV_IFC_TIME_5S)
		{
			suhwBatteryDiagCnt++;
		}
		else
		{
			DiagServ_AddIfcInfo(IFC_BAT_UNDER_VOLTAGE);  // record low voltage
		}*/
	}
	return leBatState;
}

/***********************************************************************
* Name          : DevCtrl_SM_LowVoltHandler
* Description   : Processing handler in normal voltage state
* Parameter     : None
* Returns       : None
***********************************************************************/
static E_BAT_STATE DevCtrl_SM_NormalVoltHandler(DEV_ADC_16BIT_T batteryVolt)
{
	E_BAT_STATE leBatState = E_BAT_STATE_NORMAL;
	
	if(batteryVolt > BAT_AD_THRESHOLD_NORMAL_TO_HIGH)
	{
		leBatState = E_BAT_STATE_HIGH;					//switch to high voltage
	}
	else if(batteryVolt < BAT_AD_THRESHOLD_NORMAL_TO_LOW)
	{
		leBatState = E_BAT_STATE_LOW;					//switch to low voltage
	}
	else
	{
		/* normal */
	}
	return leBatState;
}

/***********************************************************************
* Name          : DevCtrl_SM_HighVoltHandler
* Description   : Processing handler in high voltage state
* Parameter     : None
* Returns       : None
***********************************************************************/
static E_BAT_STATE DevCtrl_SM_HighVoltHandler(DEV_ADC_16BIT_T batteryVolt)
{
	E_BAT_STATE leBatState = E_BAT_STATE_HIGH;
	if(batteryVolt < BAT_AD_THRESHOLD_NORMAL_TO_LOW)
	{
		leBatState = E_BAT_STATE_LOW;					//switch to low voltage
		suhwBatteryDiagCnt = 0;//clear counter
	}
	else if(batteryVolt < BAT_AD_THRESHOLD_HIGH_TO_NORMAL)
	{
		leBatState = E_BAT_STATE_NORMAL;				//switch to normal voltage
		suhwBatteryDiagCnt = 0;//clear counter
	}
	else
	{
		/* high */
		/*if(suhwBatteryDiagCnt < DIAGSERV_IFC_TIME_5S)
		{
			suhwBatteryDiagCnt++;
		}
		else
		{
			DiagServ_AddIfcInfo(IFC_BAT_OVER_VOLTAGE);  // record high voltage
		}*/

	}
	return leBatState;
}

/***********************************************************************
* Name          : DevCtrl_SM_BatteryStateMachine
* Description   : Battery management state machine
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_SM_BatteryStateMachine(void)
{
	E_BAT_STATE leBatState = E_BAT_STATE_LOW;
	DEV_ADC_16BIT_T ltBatADValue = DEV_ADC_ASYNC_READ(ADC_BAT_DET);//get battery status

	CS_SIG_SyncReceive(SIG_ENUM_BATTERY_STATE,&leBatState);//receive last status
	
	switch (leBatState)
	{
		case E_BAT_STATE_LOW:
			leBatState = DevCtrl_SM_LowVoltHandler(ltBatADValue);		//processing for low voltage state
			break;
		case E_BAT_STATE_NORMAL:
			leBatState = DevCtrl_SM_NormalVoltHandler(ltBatADValue);	//processing for normal voltage state
			break;
		case E_BAT_STATE_HIGH:
			leBatState = DevCtrl_SM_HighVoltHandler(ltBatADValue);		//processing for high voltage state
			break;
		default:
			leBatState = E_BAT_STATE_LOW;								//switch to default voltage
			break;
	}
	CS_SIG_SyncSend(SIG_ENUM_BATTERY_STATE,&leBatState);		//update battery status
}

/***********************************************************************
* Name          : DevCtrl_SM_GetNextState
* Description   : Return next power management state
* Parameter     : None
* Returns       : seDevCtrlSmNextState
***********************************************************************/
E_DEVCTRL_SM_STATE DevCtrl_SM_GetNextState(void)
{
	return seDevCtrlSmNextState;
}

/***********************************************************************
* Name          : DevCtrl_SM_GetCurrState
* Description   : Return current power management state
* Parameter     : None
* Returns       : seDevCtrlSmCurrState
***********************************************************************/
E_DEVCTRL_SM_STATE DevCtrl_SM_GetCurrState(void)
{
	return seDevCtrlSmCurrState;
}

/***********************************************************************
* Name          : DevCtrl_SM_ClearDevAttri
* Description   : Clear device attribute content
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_SM_ClearDevAttri(S_DEV_INFO *dev)
{
	dev->faultState = E_DEVCTRL_FAULT_STATE_NONE;
}

/***********************************************************************
* Name          : DevCtrl_SM_DevIsAllowPowerOn
* Description   : Whether the device can be powered on
* Parameter     : None
* Returns       : None
***********************************************************************/
MYBOOL DevCtrl_SM_DevIsAllowPowerOn(S_DEV_INFO* dev)
{
	MYBOOL lmbRet = FALSE;
	
	if(E_DEVCTRL_SM_STATE_POWERON == seDevCtrlSmNextState)
	{
		lmbRet = TRUE;
	}
	else
	{
		lmbRet = FALSE;
	}

	return lmbRet;
}
/***********************************************************************
* Name          : DevCtrl_SM_UpdateDevState
* Description   : Update device status
* Parameter     : dev:device pointer,state:next status
* Returns       : None
***********************************************************************/
void DevCtrl_SM_UpdateDevState(S_DEV_INFO *dev,E_DEVCTRL_STATE state)
{
	dev->devState = state;
	TraceServ_SendDevStateMsg(dev->id,state);//update to trace
	/* clear device attributes */
	DevCtrl_SM_ClearDevAttri(dev);
}

/***********************************************************************
* Name          : DevCtrl_SM_ChangeStateIsAllow
* Description   : If the state of the power-down sequence is active, it is not allowed to change to new status
* Parameter     : None
* Returns       : TRUE:allow to change state,FALSE:reject change
***********************************************************************/
static MYBOOL DevCtrl_SM_ChangeStateIsAllow(void)
{
	MYBOOL lmbRet = TRUE;
	if(seDevCtrlSmNextState == seDevCtrlSmCurrState)
	{
		lmbRet = TRUE;
	}
	else
	{
		/* If the state of the power-down sequence is active, it is not allowed to change to new status */
		if(seDevCtrlSmNextState == E_DEVCTRL_SM_STATE_SHUTDOWN)
		{
			lmbRet = FALSE;
		}
		else
		{
			lmbRet = TRUE;
		}
	}
	return lmbRet;
}

/***********************************************************************
* Name          : DevCtrl_SM_RequestUpdateState
* Description   : Request to update power management to new status
* Parameter     : nextState:request change to next status
* Returns       : TRUE:update sucessful,FALSE:update failed
***********************************************************************/
MYBOOL DevCtrl_SM_RequestUpdateState(E_DEVCTRL_SM_STATE nextState)
{
	MYBOOL lmbRet = FALSE;
	/* Check validity */
	if(nextState >= E_DEVCTRL_SM_STATE_MAX)
	{
		lmbRet = FALSE;
		PRINT_IMPORTANT_MSG("transmit next state invalid!");
	}
	else
	{
		if((seDevCtrlSmCurrState == nextState) || (seDevCtrlSmNextState == nextState))	//keeping
		{
			lmbRet = TRUE;
		}
		else
		{
			/* If the state of the power-down sequence is active, it is not allowed to change to new status */
			if(DevCtrl_SM_ChangeStateIsAllow() == TRUE)
			{
				seDevCtrlSmNextState = nextState;

				suhwSmSequenceTimeoutCnt = 0U;
				
				DevCtrl_Tree_SetPowerOnIndex(0);//reset power on sequence
				DevCtrl_Tree_SetPowerOffIndex(0);//reset power down sequence
#ifdef DC_DC_CONTROL
				if(nextState == E_DEVCTRL_SM_STATE_POWERON)
				{
					DevCtrl_ControlDcDcEnable(HIGH_LEVEL);//open dc-dc first
				}
				else
				{
					/* do nothing... */
				}
#endif
				TraceServ_SendDevPMMsg(seDevCtrlSmCurrState,seDevCtrlSmNextState);//update status to trace
				lmbRet = TRUE;
			}
			else
			{
				lmbRet = FALSE;
			}
		}
	}
	return (lmbRet);
}

/***********************************************************************
* Name          : DevCtrl_SM_ChangetoNewState
* Description   : Update status
* Parameter     : newState: 
* Returns       : None
***********************************************************************/
static void DevCtrl_SM_ChangetoNewState(E_DEVCTRL_SM_STATE newState)
{	
	seDevCtrlSmCurrState = newState;//update state
	suhwSmSequenceTimeoutCnt = 0U;//clear counter
	
	switch(seDevCtrlSmCurrState)
	{
		case E_DEVCTRL_SM_STATE_SHUTDOWN:
#ifdef DC_DC_CONTROL		
			DevCtrl_ControlDcDcEnable(LOW_LEVEL);//close dc-dc
#endif
			break;
		case E_DEVCTRL_SM_STATE_POWERON:
			break;
		default:
			break;
	}
}

/***********************************************************************
* Name          : DevCtrl_SM_ExecPowerOffHandler
* Description   : Execute the power down sequence of the device
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
void DevCtrl_SM_ExecPowerOffHandler(S_DEV_INFO *dev)
{
    DEV_POWER_SQUENCE_T ltIndex = 0u;
	MYBOOL lmbRet = FALSE;
	ltIndex = DevCtrl_SM_GetDevicePowerOffStage(dev->id);		//Check if the device is registered
	if(ltIndex == DEV_INVALID_VALUE)
	{
		PRINT_DEV_MSG(dev->name, "The device is not registered in the power-down sequence");
	}
	else
	{
	    if(ltIndex == DevCtrl_Tree_GetPowerOffIndex())
	    {
			CHECK_FUNC_POINTER(dev->powerCtrl->shutDownFunc,lmbRet);
	        if(lmbRet == TRUE)
	        {
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSE);
	        }
			else
			{
				/* do nothing... */
			}
	    }
		else
		{
			/* do nothing... */
		}
	}
}

/***********************************************************************
* Name          : DevCtrl_SM_ExecPowerOnHandler
* Description   : Execute the power on sequence of the device
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
void DevCtrl_SM_ExecPowerOnHandler(S_DEV_INFO *dev)
{
    DEV_POWER_SQUENCE_T ltIndex = 0;
    MYBOOL lmbRet = FALSE;

    ltIndex = DevCtrl_SM_GetDevicePowerOnStage(dev->id);
	if(ltIndex == DEV_INVALID_VALUE)//Check if the device is registered
	{
		PRINT_DEV_MSG(dev->name, "The device is not registered in the power-on sequence");
	}
	else
	{
	    if(ltIndex == DevCtrl_Tree_GetPowerOnIndex())
	    {
	        CHECK_FUNC_POINTER(dev->powerCtrl->powerOnFunc,lmbRet);
	        if(lmbRet == TRUE)
	        {
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPEN);
	        }
			else
			{
				/* do nothing... */
			}
	    }
		else
		{
			/* do nothing... */
		}
	}
}

/***********************************************************************
* Name          : DevCtrl_SM_SubShutDownTaskEntry
* Description   : Device power-down sequence processing function
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_SM_SubShutDownTaskEntry(void)
{
    if((DevCtrl_SM_PowerOffStageIsComplete() == TRUE) || (suhwSmSequenceTimeoutCnt >= PM_SEQUENCE_TIMEOUT_IN_STAGE))
    {
		suhwSmSequenceTimeoutCnt = 0U;
        if(DevCtrl_SM_IsDevicePowerOffTail() == TRUE)
        {

            DevCtrl_SM_ChangetoNewState(E_DEVCTRL_SM_STATE_SHUTDOWN);//sqeuence completed
        }
		else
		{
			/* do nothing... */
		}
    }
	else
	{
		suhwSmSequenceTimeoutCnt++;
	}
}

/***********************************************************************
* Name          : DevCtrl_SM_SubPowerOnTaskEntry
* Description   : Device power-on sequence processing function
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_SM_SubPowerOnTaskEntry(void)
{

    if((DevCtrl_SM_PowerOnStageIsComplete() == TRUE) || (suhwSmSequenceTimeoutCnt >= PM_SEQUENCE_TIMEOUT_IN_STAGE))
    {
		suhwSmSequenceTimeoutCnt = 0U;
        if(DevCtrl_SM_IsDevicePowerOnTail() == TRUE)
        {
            DevCtrl_SM_ChangetoNewState(E_DEVCTRL_SM_STATE_POWERON);	//sqeuence completed
        }
		else
		{
			/* do nothing...*/
		}
    }
	else
	{
		suhwSmSequenceTimeoutCnt++;
	}
}

/***********************************************************************
* Name          : DevCtrl_SM_PowerOffStageDevCheck
* Description   : Check device sequence state in power off stage
* Parameter     : None
* Returns       : TRUE:power sequence is completed, FALSE:power sequence is executing
***********************************************************************/
static MYBOOL DevCtrl_SM_PowerOffStageDevCheck(S_DEV_INFO *dev)
{
	MYBOOL lmbRet = FALSE;

	if((dev->powerReq == E_DEVCTRL_POWER_REQ_NONE) || (dev->powerReq == E_DEVCTRL_POWER_REQ_RELEASE))
	{
        if(dev->devState != E_DEVCTRL_STATE_CLOSE)			//check the device status
        {
			lmbRet = FALSE;
        }
		else
		{
			lmbRet = TRUE;
		}
	}
	else
	{
		lmbRet = TRUE;//if application request device to open or close,skip
	}
	return lmbRet;
}

/***********************************************************************
* Name          : DevCtrl_SM_PowerOffStageIsComplete
* Description   : Check whether the current position of power-down sequence is completed
* Parameter     : None
* Returns       : TRUE:power sequence is completed, FALSE:power sequence is executing
***********************************************************************/
static MYBOOL DevCtrl_SM_PowerOffStageIsComplete(void)
{
    S_DEV_INFO *lsDev = NULL;
    DEV_POWER_SQUENCE_T i = 0u;
    DEV_POWER_SQUENCE_T ltIndex = DevCtrl_Tree_GetPowerOffIndex();//Get the position of the current power-down sequence
	MYBOOL lmbRet = FALSE;

    for(i = 0;i < stPowerOffSize;i++)
    {
        if(ltIndex == gsDevPowerOffSeq[i].index)
        {
            lsDev = DevCtrl_Tree_FindDeviceForKey(&gsDevPowerOffSeq[i].id,DevCtrl_CmpId);//find device from the device tree
			if(lsDev == NULL)
			{
				PRINT_DEV_ID_MSG(gsDevPowerOffSeq[i].id, "In the power-down sequence, the device's id is not found in the device tree");
				lmbRet = TRUE;
				break;
			}
			else
			{
				lmbRet = DevCtrl_SM_PowerOffStageDevCheck(lsDev);
				if(lmbRet == TRUE)
				{
					continue;//sequence is completed
				}
				else
				{
					break;//sequence is excuting
				}
				
			}
        }
		else
		{
			continue;
		}
    }

    return (lmbRet);
}

/***********************************************************************
* Name          : DevCtrl_SM_PowerOnStageIsComplete
* Description   : Check whether the current position of power-on sequence is completed
* Parameter     : None
* Returns       : TRUE:power sequence is completed, FALSE:power sequence is executing
***********************************************************************/
static MYBOOL DevCtrl_SM_PowerOnStageIsComplete(void)
{
    S_DEV_INFO *lsDev = NULL;
    DEV_POWER_SQUENCE_T i = 0u;
    DEV_POWER_SQUENCE_T ltIndex = DevCtrl_Tree_GetPowerOnIndex();//Get the location of the current power-on sequence
	MYBOOL lmbRet = FALSE;
    for(i = 0;i < stPowerOnSize;i++)
    {
        if(ltIndex == gsDevPowerOnSeq[i].index)
        {
            lsDev = DevCtrl_Tree_FindDeviceForKey(&gsDevPowerOnSeq[i].id,DevCtrl_CmpId);//find device from the device tree
			if(lsDev == NULL)
			{
				PRINT_DEV_ID_MSG(gsDevPowerOnSeq[i].id, "In the power-on sequence, the device's id is not found in the device tree");
				lmbRet = TRUE;
				break;
			}
			else
			{
				if((lsDev->powerReq == E_DEVCTRL_POWER_REQ_NONE) || (lsDev->powerReq == E_DEVCTRL_POWER_REQ_RELEASE))//check the request status
				{
					if(lsDev->devState != E_DEVCTRL_STATE_OPEN)//check the device status
		            {
						lmbRet = FALSE;
		                break;
		            }
					else
					{
						lmbRet = TRUE;
					}
				}
				else
				{
					lmbRet = TRUE;
				}
			}
        }
		else
		{
			continue;
		}
    }

    return (lmbRet);
}

/***********************************************************************
* Name          : DevCtrl_SM_GetDevicePowerOnStage
* Description   : Get current position of power-on sequence
* Parameter     : name: device name
* Returns       : position of the sequence
***********************************************************************/
static DEV_POWER_SQUENCE_T DevCtrl_SM_GetDevicePowerOnStage(DEV_ID_T id)
{
    DEV_POWER_SQUENCE_T i = 0u;
    DEV_POWER_SQUENCE_T index = DEV_INVALID_VALUE;
	
    for(i = 0;i < stPowerOnSize;i++)
    {
        if(id == (gsDevPowerOnSeq[i].id))
        {
            index = gsDevPowerOnSeq[i].index;
            break;
        }
		else
		{
			continue;
		}
    }
    return (index);
}

/***********************************************************************
* Name          : DevCtrl_SM_GetDevicePowerOffStage
* Description   : Get current position of power-down sequence
* Parameter     : name: device name
* Returns       : position of the sequence
***********************************************************************/
static DEV_POWER_SQUENCE_T DevCtrl_SM_GetDevicePowerOffStage(DEV_ID_T id)
{
    DEV_POWER_SQUENCE_T i = 0u;
    DEV_POWER_SQUENCE_T index = DEV_INVALID_VALUE;
	
    for(i = 0;i < stPowerOffSize;i++)
    {
        if(id == (gsDevPowerOffSeq[i].id))
        {
            index = gsDevPowerOffSeq[i].index;
            break;
        }
		else
		{
			continue;
		}
    }
    return (index);
}


/***********************************************************************
* Name          : DevCtrl_SM_IsDevicePowerOffTail
* Description   : Check current index whether end of power-off sequence
* Parameter     : None
* Returns       : TRUE:sequence has completed,FALSE:sequence is going
***********************************************************************/
static MYBOOL DevCtrl_SM_IsDevicePowerOffTail(void)
{
    DEV_POWER_SQUENCE_T ltIndex = DevCtrl_Tree_GetPowerOffIndex();
	MYBOOL lmbRet = FALSE;
	
	if(ltIndex >= stPowerOffSize)
	{
		PRINT_DEV_ID_MSG(ltIndex,"current index is range out the gsDevPowerOffSeq!");
		lmbRet = TRUE;//Exceeded the ID of the sequence setting,forced completion
	}
    else 
	{
		if(stPowerOffTail == ltIndex)
	    {
	        lmbRet = TRUE;
	    }
	    else
	    {
	        ltIndex++;
	        DevCtrl_Tree_SetPowerOffIndex(ltIndex);
			lmbRet = FALSE;
	    }
    }
    return (lmbRet);
}


/***********************************************************************
* Name          : DevCtrl_SM_IsDevicePowerOnTail
* Description   : Check current index whether end of power-on sequence
* Parameter     : None
* Returns       : TRUE:sequence has completed,FALSE:sequence is going
***********************************************************************/
static MYBOOL DevCtrl_SM_IsDevicePowerOnTail(void)
{

    DEV_POWER_SQUENCE_T ltIndex = DevCtrl_Tree_GetPowerOnIndex();
	MYBOOL lmbRet = FALSE;
	if(ltIndex >= stPowerOnSize)
	{
		PRINT_DEV_ID_MSG(ltIndex,"current index is range out the gsDevPowerOnSeq!");
		lmbRet = TRUE;//Exceeded the ID of the sequence setting,forced completion
	}
	else
	{
	    if(stPowerOnTail == ltIndex)
	    {
	        lmbRet = TRUE;
	    }
	    else
	    {
	        ltIndex++;
	        DevCtrl_Tree_SetPowerOnIndex(ltIndex);
			lmbRet = FALSE;
	    }
	}

    return (lmbRet);
}

/***********************************************************************
* Name          : DevCtrl_SM_TaskEntry_10ms
* Description   : 10ms cyclic Task handler
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
void DevCtrl_SM_TaskEntry_10ms(void)
{

	if(seDevCtrlSmCurrState == seDevCtrlSmNextState)		//no change
	{
		/* do nothing... */
	}
	else
	{
	    switch (seDevCtrlSmNextState)
	    {
	        case E_DEVCTRL_SM_STATE_SHUTDOWN:
	            DevCtrl_SM_SubShutDownTaskEntry();
	            break;
	        case E_DEVCTRL_SM_STATE_POWERON:
	            DevCtrl_SM_SubPowerOnTaskEntry();
	            break;
	        default:
				seDevCtrlSmNextState = seDevCtrlSmCurrState;
	            break;
	    }
	}
}

