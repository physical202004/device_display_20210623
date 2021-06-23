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
#ifndef TIMER_ADAPT_IF_H
#define TIMER_ADAPT_IF_H
#include "define.h"
/**** Definition of types ****/
typedef enum
{ 
    FTM_U0,
    FTM_U1,
    FTM_U2,
    FTM_U_MAX
}FTM_UNIT;

typedef enum
{ 
    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH_MAX
}FTM_CHANNEL;


typedef enum
{ 
    TIMER_CH0,
    TIMER_CH1,
    TIMER_CH_MAX
}TIMER_CHANNEL;


extern void Timer_Adapt_Init(void);
extern void Timer_Adapt_Start(TIMER_CHANNEL channel,U32 val);
#endif/* TIMER_ADAPT_IF_H */
/**************** END OF FILE *****************************************/

