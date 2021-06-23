/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Capture driver
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/07/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2020/07/08  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "Capture_Adapt.h"



const static FTM_CHANNEL cseCaptureChannel = FTM_CH0;		   		//channel number must be 0 or 2
static U16 suhwCurrPeriodVal = 0U;									//current capture period value
static U16 suhwCurrDurationVal = 0U;								//current capture duration value
static U16 suhwPrevPeriodVal = 0U;									//last capture period value
static U16 suhwPrevDurationVal = 0U;								//last capture duration value
static U16 suhwCurrDutyVal = 0U;									//current duty value
static U16 suhwPrevMasterChVal = 0U;								//last master channel value

#ifdef USE_CAPTURE_FR_HU_FUNC
static U16 suhwCaptureCnt = 0U;										//calculation period and duration counter
static U8 subMasterEdgeVal = FTM_INPUTCAPTURE_DUALEDGE_RISINGEDGE;	//master edge config value
static MYBOOL smbCaptureInitState = FALSE;							//initial state

static void Capture_IrqHandler(void);
#endif

/***********************************************************************
*  Name        : Capture_Adapt_Init
*  Description : Initial Capture data
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void Capture_Adapt_Init(void)
{
#ifdef USE_CAPTURE_FR_HU_FUNC
	if(smbCaptureInitState == TRUE)//first initial need not disable channel
	{
		FTM_DisableChannelInt(FTM2, (cseCaptureChannel+1));
		NVIC_DisableIRQ(FTM2_IRQn);
		
		FTM2->CONTROLS[0].CnSC = 0;	 /* select detec edge */
		FTM2->CONTROLS[1].CnSC = 0;
	}	
	else
	{
		smbCaptureInitState = TRUE;//update state
	}
	
    /* configure slave ch and master ch+1, to measuer the input wave period */
    FTM_DualEdgeCaptureInit(FTM2,    
                      		cseCaptureChannel,                		/* channel */
                            FTM_INPUTCAPTURE_DUALEDGE_ONESHOT,     	/* one shot mode */
                            FTM_INPUTCAPTURE_DUALEDGE_RISINGEDGE,  	/* channel 0,  detect rising edge */
                            subMasterEdgeVal					  	/* channel 1 */
                            );
    FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV2);
    
    FTM_SetCallback(FTM2, Capture_IrqHandler);
	NVIC_EnableIRQ(FTM2_IRQn);
    FTM_EnableChannelInt(FTM2, (cseCaptureChannel+1));

#endif
}

/***********************************************************************
*  Name        : Capture_Adapt_Deinit
*  Description : Deinitial Capture data
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void Capture_Adapt_Deinit(void)
{
	FTM_DeInit(FTM2);
}

/***********************************************************************
*  Name        : Capture_Adapt_IsFullEmpty
*  Description : check the duty is empty or 100%
*  Parameter   : unit:FTM unit,channel:FTM channel
*  Returns     : MYBOOL
***********************************************************************/
MYBOOL Capture_Adapt_IsFullEmpty(FTM_UNIT unit,FTM_CHANNEL channel)
{
	U16 luhwMasterChVal = 0U;
	MYBOOL lbRet= FALSE;

    luhwMasterChVal = FTM2->CONTROLS[cseCaptureChannel + 1].CnV;
    if(suhwPrevMasterChVal == luhwMasterChVal)
    {
    	lbRet = TRUE;
    }
    else
    {
    	lbRet = FALSE;
    }
	suhwPrevMasterChVal = luhwMasterChVal;
	
    return lbRet;
}


/***********************************************************************
*  Name        : Capture_Adapt_CycleTask
*  Description : Cycle Task
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void Capture_Adapt_CycleTask(void)
{
#ifdef USE_CAPTURE_FR_HU_FUNC
	if(suhwCaptureCnt == 0)
	{
		subMasterEdgeVal = FTM_INPUTCAPTURE_DUALEDGE_RISINGEDGE;//set rising,capture period
		Capture_Adapt_Init();//reconfiguration capture
		suhwCaptureCnt++;
	}
	else if(suhwCaptureCnt == CAPTURE_CAL_PERIOD_CNT)
	{
		subMasterEdgeVal = FTM_INPUTCAPTURE_DUALEDGE_FALLINGEDGE;//set falling,capture duration
		Capture_Adapt_Init();//reconfiguration capture
		suhwCaptureCnt++;

	}
	else
	{
		/* do nothing... */
	}
