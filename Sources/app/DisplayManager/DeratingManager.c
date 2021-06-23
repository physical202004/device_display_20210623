/***********************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           	*
* All Rights Reserved.             									*
* Department	: ADS R&D SW		                   				*
* AUTHOR	   	: Huang Haoxiong                    				*
************************************************************************************************
* Object        :
* Module        : Display
* Instance      :
* Description   : Derating curve function
*---------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/05/22
* Author        : Huang Haoxiong
**************************************************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------
*  0.1    2019/05/22  Huang Haoxiong      Init
*************************************************************************************************/
/* System-headerfiles */

/* Own headerfiles */
#include "DeratingManager.h"
#include "Global_Definition.h"

static void DeratingMgr_InitialHandler(void);
#ifdef USE_DERATING_CLOSE_BRN_FUNC
static DEV_BRN_16BIT_T DeratingMgr_DelayToCloseBrn(void);
#endif
static U8 DeratingMgr_CheckTrend(DEV_TEMPERATURE_8BIT_T oldT,DEV_TEMPERATURE_8BIT_T newT);
static void DeratingMgr_NormalHandler(void);
static DEV_BRN_16BIT_T DeratingMgr_GetMinimumBrn(void);
static void DeratingMgr_HighTDelayFilterState(void);
static void DeratingMgr_HighTDerating(U8 NTCTemperature);
static void DeratingMgr_HighTHandler(U8 NTCTemperature,U8 i);

static MYBOOL smbIsDeratingCloseState = FALSE;										//check if in derating mode and closed display,TRUE:closed display
static MYBOOL smbInDeratingState = FALSE;											//check if in derating mode and closed display,TRUE:closed display
static MYBOOL smbIsFisrtWorking = TRUE;												//check if it is the first time to read the NTC value
static U8 subTemperatureIndex = 0U;													//index of backup for temperature
static U8 subTemperatureTrend = TEMPERATURE_NO_CHANGE;								//temperature trend
static U16 suhwHighTCloseBrnCnt = 0U;												//delay to close brightness counter,default 30s
static U16 suhwCheckDeratingCnt = 0U;												//check Derating cycle counter
static U16 suhwDelayCloseCnt = 0U;													//delay to close display counter
static U16 suhwCurrLimitBrn = DEV_BL_BRN_MAX;										//current limit brightness value
static U16 suhwPrevLimitBrn = 0U;													//store previous brightness before high temperature need to close display state
static DEV_TEMPERATURE_8BIT_T stTemperatureBackup[TEMPERATURE_BACKUP_NUM] = {0};	//backup for temperature
static E_HIGH_T_FILTER_STATE seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;			//filter state in high temperature
const static U16 scuhwMinBrightness = DERATING_MIN_BRN;								//minmum brightness value


/***********************************************************************
*  Name        	: DeratingMgr_Init
*  Description 	: Initial data
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void DeratingMgr_Init(void)
{
	/* clear data */
	smbIsDeratingCloseState = FALSE;
	smbInDeratingState = FALSE;
	smbIsFisrtWorking = TRUE;
	subTemperatureIndex = 0U;
	suhwHighTCloseBrnCnt = 0U;
	suhwDelayCloseCnt = 0U;
	suhwCheckDeratingCnt = 0U;
	subTemperatureTrend = TEMPERATURE_NO_CHANGE;
	suhwCurrLimitBrn = DEV_BL_BRN_MAX;
	suhwPrevLimitBrn = DeratingMgr_GetMinimumBrn();
	seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;
	memset(stTemperatureBackup,0,TEMPERATURE_BACKUP_NUM);
}


