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

#include "DevCtrl.h"

static void DevCtrl_Init(void);
static void DevCtrl_Deinit(void);
static void DevCtrl_OpenHandler(S_DEV_INFO* dev);
static void DevCtrl_OpeningHandler(S_DEV_INFO* dev);
static void DevCtrl_ClosingHandler(S_DEV_INFO* dev);
static void DevCtrl_CloseHandler(S_DEV_INFO* dev);

static E_HAL_STATE DevCtrl_IOControl(U16 type,void *buffer);

cdev gsDevCtrl =
{
    DevCtrl_Init,
    DevCtrl_Deinit,
    NULL,
    NULL,
    NULL,
    NULL,
    DevCtrl_IOControl,
};


/***********************************************************************
* Name          : DevCtrl_Init
* Description   : Initial Device Controller data
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_Init(void)
{
	DevCtrl_IO_Register();		//initial IO
	DevCtrl_IO_Init();

	DevCtrl_ADC_Register();		//initial ADC
    DevCtrl_ADC_Init();

	Timer_Adapt_Init();			//initial timer
	
	DevCtrl_Tree_Init();		//initial device tree
    DevCtrl_SM_Init();			//initial sequence management data
}

/***********************************************************************
* Name          : DevCtrl_Deinit
* Description   : Deinitial Device Controller data
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_Deinit(void)
{
	DevCtrl_IO_Deinit();
    DevCtrl_ADC_Deinit();
}


/***********************************************************************
* Name      	: DevCtrl_IOControl
* Description	: IO Control
* Parameter   	: None
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE DevCtrl_IOControl(U16 type,void *buffer)
{
    E_HAL_STATE leState = E_HAL_STATE_OK;
	S_DEV_INFO *dev = NULL;
    U16 i = 0u;
	U16 luhwADCValue = 0;
	const U8 lcubAdcSize = sizeof(DEV_ADC_16BIT_T);
	
	switch(type)
	{
		case DEV_FUNC_REQUEST_CHANGE_PM_STATE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				if(DevCtrl_SM_RequestUpdateState(*(E_DEVCTRL_SM_STATE *)buffer) == TRUE)
				{
					if(DevCtrl_SM_GetCurrState() == DevCtrl_SM_GetNextState())//check if sequence is completed
					{
						leState = E_HAL_STATE_OK;
					}
					else
					{
						leState = E_HAL_STATE_FAIL;
					}
				}
				else
				{
					leState = E_HAL_STATE_FAIL;
				}
			}
			break;
		case DEV_FUNC_REQ_OPEN_DEVICE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				dev = DevCtrl_Tree_FindDeviceForKey((DEV_ID_T *)buffer,DevCtrl_CmpId);
				if(dev != NULL)
				{
					if(dev->devState == E_DEVCTRL_STATE_CLOSING)
					{
						leState = E_HAL_STATE_FAIL;
					}
					else
					{
						dev->powerReq = E_DEVCTRL_POWER_REQ_ON;
						leState = E_HAL_STATE_OK;
					}
				}
				else
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
			}
			break;
		case DEV_FUNC_REQ_CLOSE_DEVICE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				dev = DevCtrl_Tree_FindDeviceForKey((DEV_ID_T *)buffer,DevCtrl_CmpId);
				if(dev != NULL)
				{
					dev->powerReq = E_DEVCTRL_POWER_REQ_OFF;
					leState = E_HAL_STATE_OK;
				}
				else
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
			}
			break;
		case DEV_FUNC_RELEASE_DEVICE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				dev = DevCtrl_Tree_FindDeviceForKey((DEV_ID_T *)buffer,DevCtrl_CmpId);
				if(dev != NULL)
				{
					if(dev->powerReq == E_DEVCTRL_POWER_REQ_NONE)
					{
						/* device is released,do nothing... */
					}
					else
					{
						dev->powerReq = E_DEVCTRL_POWER_REQ_RELEASE;//change release state first
					}
					leState = E_HAL_STATE_OK;
				}
				else
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
			}
			break;
		case DEV_FUNC_DEVICE_IS_OPEN:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				dev = DevCtrl_Tree_FindDeviceForKey((DEV_ID_T *)buffer,DevCtrl_CmpId);
				if(dev != NULL)
				{
					leState = dev->devState == E_DEVCTRL_STATE_OPEN ? E_HAL_STATE_OK : E_HAL_STATE_FAIL;
				}
				else
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
			}
			break;
		case DEV_FUNC_DEVICE_IS_CLOSE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				dev = DevCtrl_Tree_FindDeviceForKey((DEV_ID_T *)buffer,DevCtrl_CmpId);
				if(dev != NULL)
				{
					leState = dev->devState == E_DEVCTRL_STATE_CLOSE ? E_HAL_STATE_OK : E_HAL_STATE_FAIL;
				}
				else
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
			}
			break;
		case DEV_FUNC_GET_ALL_ADC_CHANNEL:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				for(i = 0u;i < E_DEV_ADC_NUM;i++)
				{
					luhwADCValue = DEV_ADC_ASYNC_READ(i);//get ADC value
					*(DEV_ADC_16BIT_T *)buffer = ((luhwADCValue >> 8) & 0x00FF) + ((luhwADCValue << 8) & 0xFF00);
					buffer += lcubAdcSize;//offset
				}
				leState = E_HAL_STATE_OK;
			}
			break;
		case DEV_FUNC_GET_NTC_ADC_VALUE:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				luhwADCValue = DEV_ADC_ASYNC_READ(ADC_BL_TEMP_DET);//get ADC value
				*(DEV_ADC_16BIT_T *)buffer = ((luhwADCValue >> 8) & 0x00FF) + ((luhwADCValue << 8) & 0xFF00);
				leState = E_HAL_STATE_OK;
			}
			break;
		case DEV_FUNC_CHECK_VOLT_DISCHARGE:
			leState = DevCtrl_DetectAllPowerVoltage() == TRUE ? E_HAL_STATE_OK : E_HAL_STATE_FAIL;
			break;
		case DEV_FUNC_SOFT_RESET:
			mcu_sw_reset();//sw reset
			break;
		//case DEV_FUNC_DEEP_SLEEP:
		//	break;
		default:
			leState = E_HAL_STATE_NO_FUNC;
			break;
	}
	return (leState);

}

