/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           *
*				  All Rights Reserved.             *
*   Department : ADS R&D SW		                   *
*   AUTHOR	   : Huang Haoxiong                    *
************************************************************************
* Object        :
* Module        : Backlight
* Instance      :
* Description   : Backlight Modules
*-----------------------------------------------------------------------
* Version       : 0.1
* Date          : 2018/08/22
* Author        : Huang Haoxiong
***********************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1    2018/08/22  Huang Haoxiong      Init
************************************************************************/
#ifndef LP8860_DRIVER_IF_H
#define LP8860_DRIVER_IF_H
#define USE_LP8860_DRIVER

#ifdef USE_LP8860_DRIVER

#define LP8860_DRIVER_ID            DEVBLOCK_LP8860
#define LP8860_DRIVER_NAME          "LP8860 Device"


#define BL_DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"


#endif  //USE_LP8860_DRIVER


#endif

