/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Driver for IIC communication for NXP platform,use slave mode
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/08/28
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/28  Huang Haoxiong      Initial
*************************************************************************************************************/

#ifndef COMM_DRIVER_IF_H
#define COMM_DRIVER_IF_H

#define USE_COMM_DRIVER

#ifdef USE_COMM_DRIVER
#define COMM_DRIVER_ID            DEVBLOCK_COMM
#define COMM_DRIVER_NAME          "Comm Device"

#define COMM_DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"

#endif//USE_COMM_DRIVER

#endif//COMM_DRIVER_IF_H


