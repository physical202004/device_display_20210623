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

#ifndef SYNAPTICS_TP_DRIVER_IF_H
#define SYNAPTICS_TP_DRIVER_IF_H
#include "Define.h"
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#define USE_SYNAPTICS_TP_DRIVER
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif

#ifdef USE_SYNAPTICS_TP_DRIVER

#define SYNAPTICS_TP_DRIVER_ID            DEVBLOCK_SYNAPTICS_TP
#define SYNAPTICS_TP_DRIVER_NAME          "Synaptics TP Device"


#define TP_DEV_FUNC_PASS_1
#include "DevCtrl_Def.h"

#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#endif  //USE_SYNAPTICS_TP_DRIVER
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif

#endif


