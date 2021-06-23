/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
***************************************************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department : ADS AD                                                 *
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
* 0.1	   April 15,2020   Huang, Haoxiong  Initial version
****************************************************************************************************/

#ifndef  ADC_ADAPT_IF_H
#define  ADC_ADAPT_IF_H
#include "Define.h"
#include "SKEAZN642.h"
#include "adc.h"

/**** Definition of types ****/
/*
    The index of physical channel
*/
#define ADC_CHANNEL_MAX 16


/**** Declaration of functions ****/
extern void ADC_Adapt_Init(void);
extern void ADC_Adapt_Deinit(void);
extern void  ADC_Adapt_Conversion(void);
extern U16 ADC_Adapt_GetValue(U8 group,U8 channel);

#endif/*_ADC_IF_H_*/

