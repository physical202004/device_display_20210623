/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : display module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H
#include "Global_Definition.h"

#define TFT_ENABLE_CHECK_FILTER_TIME		(7U)//70ms,unit by 10ms
#define BL_BRIGHTNESS_CHECK_FILTER_TIME		(50U)//500ms,unit by 10ms
#define BL_BRIGHTNESS_ERROR_TIMES		    (4U)//If the brightness cannot be read from the register for 4 times(4*500ms)

#ifdef GRADIENT_FUNCTION
typedef struct
{
	U8 cnt;
	short step;
	U16 prevVal;
	U16 currVal;
}S_GRADIENT_INFO;

#define BRIGHTNESS_GRADIENT_FILTER_TIME		(2U)//20ms,unit by 10ms

#define BRN_25_PERCENT						(DEV_BL_BRN_MAX * 0.25)//25%
#define BRN_50_PERCENT						(DEV_BL_BRN_MAX * 0.50)//50%
#define BRN_75_PERCENT						(DEV_BL_BRN_MAX * 0.75)//75%

#define BRN_GRADIENT_STEP1_VALUE			(DEV_BL_BRN_MAX * 0.01)//1%
#define BRN_GRADIENT_STEP2_VALUE			(DEV_BL_BRN_MAX * 0.02)//2%
#define BRN_GRADIENT_STEP3_VALUE			(DEV_BL_BRN_MAX * 0.03)//3%
#define BRN_GRADIENT_STEP4_VALUE			(DEV_BL_BRN_MAX * 0.04)//4%


#define CHECK_BRN_IS_STEP1(x)      			((x) <= BRN_25_PERCENT)
#define CHECK_BRN_IS_STEP2(x)      			(BRN_25_PERCENT  < (x)) && ((x) <= BRN_50_PERCENT)
#define CHECK_BRN_IS_STEP3(x)      			(BRN_50_PERCENT  < (x)) && ((x) <= BRN_75_PERCENT)
#define CHECK_BRN_IS_STEP4(x)      			(BRN_75_PERCENT  < (x)) && ((x) <= DEV_BL_BRN_MAX)
#endif

#endif



