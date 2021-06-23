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
* ----------------------------------------------------------------------
* 0.1    Jun 26,2016  PEI Jingli  Initial version. Include config file
* ----------------------------------------------------------------------
* 0.2   Nov 17,2016  PEI Jingli  Including clock_cfg.h
* ----------------------------------------------------------------------
* 0.3   Dec 29,2016  PEI Jingli  Delete Mcu_Cfg.h
*-----------------------------------------------------------------------
* 0.4   Jan 24,2017   ZhangJie   Fix Coverity warning.
*=====================================================================*/
#ifndef _BASICDRIVER_CFG_H_
#define _BASICDRIVER_CFG_H_ 
#include "IRQ_Cfg.h"
//#include "Clock_cfg.h"
//#include "GPIO_cfg.h"
//#include "Stbc_Cfg.h"
//#include "CSI_Cfg.h"
//#include "IIC_Cfg.h"
#include "Timer_adapt_cfg.h"
//#include "Gpio_Cfg.h"
//#include "EXTI_cfg.h"


/**** Definition of constants ****/

/**** Definition of types ****/

/**** Declaration of constants ****/
/**** Declaration of variables ****/


/**** Declaration of functions ****/

void MCU_HardwareInit(void);
void MCU_MainInit(void);
void MCU_EnterSleep(void);
void MCU_Wakeup(void);

#endif /*_BASICDRIVER_CFG_H_*/

/****************************** END OF FILE ***************************/

