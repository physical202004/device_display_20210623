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


#include "Spi_Adapt.h"

static SPI_FRAME_S ssSpiFrameInfo[SPI_CH_MAX] = {
	{0,0,0,NULL,E_COMM_MODE_MASTER,SPI0},
	{0,0,0,NULL,E_COMM_MODE_MASTER,SPI1},
};

static void SPI0_Adapt_ISR (void);
static void SPI1_Adapt_ISR(void);
static void Spi_Adapt_RxHandler(SPI_CH_TYPE spi);
static void Spi_Adapt_TxHandler(SPI_CH_TYPE spi);



/***********************************************************************
* Name        	: Spi_Adapt_SetSendCallback
* Description	: Register call back function
* Parameter		: sendFunc:send function
* Returns     	: None
***********************************************************************/
void Spi_Adapt_SetSendCallback(SPI_CH_TYPE spi,COMM_SEND_DATA_CALLBACK sendFunc)
{
	gtfSpiTxCallbackHandler[spi] = sendFunc;
}

/***********************************************************************
* Name        	: Spi_Adapt_SetRecvCallback
* Description	: Register call back function
* Parameter		: recvFunc:receive function
* Returns     	: None
***********************************************************************/
void Spi_Adapt_SetRecvCallback(SPI_CH_TYPE spi,COMM_RECEIVE_DATA_CALLBACK recvFunc)
{
	gtfSpiRxCallbackHandler[spi] = recvFunc;
}


/***********************************************************************
* Name        	: Spi_Adapt_Start
* Description	: Initiail spi module
* Parameter		: None
* Returns     	: None
***********************************************************************/
void Spi_Adapt_Start(E_COMM_MODE mode,SPI_CH_TYPE spi,U32 baudrate)
{
    SPI_ConfigType lsSPIConfig = {{0}};
    /* initialize SPI0 as master    */
    lsSPIConfig.u32BitRate = baudrate;
    lsSPIConfig.u32BusClkHz = SYS_CLK_HZ;
	lsSPIConfig.sSettings = gcsSPISettings[spi];
	
	ssSpiFrameInfo[spi].mode = mode;//set mode
	if(E_COMM_MODE_SLAVE == mode)
	{
		lsSPIConfig.sSettings.bMasterMode = SPI_C1_MSTR_SLAVE_MODE;
	}
	else
	{
		lsSPIConfig.sSettings.bMasterMode = SPI_C1_MSTR_MASTER_MODE;
	}
	
    if(SPI_CH0 == spi)
    {
    	SPI_SetCallback(SPI0,SPI0_Adapt_ISR);
    }
    else
    {
    	SPI_SetCallback(SPI1,SPI1_Adapt_ISR);
    }

    SPI_Init(ssSpiFrameInfo[spi].spiChannel, &lsSPIConfig);

}

/***********************************************************************
* Name        	: Spi_Adapt_Stop
* Description	: Denitiail spi module
* Parameter		: None
* Returns     	: None
***********************************************************************/
void Spi_Adapt_Stop(E_COMM_MODE mode,SPI_CH_TYPE spi)
{
	//SPI_DeInit(ssSpiFrameInfo[spi].spiChannel);
	//SPI_TxIntDisable(ssSpiFrameInfo[spi].spiChannel);		/*disable tx interrupt*/
	//SPI_IntDisable(ssSpiFrameInfo[spi].spiChannel); 		/*disable spi interrupt*/
}

/***********************************************************************
* Name        	: Spi_Adapt_Write
* Description	: Send message
* Parameter		: spi:spi channel,pBuf:send out buffer,length:length of data
* Returns     	: TRUE:succeeful,FALSE:failed
***********************************************************************/
 MYBOOL Spi_Adapt_Write(SPI_CH_TYPE spi,U8 *pBuf,U8 length)
 {
	MYBOOL lbRet = TRUE;
	if((length < 1) || (pBuf == NULL))
	{
		lbRet = FALSE;
	}
	else
	{
		/* reset status*/
		ssSpiFrameInfo[spi].txBuf = pBuf;
		ssSpiFrameInfo[spi].txCnt = length;
		ssSpiFrameInfo[spi].rxCnt = 0;
		SPI_TxIntEnable(ssSpiFrameInfo[spi].spiChannel);	/*enable tx interrupt*/
		SPI_IntEnable(ssSpiFrameInfo[spi].spiChannel); 		/*enable spi interrupt*/
	}
	
	return lbRet;
}

