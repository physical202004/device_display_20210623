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


#ifdef DS90UB948_CTRL_PASS_1
#define BEGIN_DS90UB948_CTRL           enum DS90UB948CtrlAct{
#define ADD_DS90UB948_CTRL(x)          x,
#define END_DS90UB948_CTRL             DS90UB948_CTRL_ACT_NUMBER};
#include "DS90UB948_Driver_Cfg.h"
#endif