/***********************************************************************
*  Name        	: DeratingMgr_GetMinimumBrn
*  Description 	: get the minimum brightness,except zero
*  Parameter   	: None
*  Returns     	: brightness
***********************************************************************/
static DEV_BRN_16BIT_T DeratingMgr_GetMinimumBrn(void)
{
	int i = 0;
	DEV_BRN_16BIT_T ltBrightness = 0U;
	for(i = DERATING_TEMP_NUMBER - 1;i > 0U;i--)
	{
		if(sctBrightnessVal[i] != scuhwMinBrightness)//find minimum brightness
		{
			ltBrightness = sctBrightnessVal[i];
			break;
		}
		else
		{
			continue;
		}
	}

	return ltBrightness;
}

/***********************************************************************
* Name   		: DeratingMgr_HighTDelayFilterState
* Description 	: Counter for need to close brightness in high temperature state
* Parameter   	: None
* Returns     	: TRUE:can close brightness,FALSE:can't close brightness
***********************************************************************/
static void DeratingMgr_HighTDelayFilterState(void)
{
	if((seHighTFilterState == E_HIGH_T_NO_FILTER_STATE) || (seHighTFilterState == E_HIGH_T_FILTER_COMPLETED_STATE))
	{
		suhwHighTCloseBrnCnt = 0;
	}
	else
	{
		/* do nothing... */
	}

	if(seHighTFilterState == E_HIGH_T_FILTER_COMPLETED_STATE)
	{
		/* do nothing... */
	}
	else
	{
		if(suhwHighTCloseBrnCnt < HIGH_T_CLOSE_BRIGHTNESS_TIME)
		{
			suhwHighTCloseBrnCnt++;
			seHighTFilterState = E_HIGH_T_FILTERING_STATE;
		}
		else
		{
			suhwHighTCloseBrnCnt = 0;
			seHighTFilterState = E_HIGH_T_FILTER_COMPLETED_STATE;
			TraceServ_SendStrMsg("High Temprature filter timeout!", E_TRACE_EVENT_SIGNAL);
		}
	}
}
#ifdef USE_DERATING_CLOSE_BRN_FUNC
/***********************************************************************
*  Name        	: DeratingMgr_DelayToCloseBrn
*  Description 	: Delay to close brightness handler
*  Parameter   	: None
*  Returns     	: brightness
***********************************************************************/
static DEV_BRN_16BIT_T DeratingMgr_DelayToCloseBrn(void)
{
	DEV_BRN_16BIT_T ltBrightness = 0U;

	if(suhwDelayCloseCnt < DELAY_CLOSE_BRIGHTNESS_TIME)//delay to close dispaly
	{
		suhwDelayCloseCnt++;
		ltBrightness = DeratingMgr_GetMinimumBrn();//use minimum brightness,except zero
	}
	else
	{
		ltBrightness = scuhwMinBrightness;
		TraceServ_SendStrMsg("High temprature to close the brightness!", E_TRACE_EVENT_SIGNAL);
	}
	return ltBrightness;
}
#endif

