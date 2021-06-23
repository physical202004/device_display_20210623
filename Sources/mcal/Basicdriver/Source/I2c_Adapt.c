/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department : ADS AD                                                 *
*   AUTHOR     :                                                       *
************************************************************************
* Object        :
* Module        :	IIC
* Instance      :
* Description   :	
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    	April 16,2020 Huang Haoxiong	Initial
*=====================================================================*/
#include "I2c_Adapt.h"
#include "Global_Definition.h"


static U8 subI2cTxCnt = 0U;
static U8 subI2cRxCnt = 0U;
static U8 subI2cRxLen = 0U;
static MYBOOL smbI2cAddrIsPair = FALSE;
static U8 subRestartCnt = 0U;
static U8 subI2cSlaveAddr = 0U;
static U8 *subpI2cSendBuf = NULL;
static E_COMM_MODE seI2cMode = E_COMM_MODE_NONE;
static E_COMM_DIR seI2cDir = E_COMM_DIR_R;

static void IIC_Adapt_ClearData(void);
static void IIC_Adapt_SMBHandler(void);
static void IIC_Adapt_MasterHandler(void);
static void IIC_Adapt_SlaveHandler(void);
static void IIC_Adapt_SlaveStopHandler(void);
static void IIC_Adapt_MasterStopHandler(void);
static void IIC_Adapt_ISR_Handler(void);



/***********************************************************************
* Name        	: IIC_Adapt_ClearData
* Description	: Clear module all data
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void IIC_Adapt_ClearData(void)
{
	subI2cTxCnt = 0U;
	subI2cRxCnt = 0U;
	subI2cRxLen = 0U;
	smbI2cAddrIsPair = FALSE;
	subpI2cSendBuf = NULL;
}

/***********************************************************************
* Name        	: IIC_Adapt_SetRecvCallback
* Description	: Register call back function
* Parameter		: recvFunc:receive function
* Returns     	: None
***********************************************************************/
void IIC_Adapt_SetRecvCallback(COMM_RECEIVE_DATA_CALLBACK recvFunc)
{
	gtfI2cRecvCallbackFunc = recvFunc;
}

/***********************************************************************
* Name        	: IIC_Adapt_SetStopCallback
* Description	: Register call back function
* Parameter		: stopFunc:finish function
* Returns     	: None
***********************************************************************/
void IIC_Adapt_SetStopCallback(COMM_FINISHED_CALLBACK stopFunc)
{
	gtfI2cStopCallbackFunc = stopFunc;
}

/***********************************************************************
* Name        	: IIC_Adapt_SetErrCallback
* Description	: Register call back function
* Parameter		: errFunc:error function
* Returns     	: None
***********************************************************************/
void IIC_Adapt_SetErrCallback(COMM_ERR_CALLBACK errFunc)
{
	gtfI2cErrCallbackFunc = errFunc;
}

/***********************************************************************
* Name        	: IIC_Adapt_SetPairCallback
* Description	: Register call back function
* Parameter		: pairFunc:pair function
* Returns     	: None
***********************************************************************/
void IIC_Adapt_SetPairCallback(COMM_PAIRED_CALLBACK pairFunc)
{
	gtfI2cAddrPairedCallbackFunc = pairFunc;
}


