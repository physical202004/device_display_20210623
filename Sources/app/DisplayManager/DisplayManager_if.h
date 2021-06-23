/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : display module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef DISPLAY_MANAGER_IF_H
#define DISPLAY_MANAGER_IF_H

extern void DisplayMgr_StandbyExitHandler(void);
extern void DisplayMgr_NormalEntryHandler(void);
extern void DisplayMgr_NormalActiveHandler(void);
extern void DisplayMgr_NormalExitHandler(void);
extern void DisplayMgr_EOLEntryHandler(void);
extern void DisplayMgr_EOLActiveHandler(void);

#endif

