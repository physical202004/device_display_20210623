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
#undef FOG_CTRL_PASS_1

BEGIN_FOG_CTRL
ADD_FOG_CTRL(FOG_VDD)
ADD_FOG_CTRL(FOG_RST)
ADD_FOG_CTRL(FOG_STBYB)
ADD_FOG_CTRL(FOG_BIAS)
ADD_FOG_CTRL(FOG_VCOM)
ADD_FOG_CTRL(FOG_VGH)
ADD_FOG_CTRL(FOG_VGL)
ADD_FOG_CTRL(FOG_CONFIG)
END_FOG_CTRL


#undef BEGIN_FOG_CTRL
#undef ADD_FOG_CTRL
#undef END_FOG_CTRL
#endif