/***********************************************************************
*  Name        	: DeratingMgr_LimitBrn
*  Description 	: limit brightness handler
*  Parameter   	: brightness:target brightness value
*  Returns     	: None
***********************************************************************/
DEV_BRN_16BIT_T DeratingMgr_LimitBrn(DEV_BRN_16BIT_T brightness)
{

	E_TEMPERATURE_STATE leTemperatureState = E_TEMPERATURE_STATE_NORMAL;
	DEV_BRN_16BIT_T ltLimitBrightness = 0U;
	DEV_TEMPERATURE_8BIT_T ltNTCTemperature = 0U;
	
	CS_SIG_SyncReceive(SIG_CHAR_NTC_TEMPERATURE,&ltNTCTemperature);//get temperature

	if(TRUE == smbIsFisrtWorking)//first detect
	{
		smbIsFisrtWorking = FALSE;
		DeratingMgr_InitialHandler();
		if(suhwCurrLimitBrn == scuhwMinBrightness)
		{
			suhwPrevLimitBrn = DeratingMgr_GetMinimumBrn();
		}
		else
		{
			/* do nothing... */
		}
	}
	else
	{
		if(suhwCheckDeratingCnt < CHECK_DERATING_CYCLE_TIME)//cycle detect
		{
			suhwCheckDeratingCnt++;
		}
		else
		{
			suhwCheckDeratingCnt = 0;
			DeratingMgr_NormalHandler();
		}
	}
	
	ltLimitBrightness = suhwCurrLimitBrn;
	if(suhwCurrLimitBrn < DEV_BL_BRN_MAX)
	{
		smbInDeratingState = TRUE;
		if(suhwCurrLimitBrn == scuhwMinBrightness)	//cloes brightness
		{	
			DeratingMgr_HighTDelayFilterState();
			if(seHighTFilterState == E_HIGH_T_FILTER_COMPLETED_STATE)                     
			{
				suhwPrevLimitBrn = suhwCurrLimitBrn;				
#ifdef USE_DERATING_CLOSE_BRN_FUNC
				if(suhwCurrLimitBrn == 0U)
				{
					smbIsDeratingCloseState = TRUE;
					leTemperatureState = E_TEMPERATURE_STATE_HIGH;
					ltLimitBrightness = DeratingMgr_DelayToCloseBrn();					//high temperature, delay 10Sto close the brightness, in 10s period, use the min brightness
					//IFC_Adapter_AddConfirmedIfc(IFC_DERATING_SHUTDOWN);
				}
				else
				{
					leTemperatureState = E_TEMPERATURE_STATE_CLOSE_HIGH;
					ltLimitBrightness = scuhwMinBrightness;
				}
#else
				leTemperatureState = E_TEMPERATURE_STATE_CLOSE_HIGH;
				ltLimitBrightness = scuhwMinBrightness;
#endif
			}
			else if(seHighTFilterState == E_HIGH_T_FILTERING_STATE)
			{
				ltLimitBrightness = suhwPrevLimitBrn;//use the previous brightness , delay 30S
			}
			else
			{
				/* do nothing */
			}
			
		}
		else
		{
			seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;
			suhwHighTCloseBrnCnt = 0U;
#ifdef USE_DERATING_CLOSE_BRN_FUNC
	if((smbIsDeratingCloseState == TRUE) && (ltNTCTemperature > OPEN_BRIGHTNESS_TEMPERATURE)\
				&& (ltNTCTemperature != NTC_TEMPERATURE_INVALID_STATE))				//if temp back but under open screen temp,still run delay close brightness func
			{
				brightness = DeratingMgr_DelayToCloseBrn();
			}
			else
			{
				suhwDelayCloseCnt = 0U;
				smbIsDeratingCloseState = FALSE;
				leTemperatureState = E_TEMPERATURE_STATE_CLOSE_HIGH;
			}
#else
			suhwDelayCloseCnt = 0U;
			smbIsDeratingCloseState = FALSE;
			leTemperatureState = E_TEMPERATURE_STATE_CLOSE_HIGH;
#endif			
		}
		//TraceServ_SendStrMsg("in derating curve",E_TRACE_EVENT_SIGNAL);
	}
	else/* no limit brightness */
	{
		seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;
		smbInDeratingState = FALSE;
		suhwDelayCloseCnt = 0U;
		suhwHighTCloseBrnCnt = 0U;
		suhwPrevLimitBrn = suhwCurrLimitBrn;
#ifdef USE_DERATING_CLOSE_BRN_FUNC
		if(smbIsDeratingCloseState == TRUE)					//if happened close screen ,then back to normal temperature ,need record IFC
		{	
			smbIsDeratingCloseState = FALSE;
            TraceServ_SendStrMsg("exit derating curve",E_TRACE_EVENT_SIGNAL);
			//IFC_Adapter_AddConfirmedIfc(IFC_DERATING_RESTART);
		}
		else
		{
			/* do nothing... */
		}
#endif
		leTemperatureState = E_TEMPERATURE_STATE_NORMAL;
	}

	if(brightness <= ltLimitBrightness)		//get the minor temperature
	{
		/* do nothing... */
	}
	else
	{
		brightness = ltLimitBrightness;
	}		
	
	CS_SIG_SyncSend(SIG_ENUM_TEMPERATURE_STATE,&leTemperatureState);//update temperature state
	return brightness;
}

