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
------------------------------------------------------------------------
* 0.1	   Jun 26,2016   PEI Jingli  Initial version
------------------------------------------------------------------------
* 0.2	   Jun 29,2016   LIU Chenxi  Correct errors during merging
------------------------------------------------------------------------
* 0.3	   July 07,2016  PEI Jingli  Adapt 64pin MCU
------------------------------------------------------------------------
* 0.4	   July 21,2016  PEI Jingli  Remove test code
------------------------------------------------------------------------
* 0.5	   Oct 28,2016  PEI Jingli  del R_PORT_Init/R_TAUD_Start for unused
*----------------------------------------------------------------------
* 0.6          Dec 20,2016   ShangQiuju  remove unused include  files.
*----------------------------------------------------------------------
* 0.7          Dec 29,2016 PEI Jingli  delete unused include  files and test code
*------------------------------------------------------------------------
* 0.8          Jan 24,2017  ZhangJie  Fix Coverity warnings.
*=====================================================================*/
#include "Mcu_if.h"
#include "Irq_Adapt_if.h"
#include "Global_Definition.h"

/*============================================================================
Includes
============================================================================*/

extern void CS_SIG_RegisterSignal(void);


/***********************************************************************
* Name        	: MCU_HardwareInit
* Description	: hardware is reset, run in rest isr, before call C_Lib
* Description	: None
* Returns     	: None
***********************************************************************/
void MCU_HardwareInit(void)
{
  /*Step 1: Init IRQ*/

  /*Step 2: init clock and CPU Mode*/
    mcu_init_clock();
  /*Step 3: init IO*/

  //Wdg_Adapt_Disable();
    
   mcu_get_reset_reason();
   /********************************************************/
   /*  PLL setting                     */
   /********************************************************/
   /*the high clock is enable in bootloader,so we don't need*/
   /*to enable again in here!*/
   /*mcu_setup_high_clock();*/

   /********************************************************/
   /* setup Interrupt table address                */
   /********************************************************/

   /********************************************************/
   /*  All Interrupt setup               */
   /********************************************************/

   /********************************************************/
   /*  All IO config                    */
   /********************************************************/
   mcu_io_init_reset();

}

/***********************************************************************
* Name        	: MCU_MainInit
* Description	: System is init, run in main function, C enviroment is OK
* Description	: None
* Returns     	: None
***********************************************************************/
void MCU_MainInit(void)
{
    
    /**************************************************************/
    /*set all registered io direction*/
    /**************************************************************/
}

/***********************************************************************
* Name        	: MCU_EnterSleep
* Description	: System will enter sleep, set IO,TAU, domains,clock,  wakeup factors.
*                 also enable wakeup interruput
* Description	: None
* Returns     	: None
***********************************************************************/
void MCU_EnterSleep(void)
{
  /*MCU IO for sleep*/
  mcu_io_init_sleep();
}

/***********************************************************************
* Name        	: MCU_Wakeup
* Description	: System is wakeup, init device
* Description	: None
* Returns     	: None
***********************************************************************/
void MCU_Wakeup(void)
{
  /*Init MCU IO for wakeup*/
  mcu_io_init_wakeup();
}

