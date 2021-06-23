/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD      									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        : Mcu.h
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
* 0.1    Jan 20,2016  Init Version
* ----------------------------------------------------------------------
* 0.2    Dec 20,2016  ShangQIuju remove other module used interface here.
* ----------------------------------------------------------------------
* 0.3    Dec 29,2016  PEI Jingli  Change for codeReview result
*------------------------------------------------------------------------
* 0.4    Jan 11,2017  ZhangJie   And add u for constant value to avoid Coverity warning.
 ----------------------------------------------------------------------
* 0.5    Jan 16,2017   RenWeimin  Add test code.
*-----------------------------------------------------------------------
* 0.6    Jan 24,2017    ZhangJie     Remove unused include headfile.
*-----------------------------------------------------------------------
* 0.7    Feb 09,2017   ZhangJie   Using micro to call app function.
*=====================================================================*/
#ifndef _MCU_H_
#define _MCU_H_

/**** Definition of constants ****/
/*SWRESA:software reset register*/
//#define MCU_REG_RESET                        (*(CPU_REG32 *)0xFFF80A04u)
/*RESF:reset factor register*/
//#define MCU_REG_RESET_REASON         (*(CPU_REG32 *)0xFFF80760u)
/*RESFC:Reset factor clear reg*/
//#define MCU_REG_RESETFLAG_CLEAR    (*(CPU_REG32 *)0xFFF80768u)

#define MCU_REG_RESET                      0
/*RESF:reset factor register*/
#define MCU_REG_RESET_REASON         0
/*RESFC:Reset factor clear reg*/
#define MCU_REG_RESETFLAG_CLEAR    0



#define WAKE_UP_BY_SECOND_WAKEUP    1
#define WAKE_UP_BY_WAKEUP           2

#endif /* _MCU_H_ */
/****************************** END OF FILE ***************************/

