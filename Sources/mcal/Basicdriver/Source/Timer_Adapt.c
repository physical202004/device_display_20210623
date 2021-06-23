/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Timer driver
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
#include "Timer_Adapt.h"


void TriggerTimer_Start(TIMER_CHANNEL chm, U32 cnt);
static void Timer_Adapt_Ch0ISR(void);
static void Timer_Adapt_Ch1ISR(void);

/***********************************************************************
*  Name        : Timer_Adapt_Init
*  Description : Initial Timer data
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void Timer_Adapt_Init(void)
{
	Capture_Adapt_Init();
}

/***********************************************************************
*  Name        : Timer_Adapt_Init
*  Description : Initial Timer data
*  Parameter   : channel:channel index 0~1
*                val:load value register.select the timeout period for the timer interrupts with bus clock.
*                max value 16000000 - 1
*  Returns     : None
***********************************************************************/
void Timer_Adapt_Start(TIMER_CHANNEL channel,U32 val)
{
	PIT_ConfigType  lsConfig = {0};

	/* configure PIT module in chain mode */
	/* PIT clock source is bus clock,16MHz */
	/* eg. PIT channel  load value = (16000000-1)*/
	lsConfig.u32LoadValue = val;
	lsConfig.bFreeze = FALSE;
	lsConfig.bModuleDis = FALSE;    /*!< enable PIT module */
	lsConfig.bInterruptEn = TRUE;
	lsConfig.bChainMode = TRUE;
	lsConfig.bTimerEn = TRUE;
	if(channel == TIMER_CH0)
    {
        PIT_SetCallback(PIT_CHANNEL0, Timer_Adapt_Ch0ISR);
        PIT_Init(PIT_CHANNEL0, &lsConfig);
    }
    else if(channel == TIMER_CH1)
    {
        PIT_SetCallback(PIT_CHANNEL1, Timer_Adapt_Ch1ISR);
        PIT_Init(PIT_CHANNEL1, &lsConfig);
    }
	else
	{
		/* return... */
	}

}

/***********************************************************************
*  Name        : Timer_Adapt_Ch0ISR
*  Description : the timer channel0 interrupt callback function
*  Parameter   : None
*  Returns     : None
***********************************************************************/
static void Timer_Adapt_Ch0ISR(void)
{
    PIT_ChannelClrFlags(PIT_CHANNEL0);
    if(gfTimerCallBack[TIMER_CH0] != NULL)
    {
        gfTimerCallBack[TIMER_CH0]();
    }
	else
	{
		/* return... */
	}
}

/***********************************************************************
*  Name        : Timer_Adapt_Ch0ISR
*  Description : the timer channel1 interrupt callback function
*  Parameter   : None
*  Returns     : None
***********************************************************************/
static void Timer_Adapt_Ch1ISR(void)
{
    PIT_ChannelClrFlags(PIT_CHANNEL1);
    if(gfTimerCallBack[TIMER_CH1] != NULL)
    {
        gfTimerCallBack[TIMER_CH1]();
    }
	else
	{
		/* return... */
	}
}

/***********************************************************************
*  Name        : SysTick_Handler
*  Description : system tick interrupt callback function
*  Returns     : None
***********************************************************************/
void SysTick_Handler(void)
{
	Active10msTask();
}

/***********************************************************************
*  Name        : SysTick_Stop
*  Description : stop system tick
*  Returns     : None
***********************************************************************/
void SysTick_Stop(void)
{
    SysTick->CTRL = 0x00;       //clear tick timer
    SysTick->VAL = 0x00;        //clear value
}


