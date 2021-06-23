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
#ifdef LP8860_CTRL_PASS_1
#define BEGIN_LP8860_CTRL           enum LP8860CtrlAct{
#define ADD_LP8860_CTRL(x)          x,
#define END_LP8860_CTRL             LP8860_CTRL_ACT_NUMBER};
#include "LP8860_Driver_Cfg.h"
#endif

