/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department :  CT AD                                                 *
*   AUTHOR	   :										   *
************************************************************************
* Object        :
* Module        : WATCHDOG
* Instance      :
* Description   : Hardware watch dog module.
			   note, WDT_Enable must be set in option byte.
*-----------------------------------------------------------------------
* Version: 0.1
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*------------------------------------------------------------------------
* 0.1	   Jun 26,2016                   Initial version
*------------------------------------------------------------------------
* 0.2	   Jan 11,2017  ZHANG Jie  Add Clock_if.h and ISR_WDTA0func for compile warning,  add explicit variable cast
*                                                   and add u for constant value to avoid Coverity warning.
*----------------------------------------------------------------------
* 0.3          Jan 24,2017    ZhangJie     Fix Coverity warning.
*=====================================================================*/
#include "define.h"
#include "Wdg_Adapt.h"
#include "Wdg_Adapt_Cfg.h"
#include "wdog.h"

/***********************************************************************
* Name        	: Wdg_Adapt_Init
* Description	: Init clock, set time, error mode.
* Description	: None
* Returns     	: None
***********************************************************************/
void Wdg_Adapt_Init( void )
{
    WDOG_ConfigType WDOGConfig = {{0}};     /*! < watchdog configuration structure */

    WDOGConfig.sBits.bWaitEnable   = TRUE;
    WDOGConfig.sBits.bStopEnable   = TRUE;
    WDOGConfig.sBits.bDbgEnable    = FALSE;
    WDOGConfig.sBits.bUpdateEnable = FALSE;
    WDOGConfig.sBits.bDisable      = FALSE; /* enable WDOG */
    WDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    WDOGConfig.u16TimeOut          = 1000;  /* unit by 1ms */
    WDOGConfig.u16WinTime          = 0;

    WDOG_Init(&WDOGConfig);
}


/***********************************************************************
* Name        	: Wdg_Adapt_Enable
* Description	: Enable the WATCHDOG
* Description	: None
* Returns     	: None
***********************************************************************/
void Wdg_Adapt_Enable( void )
{
    WDOG_Enable();
}

/***********************************************************************
* Name        	: Wdg_Adapt_Disable
* Description	: Disable WDT0 in standby mode.
			  WDT1 is automatically stopped in Deep Stop/Cyclic Run.
* Description	: None
* Returns     	: None
***********************************************************************/
void Wdg_Adapt_Disable( void )
{
    WDOG_Disable();
    /* once started, no way to stop */
}

/***********************************************************************
* Name        	: Wdg_Adapt_Clear
* Description	: Clear the WATCHDOG
* Description	: None
* Returns     	: None
***********************************************************************/
void Wdg_Adapt_Clear(void)
{
    WDOG_Feed();
}

void ISR_WDTA0func(void)
{
	/*Do nothing*/
}
/****************************** END OF FILE ***************************/
