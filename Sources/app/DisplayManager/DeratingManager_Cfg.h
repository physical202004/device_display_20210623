/***********************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           	*
* All Rights Reserved.             									*
* Department	: ADS R&D SW		                   				*
* AUTHOR	   	: Huang Haoxiong                    				*
************************************************************************************************
* Object        :
* Module        : Display
* Instance      :
* Description   : Derating curve function
*---------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/05/22
* Author        : Huang Haoxiong
**************************************************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------
*  0.1    2019/05/22  Huang Haoxiong      Init
*************************************************************************************************/

BEGIN_DERATING_CTRL
/* 				temperature,	brightness percentage*/
ADD_DERATING_CTRL(83,			(DEV_BL_BRN_MAX * 1))
ADD_DERATING_CTRL(84,			(DEV_BL_BRN_MAX * 1))
ADD_DERATING_CTRL(85,			(DEV_BL_BRN_MAX * 1))
ADD_DERATING_CTRL(86,			(DEV_BL_BRN_MAX * 0.9))
ADD_DERATING_CTRL(87,			(DEV_BL_BRN_MAX * 0.8))
ADD_DERATING_CTRL(88,			(DEV_BL_BRN_MAX * 0.7))
ADD_DERATING_CTRL(89,			(DEV_BL_BRN_MAX * 0.6))
ADD_DERATING_CTRL(90,			(DEV_BL_BRN_MAX * 0.5))
ADD_DERATING_CTRL(91,			(DEV_BL_BRN_MAX * 0.4))
ADD_DERATING_CTRL(92,			(DEV_BL_BRN_MAX * 0.3))
ADD_DERATING_CTRL(93,			(DEV_BL_BRN_MAX * 0.2))
ADD_DERATING_CTRL(94,			(DEV_BL_BRN_MAX * 0.2))
ADD_DERATING_CTRL(95,			(DEV_BL_BRN_MAX * 0.1))
ADD_DERATING_CTRL(96,			(DEV_BL_BRN_MAX * 0.1))
ADD_DERATING_CTRL(97,			(DEV_BL_BRN_MAX * 0.1))
END_DERATING_CTRL

		
#undef BEGIN_DERATING_CTRL
#undef ADD_DERATING_CTRL
#undef END_DERATING_CTRL



