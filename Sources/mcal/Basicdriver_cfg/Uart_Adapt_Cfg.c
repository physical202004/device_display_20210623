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

#include "Uart_adapt.h"
#include "TraceServ_if.h"

/*setting1,mapping to C1 register */
const UART_Ctrl1Type gcsUartCtrl1Settings[UART_CH_MAX] = {
/* UART0*/
	{UART_C1_PT_EVENT_PARITY|UART_C1_PE_PARITY_DISABLE|UART_C1_ILT_CNT_START_AFTER_START_BIT|UART_C1_WAKE_FOR_IDLE_LINE|UART_C1_M_8BIT_MODE|UART_C1_RSRC_LOOP|UART_C1_UARTSWAI_NORMAL_MODE|UART_C1_LOOPS_NORMAL_MODES},

/* UART1*/
	{UART_C1_PT_EVENT_PARITY|UART_C1_PE_PARITY_DISABLE|UART_C1_ILT_CNT_START_AFTER_START_BIT|UART_C1_WAKE_FOR_IDLE_LINE|UART_C1_M_8BIT_MODE|UART_C1_RSRC_LOOP|UART_C1_UARTSWAI_NORMAL_MODE|UART_C1_LOOPS_NORMAL_MODES},
};

/*setting2,mapping to C2 register */
const UART_Ctrl2Type gcsUartCtrl2Settings[UART_CH_MAX] = {
/* UART0*/
	{UART_C2_SBK_NORMAL_OPT|UART_C2_RWU_NORMAL_MODE|UART_C2_RE_RECEIVER_ON|UART_C2_TE_TRANSMITTER_ON|UART_C2_ILIE_USE_POLLING|UART_C2_RIE_USE_INPTERRUPT|UART_C2_TCIE_USE_POLLING|UART_C2_TIE_USE_POLLING},

/* UART1*/
	{UART_C2_SBK_NORMAL_OPT|UART_C2_RWU_NORMAL_MODE|UART_C2_RE_RECEIVER_ON|UART_C2_TE_TRANSMITTER_ON|UART_C2_ILIE_USE_POLLING|UART_C2_RIE_USE_INPTERRUPT|UART_C2_TCIE_USE_POLLING|UART_C2_TIE_USE_POLLING},
};

/*setting3,mapping to C3 register */
const UART_Ctrl3Type gcsUartCtrl3Settings[UART_CH_MAX] = {
/* UART0*/
	{UART_C3_PEIE_PARITY_ERR_INT_DISABLE|UART_C3_FEIE_FRAME_ERR_INT_DISABLE|UART_C3_NEIE_NOISE_ERR_INT_DISABLE|UART_C3_ORIE_OVERRUN_ERR_INT_DISABLE|UART_C3_TXINV_TRANS_DATA_NOT_INVERT|UART_C3_TXDIR_INPUT_SINGLE_WIRE_MODE},

/* UART1*/
	{UART_C3_PEIE_PARITY_ERR_INT_DISABLE|UART_C3_FEIE_FRAME_ERR_INT_DISABLE|UART_C3_NEIE_NOISE_ERR_INT_DISABLE|UART_C3_ORIE_OVERRUN_ERR_INT_DISABLE|UART_C3_TXINV_TRANS_DATA_NOT_INVERT|UART_C3_TXDIR_INPUT_SINGLE_WIRE_MODE},
};


/*register rx callback */
COMM_RECEIVE_DATA_CALLBACK gtfUartRxCallbackHandler[UART_CH_MAX] = {
    NULL,
	NULL,
};
	
/* register tx callback */	
COMM_SEND_DATA_CALLBACK gtfUartTxCallbackHandler[UART_CH_MAX] = {
    NULL,
	NULL,
};