/***********************************************************************
*  Name         : DeratingMgr_InitialHandler
*  Description  : Initial and calculate derating value
*  Parameter    : None
*  Returns      : None
***********************************************************************/
static void DeratingMgr_InitialHandler(void)
{
 	int i = 0;
	DEV_TEMPERATURE_8BIT_T ltNTCTemperature = 0U;
	
	CS_SIG_SyncReceive(SIG_CHAR_NTC_TEMPERATURE,&ltNTCTemperature);//get temperature
	TraceServ_SendStrWithHexMsg("Startup temperature",&ltNTCTemperature,1, E_TRACE_EVENT_SIGNAL);
	if((ltNTCTemperature <= sctTemperature[0]) || (ltNTCTemperature == NTC_TEMPERATURE_INVALID_STATE))//no limit brightness
	{
		suhwCurrLimitBrn = DEV_BL_BRN_MAX;
	}
	else if(ltNTCTemperature >= sctTemperature[DERATING_TEMP_NUMBER - 1])//highest temperature
	{
		suhwCurrLimitBrn = scuhwMinBrightness;		//use minimun value 
	}
	else
	{
	 	for(i = 0 ;i < DERATING_TEMP_NUMBER;i++)
	 	{
			if(ltNTCTemperature == sctTemperature[i])	//compair temperature
			{
				suhwCurrLimitBrn = sctBrightnessVal[i];//get limit brightness
				break;
			}
			else
			{
				continue;
			}
	 	}
	}

}

/***********************************************************************
* Name   		: DeratingMgr_CheckTrend
* Description 	: Check temperature trend
* Parameter   	: oldT:last temperature
				  newT:current temperature
* Returns     	: Temperature trend state
***********************************************************************/
static U8 DeratingMgr_CheckTrend(DEV_TEMPERATURE_8BIT_T oldT,DEV_TEMPERATURE_8BIT_T newT)
{
	U8 lubTemperatureTrend = TEMPERATURE_NO_CHANGE;
	if(newT > oldT)
	{
	 	lubTemperatureTrend  = TEMPERATURE_RISING;//raise temperature
	 	TraceServ_SendStrWithHexMsg("Temperature change",&newT,1, E_TRACE_EVENT_SIGNAL);
 	}
 	else if(newT < oldT)
 	{
	 	lubTemperatureTrend  = TEMPERATURE_FALLING;//drop in temperature
	 	TraceServ_SendStrWithHexMsg("Temperature change",&newT,1, E_TRACE_EVENT_SIGNAL);
 	}
	else
	{
		/* temperature no change,return */	
	}
	return lubTemperatureTrend;
}

/***********************************************************************
* Name   		: DeratingMgr_NormalHandler
* Description 	: Derating handler after initial
* Parameter   	: None
* Returns     	: Limit brightness
***********************************************************************/
static void DeratingMgr_NormalHandler(void)
{
	DEV_TEMPERATURE_8BIT_T ltNTCTemperature = 0U;

	CS_SIG_SyncReceive(SIG_CHAR_NTC_TEMPERATURE,&ltNTCTemperature);//get temperature
	if((ltNTCTemperature <= sctTemperature[0]) || (ltNTCTemperature == NTC_TEMPERATURE_INVALID_STATE))//less than the lowest temperature
	{
		suhwCurrLimitBrn = DEV_BL_BRN_MAX;/* not limited brightness */
	}
	else if(ltNTCTemperature >= sctTemperature[DERATING_TEMP_NUMBER - TEMPERATURE_BACKUP_NUM])//high temperature
	{
		suhwCurrLimitBrn = scuhwMinBrightness;
	}
	else
	{
	    stTemperatureBackup[subTemperatureIndex] = ltNTCTemperature;//store in array

		if(subTemperatureIndex == 0U)//first temperature
		{
			subTemperatureIndex++;
		}
		else if(subTemperatureIndex < (TEMPERATURE_BACKUP_NUM - 1))//increase continue
		{
			subTemperatureTrend = DeratingMgr_CheckTrend(stTemperatureBackup[subTemperatureIndex - 1],stTemperatureBackup[subTemperatureIndex]);
			if(subTemperatureTrend != TEMPERATURE_NO_CHANGE)
			{
				subTemperatureIndex++;
			}
			else
			{
				/* if same will exit */
			}
		}
		else
		{
			DeratingMgr_HighTDerating(ltNTCTemperature);
		}
	}

}

