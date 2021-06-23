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
#include "I2c_Adapt.h"

const I2C_SettingType gctIICSettings = {
	IIC_C1_IICEN_ENABLE,IIC_C1_IICIE_ENABLE,IIC_C1_MAT_SLAVE_MODE,IIC_C1_WUEN_DISABLE,\
	IIC_C2_GCAEN_DISABLE,IIC_C2_ADEXT_7BIT_ADDRESS,IIC_C2_RMEN_DISABLE,\
	IIC_SMB_FACK_ENABLE,IIC_SMB_ALERTEN_DISABLE,IIC_SMB_SIICAEN_DISABLE,IIC_SMB_TCKSEL_DISABLE,IIC_SMB_SHTF2IE_ENABLE,IIC_RESERVED,
};

COMM_RECEIVE_DATA_CALLBACK gtfI2cRecvCallbackFunc = NULL;
COMM_FINISHED_CALLBACK gtfI2cStopCallbackFunc = NULL;
COMM_ERR_CALLBACK gtfI2cErrCallbackFunc = NULL;
COMM_PAIRED_CALLBACK gtfI2cAddrPairedCallbackFunc = NULL;


