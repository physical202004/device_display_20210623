/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : System manager
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/04
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2019/09/04  Huang Haoxiong      Initial
*************************************************************************************************************/


#include "SystemManager.h"


/***********************************************************************
* Name          : SystemMgr_CyclicContainer_10ms
* Description   : Execute the function for written
* Parameter     : None
* Returns       : None
***********************************************************************/
void SystemMgr_CyclicContainer_10ms(void)
{

	DevCtrl_CycleTask10ms();
	DiagServ_CycleTask10ms();
	TraceServ_CycleTask10ms();
}


