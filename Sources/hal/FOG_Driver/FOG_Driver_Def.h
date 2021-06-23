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



#ifdef FOG_CTRL_PASS_1
#define BEGIN_FOG_CTRL           enum FOGCtrlAct{
#define ADD_FOG_CTRL(x)          x,
#define END_FOG_CTRL             FOG_CTRL_ACT_NUMBER};
#include "FOG_Driver_Cfg.h"
#endif



