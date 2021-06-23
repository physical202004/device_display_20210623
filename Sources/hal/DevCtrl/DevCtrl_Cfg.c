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

static U16 suhwNtcOpenShortCnt = 0U;//ntc timeout counter
static U8 subWaitAdcStableCnt = 0U;//waiting ADC filter stable,counter once

static void DevCtrl_WakeupSignalHandler(void);
static void DevCtrl_CaptureHandler(void);
static void DevCtrl_NTCHandler(void);

#ifdef USE_ADC_INTERRUPT_DETECT
static MYBOOL smbOpenAdcIntState = FALSE;//if need open adc interrupt detect function, set to TRUE
static void DevCtrl_AdcIntCallback(void);
static void DevCtrl_AdcIntHandler(void);
static void DevCtrl_AdcIntControl(MYBOOL state);

/***********************************************************************
* Name          : DevCtrl_AdcInt_Callback
* Description   : Callback for ADC interrupt
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_AdcIntCallback(void)
{
	MYBOOL lmbAdcDetectState = TRUE;
	CS_SIG_SyncSend(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);//set adc interrupt detect state
	Exti_Adapt_Cmd(E_EXTI_PIN_05,DevCtrl_AdcIntCallback,FALSE);//disable interrupt
}
/***********************************************************************
* Name          : DevCtrl_AdcIntHandler
* Description   : Adc interrupt handler
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_AdcIntHandler(void)
{
	if(smbOpenAdcIntState == TRUE)
	{
		DevCtrl_AdcIntControl(TRUE);
		smbOpenAdcIntState = FALSE;
	}
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
* Name          : DevCtrl_AdcIntControl
* Description   : Adc interrupt control
* Parameter     : state:TRUE->enable interrupt,FALSE->disable interrupt
* Returns       : None
***********************************************************************/
static void DevCtrl_AdcIntControl(MYBOOL state)
{
	if(state == FALSE)
	{
		/*disable interrupt in callback */
	}
	else
	{
		Exti_Adapt_Cmd(E_EXTI_PIN_05,DevCtrl_AdcIntCallback,state);
	}
}
#endif

