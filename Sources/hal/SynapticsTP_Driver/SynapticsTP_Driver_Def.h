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


#ifdef TP_CTRL_PASS_1
#define BEGIN_TP_CTRL           enum TPCtrlAct{
#define ADD_TP_CTRL(x)          x,
#define END_TP_CTRL             TP_CTRL_ACT_NUMBER};
#include "SynapticsTP_Driver_Cfg.h"
#endif



