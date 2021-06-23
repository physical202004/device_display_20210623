/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : watchdog adapt
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/08/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2020/08/08  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef WDG_ADAPT_H
#define WDG_ADAPT_H


/**** Definition of types ****/
enum
{
	WDG_START_SW=0,			//Writing an activation code to the WDTA trigger register starts WDTA
	WDG_START_DEFAULT		//counter starts after reset release.
};

enum
{
	WDG_FIXED=0,
	WDG_VARIABLE
};	

/**** Definition of constants ****/

/**** Definition of macros ****/

/**** Declaration of variables ****/

/**** Declaration of functions ****/

#endif /* WDG_ADAPT_H */

