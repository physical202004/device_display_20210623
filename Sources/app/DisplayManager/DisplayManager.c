/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : display module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "DisplayManager.h"
#include "DeratingManager_if.h"

static void DisplayMgr_GradientBrn(DEV_BRN_16BIT_T brightness);
static void DisplayMgr_ClearDataHandle(void);
static MYBOOL DisplayMgr_UpdateBrn(DEV_BRN_16BIT_T brightness);
static MYBOOL DisplayMgr_CycleCheckBrn(void);
static MYBOOL DisplayMgr_CheckBrnState(void);
#ifdef DISPLAY_PROCESS_THE_LOCK_SIGNAL
static void DisplayMgr_NoLockHandler(void);
static void DisplayMgr_LockResumeHandler(void);
#endif

#ifdef GRADIENT_FUNCTION
/* gradient information structure */
static S_GRADIENT_INFO ssGradientInfo = {
	0,
	0,
	0,
	0,
};
#endif

static U8 subCheckBrightnessCnt = 0;//Check brightness 500ms cycle counting
static U8 subBriErrCnt = 0;//Brightness error times

/***********************************************************************
*  Name        	: DisplayMgr_StandbyExitHandler
*  Description 	: Exit Handler in standby mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_StandbyExitHandler(void)
{
	DEV_BRN_16BIT_T ltCurrBrnVal = 0U;//reset value
	DeratingMgr_Init();
	CS_SIG_SyncSend(SIG_SHORT_CURR_BRN_VAL,&ltCurrBrnVal);//update brightness
#ifdef GRADIENT_FUNCTION 
	memset(&ssGradientInfo,0,sizeof(S_GRADIENT_INFO));
#endif
}

/***********************************************************************
*  Name        	: DisplayMgr_ClearDataHandle
*  Description 	: Clear Data 
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DisplayMgr_ClearDataHandle(void)
{
	subCheckBrightnessCnt = 0;
	subBriErrCnt = 0;
}


/***********************************************************************
*  Name        	: DisplayMgr_NormalEntryHandler
*  Description 	: Entry Handler in normal mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_NormalEntryHandler(void)
{
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;

	DisplayMgr_ClearDataHandle();

	#ifdef DISPLAY_PROCESS_THE_LOCK_SIGNAL
	if(E_HAL_STATE_OK == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_LOCK_STATE,NULL))//read lock state
	{
		/* do nothing... */
	}
	else
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltFogDevId);//request to close device
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//request to close device
	}
	#endif

	if((E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_TFT_ENABLE_STATE,NULL)) && (E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_BL_ENABLE_STATE,NULL)))
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//allow to close device
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltFogDevId);//allow to close device
	}
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
*  Name        	: DisplayMgr_NoLockHandler
*  Description 	: Processing without lock signal
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#ifdef DISPLAY_PROCESS_THE_LOCK_SIGNAL
static void DisplayMgr_NoLockHandler(void)
{
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;
	if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_OPEN,&ltBLDevId))//check backlight device state
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//request to close backlight device
	}
	else
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//request to close backlight device
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltFogDevId);//request to close fog device
	}
}
#endif

/***********************************************************************
*  Name        	: DisplayMgr_NoLockHandler
*  Description 	: Processing for resume lock signal
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#ifdef DISPLAY_PROCESS_THE_LOCK_SIGNAL
static void DisplayMgr_LockResumeHandler(void)
{
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;
	if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_OPEN,&ltFogDevId))//check fog device state
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_OPEN_DEVICE,&ltBLDevId);//allow to open device
		DeratingMgr_Init();//clear message
	}
	else
	{
		(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_OPEN_DEVICE,&ltFogDevId);//allow to open device
	}
}
#endif

/***********************************************************************
*  Name        	: DisplayMgr_CheckBrnState
*  Description 	: Check the brightness related control signal is valid
*  Parameter   	: brightness
*  Returns     	: Valid returns TRUE, otherwise returns FALSE
***********************************************************************/
static MYBOOL DisplayMgr_CheckBrnState(void)
{
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;
	MYBOOL lmbRet = TRUE;

	if((E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_TFT_ENABLE_STATE,NULL)) \
		&& (E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_BL_ENABLE_STATE,NULL)))
	{
		if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_CLOSE,&ltBLDevId))
		{
			(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltFogDevId);//allow to close device
		}
		else
		{
			(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//allow to close device
		}
		lmbRet = FALSE;
	}
	else
	{
		if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_OPEN,&ltFogDevId))
		{
			if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_OPEN,&ltBLDevId))
			{
				if((E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_TFT_ENABLE_STATE,NULL)) \
					|| (E_HAL_STATE_FAIL == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_BL_ENABLE_STATE,NULL)))
				{
					lmbRet = FALSE;
				}
				else
				{
					lmbRet = DisplayMgr_CycleCheckBrn();//cycle detect brightness
				}
			}
			else
			{
				(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_OPEN_DEVICE,&ltBLDevId);//allow to open device
				lmbRet = FALSE;
			}
		}
		else
		{
			(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_OPEN_DEVICE,&ltFogDevId);//allow to open device
			lmbRet = FALSE;
		}
		
	}

	return lmbRet;
}

