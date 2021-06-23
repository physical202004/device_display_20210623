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

// callback for master send data
#include "Spi_adapt.h"


extern void backlightSPIRecvFunc(U16 data_number,U8 data);
extern void backlightSPISendFunc(void);

const SPI_SettingType gcsSPISettings[SPI_CH_MAX] = {
/*SPI_0*/	
{SPI_C1_SPIE_INT_ENABLE,SPI_C1_SPE_SYSTEM_ENABLE,SPI_C1_SPTIE_TX_INT_ENABLE,SPI_C1_MSTR_MASTER_MODE,SPI_C1_SPOL_CLK_ACTIVE_HIGH,SPI_C1_CPHA_CLOCK_PHASE_0,SPI_C1_SSOE_SALVE_OUTPUT_DISABLE,\
SPI_C1_LSBFE_DATA_MSB,SPI_C2_SPMIE_MATCH_DISABLE,SPI_C2_MODFEN_FAULT_DISABLE,SPI_C2_BIDIROE_BID_DISABLE,SPI_C2_SPC0_BID_PIN_DISABLE,SPI_C2_SPISWAI_CONTINUE_OPT,SPI_C2_RESERVED_BIT},

/*SPI_1*/
{SPI_C1_SPIE_INT_ENABLE,SPI_C1_SPE_SYSTEM_ENABLE,SPI_C1_SPTIE_TX_INT_ENABLE,SPI_C1_MSTR_MASTER_MODE,SPI_C1_SPOL_CLK_ACTIVE_LOW,SPI_C1_CPHA_CLOCK_PHASE_0,SPI_C1_SSOE_SALVE_OUTPUT_DISABLE,\
SPI_C1_LSBFE_DATA_MSB,SPI_C2_SPMIE_MATCH_DISABLE,SPI_C2_MODFEN_FAULT_DISABLE,SPI_C2_BIDIROE_BID_DISABLE,SPI_C2_SPC0_BID_PIN_DISABLE,SPI_C2_SPISWAI_CONTINUE_OPT,SPI_C2_RESERVED_BIT},
};

COMM_SEND_DATA_CALLBACK gtfSpiTxCallbackHandler[SPI_CH_MAX] = {
	NULL,
	NULL,
};

COMM_RECEIVE_DATA_CALLBACK gtfSpiRxCallbackHandler[SPI_CH_MAX] = {
	NULL,
	NULL,
};