#endif
}

/***********************************************************************
*  Name        : Capture_Adapt_GetDuty
*  Description : return current duty
*  Parameter   : unit:FTM unit,channel:FTM channel
*  Returns     : the duty, don't contain 0 or 100%. 0 and 100% duty 
*				 are checked by Capture_Adapt_IsFullEmpty function
***********************************************************************/
U16 Capture_Adapt_GetDuty(FTM_UNIT unit,FTM_CHANNEL channel)
{
	U32 luwDurationVal = suhwCurrDurationVal;//get value first
	U32 luwPeriodVal = suhwCurrPeriodVal;//get value first
	
	if((abs(luwPeriodVal - suhwPrevPeriodVal) >= CAPTURE_FILTER_VAL) \
	|| (abs(luwDurationVal - suhwPrevDurationVal) >= CAPTURE_FILTER_VAL))//check deviation
	{
		
		suhwPrevDurationVal = luwDurationVal;//update value
		suhwPrevPeriodVal = luwPeriodVal;//update value
		suhwCurrDutyVal = luwDurationVal * CAPTURE_PERIOD_MAX / luwPeriodVal;//calculation duty
		
	}
	else
	{
		// don't calculation,return last value
	}

	
	UNUSED(unit);
	UNUSED(channel);
 	return suhwCurrDutyVal;
}

/********************** interrupt callback **********************/
#ifdef USE_CAPTURE_FR_HU_FUNC
/***********************************************************************
*  Name        : Capture_IrqHandler
*  Description : irq handler
*  Parameter   : 
*  Returns     : None
***********************************************************************/
static void Capture_IrqHandler(void)
{
	U16 luhwSlaveChVal = 0U;
	U16 luhwMasterChVal = 0U;
	FTM_ClrChannelFlag(FTM2, cseCaptureChannel);
	FTM_ClrChannelFlag(FTM2, cseCaptureChannel + 1);
	luhwSlaveChVal = FTM2->CONTROLS[cseCaptureChannel].CnV;
    luhwMasterChVal = FTM2->CONTROLS[cseCaptureChannel + 1].CnV;
	if((suhwCaptureCnt == CAPTURE_CAL_PERIOD_CNT) || (suhwCaptureCnt == 0))
	{
		/* doing in task */
	}
	else if(suhwCaptureCnt < CAPTURE_CAL_PERIOD_CNT)//calculation period
	{
		suhwCurrPeriodVal = luhwMasterChVal - luhwSlaveChVal;//update value
		suhwCaptureCnt++;
	}
	else if(suhwCaptureCnt > CAPTURE_CAL_PERIOD_CNT)//calculation duration
	{
		if(suhwCaptureCnt > CAPTURE_RECAL_PERIOD_CNT)//counter overflow,recalculation period
		{
			suhwCaptureCnt = 0;//reset counter
		}
		else
		{
			suhwCaptureCnt++;
		}
		suhwCurrDurationVal = luhwMasterChVal - luhwSlaveChVal;//update value

	}
    /* restart the dual edge capture */
    if(cseCaptureChannel == 0)
    {
        FTM2->COMBINE |=  FTM_COMBINE_DECAP0_MASK | FTM_COMBINE_DECAPEN0_MASK;
    }
    else if(cseCaptureChannel == 2)
    {
        FTM2->COMBINE |=  FTM_COMBINE_DECAP1_MASK | FTM_COMBINE_DECAPEN1_MASK;
    }
    else if(cseCaptureChannel == 4)
    {
        FTM2->COMBINE |=  FTM_COMBINE_DECAP2_MASK | FTM_COMBINE_DECAPEN2_MASK;
    }
	else
	{
		FTM2->COMBINE |=  FTM_COMBINE_DECAP0_MASK | FTM_COMBINE_DECAPEN0_MASK;/* default value */
	}
}
#endif

