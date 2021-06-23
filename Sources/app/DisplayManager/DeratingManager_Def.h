/***********************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           	*
* All Rights Reserved.             									*
* Department	: ADS R&D SW		                   				*
* AUTHOR	   	: Huang Haoxiong                    				*
************************************************************************************************
* Object        :
* Module        : Display
* Instance      :
* Description   : Derating curve function
*---------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/05/22
* Author        : Huang Haoxiong
**************************************************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------
*  0.1    2019/05/22  Huang Haoxiong      Init
*************************************************************************************************/

#ifdef DERATING_CTRL_PASS_1
#undef DERATING_CTRL_PASS_1
#define BEGIN_DERATING_CTRL         	enum DeratingTemp{
#define ADD_DERATING_CTRL(a,b)     		DERATING_##a##_TEMP,
#define END_DERATING_CTRL             	DERATING_TEMP_NUMBER};
#include "DeratingManager_Cfg.h"
#endif

#ifdef DERATING_CTRL_PASS_2
#undef DERATING_CTRL_PASS_2
#define BEGIN_DERATING_CTRL         	static const DEV_TEMPERATURE_8BIT_T sctTemperature[DERATING_TEMP_NUMBER] = {
#define ADD_DERATING_CTRL(a,b)     		a,
#define END_DERATING_CTRL             	};
#include "DeratingManager_Cfg.h"
#endif

#ifdef DERATING_CTRL_PASS_3
#undef DERATING_CTRL_PASS_3
#define BEGIN_DERATING_CTRL         	static const DEV_BRN_16BIT_T sctBrightnessVal[DERATING_TEMP_NUMBER] = {
#define ADD_DERATING_CTRL(a,b)     		b,
#define END_DERATING_CTRL             	};
#include "DeratingManager_Cfg.h"
#endif