/***********************************************************************
* Name   		: DeratingMgr_HighTDerating
* Description 	: High temperature Derating
* Parameter   	: NTCTemperature:NTC current temperature
* Returns     	: None
***********************************************************************/
static void DeratingMgr_HighTDerating(U8 NTCTemperature)
{
	U8 lubTemperatureTrend = TEMPERATURE_NO_CHANGE;
	U8 i = 0u;
	lubTemperatureTrend = DeratingMgr_CheckTrend(stTemperatureBackup[(TEMPERATURE_BACKUP_NUM - 2)],stTemperatureBackup[(TEMPERATURE_BACKUP_NUM - 1)]);
	if(lubTemperatureTrend == TEMPERATURE_NO_CHANGE)
	{
		/* if same will exit */
	}
	else
	{
		if(lubTemperatureTrend == subTemperatureTrend)//trend 2 times
		{
			for(i = 0U;i < DERATING_TEMP_NUMBER;i++)
			{
				if(NTCTemperature == sctTemperature[i])	//compair temperature
				{
					DeratingMgr_HighTHandler(NTCTemperature,i);
					break;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			subTemperatureTrend = lubTemperatureTrend;//update trend
		}

		/* set the second value as origin point,the third change to the second in array */
		for(i = 0U;i < (TEMPERATURE_BACKUP_NUM - 1);i++)
		{
			stTemperatureBackup[i] = stTemperatureBackup[i + 1];
		}
		subTemperatureIndex = i;
	}
}

/***********************************************************************
* Name   		: DeratingMgr_HighTHandler
* Description 	: High temperature Handler
* Parameter   	: NTCTemperature:NTC current temperature, i:the index of current temperature array
* Returns     	: None
***********************************************************************/
static void DeratingMgr_HighTHandler(U8 NTCTemperature,U8 i)
{
	if(NTCTemperature == sctTemperature[i])	//compair temperature
	{
		if(suhwCurrLimitBrn >= sctBrightnessVal[i])
		{
			suhwCurrLimitBrn = sctBrightnessVal[i];/* record limit brightness */
			if(suhwCurrLimitBrn != scuhwMinBrightness)
			{
				suhwPrevLimitBrn = suhwCurrLimitBrn;
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			if((seHighTFilterState == E_HIGH_T_FILTERING_STATE) && (suhwCurrLimitBrn == scuhwMinBrightness))  //if in filtering and current brightness is MIN
			{
				if(smbInDeratingState == TRUE)
				{
					suhwCurrLimitBrn = suhwPrevLimitBrn;
				}
				else
				{
					suhwCurrLimitBrn = sctBrightnessVal[i];
					seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;
				}
			}
			else
			{
				if(NTCTemperature <= OPEN_BRIGHTNESS_TEMPERATURE)//return to the open brightness temperature
				{
					suhwCurrLimitBrn = sctBrightnessVal[i];
					seHighTFilterState = E_HIGH_T_NO_FILTER_STATE;
				}
				else
				{
					/* do nothing... */
				}
			}
		}
	}
}

