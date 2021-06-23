/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : FOG
* Instance      :
* Description   : This driver is based on 10.25 inch FOG of ShenChao DE103IA-01A model
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/04
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2019/09/04  Huang Haoxiong      Initial
*************************************************************************************************************/


#ifndef FOG_DRIVER_H
#define FOG_DRIVER_H

#include "FOG_Driver_if.h"


#ifdef USE_FOG_DRIVER
#include "DevCtrl_if.h"
#include "Pwm_Adapt_if.h"

#define FOG_VCOM_TIMER_UNIT 			FTM_U0//unit
#define FOG_VCOM_TIMER_CHANNEL			FTM_CH0//channel
#define FOG_VCOM_TIMER_FREQ				2000//unit by hz
#define FOG_VCOM_TIMER_DEFAULT_VAL		(0x7851)//47%

#define FOG_VSP_AD_MIN		    (3243U)   //VSP ADC min value
#define FOG_VSP_AD_MAX		    (3584U)   //VSP ADC max value
#define FOG_VDD_AD_MIN		    (1946U)   //VDD ADC min value
#define FOG_VDD_AD_MAX		    (2150U)   //VDD ADC max value
#define FOG_VGL_AD_MIN		    (2496U)   //VGL ADC min value
#define FOG_VGL_AD_MAX		    (2759U)   //VGL ADC max value
#define FOG_VGH_AD_MIN		    (2170U)   //VGH ADC min value
#define FOG_VGH_AD_MAX		    (2398U)   //VGH ADC max value

#define FOG_CTRL_PASS_1
#include "FOG_Driver_Def.h"

extern void FOG_Driver_PowerSequenceInit(void);
extern MYBOOL FOG_Driver_ExecutePowerOnSequence(void);
extern MYBOOL FOG_Driver_ExecutePowerOffSequence(void);

extern S_DEV_INFO gsFOGDev;
extern U16 guhwVcomVal;
extern void FOG_Driver_CycleTask10ms(void);
extern MYBOOL FOG_Driver_DataInit(void);
extern MYBOOL FOG_Driver_DataDeinit(void);

#endif  //USE_FOG_DRIVER


#endif



