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
#include "Timer_Adapt_Cfg.h"
#include "Define.h"



Func_Void_Proc gfTimerCallBack[TIMER_CH_MAX] = 
{
    NULL,
    NULL
};