/***********************************************************************
* Name			: IIC_Adapt_Start
* Description	: Initialize the I2C module
* Parameter 	: mode - master or slave
*				  baudrate - baud rate
*                 Addr - slave address, 8bit address
* Returns		: None
***********************************************************************/
void IIC_Adapt_Start(E_COMM_MODE mode,E_COMM_BAUD baudrate,U8 SlaveAddress)
{  
    I2C_ConfigType ltI2cConfig = {{0}};

	ltI2cConfig.sSetting = gctIICSettings;								//set default mode
	ltI2cConfig.u16Filt |= I2C_FLT_SSIE_MASK;							//set filter for iic pair
	ltI2cConfig.u16Slt = IIC_LOW_TIMEOUT_VALUE;  						//set low level timeout
	seI2cMode = mode;													//set mode

    if(E_COMM_MODE_MASTER == mode)
    {
		if(E_COMM_BAUD_100K == baudrate)								//set baudrate
		{
			ltI2cConfig.u16F = IIC_BAUD_100K;
		}
		else
		{
			ltI2cConfig.u16F = IIC_BAUD_400K;
		}
		ltI2cConfig.sSetting.bMSTEn = IIC_C1_MAT_MASTER_MODE;			//set mode
    }
    else
    {
		subI2cSlaveAddr = SlaveAddress;
        ltI2cConfig.u16OwnA1 = SlaveAddress;							//set slave address
		ltI2cConfig.sSetting.bMSTEn = IIC_C1_MAT_SLAVE_MODE;			//set mode
    }
	IIC_Adapt_ClearData();												//clear data
	I2C0_SetCallBack(IIC_Adapt_ISR_Handler);							//set interrupt callback
    I2C_Init(I2C0, &ltI2cConfig);										//initial

}

/***********************************************************************
* Name        	: IIC_Adapt_Stop
* Description	: Stop IIC module
* Description	: None
* Returns     	: None
***********************************************************************/
void IIC_Adapt_Stop(void)
{
	if(seI2cMode == E_COMM_MODE_NONE)
	{
		/*do nothing... */
	}
	else
	{
		seI2cMode = E_COMM_MODE_NONE;
    	I2C_Stop(I2C0);
	}
}

/***********************************************************************
* Name			: IIC_Adapt_Send
* Description	: Prepare data to send out
* Description	: addr:Register address, this value is only valid in master mode
*				  pBuf:pointer to first byte to send
*				  len:length of bytes to send
* Returns		: TRUE:sucessful,FALSE:fail
***********************************************************************/
MYBOOL IIC_Adapt_Send(U8 addr,U8 *pBuf,U8 len)
{
	MYBOOL lmbRet = FALSE;
	if((pBuf == NULL) || (len == 0U))
	{
		/* invalid,return */
	}
	else
	{
		if(E_COMM_MODE_MASTER == seI2cMode)
		{
	    	lmbRet = I2C_MasterSend(I2C0, addr);//send register address first
		}
		else
		{
			lmbRet = TRUE;
		}
		subpI2cSendBuf = pBuf;
		subI2cTxCnt = len;
		seI2cDir = E_COMM_DIR_W;
	}
    return lmbRet;
}

/***********************************************************************
* Name			: IIC_Adapt_Read
* Description	: set receive address and length
* Description	: addr:Register address
*				  pBuf:read buffer pointer
*				  len:length of bytes to receive
* Returns		: TRUE:sucessful,FALSE:fail
***********************************************************************/
MYBOOL IIC_Adapt_Read(U8 addr,U8 *pBuf,U8 len)
{
	MYBOOL lmbRet = FALSE;
	if(len == 0U)
	{
		/* return */
	}
	else
	{
		if(E_COMM_MODE_SLAVE == seI2cMode)
		{
			/* slave cannot actively read data */
		}
		else
		{
	    	lmbRet = I2C_MasterRead(I2C0, addr);//send register address first
	    	subI2cRxCnt = 0U;
			subI2cRxLen = len;
			seI2cDir = E_COMM_DIR_R;
		}
	}
    return lmbRet;
}


/***********************************************************************
* Name			: IIC_Adapt_SMBHandler
* Description	: scl line abnormal timeout handler
* Description	: None
* Returns		: None
***********************************************************************/
static void IIC_Adapt_SMBHandler(void)
{
	ERR_TYPE leErrType = ERR_IIC_SCL_ABNORMAL;
    if(I2C_IsSMB_SLTF(I2C0))//SCL low timeout occurs, release i2c
    {
        I2C_ClearSLTF(I2C0);
    }
    else if(I2C_IsSMB_SHTF2(I2C0))//SCL high timeout occurs, release i2c
    {
        I2C_ClearSHTF2(I2C0);

    }
	else
	{
		/* Impossible happen here... */
	}
	I2C_Deinit(I2C0);//Deinit
	if(NULL != gtfI2cErrCallbackFunc)
	{
		gtfI2cErrCallbackFunc(seI2cMode,leErrType);
	}
	else
	{
		/* do nothing */
	}

}


