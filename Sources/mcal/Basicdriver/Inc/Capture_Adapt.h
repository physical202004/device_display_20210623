/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Capture driver
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
#ifndef CAPTURE_ADAPT_H
#define CAPTURE_ADAPT_H
#include "Define.h"
#include "DevCtrl_if.h"
#include "Capture_Adapt_Cfg.h"
#include "Capture_Adapt_if.h"



#define CAPTURE_PERIOD_MAX			(0xFFFFU)//Accuracy,16bit
#define CAPTURE_FILTER_VAL			(4)//Deviation rate,>0.5%
#define CAPTURE_CAL_PERIOD_CNT 		(10)//
#define CAPTURE_RECAL_PERIOD_CNT 	(100)//

#endif

