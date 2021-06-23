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
* Version       Date    Name         Changes and comments
* ----------------------------------------------------------------------
* 0.1    Jan 10,2016                    ReInit
* ----------------------------------------------------------------------
* 0.2    Oct 10,2016  PEI Jingli     add pwm,pwd,capture interface
* ----------------------------------------------------------------------
* 0.3    Jan 4,2017    ShangQiuju Update for error config.
*-----------------------------------------------------------------------
* 0.4    Jan 24,2017  ZhangJie  Fix Coverity warnings.
* ----------------------------------------------------------------------
* 0.5    Feb 09,2017  PEI Jingli     Add device of pwmd
* ----------------------------------------------------------------------
* 0.6    Feb 15,2017  ShangQiuju Remove devie fdl from dev to devblock.
* ----------------------------------------------------------------------
* 0.7    Feb 20,2017    PEI Jingli     Add device of TAU-Pwm,capture, timer
* ----------------------------------------------------------------------
* 0.8    Feb 20,2017  ZhangJie     Add device of CAN.
* ----------------------------------------------------------------------
* 0.9	Dec 19,2018		Huang,Haoxiong	improve HAL structure and remove device of CAN
*=====================================================================*/
#include "Global_Definition.h"

static void InitiaHalDev(void);


/***********************************************************************
* Name        	: RegisterHalDev
* Description 	: register char & block devices
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void RegisterHalDev(void)
{
	InitiaHalDev();
}


/***********************************************************************
* Name        	: InitiaHalDev
* Description 	: Call the init function registered by the devices
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void InitiaHalDev(void)
{
    U8 i = 0u;

    /* initial character devices */
    for(i = 0u;i < (unsigned char)E_DEVCHAR_NUM;i++)
    {
        HALChar_Init(i);
    }

    /* initial block devices */
    for(i = (U8)(E_DEVBLOCK_START + 1u);i < (unsigned char)E_DEVBLOCK_NUM;i++)
    {
        HALBlock_Init(i);
    }
}

/**************** END OF FILE *****************************************/

