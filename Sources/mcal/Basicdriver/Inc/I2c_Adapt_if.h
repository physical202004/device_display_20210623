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
* 0.1    	April 16,2020 Huang Haoxiong	Initial
*=====================================================================*/
#ifndef I2C_ADAPT_IF_H
#define I2C_ADAPT_IF_H
#include "Define.h"

enum
{
	IIC_BAUD_100K = 0x87,
	IIC_BAUD_400K = 0x40,
};/* not use for S32K serial */


extern void IIC_Adapt_SetRecvCallback(COMM_RECEIVE_DATA_CALLBACK recvFunc);
extern void IIC_Adapt_SetStopCallback(COMM_FINISHED_CALLBACK stopFunc);
extern void IIC_Adapt_SetErrCallback(COMM_ERR_CALLBACK errFunc);
extern void IIC_Adapt_SetPairCallback(COMM_PAIRED_CALLBACK pairFunc);
extern void IIC_Adapt_Start(E_COMM_MODE mode,E_COMM_BAUD baudrate,U8 SlaveAddress);
extern void IIC_Adapt_Stop(void);
extern MYBOOL IIC_Adapt_Send(U8 addr,U8 *pBuf,U8 len);
extern MYBOOL IIC_Adapt_Read(U8 addr,U8 *pBuf,U8 len);

#endif
