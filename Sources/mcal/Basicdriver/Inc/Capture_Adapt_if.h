/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department : CT AD                                                 *
*   AUTHOR     :                                                       *
************************************************************************
* Object        :
* Module        :	IIC
* Instance      :
* Description   :	
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    May 20,2019   WangShuyun
*=====================================================================*/
#ifndef CAPTURE_ADAPT_IF_H
#define CAPTURE_ADAPT_IF_H
#include "PWM_Adapt_if.h"
#include "Timer_Adapt_if.h"

#define CAPTURE_UNIT			FTM_U2
#define CAPTURE_CHANNEL			FTM_CH0

extern void Capture_Adapt_Init(void);
extern void Capture_Adapt_Deinit(void);
extern U16 Capture_Adapt_GetDuty(FTM_UNIT unit,FTM_CHANNEL channel);
extern MYBOOL Capture_Adapt_IsFullEmpty(FTM_UNIT unit,FTM_CHANNEL channel);

#endif
