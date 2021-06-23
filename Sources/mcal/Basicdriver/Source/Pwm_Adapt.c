/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : PWM driver
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
#include "Pwm_Adapt.h"

/***********************************************************************
*  Name        : Pwm_Adapt_Init
*  Description : Initialize the pwm module
*  Parameter   : ftmunit:ftm unit
*                channel:checked channel
*                freq:requency of PWM wave by hz
*                duty:decide the wave precision
*  Returns     : None
***********************************************************************/
 void Pwm_Adapt_Init(FTM_UNIT ftmunit,FTM_CHANNEL channel,U32 freq,U16 duty)
{
    FTM_ConfigType lsFTM_Config = {0};
    FTM_ChParamsType lsFTMCH_Config = {0};

	lsFTM_Config.modulo = (SYS_CLK_HZ / freq) - 1;
    lsFTM_Config.clk_source=FTM_CLOCK_SYSTEMCLOCK;
    lsFTM_Config.prescaler=FTM_CLOCK_PS_DIV1;
    lsFTM_Config.mode = 1;
    lsFTM_Config.toie = 0;

    lsFTMCH_Config.ctrl.bits.bMode=FTM_PWMMODE_EDGEALLIGNED;
    lsFTMCH_Config.ctrl.bits.bPWMPol=FTM_PWM_HIGHTRUEPULSE;
    //lsFTMCH_Config.u16CnV=((SYS_CLK_HZ - 1) * duty) / freq / Pwm_Adapt_DUTY_MAX;
    lsFTMCH_Config.u16CnV = freq;
    if(FTM_U0 == ftmunit)
    {
    	FTM_ChannelInit(FTM0,channel,lsFTMCH_Config);
		Pwm_Adapt_SetDuty(ftmunit,channel,duty);//set default duty
    	FTM_Init(FTM0,&lsFTM_Config);
    }
    else if(FTM_U1 == ftmunit)
    {
    	FTM_ChannelInit(FTM1,channel,lsFTMCH_Config);
		Pwm_Adapt_SetDuty(ftmunit,channel,duty);//set default duty
    	FTM_Init(FTM1,&lsFTM_Config);
    }
    else if(FTM_U2 == ftmunit)
    {
    	FTM_ChannelInit(FTM2,channel,lsFTMCH_Config);
		Pwm_Adapt_SetDuty(ftmunit,channel,duty);//set default duty
    	FTM_Init(FTM2,&lsFTM_Config);
    }
	else
	{
		/* invalid,return */
	}
}

/***********************************************************************
*  Name        : Pwm_Adapt_Deinit
*  Description : DeInitialize the pwm module
*  Parameter   : ftmunit  ftm unit
*  Returns     : None
***********************************************************************/
void Pwm_Adapt_Deinit(FTM_UNIT ftmunit)
{
    if(FTM_U0 == ftmunit)
    {
        FTM_DeInit(FTM0);
    }
    else if(FTM_U1 == ftmunit)
    {
        FTM_DeInit(FTM1);
    }
    else if(FTM_U2 == ftmunit)
    {
        FTM_DeInit(FTM2);
    }
	else
	{
		/* invalid */
	}
}


/***********************************************************************
*  Name        : Pwm_Adapt_SetDuty
*  Description : set the duty
*  Parameter   : ftmunit:ftm unit
*                channel:checked channel
*                duty:decide the wave precision
*  Returns     : MYBOOL
***********************************************************************/
MYBOOL Pwm_Adapt_SetDuty(FTM_UNIT ftmunit, FTM_CHANNEL channel,U16 duty)
{

	FTM_Type* pFTM = FTM0_BASE + (ftmunit * 0x1000);//get unit addr
	U16 luhwDutyValue = 0u;
	MYBOOL lmbRet = TRUE;

	if(ftmunit > FTM_U_MAX)
	{
		lmbRet = FALSE;
	}
	else
	{
		luhwDutyValue = (pFTM->MOD + 1) * duty / PWM_ADAPT_DUTY_MAX;
    	FTM_SetChannelValue(pFTM, channel, luhwDutyValue);
	}
    return lmbRet;
}

/***********************************************************************
*  Name        : Pwm_Adapt_GetFrequency
*  Description : get the frequency
*  Parameter   : ftmunit  ftm unit
*                      channel  checked channel
*  Returns     : clock frequency, HZ
***********************************************************************/
U32 Pwm_Adapt_GetFrequency(FTM_UNIT  ftmunit, FTM_CHANNEL channel)
{
    U32 luwPeriod_cnt = 0u;
    U32 luwfreq = 0u;
    if(FTM_U0 == ftmunit)
    {
        luwPeriod_cnt = FTM0->MOD;
        luwfreq = (SYS_CLK_HZ)/(luwPeriod_cnt + 1);
    }
    else if(FTM_U1 == ftmunit)
    {
        luwPeriod_cnt = FTM1->MOD;
        luwfreq = (SYS_CLK_HZ)/(luwPeriod_cnt + 1);
    }
    else if(FTM_U2 == ftmunit)
    {
        luwPeriod_cnt = FTM2->MOD;
        luwfreq = (SYS_CLK_HZ)/(luwPeriod_cnt + 1);
    }
	else
	{
		/* invalid */
	}
    return luwfreq;
}

