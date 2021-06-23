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
#ifndef DERATING_MANAGER_IF_H
#define DERATING_MANAGER_IF_H
#include "Global_Definition.h"

extern DEV_BRN_16BIT_T DeratingMgr_LimitBrn(DEV_BRN_16BIT_T brightness);
extern void DeratingMgr_Init(void);

#endif