/***********************************************************************
*  Name        	: DisplayMgr_NormalActiveHandler
*  Description 	: 10ms task in normal mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_NormalActiveHandler(void)
{
	DEV_BRN_16BIT_T ltTargetBrnVal = DEV_BL_BRN_MIN;
	DEV_BRN_16BIT_T ltCurrBrnVal = DEV_BL_BRN_MAX;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;
#ifdef DISPLAY_PROCESS_THE_LOCK_SIGNAL
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	E_TEMPERATURE_STATE lePrevTempState = E_TEMPERATURE_STATE_NORMAL;
	E_TEMPERATURE_STATE leCurrTempState = E_TEMPERATURE_STATE_NORMAL;

	if(E_HAL_STATE_OK == HALBlock_IoControl(LVDS_DEVICE_ID,LVDS_FUNC_LOCK_STATE,NULL))//check lock signal
	{
		/* check fog && backlight device state */
		if((E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_CLOSE,&ltFogDevId)) || \
			(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_CLOSE,&ltBLDevId)))
		{
			ltTargetBrnVal = 0U;//waiting device open to set brightness
			DisplayMgr_LockResumeHandler();
		}
		else
		{ 
			CS_SIG_SyncReceive(SIG_ENUM_TEMPERATURE_STATE,&lePrevTempState);//get temperature status first
			CS_SIG_SyncReceive(SIG_SHORT_PWM_FROM_HU,&ltCurrBrnVal);//get last brightness
			ltTargetBrnVal = DeratingMgr_LimitBrn(ltCurrBrnVal);//get max limit brightness
			CS_SIG_SyncReceive(SIG_ENUM_TEMPERATURE_STATE,&leCurrTempState);//get the latest temperature status
			if(leCurrTempState != lePrevTempState)//check temperature state
			{
				/* update to HU */
			}
			else
			{
				/* do nothing... */
			}			
		}
	}
	else/* no lock signal */
	{
		ltTargetBrnVal = 0U;//close backlight brightness
		DisplayMgr_NoLockHandler();
	}
#else
	CS_SIG_SyncReceive(SIG_SHORT_PWM_FROM_HU,&ltCurrBrnVal);//get pwm value
	ltTargetBrnVal = DeratingMgr_LimitBrn(ltCurrBrnVal);//get max limit brightness
	if(DisplayMgr_CheckBrnState() == FALSE)
	{
		/* clear counter */
		subCheckBrightnessCnt = 0;
		subBriErrCnt = 0;
		if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_DEVICE_IS_OPEN,&ltBLDevId))
		{
			DisplayMgr_GradientBrn(0);//update brightness
		}
		else
		{
			/* do nothing... */
		}
		
	}
	else
	{
		DisplayMgr_GradientBrn(ltTargetBrnVal);//update brightness
	}
#endif
}

