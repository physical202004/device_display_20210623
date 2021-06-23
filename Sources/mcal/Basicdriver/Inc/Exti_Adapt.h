/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : Shao Guangxian										   *
************************************************************************
* Object        :
* Module        :	EXTI
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
* 0.1	   July 02,2020   Huang Haoxiong Initial version
*=====================================================================*/
#ifndef EXTI_ADAPT_H
#define EXTI_ADAPT_H
#include "SKEAZN642.h"
#include "kbi.h"
#include "EXTI_Adapt_Cfg.h"
#include "EXTI_Adapt_if.h"

typedef enum
{
    E_EXTI_MODULE_00 = 0x00,        			/*KBI0 */
    E_EXTI_MODULE_01 = 0x01,	     			/*KBI1 */
    E_EXTI_MODULE_NUM
}EXTI_MODULE;

typedef enum
{
    E_EXTI_MODE_EDGE_ONLY = 0,             	/* select  edge only mode  */         
    E_EXTI_MODE_EDGE_LEVEL                 	/* select  both edge and level  mode */
}EXTI_MODE_TYPE;

#endif