/***********************************************************************
* Name          : DevCtrl_Register_Dev
* Description   : Register device
* Parameter     : dev:device structure
* Returns       : TRUE:register successful,FALSE:register failed
***********************************************************************/
MYBOOL DevCtrl_Register_Dev(S_DEV_INFO *dev)
{
	MYBOOL lbRet = FALSE;
    if(DevCtrl_Tree_FindDeviceForKey(&dev->id,DevCtrl_CmpId) == NULL)//check id validity
    {
        if(DevCtrl_Tree_FindDeviceForKey(dev->name,DevCtrl_CmpName) == NULL)//check name validity
        {
            DevCtrl_Tree_AddDevice(dev);               	//add
            DevCtrl_Tree_SortDeviceByID();             	//resort
            lbRet = TRUE;
        }
        else
        {
            PRINT_DEV_MSG(dev->name,"device name is existed!");
        }
    }
    else
    {
        PRINT_DEV_MSG(dev->name,"device id is conflict!");
    }

	return lbRet;
}


/***********************************************************************
* Name          : DevCtrl_Unregister_Dev
* Description   : Unregister device
* Parameter     : dev:device structure
* Returns       : TRUE:unregister successful,FALSE:unregister failed
***********************************************************************/
BOOL DevCtrl_Unregister_Dev(S_DEV_INFO *dev)
{
    BOOL lbRet = FALSE;
    if(DevCtrl_Tree_RemoveDevice(&dev->id,DevCtrl_CmpId) == TRUE)//Delete device based on id number
    {
        PRINT_DEV_MSG(dev->name,"device unregister successful!");
        lbRet = TRUE;
    }
    else
    {
        PRINT_DEV_MSG(dev->name,"device unregister failed!");
        lbRet = FALSE;
    }
    return lbRet;
}

