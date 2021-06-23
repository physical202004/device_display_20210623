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

#ifndef SPI_ADAPT_CFG_H
#define SPI_ADAPT_CFG_H
#include "Spi_adapt_if.h"



extern const SPI_SettingType gcsSPISettings[SPI_CH_MAX];

extern COMM_SEND_DATA_CALLBACK gtfSpiTxCallbackHandler[SPI_CH_MAX];
extern COMM_RECEIVE_DATA_CALLBACK gtfSpiRxCallbackHandler[SPI_CH_MAX];

#endif