/***********************************************************************
*  Name        	: DisplayMgr_NormalExitHandler
*  Description 	: Exit Handler in normal mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_NormalExitHandler(void)
{
	DEV_ID_T ltFogDevId = FOG_DEVICE_ID;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;

	(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_RELEASE_DEVICE,&ltFogDevId);//request to release device
	(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_RELEASE_DEVICE,&ltBLDevId);//request to release device
}


/***********************************************************************
*  Name        	: DisplayMgr_EOLEntryHandler
*  Description 	: Entry EOL Handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_EOLEntryHandler(void)
{
	DEV_BRN_16BIT_T ltEolReqBrightness = DEV_BL_BRN_MAX;

	CS_SIG_SyncSend(SIG_SHORT_COMM_REQ_BRN_VAL,&ltEolReqBrightness);

	DisplayMgr_UpdateBrn(ltEolReqBrightness);
}

/***********************************************************************
*  Name        	: DisplayMgr_EOLActiveHandler
*  Description 	: cycle task in EOL active state
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DisplayMgr_EOLActiveHandler(void)
{
	DEV_BRN_16BIT_T ltEolReqBrightness = 0U;

	CS_SIG_SyncReceive(SIG_SHORT_COMM_REQ_BRN_VAL,&ltEolReqBrightness);

	DisplayMgr_UpdateBrn(ltEolReqBrightness);
}

/***********************************************************************
*  Name        	: DisplayMgr_UpdateBrn
*  Description 	: Update brightness
*  Parameter   	: brightness:brightness value
*  Returns     	: TRUE:update sucessful,FALSE:update fail
***********************************************************************/
static MYBOOL DisplayMgr_UpdateBrn(DEV_BRN_16BIT_T brightness)
{

	DEV_BRN_16BIT_T ltCurrBrnVal = DEV_BL_BRN_MIN;
	SIGNAL_CHAR_8BIT_T ltBrnRatioVal = 0U;
	MYBOOL lmbRet = TRUE;
#ifndef GRADIENT_FUNCTION
	DEV_BRN_16BIT_T ltTargetBrnVal = DEV_BL_BRN_MIN;
	CS_SIG_SyncReceive(SIG_SHORT_CURR_BRN_VAL,&ltCurrBrnVal);
	if(brightness == ltCurrBrnVal)
	{
		/* do nothing,exit */
	}
	else
	{
#endif	
		if(brightness == DEV_BL_BRN_MIN)
		{
			ltTargetBrnVal = DEV_BL_BRN_MIN;
		}
		else
		{
			/* The backlight brightness value is calculated according to the backlight weight */
			CS_SIG_SyncReceive(SIG_CHAR_BRIGHTNESS_RATIO,&ltBrnRatioVal);
			ltTargetBrnVal = brightness * ltBrnRatioVal / 100;//calculation brightness
		}
		
		if(E_HAL_STATE_OK == HALBlock_IoControl(BACKLIGHT_DEVICE_ID,BL_FUNC_SET_BRN,&ltTargetBrnVal))//set brightness
		{
#ifndef GRADIENT_FUNCTION			
			CS_SIG_SyncSend(SIG_SHORT_CURR_BRN_VAL,&brightness);//update brightness
#endif
		}
		else
		{
			lmbRet = FALSE;
		}
#ifndef GRADIENT_FUNCTION
	}
#endif
	return lmbRet;
}


