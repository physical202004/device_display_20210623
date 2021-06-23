/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : EXTI driver
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
/**** Definition of types ****/
/* INTP:external interruput channel*/
#ifndef EXTI_ADAPT_IF_H
#define EXTI_ADAPT_IF_H
#include "Define.h"

typedef enum
{
    E_EXTI_PIN_00 = 0,
    E_EXTI_PIN_01,	
    E_EXTI_PIN_02,	
    E_EXTI_PIN_03,	
    E_EXTI_PIN_04,
    E_EXTI_PIN_05,
    E_EXTI_PIN_06,
    E_EXTI_PIN_07,
    E_EXTI_PIN_08,
    E_EXTI_PIN_09,	
    E_EXTI_PIN_10,	
    E_EXTI_PIN_11,	
    E_EXTI_PIN_12,
    E_EXTI_PIN_13,
    E_EXTI_PIN_14,
    E_EXTI_PIN_15,
    E_EXTI_PIN_NUM
}EXTI_PIN;

/* INTP trigger type*/
typedef enum
{
    EXTI_TRIGGER_LOW = 0,
    EXTI_TRIGGER_HIGH = 1,
    EXTI_TRIGGER_RISING = 1,
    EXTI_TRIGGER_FALLING = 0,
    EXTI_TRIGGER_RISFALL = 1,
}EXTI_TRIGGER_TYPE;


extern void Exti_Adapt_Cmd(EXTI_PIN intp,Func_Void_Proc isrFunc,FUNCTION_STATE state);
#endif