/***********************************************************************
* Name          : DevCtrl_OpenHandler
* Description   : execute the function of the device in the open state
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
static void DevCtrl_OpenHandler(S_DEV_INFO* dev)
{
	E_DEVCTRL_SM_STATE leSmNextState = DevCtrl_SM_GetNextState();

	switch(dev->powerReq)
	{
		case E_DEVCTRL_POWER_REQ_OFF:
			DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSING);//shutdown device
		break;
		case E_DEVCTRL_POWER_REQ_RELEASE:
			if (leSmNextState == E_DEVCTRL_SM_STATE_POWERON)
			{
				dev->powerReq = E_DEVCTRL_POWER_REQ_NONE;//release state
				EXECUTE_FUNC_POINTER(dev->running10msTask);//running
			}
			else
			{
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSING);//shutdown device
			}
		break;
		case E_DEVCTRL_POWER_REQ_ON:
			EXECUTE_FUNC_POINTER(dev->running10msTask);
		break;
		default://E_DEVCTRL_POWER_REQ_NONE
		    if(E_DEVCTRL_SM_STATE_SHUTDOWN == leSmNextState)
		    {
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSING);//shutdown device
			}
			else if(E_DEVCTRL_SM_STATE_POWERON == leSmNextState)				//get next status
		    {
		        EXECUTE_FUNC_POINTER(dev->running10msTask);
		    }
			else
			{
				/* do nothing... */
			}
		break;
	}

}

/***********************************************************************
* Name          : DevCtrl_OpeningHandler
* Description   : execute the function of the device in the opening state
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
static void DevCtrl_OpeningHandler(S_DEV_INFO* dev)
{
	E_DEVCTRL_SM_STATE leSmNextState = DevCtrl_SM_GetNextState();
	MYBOOL lmbRet = FALSE;
	switch(dev->powerReq)
	{
		case E_DEVCTRL_POWER_REQ_OFF:
			DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSING);//Change to closing
		break;
		case E_DEVCTRL_POWER_REQ_RELEASE:
			if(leSmNextState == E_DEVCTRL_SM_STATE_SHUTDOWN)
			{
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSING);//Change to closing
			}
			else
			{
				CHECK_FUNC_POINTER(dev->powerCtrl->powerOnFunc,lmbRet);//execute power on sequence
		        if(lmbRet == TRUE)
		        {
					DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPEN);//sequence completed
		        }
				else
				{
					/* waiting... */
				}
			}
		break;
		case E_DEVCTRL_POWER_REQ_ON:
			CHECK_FUNC_POINTER(dev->powerCtrl->powerOnFunc,lmbRet);
	        if(lmbRet == TRUE)
	        {
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPEN);//sequence completed
	        }
			else
			{
				/* waiting... */
			}
		break;
		default://E_DEVCTRL_POWER_REQ_NONE
			DevCtrl_SM_ExecPowerOnHandler(dev);//execute sequence
		break;
	}
}

/***********************************************************************
* Name          : DevCtrl_ClosingHandler
* Description   : execute the function of the device in the closing state
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
static void DevCtrl_ClosingHandler(S_DEV_INFO* dev)
{
	
	E_DEVCTRL_SM_STATE leSmNextState = DevCtrl_SM_GetNextState();
 	MYBOOL lmbRet = FALSE;
	switch(dev->powerReq)
	{
		case E_DEVCTRL_POWER_REQ_OFF:
		case E_DEVCTRL_POWER_REQ_ON:
			CHECK_FUNC_POINTER(dev->powerCtrl->shutDownFunc,lmbRet);
			if(lmbRet == TRUE)
			{
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSE);//sequence completed
			}
			else
			{
				/* waiting... */
			}
		break;
		case E_DEVCTRL_POWER_REQ_RELEASE:
			if(leSmNextState == E_DEVCTRL_SM_STATE_POWERON)
			{
				CHECK_FUNC_POINTER(dev->powerCtrl->shutDownFunc,lmbRet);//close first
				if(lmbRet == TRUE)
				{
					DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_CLOSE);//sequence completed
				}
				else
				{
				/* waiting... */
				}
			}
			else
			{
				DevCtrl_SM_ExecPowerOffHandler(dev);//execute sequence
			}
		break;
		default://E_DEVCTRL_POWER_REQ_NONE
			DevCtrl_SM_ExecPowerOffHandler(dev);//execute sequence
		break;
	}
}


