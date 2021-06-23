/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   : Interface of IRQ module.
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    Aug 01,2016 PEI Jingli      Iinitial version
* ----------------------------------------------------------------------
* 0.2    Nov 30,2016 PEI Jingli      Add interface of IRQ_IntEnableAll
* ----------------------------------------------------------------------
* 0.3    Dec 06,2016 PEI Jingli      Merge changes from REN Weimin after code review
* ----------------------------------------------------------------------
* 0.4    Dec 20,2016 ShangQiuju  remove other module used define here
*----------------------------------------------------------------------
* 0.5    Jan 24,2017  ZhangJie     Fix Coverity warning.
*=====================================================================*/

#ifndef IRQ_ADAPT_IF_H
#define IRQ_ADAPT_IF_H
#include "Define.h"   
#include "SKEAZN642.h"

#define IRQ_INT_ID_MAX       (Reserved47_IRQn)

extern void IRQ_IntDis(U16 int_id);
extern void IRQ_IntEn(U16 int_id);
extern void IRQ_IntDisAll(void);
extern void  IRQ_IntEnableAll(void);


#endif
/****************************** END OF FILE ***************************/
