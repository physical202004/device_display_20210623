/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*-----------------------------------------------------------------------
* 0.1  Dec 19,2018  Huang Haoxiong  Initial version

*=====================================================================*/


#ifdef HAL_PASS_1
#undef HAL_PASS_1
#define BEGIN_DEVCHAR           enum DEVCHAR_Name {
#define ADD_DEVCHAR(x,y)        x,
#define END_DEVCHAR             E_DEVCHAR_NUM};

#define BEGIN_DEVBLOCK          enum DEVBLOCK_Name {E_DEVBLOCK_START = (E_DEVCHAR_NUM - 1),
#define ADD_DEVBLOCK(x,y)       x,
#define END_DEVBLOCK            E_DEVBLOCK_NUM};

#define HAL_DEVCHAR_MAX         ((unsigned char)(E_DEVCHAR_NUM))
#define HAL_DEVBLOCK_MAX  	    ((unsigned char)(E_DEVBLOCK_NUM  - E_DEVBLOCK_START - 1))
#include "Hal_Cfg.h"
#endif

#ifdef HAL_PASS_2
#undef HAL_PASS_2
#define BEGIN_DEVCHAR
#define ADD_DEVCHAR(x,y)   		extern cdev y;
#define END_DEVCHAR

#define BEGIN_DEVBLOCK
#define ADD_DEVBLOCK(x,y)		extern bdev y;
#define END_DEVBLOCK
#include "Hal_Cfg.h"
#endif

#ifdef HAL_CHAR_PASS_1
#undef HAL_CHAR_PASS_1
#define BEGIN_DEVCHAR			static cdev *ssDevChar[HAL_DEVCHAR_MAX] = {
#define ADD_DEVCHAR(x,y)       	&y,
#define END_DEVCHAR				};

#define BEGIN_DEVBLOCK
#define ADD_DEVBLOCK(x,y)
#define END_DEVBLOCK 
#include "Hal_Cfg.h"
#endif


#ifdef HAL_BLOCK_PASS_1
#undef HAL_BLOCK_PASS_1
#define BEGIN_DEVCHAR
#define ADD_DEVCHAR(x,y)
#define END_DEVCHAR

#define BEGIN_DEVBLOCK          static bdev *ssDevBlock[HAL_DEVBLOCK_MAX] = {
#define ADD_DEVBLOCK(x,y)   	&y,
#define END_DEVBLOCK         	};
#include "Hal_Cfg.h"
#endif

