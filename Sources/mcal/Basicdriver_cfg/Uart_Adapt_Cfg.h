/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : UART adapt
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/25  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef UART_ADAPT_CFG_H
#define UART_ADAPT_CFG_H
#include "uart.h"

extern const UART_Ctrl1Type gcsUartCtrl1Settings[UART_CH_MAX];
extern const UART_Ctrl2Type gcsUartCtrl2Settings[UART_CH_MAX];
extern const UART_Ctrl3Type gcsUartCtrl3Settings[UART_CH_MAX];

extern COMM_RECEIVE_DATA_CALLBACK gtfUartRxCallbackHandler[UART_CH_MAX];
extern COMM_SEND_DATA_CALLBACK gtfUartTxCallbackHandler[UART_CH_MAX];



#endif


