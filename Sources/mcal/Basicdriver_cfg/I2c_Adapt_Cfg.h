/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department : ADS AD                                                 *
*   AUTHOR     :                                                       *
************************************************************************
* Object        :
* Module        :	IIC
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
* 0.1    	Aug 20,2019 Huang Haoxiong	Initial
*=====================================================================*/
#ifndef I2C_ADAPT_CFG_H
#define I2C_ADAPT_CFG_H
#include "I2C_Adapt_if.h"
#include "i2c.h"

extern const I2C_SettingType gctIICSettings;

extern COMM_RECEIVE_DATA_CALLBACK gtfI2cRecvCallbackFunc;
extern COMM_FINISHED_CALLBACK gtfI2cStopCallbackFunc;
extern COMM_ERR_CALLBACK gtfI2cErrCallbackFunc;
extern COMM_PAIRED_CALLBACK gtfI2cAddrPairedCallbackFunc;


#endif



