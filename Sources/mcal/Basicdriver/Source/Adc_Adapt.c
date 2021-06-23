/***************************************************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department :  ADS AD                                               *
*   AUTHOR     :                                                       *
****************************************************************************************************
* Object        :
* Module        :	ADC
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***************************************************************************************************/
/*-History--------------------------------------------------------------
* Version       Date        Name    Changes and comments
* 0.1	   Aug 21,2019   Huang, Haoxiong  Initial version
****************************************************************************************************/
#include "Adc_Adapt.h"

static ADC_ConfigType ssAdcConfig = {
	.u8ClockDiv = ADC_ADIV_DIVIDE_2,
	.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK,
	.u8Mode = ADC_MODE_12BIT,
	.sSetting.bContinuousEn = 1,
	.u16PinControl= 0xFFF8, /* Disable I/O control on ADC channel*/
};


/***********************************************************************
* Name        	: ADC_Adapter_Init
* Description 	: AD Initial
* Parameter   	: void
* Returns     	: None
***********************************************************************/
void ADC_Adapt_Init(void)
{
    ADC_Init(ADC,&ssAdcConfig);
}

/***********************************************************************
* Name        	: ADC_Adapter_DeInit
* Description 	: AD Deinitial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void ADC_Adapt_Deinit(void)
{
    ADC_DeInit(ADC);
}

/***********************************************************************
* Name        	: ADC_Adapt_Conversion
* Description 	: conversion all adc channel
* Parameter   	: group:adc group,channelId:adc Channel
* Returns     	: Channel corresponding value
***********************************************************************/
void ADC_Adapt_Conversion(void)
{
	/* reserved */
}


/***********************************************************************
* Name        	: ADC_Adapt_GetValue
* Description 	: Get ADC value From channel
* Parameter   	: group:adc group,channelId:adc Channel
* Returns     	: Channel corresponding value
***********************************************************************/
U16 ADC_Adapt_GetValue(U8 group,U8 channel)
{
    U16 luhwADValue = 0xFFFFU;

    if((channel >= ADC_CHANNEL_MAX))
    {
		/* do nothing... */
    }
    else
    {
        luhwADValue = ADC_PollRead(ADC, channel);
    }
	UNUSED(group);
    return luhwADValue;
}

