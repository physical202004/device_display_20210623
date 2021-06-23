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
* 0.1    Aug 01,2016 PEI Jingli  Iinitial version
* ----------------------------------------------------------------------
* 0.2    Nov 30,2016 PEI Jingli  Add interface
* ----------------------------------------------------------------------
* 0.3    Dec 09,2016 REN Weimin  Update after code review.
* ----------------------------------------------------------------------
* 0.4    Dec 09,2016 LIU Chenxi  Correct version conflcit during merge.
*------------------------------------------------------------------------
* 0.5    Jan 24,2017  ZhangJie  Fix Coverity warning.
*-----------------------------------------------------------------------
* 0.6    Apr 1,2017   PEI Jingli  Fix include files.
*=====================================================================*/
#include "Define.h"
#include "Irq_Adapt.h"


void  Irq_Adapt_IntDis (U16  int_id)
{                                            
    if (int_id > IRQ_INT_ID_MAX) 
    {
        return;
    }

    NVIC_SetPendingIRQ(int_id); /* Set pending interrupt register */
    NVIC_DisableIRQ(int_id); /* Disable Interrupt */
}

void  Irq_Adapt_IntDisAll (void)
{
    __asm  volatile ("cpsid i");
    return;
}

void  Irq_Adapt_IntEnableAll (void)
{
    __asm volatile ("cpsie i");
    return;
}

void  Irq_Adapt_IntEn (U16  int_id)
{
    if (int_id > IRQ_INT_ID_MAX) 
    {
        return;
    }

    NVIC_ClearPendingIRQ(int_id); /* Clear pending interrupt register */
    NVIC_EnableIRQ(int_id); /* Enable required Interrupt */
}
   
