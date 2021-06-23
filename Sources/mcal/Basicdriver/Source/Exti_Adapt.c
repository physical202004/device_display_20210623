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
#include "Exti_Adapt.h"

static const EXTI_MODULE sceExitModule = E_EXTI_MODULE_01;
/***********************************************************************
*  Name        : Exti_Adapt_Cmd
*  Description : enable/disable interrupt
*  Parameter   : intp:interrupt number
				 isrFunc:callback
                 state :ENABLE/DISABLE
*  Returns     : None
***********************************************************************/
void Exti_Adapt_Cmd(EXTI_PIN intp,Func_Void_Proc isrFunc,FUNCTION_STATE state)

{
	KBI_ConfigType  lsKBIConfig={{0}};
	
    if(state == FALSE)
    {
        if(sceExitModule == E_EXTI_MODULE_00)
        {
            KBI_DeInit(KBI0);
        }
        else
        {
            KBI_DeInit(KBI1);
        }   
    }
    else
    {		

        lsKBIConfig.sBits.bMode = KBI_MODE_EDGE_ONLY;	
        lsKBIConfig.sBits.bIntEn = 1;   	       	
        lsKBIConfig.sPin[intp].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;	
        lsKBIConfig.sPin[intp].bEn = 1;  	
        if(sceExitModule == E_EXTI_MODULE_00)
        {
            KBI_SetCallback(KBI0, isrFunc);
            KBI_Init(KBI0, &lsKBIConfig);
        }
        else
        {
            KBI_SetCallback(KBI1, isrFunc);	
            KBI_Init(KBI1, &lsKBIConfig);
        }
    }

}

