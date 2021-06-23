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
#undef LP8860_CTRL_PASS_1

BEGIN_LP8860_CTRL
ADD_LP8860_CTRL(LP8860_ONOFF)
ADD_LP8860_CTRL(LP8860_VDD)
ADD_LP8860_CTRL(LP8860_ENABLE)
ADD_LP8860_CTRL(LP8860_NSS)
ADD_LP8860_CTRL(LP8860_CONFIG)
END_LP8860_CTRL


#undef BEGIN_LP8860_CTRL
#undef ADD_LP8860_CTRL
#undef END_LP8860_CTRL
#endif

