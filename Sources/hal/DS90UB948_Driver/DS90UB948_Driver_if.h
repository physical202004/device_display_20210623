/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Deserializer
* Instance      :
* Description   : 
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/27
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2019/09/27  Huang Haoxiong      Initial
*************************************************************************************************************/


#ifndef DS90UB948_DRIVER_IF_H
#define DS90UB948_DRIVER_IF_H

#define USE_DS90UB948_DRIVER

#ifdef USE_DS90UB948_DRIVER

#define DS90UB948_DRIVER_ID            DEVBLOCK_DS90UB948
#define DS90UB948_DRIVER_NAME          "DS90UB948 Device"


#define LVDS_DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"

#endif  //USE_DS90UB948_DRIVER


#endif