/***********************************************************************
* Name			: IIC_Adapt_MasterHandler
* Description	: Send and receive handler in master mode
* Description	: None
* Returns		: None
***********************************************************************/
static void IIC_Adapt_MasterHandler(void)
{
#if 0	//not use master driver
   U8 lubRecvData = 0U;

	if(I2C_IsTxMode(I2C0))//tx
	{
		seI2cDir = E_COMM_DIR_W;
		if((subI2cTxCnt > 0) && (subpI2cSendBuf != NULL))
	    {
	        if(!I2C_IsReceivedAck(I2C0))	//ack
	        {
	            if(I2C_IsTxMode(I2C0))
	            {
	                I2C_WriteDataReg(I2C0,*subpI2cSendBuf);
					subpI2cSendBuf++;
					subI2cTxCnt--;
	            }
	            else
	            {
	                I2C_RxEnable(I2C0);							//addr cycle ack, change to rx mode
	                lubRecvData = I2C_ReadDataReg(I2C0);		//dummy read
	            }
	        }
	        else
	        {
	            I2C_Stop(I2C0);
	        }
	    }
	    else
	    {
	        I2C_Stop(I2C0);
	    }
	}
	else
	{
		seI2cDir = E_COMM_DIR_R;
		lubRecvData = I2C_ReadDataReg(I2C0);//read data
		/* Master receive data from slave */
		if(subI2cRxCnt < subI2cRxLen)
		{
			if(subI2cRxCnt < (subI2cRxLen - 1))
			{
				I2C_SendNack(I2C0);
			}
			else
			{
				I2C_SendAck(I2C0);
			}

			if(NULL != gtfI2cRecvCallbackFunc)
			{
				gtfI2cRecvCallbackFunc(E_COMM_MODE_MASTER,subI2cRxCnt,lubRecvData);
			}
			else
			{
				/* do nothing... */
			}
			subI2cRxCnt++;
		}
		else
		{
			I2C_Stop(I2C0);
		}

	}  
#endif
}

/***********************************************************************
* Name			: IIC_Adapt_SlaveHandler
* Description	: Send and receive handler in slave mode
* Description	: None
* Returns		: None
***********************************************************************/
static void IIC_Adapt_SlaveHandler(void)
{	
	U8 lubRecvData = 0U;
    /* Master send data to slave */
    if(I2C_GetStatus(I2C0) & I2C_S_IAAS_MASK)//check if the address matches
    {
		smbI2cAddrIsPair = TRUE;
		/* Slave send acknowledge*/
		I2C_SendAck(I2C0);
		
		if(I2C_GetStatus(I2C0)& I2C_S_SRW_MASK)//tx
        {
			seI2cDir = E_COMM_DIR_W;
			lubRecvData = subI2cSlaveAddr + 1;
			I2C_TxEnable(I2C0);
			if((subI2cTxCnt > 0) && (subpI2cSendBuf != NULL))
			{
				I2C_WriteOneByte(I2C0,*subpI2cSendBuf);	//include I2C_WriteDataReg function
				subpI2cSendBuf++;
				subI2cTxCnt--;
			}
			else
			{
				I2C_WriteOneByte(I2C0,0xFFU);
			}
        }
        else
        {
			I2C_RxEnable(I2C0);
			lubRecvData = I2C_ReadDataReg(I2C0); //dummy read,this byte is address,not use
        }
		/* request is respond */
		if(NULL != gtfI2cAddrPairedCallbackFunc)
		{
			gtfI2cAddrPairedCallbackFunc(seI2cDir,&lubRecvData);
		}
		else
		{
			/* do nothing... */
		}

		return;
    }
	else
	{
		/* Mismatch... */
	}
	if(smbI2cAddrIsPair == TRUE)
	{
        if(I2C_GetStatus(I2C0)& I2C_S_SRW_MASK)//tx
        {
        	if(I2C_IsReceivedAck(I2C0))
        	{
        		//seI2cDir = E_COMM_DIR_R;
        		I2C_RxEnable(I2C0);
        		lubRecvData = I2C_ReadDataReg(I2C0);
        	}
        	else
        	{
				seI2cDir = E_COMM_DIR_W;
				I2C_TxEnable(I2C0);
				if((subI2cTxCnt > 0) && (subpI2cSendBuf != NULL))
				{
					I2C_WriteOneByte(I2C0,*subpI2cSendBuf);	//include I2C_WriteDataReg function
					subpI2cSendBuf++;
					subI2cTxCnt--;
				}
				else
				{
					I2C_WriteOneByte(I2C0,0xFFU);
				}
        	}
        }
        else
        {
			seI2cDir = E_COMM_DIR_R;
            I2C_RxEnable(I2C0);
            lubRecvData = I2C_ReadDataReg(I2C0); 
		    if(NULL != gtfI2cRecvCallbackFunc)
			{
				gtfI2cRecvCallbackFunc(E_COMM_MODE_SLAVE,subI2cRxCnt,lubRecvData);
			}
			else
			{
				/* do nothing... */
			}
			subI2cRxCnt++;
        }
    }
    else
    {
		/* Mismatch... */
    }
}