/***********************************************************************
* Name 		 : Spi_Adapt_Read
* Description	 : Read data
* Parameter	 : spi:spi channel,pBuf:buffer for read,length:length of data
* Returns		 : TRUE:succeeful,FALSE:failed
***********************************************************************/
MYBOOL Spi_Adapt_Read(SPI_CH_TYPE spi,U8 *pBuf,U8 length)
{
	MYBOOL lbRet = TRUE;
	if(length < 1)
	{
		lbRet = FALSE;
	}
	else
	{
		/* reset status*/
		ssSpiFrameInfo[spi].txBuf = NULL;
		ssSpiFrameInfo[spi].txCnt = length;
		ssSpiFrameInfo[spi].rxCnt = 0;
		SPI_TxIntEnable(ssSpiFrameInfo[spi].spiChannel);	/*enable tx interrupt*/
		SPI_IntEnable(ssSpiFrameInfo[spi].spiChannel); 		/*enable spi interrupt*/
	}
	UNUSED(pBuf);
	return lbRet;
}

/***********************************************************************
* Name 		 	: SPI0_Adapt_ISR
* Description	: SPI0 Interrupt callback function
* Parameter		: None
* Returns		: None
***********************************************************************/
static void SPI0_Adapt_ISR(void)
{
    /* check Receiver */
    if(SPI_IsSPRF(SPI0)&& (SPI0_C1 & SPI_C1_SPIE_MASK))	
    {		
    	Spi_Adapt_RxHandler(SPI_CH0);

    }
	else
	{
		/* do noting... */
	}
	/* check transmitter */	
    if(SPI_IsSPTEF(SPI0) && (SPI0_C1 & SPI_C1_SPTIE_MASK))
    {
    	Spi_Adapt_TxHandler(SPI_CH0);
    }	
    else	
    {		
        /* do nothing... */
    }
}

/***********************************************************************
* Name 		 	: SPI1_Adapt_ISR
* Description	: SPI1 Interrupt callback function
* Parameter		: None
* Returns		: None
***********************************************************************/
static void SPI1_Adapt_ISR(void)
{
	/* check Receiver */
	if(SPI_IsSPRF(SPI1)&& (SPI1_C1 & SPI_C1_SPIE_MASK))
	{
		Spi_Adapt_RxHandler(SPI_CH1);

	}
	else
	{
		/* do noting... */
	}
	/* check transmitter */
	if(SPI_IsSPTEF(SPI1) && (SPI1_C1 & SPI_C1_SPTIE_MASK))
	{
		Spi_Adapt_TxHandler(SPI_CH1);
	}
	else
	{
		/* do noting... */
	}
}

/***********************************************************************
* Name 		 	: Spi_Adapt_RxHandler
* Description	: SPI Rx handler
* Parameter		: spi:spi channel
* Returns		: None
***********************************************************************/
static void Spi_Adapt_RxHandler(SPI_CH_TYPE spi)
{
	U8 lubDataRecv = SPI_ReadDataReg(ssSpiFrameInfo[spi].spiChannel);
	if(gtfSpiRxCallbackHandler[spi] != NULL)
	{
		gtfSpiRxCallbackHandler[spi](ssSpiFrameInfo[spi].mode,ssSpiFrameInfo[spi].rxCnt,lubDataRecv);
		ssSpiFrameInfo[spi].rxCnt++;
	}
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
* Name 		 	: Spi_Adapt_TxHandler
* Description	: SPI Tx handler
* Parameter		: None
* Returns		: None
***********************************************************************/
static void Spi_Adapt_TxHandler(SPI_CH_TYPE spi)
{
	U8 lubTxData = 0U;
	if(ssSpiFrameInfo[spi].txCnt == 0U)
	{
		
		SPI_TxIntDisable(ssSpiFrameInfo[spi].spiChannel);
		SPI_IntDisable(ssSpiFrameInfo[spi].spiChannel);
		if(NULL != gtfSpiTxCallbackHandler[spi])
		{
			gtfSpiTxCallbackHandler[spi](ssSpiFrameInfo[spi].mode);  /* send complete */
		}
		else
		{
			/* do nothing... */
		}
		/* disable interrupt */
	}
	else
	{
		if(ssSpiFrameInfo[spi].txBuf == NULL)//read mode is NULL
		{
			lubTxData = 0;//send default value
		}
		else
		{
			lubTxData = *(ssSpiFrameInfo[spi].txBuf);
			ssSpiFrameInfo[spi].txBuf++;//offset buffer
		}
		SPI_WriteDataReg(ssSpiFrameInfo[spi].spiChannel,lubTxData);
		ssSpiFrameInfo[spi].txCnt--;
	}


}