/***********************************************************************
* Name          : DevCtrl_WakeupSignalHandler
* Description   : Wakeup signal detection handler
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_WakeupSignalHandler(void)
{
#ifdef USE_WAKEUP_SIGNAL	
	MYBOOL lmbWakeupState = TRUE;
	if(DEV_WAKEUP_INACTIVE == DEV_IO_ASYNC_READ(IO_IN_WAKEUP))//read wakeup signal
	{
		lmbWakeupState = FALSE;
	}
	else
	{
		lmbWakeupState = TRUE;
	}
	CS_SIG_SyncSend(SIG_BOOL_WAKEUP_IS_ACTIVE,&lmbWakeupState);
#endif	
}

static void DevCtrl_CaptureHandler(void)
{
#ifdef USE_CAPTURE_FR_HU_FUNC
	SIGNAL_SHORT_16BIT_T ltCaptureVal = 0U;
	if(Capture_Adapt_IsFullEmpty(CAPTURE_UNIT,CAPTURE_CHANNEL) == TRUE)
	{
		if(LOW_LEVEL == DEV_IO_SYNC_READ(IO_IN_BL_PWM_FR_HU))
		{
			ltCaptureVal = DEV_BL_BRN_MIN;
		}
		else
		{
			ltCaptureVal = DEV_BL_BRN_MAX;
		}
	}
	else
	{
		ltCaptureVal = Capture_Adapt_GetDuty(CAPTURE_UNIT,CAPTURE_CHANNEL);
	}
	CS_SIG_SyncSend(SIG_SHORT_PWM_FROM_HU,&ltCaptureVal);
	TraceServ_SendPwmMsg(E_TRACE_PWM_CH1,ltCaptureVal,DEV_BL_BRN_MAX);//send to trace
	//TraceServ_SendStrWithUShortMsg("Capture value",&ltCaptureVal,1, E_TRACE_EVENT_SIGNAL);
	
#endif
}

/***********************************************************************
* Name          : DevCtrl_NTCHandler
* Description   : Detect NTC voltage and update the corresponding temperature
* Parameter     : None
* Returns       : None
***********************************************************************/
static void DevCtrl_NTCHandler(void)
{
	DEV_ADC_16BIT_T ltNtcAdcVal = 0U;
	U16 i = 0;
	U8 lubNtcTempVal = scubNtcTempVal[0];
	
	ltNtcAdcVal = DEV_ADC_ASYNC_READ(ADC_BL_TEMP_DET);
	/* If an open or short circuit abnormality occurs,recorded the IFC and set the temperature to a minimum */
	if(ltNtcAdcVal >= DEV_NTC_OPEN_VALUE)
	{
		if(suhwNtcOpenShortCnt > DEV_NTC_RECORD_OPENSHORT_TIME)
		{
			lubNtcTempVal = NTC_TEMPERATURE_INVALID_STATE;//invalid state
		}
		else
		{
			suhwNtcOpenShortCnt++;
		}
		DiagServ_AddIfcInfo(IFC_BL_NTC_OPEN_CIRCUIT);
	}
	else if(ltNtcAdcVal <= DEV_NTC_SHORT_VALUE)
	{
		if(suhwNtcOpenShortCnt > DEV_NTC_RECORD_OPENSHORT_TIME)
		{	
			lubNtcTempVal = NTC_TEMPERATURE_INVALID_STATE;//invalid state
		}
		else
		{
			suhwNtcOpenShortCnt++;
		}
		DiagServ_AddIfcInfo(IFC_BL_NTC_SHORT_CIRCUIT);
	}
	else
	{
		suhwNtcOpenShortCnt = 0;//clear counter
		if(ltNtcAdcVal > sctNTCAdcVal[1])
		{
			lubNtcTempVal = scubNtcTempVal[0];//set lowest temperature
		}
		else if(ltNtcAdcVal <= sctNTCAdcVal[DEV_NTC_NUMBER - 1])
		{
			lubNtcTempVal = scubNtcTempVal[DEV_NTC_NUMBER - 1];//set highest temperature
		}
		else
		{
			for(i = 1;i < (DEV_NTC_NUMBER - 1);i++)
			{
				if((sctNTCAdcVal[i] >= ltNtcAdcVal) && (sctNTCAdcVal[i+1] < ltNtcAdcVal))
				{
					lubNtcTempVal = scubNtcTempVal[i];
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}

	CS_SIG_SyncSend(SIG_CHAR_NTC_TEMPERATURE,&lubNtcTempVal);//update the temperature of NTC
}

/***********************************************************************
* Name          : DevCtrl_DetectAllPowerVoltage
* Description   : Check whether all power supply voltage have been discharged
* Parameter     : None
* Returns       : TRUE:all adc channel voltage discharge complete,FALSE:there are channels have electricity.
***********************************************************************/
MYBOOL DevCtrl_DetectAllPowerVoltage(void)
{
	MYBOOL lmbRet = FALSE;
	DEV_ADC_16BIT_T ltDcDcVolt = DEV_ADC_SYNC_READ(ADC_3V3_SW_DET);
	DEV_ADC_16BIT_T ltLvdsVolt = DEV_ADC_SYNC_READ(ADC_LVDS_VDD_DET);
	DEV_ADC_16BIT_T ltFogVolt = DEV_ADC_SYNC_READ(ADC_TFT_VDD_DET);
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
	DEV_ADC_16BIT_T ltTpVolt = DEV_ADC_SYNC_READ(ADC_CTP_VDD_DET);
	if((ltDcDcVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD) \
		&& (ltLvdsVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD) \
		&& (ltFogVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD) \
		&& (ltTpVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD))//csd has TP module
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
	if((ltDcDcVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD) \
		&& (ltLvdsVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD) \
		&& (ltFogVolt < DEV_VOLTAGE_DISCHARGE_THRESHOLD))
#endif
	{
		lmbRet = TRUE;//discharge finished
	}
	else
	{
		lmbRet = FALSE;//waiting discharge
	}
	return lmbRet;
}

/***********************************************************************
* Name          : DevCtrl_ControlDcDcEnable
* Description   : Control DC/DC Enable level
* Parameter     : level:change level
* Returns       : None
***********************************************************************/
void DevCtrl_ControlDcDcEnable(U8 level)
{

#ifdef USE_ADC_INTERRUPT_DETECT	
	MYBOOL lmbAdcDetectState = FALSE;
	CS_SIG_SyncReceive(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);//receive adc interrupt detect state
#endif
	if(level == LOW_LEVEL)
	{
#ifdef USE_ADC_INTERRUPT_DETECT
		DevCtrl_AdcIntControl(FALSE);//close adc interrupt
		if(lmbAdcDetectState == TRUE)
		{
			DEV_IO_WRITE(IO_OUT_DCDC_ENABLE,level);
		}
		else
		{
			/* do nothing... */
		}		
#endif
	}
	else
	{
#ifdef USE_ADC_INTERRUPT_DETECT		
		//CS_SIG_SyncReceive(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);//receive adc interrupt detect state
		if(lmbAdcDetectState == TRUE)//ADC interrupt happen
		{
			lmbAdcDetectState = FALSE;
			DiagServ_AddIfcInfo(IFC_3V3_SW);//record IFC
			CS_SIG_SyncSend(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);//clear adc interrupt detect state
		}
		else
		{
			/* do nothing... */
		}
		smbOpenAdcIntState = TRUE;//set state first,open in next cycle
#endif		
		DEV_IO_WRITE(IO_OUT_DCDC_ENABLE,level);
	}
}

/***********************************************************************
* Name          : DevCtrl_TaskEntry_10ms
* Description   : 10ms cyclic Task handler for hardware resources
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_Hardware_TaskEntry_10ms(void)
{
	
	DevCtrl_IO_CycleTask_10ms();		//IO Cycle task
	DevCtrl_ADC_CycleTask_10ms();		//ADC Cycle task

	DevCtrl_WakeupSignalHandler();		//Detect wakeup signal
	DevCtrl_CaptureHandler();			//Capture pwm
	if(subWaitAdcStableCnt < DEV_ADC_MAX_FILTER_TIME)
	{
		subWaitAdcStableCnt++;
	}
	else
	{
		DevCtrl_SM_BatteryStateMachine();	//Detect battery voltage
		DevCtrl_NTCHandler();				//Detect NTC voltage
	}
#ifdef USE_ADC_INTERRUPT_DETECT	
	DevCtrl_AdcIntHandler();			//ADC interrupt handler
#endif
}


