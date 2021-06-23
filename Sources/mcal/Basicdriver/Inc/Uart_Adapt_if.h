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

#ifndef UART_ADAPT_IF_H
#define UART_ADAPT_IF_H 
#include "uart.h"
#include "Define.h"

typedef enum
{
     UART_CH0 = 0,                     
     UART_CH1,
     UART_CH2,
     UART_CH_MAX
}UART_CH_TYPE;

extern void Uart_Adapt_SetSendCallback(UART_CH_TYPE uart,COMM_SEND_DATA_CALLBACK sendFunc);
extern void Uart_Adapt_SetRecvCallback(UART_CH_TYPE uart,COMM_RECEIVE_DATA_CALLBACK recvFunc);
extern void Uart_Adapt_Start(UART_CH_TYPE uart,U32 baudrate);
extern void Uart_Adapt_Stop( UART_CH_TYPE uart);
extern MYBOOL Uart_Adapt_Send(UART_CH_TYPE uart,U8 *pBuf,U8 length);


#endif /* UART_ADAPT_IF_H */

