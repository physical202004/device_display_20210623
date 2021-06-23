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
#include "Uart_Adapt.h"


static void Uart_Adapt_RxHandler(UART_CH_TYPE uart);
static void Uart_Adapt_TxHandler(UART_CH_TYPE uart);
static void UART0_Adapt_ISR(void);
static void UART1_Adapt_ISR(void);

static UART_FRAME_S ssUartFrameInfo[UART_CH_MAX] = {
	{0,0,NULL,UART_0},
	{0,0,NULL,UART_1},
};

/***********************************************************************
* Name        	: Uart_Adapt_SetSendCallback
* Description	: Register call back function
* Parameter		: sendFunc:send function
* Returns     	: None
***********************************************************************/
void Uart_Adapt_SetSendCallback(UART_CH_TYPE uart,COMM_SEND_DATA_CALLBACK sendFunc)
{
	gtfUartTxCallbackHandler[uart] = sendFunc;

}

/***********************************************************************
* Name        	: Uart_Adapt_SetRecvCallback
* Description	: Register call back function
* Parameter		: recvFunc:receive function
* Returns     	: None
***********************************************************************/
void Uart_Adapt_SetRecvCallback(UART_CH_TYPE uart,COMM_RECEIVE_DATA_CALLBACK recvFunc)
{
	gtfUartRxCallbackHandler[uart] = recvFunc;
}


/***********************************************************************
* Name        	: Uart_Adapt_Start
* Description	: Initialize uart module
* Parameter		: uart:uart channel,baudrate:baudrate
* Returns     	: None
***********************************************************************/
void Uart_Adapt_Start(UART_CH_TYPE uart,U32 baudrate)
{
    UART_ConfigType lsUartConfig = {{0}};

    lsUartConfig.u32SysClkHz = SYS_CLK_HZ;   	/* Bus clock in Hz*/
    lsUartConfig.u32Baudrate = baudrate;     	/* Baud rate*/
	lsUartConfig.sctrl1settings = gcsUartCtrl1Settings[uart];
	lsUartConfig.sctrl2settings = gcsUartCtrl2Settings[uart];
	lsUartConfig.sctrl3settings = gcsUartCtrl3Settings[uart];

    if(UART_CH0 == uart)
    {
        UART_SetCallback(UART_0,UART0_Adapt_ISR);
		UART_Init(UART_0,&lsUartConfig);
    }
    else if(UART_CH1 == uart)
    {
        UART_SetCallback(UART_1,UART1_Adapt_ISR);
		UART_Init(UART_1,&lsUartConfig);

    }
	else
	{
		/* invalid */
	}

}

/***********************************************************************
* Name        	: Uart_Adapt_Stop
* Description	: Deinitial uart channel
* Parameter		: uart:uart channel
* Returns     	: None
***********************************************************************/
void Uart_Adapt_Stop(UART_CH_TYPE uart)
{
	 /* Make sure that the transmitter and receiver are disabled*/
    ssUartFrameInfo[uart].channel->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
}

/***********************************************************************
* Name        	: Uart_Adapt_Send
* Description	: Send data
* Parameter		: uart:uart channel
				  pBuf:pointer to first byte to send
				  length:data length 
* Returns     	: TRUE:successful,FALSE:failed
***********************************************************************/
MYBOOL Uart_Adapt_Send(UART_CH_TYPE uart,U8 *pBuf,U8 length)
{
	MYBOOL lbRet = TRUE;
	if((length < 1) || (pBuf == NULL))
	{
		lbRet = FALSE;
	}
	else
	{
		/* reset status*/
		ssUartFrameInfo[uart].txBuf = pBuf;
		ssUartFrameInfo[uart].txCnt = length;
		UART_EnableTx(ssUartFrameInfo[uart].channel);
		UART_EnableTxBuffEmptyInt(ssUartFrameInfo[uart].channel);
	}
	
	return lbRet;
}

/***********************************************************************
* Name        	: Uart_Adapt_TxHandler
* Description	: Tx handler
* Parameter		: uart:uart channel
* Returns     	: None
***********************************************************************/
static void Uart_Adapt_TxHandler(UART_CH_TYPE uart)
{
	if((ssUartFrameInfo[uart].txCnt == 0U) || (ssUartFrameInfo[uart].txBuf == NULL))
	{
		UART_DisableTxBuffEmptyInt(ssUartFrameInfo[uart].channel);
		if(NULL != gtfUartTxCallbackHandler[uart])
	    {
	        gtfUartTxCallbackHandler[uart](E_COMM_MODE_NONE);
	    }
		else
		{
			/* do nothing... */
		}
	}
	else
	{
		UART_WriteDataReg(ssUartFrameInfo[uart].channel,*(ssUartFrameInfo[uart].txBuf));
		ssUartFrameInfo[uart].txBuf++;
		ssUartFrameInfo[uart].txCnt--;
	}
    
}

/***********************************************************************
* Name        	: Uart_Adapt_RxHandler
* Description	: Rx handler
* Parameter		: uart:uart channel
* Returns     	: None
***********************************************************************/
static void Uart_Adapt_RxHandler(UART_CH_TYPE uart)
{
    U8 lubData = 0U;
    lubData = UART_ReadDataReg(ssUartFrameInfo[uart].channel);
    if(NULL != gtfUartRxCallbackHandler[uart])
    {
        gtfUartRxCallbackHandler[uart](E_COMM_MODE_NONE,0,lubData);
    }
	else
	{
		/* do nothing... */
	}
	//UART_EnableRx(ssUartFrameInfo[uart].channel);
    UART_EnableRxBuffFullInt(ssUartFrameInfo[uart].channel);
}

    
/***********************************************************************
* Name        	: UART0_Adapt_ISR
* Description	: Interrupt callback handler
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void UART0_Adapt_ISR(void)
{
	
	/* RX */
    if(UART_IsRxBuffFull(UART_0))
    {
        Uart_Adapt_RxHandler(UART_CH0);
    }
	/* TX */
    else if(UART_IsTxBuffEmpty(UART_0))
    {
        Uart_Adapt_TxHandler(UART_CH0);
    }
	else
	{
		/* do nothing... */
	} 
}

/***********************************************************************
* Name        	: UART1_Adapt_ISR
* Description	: Interrupt callback handler
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void UART1_Adapt_ISR(void)
{
	
	/* RX */
    if(UART_IsRxBuffFull(UART_1))
    {
        Uart_Adapt_RxHandler(UART_CH1);
    }
	/* TX*/
    else if(UART_IsTxBuffEmpty(UART_1))
    {
        Uart_Adapt_TxHandler(UART_CH1);
    }
	else
	{
		/* do nothing... */
	}
}

