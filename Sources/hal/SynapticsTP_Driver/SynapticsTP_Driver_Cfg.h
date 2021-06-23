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
#undef TP_CTRL_PASS_1

BEGIN_TP_CTRL
ADD_TP_CTRL(TP_VDD)
ADD_TP_CTRL(TP_RST)
ADD_TP_CTRL(TP_CONFIG)
END_TP_CTRL


#undef BEGIN_TP_CTRL
#undef ADD_TP_CTRL
#undef END_TP_CTRL
#endif