/***********************************************************************
* Name          : DevCtrl_ShutdownHandler
* Description   : execute the function of the device in the shut down state
* Parameter     : dev:device structure
* Returns       : None
***********************************************************************/
static void DevCtrl_CloseHandler(S_DEV_INFO* dev)
{
	E_DEVCTRL_SM_STATE leSmNextState = DevCtrl_SM_GetNextState();

	switch(dev->powerReq)
	{
		case E_DEVCTRL_POWER_REQ_OFF:
			/* stay in close state,do nothing... */
		break;
		case E_DEVCTRL_POWER_REQ_RELEASE:
			if(leSmNextState == E_DEVCTRL_SM_STATE_POWERON)
			{
				DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPENING);
			}
			else
			{
				dev->powerReq = E_DEVCTRL_POWER_REQ_NONE;
			}
		break;
		case E_DEVCTRL_POWER_REQ_ON:
			DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPENING);//opening device
		break;
		default://E_DEVCTRL_POWER_REQ_NONE
			if(DevCtrl_SM_DevIsAllowPowerOn(dev) == TRUE)
		    {
		        DevCtrl_SM_UpdateDevState(dev,E_DEVCTRL_STATE_OPENING);//opening device
		    }
		    else
		    {
		        /* do nothing... */
		    }
		break;
	}
}

/***********************************************************************
* Name          : DevCtrl_CycleTask10ms
* Description   : 10ms cyclic Task handler
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_CycleTask10ms(void)
{

	S_DEV_TREE *lsDevMgt = NULL;
	S_DEV_INFO *lsDev = NULL;
	DevCtrl_Hardware_TaskEntry_10ms();									//Hardware resources task
	lsDevMgt = DevCtrl_Tree_GetDevicesManager();						//get devices tree
    for(lsDev = lsDevMgt->head;lsDev != NULL;lsDev = lsDev->next)
    {
        switch (lsDev->devState) {
            case E_DEVCTRL_STATE_OPEN:
                DevCtrl_OpenHandler(lsDev);
            break;
			case E_DEVCTRL_STATE_OPENING:
				DevCtrl_OpeningHandler(lsDev);
			break;
            case E_DEVCTRL_STATE_CLOSE:
                DevCtrl_CloseHandler(lsDev);
            break;
			case E_DEVCTRL_STATE_CLOSING:
				DevCtrl_ClosingHandler(lsDev);
			break;
            default:
            break;
        }
    }
	DevCtrl_SM_TaskEntry_10ms();										//Power Manager Cycle task
}


/***********************************************************************
* Name          : DevCtrl_UpdateAllDevsState
* Description   : Update all devices status
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_UpdateAllDevsState(void)
{
    S_DEV_TREE *lsDevMgt = NULL;
    S_DEV_INFO *lsDev = NULL;
	
	lsDevMgt = DevCtrl_Tree_GetDevicesManager();						//get devices tree
    for(lsDev = lsDevMgt->head;lsDev != NULL;lsDev = lsDev->next)
    {
		TraceServ_SendDevStateMsg(lsDev->id,lsDev->devState);			//update to trace
    }
}

/***********************************************************************
* Name          : DevCtrl_CmpId
* Description   : Compare the attribute for id
* Parameter     : dev1:first device id,dev2:second device
* Returns       : dev1.id - dev2.id
***********************************************************************/
int DevCtrl_CmpId(const void *id,S_DEV_INFO *dev2)
{
    return (*(DEV_ID_T *)id) - (dev2->id);
}


/***********************************************************************
* Name          : DevCtrl_CmpName
* Description   : Compare the attribute for name
* Parameter     : dev1:first device,dev2:second device
* Returns       : dev1.id - dev2.id
***********************************************************************/
int DevCtrl_CmpName(const void *name,S_DEV_INFO *dev2)
{
    return strcmp((char *)name,(dev2->name));
}


