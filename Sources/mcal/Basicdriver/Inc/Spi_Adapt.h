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
#ifndef SPI_ADAPT_H
#define SPI_ADAPT_H

#include "Spi_adapt_cfg.h"
#include "Spi_adapt_if.h"

typedef struct
{

	U8 rxCnt;
	U8 rxBuf;
	U8 txCnt;
	U8 *txBuf;
	E_COMM_MODE mode;
	SPI_MemMapPtr spiChannel;
}SPI_FRAME_S;

/* 1: Interrupt Enable, 0: Interrupt disable */
#define SPI_C1_SPIE_INT_ENABLE					1U//Request a hardware interrupt when SPRF or MODF is 1
#define SPI_C1_SPIE_INT_DISABLE					0U//Interrupts from SPRF and MODF are inhibited¡ªuse polling

/* 1: SPI module Enable, 0: SPI module disable */
#define SPI_C1_SPE_SYSTEM_ENABLE				1U
#define SPI_C1_SPE_SYSTEM_DISABLE				0U

/* 1: Tx Interrupt Enable, 0: Tx Interrupt disable */
#define SPI_C1_SPTIE_TX_INT_ENABLE				1U//When SPTEF is 1, hardware interrupt requested
#define SPI_C1_SPTIE_TX_INT_DISABLE				0U//Interrupts from SPTEF inhibited (use polling)


/* 1: Master mode, 0: Slave mode */
#define SPI_C1_MSTR_MASTER_MODE					1U//SPI module configured as a master SPI device
#define SPI_C1_MSTR_SLAVE_MODE					0U//SPI module configured as a slave SPI device

/*1: Active-low SPI clock, 0: Active-HIgh SPI clock */
#define SPI_C1_SPOL_CLK_ACTIVE_LOW				1U//Active-low SPI clock (idles high)
#define SPI_C1_SPOL_CLK_ACTIVE_HIGH				0U//Active-high SPI clock (idles low)

/* Set clock phase, */
#define SPI_C1_CPHA_CLOCK_PHASE_1				1U//First edge on SPSCK occurs at the start of the first cycle of a data transfer
#define SPI_C1_CPHA_CLOCK_PHASE_0				0U//First edge on SPSCK occurs at the middle of the first cycle of a data transfer.

/* Slave select output enable */
/*
0: When C2[MODFEN] is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input.
When C2[MODFEN] is 1: In master mode, SS pin function is SS input for mode fault. In slave mode,SS pin function is slave select input.

1: When C2[MODFEN] is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input.
When C2[MODFEN] is 1: In master mode, SS pin function is automatic SS output. In slave mode: SS pin function is slave select input.
*/
#define SPI_C1_SSOE_SALVE_OUTPUT_ENABLE			1U
#define SPI_C1_SSOE_SALVE_OUTPUT_DISABLE		0U

/* 1: LSB first, 0: MSB first */
#define SPI_C1_LSBFE_DATA_LSB					1U//SPI serial data transfers start with the least significant bit.
#define SPI_C1_LSBFE_DATA_MSB					0U//SPI serial data transfers start with the most significant bit.

/*1: Match interrupt Enable, 0: Match interrupt disable,interrupt enable bit for the SPI receive data buffer hardware match (SPMF) function. */
#define SPI_C2_SPMIE_MATCH_ENABLE				1U//When SPMF is 1, requests a hardware interrupt
#define SPI_C2_SPMIE_MATCH_DISABLE				0U//Interrupts from SPMF inhibited (use polling)

/* Master mode-fault function enable */
#define SPI_C2_MODFEN_FAULT_ENABLE				1U//Mode fault function enabled, master SS pin acts as the mode fault input or the slave select output
#define SPI_C2_MODFEN_FAULT_DISABLE				0U//Mode fault function disabled, master SS pin reverts to general-purpose I/O not controlled by SPI

/* Bidirectional mode output enable */
#define SPI_C2_BIDIROE_BID_ENABLE				1U//SPI I/O pin enabled as an output
#define SPI_C2_BIDIROE_BID_DISABLE				0U//Output driver disabled so SPI data I/O pin acts as an input

/* enables bidirectional pin configurations */
#define SPI_C2_SPC0_BID_PIN_ENABLE				1U//SPI configured for single-wire bidirectional operation (pin mode is bidirectional).
#define SPI_C2_SPC0_BID_PIN_DISABLE				0U//SPI uses separate pins for data input and data output (pin mode is normal)

/* SPI stop in wait mode */
#define SPI_C2_SPISWAI_STOP_OPT					1U//SPI clocks stop when the MCU enters Wait mode.
#define SPI_C2_SPISWAI_CONTINUE_OPT				0U//SPI clocks continue to operate in Wait mode.

#define SPI_C2_RESERVED_BIT						0U//reserved

#endif /* SPI_ADAPT_H */

