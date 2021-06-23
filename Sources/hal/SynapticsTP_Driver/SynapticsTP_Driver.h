/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Synaptics TP Driver
* Instance      :
* Description   : Direct reading
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/08/22
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2020/08/22  Huang Haoxiong      Initial
*************************************************************************************************************/

#ifndef SYNAPTICS_TP_DRIVER_H
#define SYNAPTICS_TP_DRIVER_H

#include "SynapticsTP_Driver_if.h"


#ifdef USE_SYNAPTICS_TP_DRIVER
#include "DevCtrl_if.h"
#define TP_CTRL_PASS_1
#include "SynapticsTP_Driver_Def.h"

#define TP_AVDD_AD_MIN		    	(1946U)   //AVDD ADC min value
#define TP_AVDD_AD_MAX		    	(2150U)   //AVDD ADC max value
#define TP_VDD_AD_MIN		    	(1946U)   //VDD ADC min value
#define TP_VDD_AD_MAX		    	(2150U)   //VDD ADC max value


extern void TP_Driver_PowerSequenceInit(void);
extern MYBOOL TP_Driver_ExecutePowerOnSequence(void);
extern MYBOOL TP_Driver_ExecutePowerOffSequence(void);

extern S_DEV_INFO gsSynapticsTPDev;
extern void TP_Driver_CycleTask10ms(void);
extern MYBOOL TP_Driver_DataInit(void);

#endif  //USE_TP_DRIVER


#endif



