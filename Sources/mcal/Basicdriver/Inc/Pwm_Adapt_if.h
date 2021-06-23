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
#ifndef PWM_ADAPT_IF_H
#define PWM_ADAPT_IF_H
#include "define.h"
#include "Timer_Adapt_if.h"
/**** Definition of micros ****/



/**** Definition of macros ****/

/**** Declaration of constants ****/


/**** Declaration of variables ****/
 

/**** Declaration of functions ****/

extern void Pwm_Adapt_Init(FTM_UNIT ftmunit,FTM_CHANNEL channel,U32 freq,U16 duty);
extern void Pwm_Adapt_Deinit(FTM_UNIT ftmunit);
extern MYBOOL Pwm_Adapt_SetDuty(FTM_UNIT  ftmunit,FTM_CHANNEL channel,U16 duty);
extern U32 Pwm_Adapt_GetFrequency(FTM_UNIT  ftmunit, FTM_CHANNEL channel);


#endif/*_PWMD_IF_H_*/
/**************** END OF FILE *****************************************/

