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
#ifndef DERATING_MANAGER_H
#define DERATING_MANAGER_H
#include "DeratingManager_if.h"

#define OPEN_BRIGHTNESS_TEMPERATURE				(85)	//open display temperature
#define	DERATING_MIN_ZERO_PER_BRN				(DEV_BL_BRN_MAX * 0)
#define	DERATING_MIN_FIVE_PER_BRN				(DEV_BL_BRN_MAX * 0.05)
#define DERATING_MIN_TEN_PER_BRN				(DEV_BL_BRN_MAX * 0.1)
//#define DERATING_MIN_BRN						DERATING_MIN_ZERO_PER_BRN
#define DERATING_MIN_BRN						DERATING_MIN_TEN_PER_BRN

//#define USE_DERATING_CLOSE_BRN_FUNC						//Whether to use the closing britghtness function in the derating mechanism

#define TEMPERATURE_BACKUP_NUM					(3)		//store history temperature number
#define CHECK_DERATING_CYCLE_TIME				(10U)	//processing derating cycle time,unit by 10ms
#define DELAY_CLOSE_BRIGHTNESS_TIME				(1000U)	//delay to close diaply in high temperature,unit by 10ms
#define HIGH_T_CLOSE_BRIGHTNESS_TIME			(3000U)	//delay to close brightness time,unit by 10ms
#define TEMPERATURE_NO_CHANGE					(0)		//temperature trend no change
#define TEMPERATURE_RISING						(1)		//temperature is rising
#define TEMPERATURE_FALLING						(2)		//temperature is falling

typedef enum{
	E_HIGH_T_NO_FILTER_STATE,							//normal state
	E_HIGH_T_FILTERING_STATE,							//filter in high temperature
	E_HIGH_T_FILTER_COMPLETED_STATE,					//filter timeout for 30s in high temperature
}E_HIGH_T_FILTER_STATE;

#define DERATING_CTRL_PASS_1
#include "DeratingManager_Def.h"

#define DERATING_CTRL_PASS_2
#include "DeratingManager_Def.h"

#define DERATING_CTRL_PASS_3
#include "DeratingManager_Def.h"

#endif 		/* DERATING_MANAGER_H */