/***********************************************************************
*  Name        	: DisplayMgr_CycleCheckBrn
*  Description 	: Check backlight chip's brightness value and handle 
*  Parameter   	: None
*  Returns     	: MYBOOL
***********************************************************************/
static MYBOOL DisplayMgr_CycleCheckBrn(void)
{
	DEV_BRN_16BIT_T ltCurrBrnVal = 0U;
	DEV_BRN_16BIT_T ltBLBrightness = 0U;
	SIGNAL_CHAR_8BIT_T ltBrnRatioVal = 0U;
	DEV_ID_T ltBLDevId = BACKLIGHT_DEVICE_ID;
	MYBOOL lmbRet = TRUE;

	if(subCheckBrightnessCnt < BL_BRIGHTNESS_CHECK_FILTER_TIME)
	{
		subCheckBrightnessCnt++;
	}
	else
	{
		subCheckBrightnessCnt = 0;
		if(E_HAL_STATE_OK == HALBlock_IoControl(BACKLIGHT_DEVICE_ID,BL_FUNC_READ_BRN,&ltBLBrightness))
		{
			CS_SIG_SyncReceive(SIG_SHORT_CURR_BRN_VAL,&ltCurrBrnVal);
			CS_SIG_SyncReceive(SIG_CHAR_BRIGHTNESS_RATIO,&ltBrnRatioVal);
			ltCurrBrnVal = ltCurrBrnVal * ltBrnRatioVal / 100;
			if(ltBLBrightness != ltCurrBrnVal)
			{
				subBriErrCnt++;
			}
			else
			{
				subBriErrCnt = 0;
			}
			HALBlock_IoControl(BACKLIGHT_DEVICE_ID,BL_FUNC_SET_BRN,&ltCurrBrnVal);
			if(subBriErrCnt >= BL_BRIGHTNESS_ERROR_TIMES)
			{
				subBriErrCnt = 0;
				lmbRet = FALSE;
				(void)HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_REQ_CLOSE_DEVICE,&ltBLDevId);//allow to restart device
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
	return lmbRet;
}

/***********************************************************************
*  Name        	: DisplayMgr_GradientBrn
*  Description 	: Brightness gradation processing
*  Parameter   	: brightness:target brightness value
*  Returns     	: None
***********************************************************************/
static void DisplayMgr_GradientBrn(DEV_BRN_16BIT_T brightness)
{

#ifdef GRADIENT_FUNCTION
	DEV_BRN_16BIT_T ltBrnDeviation = 0U;    //current brightness deviation
	DEV_BRN_16BIT_T ltBrnValue = 0U;

	if(brightness != ssGradientInfo.currVal)               //check backlight deviation
	{
		if(brightness == DEV_BL_BRN_MIN)//close immediately
		{
			if(DisplayMgr_UpdateBrn(brightness) == TRUE)
			{
				memset(&ssGradientInfo,0,sizeof(S_GRADIENT_INFO));//clear info
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			ltBrnDeviation = abs(brightness - ssGradientInfo.prevVal);
			/* calculate step */
			if(CHECK_BRN_IS_STEP1(ltBrnDeviation))
			{
				ssGradientInfo.step = BRN_GRADIENT_STEP1_VALUE;//step 1
			}
			else if(CHECK_BRN_IS_STEP2(ltBrnDeviation))
			{
				ssGradientInfo.step = BRN_GRADIENT_STEP2_VALUE;//step 2
			}
			else if(CHECK_BRN_IS_STEP3(ltBrnDeviation))
			{
				ssGradientInfo.step = BRN_GRADIENT_STEP3_VALUE;//step 3
			}
			else
			{
				ssGradientInfo.step = BRN_GRADIENT_STEP4_VALUE;//step 4
			}
			
			if(brightness < ssGradientInfo.prevVal)
			{
				ssGradientInfo.step = 0 - ssGradientInfo.step;    //negation
			}
			else
			{
			/* do nothing... */
			}
			ssGradientInfo.currVal = brightness; 		//set target value
			ssGradientInfo.cnt = BRIGHTNESS_GRADIENT_FILTER_TIME;//Set the value for change to brightness operation immediately.
		}
		CS_SIG_SyncSend(SIG_SHORT_CURR_BRN_VAL,&brightness);//update brightness
	}
	else
	{
		/* do nothing... */
	}

	if(ssGradientInfo.currVal != ssGradientInfo.prevVal)
	{
		if(ssGradientInfo.cnt >= BRIGHTNESS_GRADIENT_FILTER_TIME)//check filter time
		{
			ssGradientInfo.cnt = 0U;
			if(ssGradientInfo.step < 0U)//negation
			{
				if(ssGradientInfo.prevVal + ssGradientInfo.step <= ssGradientInfo.currVal)    //overstep
				{
					ltBrnValue = ssGradientInfo.currVal;
				}
				else
				{
					ltBrnValue = ssGradientInfo.prevVal + ssGradientInfo.step;               //calculation brightness value
				}
			}
			else
			{
				if(ssGradientInfo.prevVal + ssGradientInfo.step >= ssGradientInfo.currVal)	//overstep
				{
					ltBrnValue = ssGradientInfo.currVal;
				}
				else
				{
					ltBrnValue = ssGradientInfo.prevVal + ssGradientInfo.step;               //calculate brightness value
				}
			}
			if(DisplayMgr_UpdateBrn(ltBrnValue) == TRUE)
			{
				ssGradientInfo.prevVal = ltBrnValue;										//update brightness to prevVal
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			ssGradientInfo.cnt++;
		}
	}
	else
	{
	/*do nothing... */
	}
#else
	 (void)DisplayMgr_UpdateBrn(brightness);
#endif
}


