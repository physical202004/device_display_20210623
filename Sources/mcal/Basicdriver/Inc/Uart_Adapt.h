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

#ifndef UART_ADAPT_H
#define UART_ADAPT_H
#include "Uart_Adapt_if.h"
#include "Uart_adapt_cfg.h"

typedef struct
{
	U8 rxBuf;
	U8 txCnt;
	U8 *txBuf;
	UART_MemMapPtr channel;
}UART_FRAME_S;

/*Patity type*/
#define UART_C1_PT_EVENT_PARITY						0U//event parity
#define UART_C1_PT_ODD_PARITY						0x01U//odd parity

/* Parity enable*/
#define UART_C1_PE_PARITY_DISABLE					0U//No hardware parity generation or checking.
#define UART_C1_PE_PARITY_ENABLE					0x02U//Parity enabled.

/*!< Idle Line Type Select*/
#define UART_C1_ILT_CNT_START_AFTER_START_BIT		0U//Idle character bit count starts after start bit.
#define UART_C1_ILT_CNT_START_AFTER_STOP_BIT		0x04U//Idle character bit count starts after stop bit.

/*!< Receiver Wakeup Method Select*/
#define UART_C1_WAKE_FOR_IDLE_LINE					0U//Idle-line wake-up.
#define UART_C1_WAKE_FOR_ADDR_MARK					0x08U//Address-mark wake-up.

/*!< 9-Bit or 8-Bit Mode Select*/
#define UART_C1_M_8BIT_MODE							0U//Normal - start + 8 data bits (lsb first) + stop.
#define UART_C1_M_9BIT_MODE							0x10U//Receiver and transmitter use 9-bit data characters start + 8 data bits (lsb first) + 9th data bit + stop.

/*!< Receiver Source Select*/
#define UART_C1_RSRC_LOOP							0U//Provided LOOPS is set, RSRC is cleared, selects internal loop back mode and the UART does not use the RxD pins.
#define UART_C1_RSRC_NO_LOOP						0x20U//Single-wire UART mode where the TxD pin is connected to the transmitter output and receiver input.

/*!< UART Stops in Wait Mode*/
#define UART_C1_UARTSWAI_NORMAL_MODE				0U//UART clocks continue to run in Wait mode so the UART can be the source of an interrupt that wakes up the CPU.
#define UART_C1_UARTSWAI_WAIT_MODE					0x40U//UART clocks freeze while CPU is in Wait mode.

/*!< Loop Mode Select*/
#define UART_C1_LOOPS_NORMAL_MODES					0U//Normal operation - RxD and TxD use separate pins.
#define UART_C1_LOOPS_LOOP_MODE						0x80U//Loop mode or single-wire mode where transmitter outputs are internally connected to receiver input.(See RSRC bit.) RxD pin is not used by UART.

/* Send Break */
#define UART_C2_SBK_NORMAL_OPT						0U//Normal transmitter operation.
#define UART_C2_SBK_QUEUE_OPT						0x01U//Queue break character(s) to be sent.

/* Receiver Wakeup Control */
#define UART_C2_RWU_NORMAL_MODE						0U//Normal UART receiver operation.
#define UART_C2_RWU_STANDBY_MODE					0x02U//UART receiver in standby waiting for wake-up condition.

/* Receiver Enable */
#define UART_C2_RE_RECEIVER_OFF						0U//Receiver off
#define UART_C2_RE_RECEIVER_ON						0x04U//Receiver on

/* Transmitter Enable */
#define UART_C2_TE_TRANSMITTER_OFF					0U//Transmitter off
#define UART_C2_TE_TRANSMITTER_ON					0x08U//Transmitter on

/* Idle Line Interrupt Enable for IDLE */
#define UART_C2_ILIE_USE_POLLING					0U//Hardware interrupts from S1[IDLE] disabled; use polling.
#define UART_C2_ILIE_USE_INTERRUPT					0x10U//Hardware interrupt requested when S1[IDLE] flag is 1.

/* Receiver Interrupt Enable for RDRF */
#define UART_C2_RIE_USE_POLLING						0U//Hardware interrupts from S1[RDRF] disabled; use polling.
#define UART_C2_RIE_USE_INPTERRUPT					0x20U//Hardware interrupt requested when S1[RDRF] flag is 1.

/* Transmission Complete Interrupt Enable for TC */
#define UART_C2_TCIE_USE_POLLING					0U//Hardware interrupts from TC disabled; use polling
#define UART_C2_TCIE_USE_INTERRUPT					0x40U//Hardware interrupt requested when TC flag is 1.

/* Transmit Interrupt Enable for TDRE */
#define UART_C2_TIE_USE_POLLING						0U//Hardware interrupts from TDRE disabled; use polling.
#define UART_C2_TIE_USE_INTERRUPT					0x80U//Hardware interrupt requested when TDRE flag is 1.


/*!< Parity Error Interrupt Enable */
#define UART_C3_PEIE_PARITY_ERR_INT_DISABLE			0U//PF interrupts disabled; use polling).
#define UART_C3_PEIE_PARITY_ERR_INT_ENABLE			0x01U//Hardware interrupt requested when PF is set.

/*!< Framing Error Interrupt Enable*/
#define UART_C3_FEIE_FRAME_ERR_INT_DISABLE			0U//FE interrupts disabled; use polling).
#define UART_C3_FEIE_FRAME_ERR_INT_ENABLE			0x02U//Hardware interrupt requested when FE is set.

/*!< Noise Error Interrupt Enable*/
#define UART_C3_NEIE_NOISE_ERR_INT_DISABLE			0U//NF interrupts disabled; use polling).
#define UART_C3_NEIE_NOISE_ERR_INT_ENABLE			0x04U//Hardware interrupt requested when NF is set.

/*!< Overrun Interrupt Enable*/
#define UART_C3_ORIE_OVERRUN_ERR_INT_DISABLE		0U//OR interrupts disabled; use polling.
#define UART_C3_ORIE_OVERRUN_ERR_INT_ENABLE			0x08U//Hardware interrupt requested when OR is set.

/*!< Transmit Data Inversion*/
#define UART_C3_TXINV_TRANS_DATA_NOT_INVERT			0U//Transmit data not inverted.
#define UART_C3_TXINV_TRANS_DATA_INVERT				0x10U//Transmit data inverted.

/*!< TxD Pin Direction in Single-Wire Mode*/
#define UART_C3_TXDIR_INPUT_SINGLE_WIRE_MODE		0U//TxD pin is an input in single-wire mode.
#define UART_C3_TXDIR_OUTPUT_SINGLE_WIRE_MODE		0x20U//TxD pin is an output in single-wire mode.


#endif /*UART_ADAPT_H*/

