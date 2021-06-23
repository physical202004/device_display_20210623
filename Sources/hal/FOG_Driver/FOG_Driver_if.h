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


#ifndef FOG_DRIVER_IF_H
#define FOG_DRIVER_IF_H

#define USE_FOG_DRIVER

#ifdef USE_FOG_DRIVER

#define FOG_DRIVER_ID            DEVBLOCK_FOG
#define FOG_DRIVER_NAME          "FOG Device"


#define FOG_DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"

#endif  //USE_FOG_DRIVER


#endif


