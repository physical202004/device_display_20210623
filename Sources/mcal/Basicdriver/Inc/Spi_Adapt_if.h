/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : SPI adapt
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
#ifndef SPI_ADAPT_IF_H
#define SPI_ADAPT_IF_H
#include "Define.h"
#include "spi.h"


typedef enum
{
	SPI_CH0=0,
	SPI_CH1,
	SPI_CH_MAX
}SPI_CH_TYPE;

extern void Spi_Adapt_SetSendCallback(SPI_CH_TYPE spi,COMM_SEND_DATA_CALLBACK sendFunc);
extern void Spi_Adapt_SetRecvCallback(SPI_CH_TYPE spi,COMM_RECEIVE_DATA_CALLBACK recvFunc);
extern MYBOOL Spi_Adapt_Write(SPI_CH_TYPE spi,U8 *data,U8 length);
extern MYBOOL Spi_Adapt_Read(SPI_CH_TYPE spi,U8 *pBuf,U8 length);
extern void Spi_Adapt_Start(E_COMM_MODE mode,SPI_CH_TYPE spi,U32 baudrate);
extern void Spi_Adapt_Stop(E_COMM_MODE mode,SPI_CH_TYPE spi);


#endif /*SPI_ADAPT_IF_H*/