static void IIC_Adapt_SlaveStopHandler(void)
{
	ERR_TYPE leErrType = ERR_NOT_SEND_COMPLETE;
	
	if(smbI2cAddrIsPair == TRUE)
	{
		/* Check the frame transmission status*/
		if((seI2cDir == E_COMM_DIR_W) && (subI2cTxCnt > 0) &&\
		   (NULL != gtfI2cErrCallbackFunc))
		{
		   IIC_Adapt_ClearData();//clear data
		   gtfI2cErrCallbackFunc(seI2cMode,leErrType);
		}
		else if((seI2cDir == E_COMM_DIR_R) && (subI2cRxCnt == 0))
		{
		   /* do nothing... */
		}
		else
		{
		   IIC_Adapt_ClearData();//clear data
		   if(NULL != gtfI2cStopCallbackFunc)
		   {
			   gtfI2cStopCallbackFunc(seI2cMode, seI2cDir);
		   }
		   else
		   {
			   /* do nothing... */
		   }
		   
		}
	}
	else
	{
	/* do nothing... */
	}

}

static void IIC_Adapt_MasterStopHandler(void)
{

}


/***********************************************************************
* Name			: IIC_Adapt_ISR_Handler
* Description	: IIC interrupt handler
* Description	: None
* Returns		: None
***********************************************************************/
static void IIC_Adapt_ISR_Handler(void)
{

    if(I2C_IsSMB_SLTF(I2C0) || I2C_IsSMB_SHTF2(I2C0))      				//SCL line timeout occurs
    {
		I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);								//clear mask
        IIC_Adapt_SMBHandler();
    }
    else
    {
        if(I2C0->FLT & I2C_FLT_STOPF_MASK)                      	    //stop interrupt
        {
			I2C0->FLT |= I2C_FLT_STOPF_MASK;                            //clear stop mask
			I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);						//clear mask
			subRestartCnt = 0U;											//clear restart counter
			switch (seI2cMode)
			{
				case E_COMM_MODE_MASTER:
					IIC_Adapt_MasterStopHandler();
					break;
				case E_COMM_MODE_SLAVE:
					IIC_Adapt_SlaveStopHandler();
					break;
				default:
					/* do nothing... */
					break;
			}
        }
        else
        {
            if(I2C0->FLT & I2C_FLT_STARTF_MASK)                  	    //start interrupt
            {
                I2C0->FLT |= I2C_FLT_STARTF_MASK;                    	//clear start mask
                I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);					//clear mask
                subRestartCnt++;
				if(subRestartCnt == 2)
				{
					/* restart detected */
				}
				else
				{
                	smbI2cAddrIsPair = FALSE;								//clear status
				}
            }
			else
			{
				I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);								//clear mask
				switch (seI2cMode)
				{
					case E_COMM_MODE_MASTER:
						IIC_Adapt_MasterHandler();
						break;
					case E_COMM_MODE_SLAVE:
						IIC_Adapt_SlaveHandler();
						break;
					default:
						break;
				}
			}

        }
    }

}

