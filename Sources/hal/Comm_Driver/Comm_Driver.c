/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Driver for IIC communication for NXP platform,use slave mode
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/08/28
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/28  Huang Haoxiong      Initial
*************************************************************************************************************/

#include "Comm_Driver.h"
#ifdef USE_COMM_DRIVER

static E_HAL_STATE Comm_Driver_IOControl(U16 type,void *buffer);
static E_HAL_STATE Comm_Driver_Write(U32 address,U8* buffer, U16 Len);
static void Comm_Driver_RequestINT(void);
static void Comm_Driver_ReleaseINT(void);
static void Comm_Driver_Start(void);
static void Comm_Driver_Stop(void);

static U16 subI2cSlaveAddr = 0U;
static MYBOOL smbI2cIsOpen = FALSE;

bdev gsCommDrv =
{
	.hal_block_Init 		= NULL,
	.hal_block_DeInit	 	= NULL,
	.hal_block_open			= NULL,
	.hal_block_close 		= NULL,
	.hal_block_read 		= NULL,
	.hal_block_write		= Comm_Driver_Write,
	.hal_block_iocontrol 	= Comm_Driver_IOControl,
};

/***********************************************************************
* Name        	: Comm_Driver_Write
* Description 	: Write Driver
* Parameter   	: address:target address,buffer:data pointer,Len:data length
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE Comm_Driver_Write(U32 address,U8* buffer, U16 Len)
{
	E_HAL_STATE leState = E_HAL_STATE_OK;
	if(IIC_Adapt_Send((U8)address,buffer,(U8)Len) == TRUE)
	{
		leState = E_HAL_STATE_OK;
	}
	else
	{
		leState = E_HAL_STATE_FAIL;
	}
	return leState;
}

/***********************************************************************
* Name        	: Comm_Driver_IOControl
* Description	: IOControl Driver
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static E_HAL_STATE Comm_Driver_IOControl(U16 type,void *buffer)
{
    E_HAL_STATE leState = E_HAL_STATE_OK;

	switch(type)
	{
		case COMM_FUNC_RECV_CALLBACK_REGIST:
			IIC_Adapt_SetRecvCallback((COMM_RECEIVE_DATA_CALLBACK)buffer);
			break;
		case COMM_FUNC_ERR_CALLBACK_REGIST:
			IIC_Adapt_SetErrCallback((COMM_ERR_CALLBACK)buffer);
			break;
		case COMM_FUNC_FINISH_CALLBACK_REGIST:
			IIC_Adapt_SetStopCallback((COMM_FINISHED_CALLBACK)buffer);
			break;
		case COMM_FUNC_PAIRED_CALLBACK_REGIST:
			IIC_Adapt_SetPairCallback((COMM_PAIRED_CALLBACK)buffer);
			break;
		case COMM_FUNC_START:
			if(NULL == buffer)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				if((smbI2cIsOpen == TRUE) && (subI2cSlaveAddr == *(U8 *)buffer))
				{
					/* do nothing... */
				}
				else
				{
					subI2cSlaveAddr = *(U8 *)buffer;
					Comm_Driver_Start();
				}
			}
			break;
		case COMM_FUNC_STOP:
			Comm_Driver_Stop();
			break;
		case COMM_FUNC_RESTART:
			Comm_Driver_Start();
			break;
		case COMM_FUNC_REQUEST_CRQ:
			Comm_Driver_RequestINT();
			break;
		case COMM_FUNC_RELEASE_CRQ:
			Comm_Driver_ReleaseINT();
			break;
		default:
			leState = E_HAL_STATE_NO_FUNC;
			break;
	}
	
	return leState;

}

/***********************************************************************
* Name        	: Comm_Driver_RequestINT
* Description	: Request CRQ signal
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void Comm_Driver_RequestINT(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_CRQ,HIGH_LEVEL);
}

/***********************************************************************
* Name        	: Comm_Driver_ReleaseINT
* Description	: Release the CRQ signal
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void Comm_Driver_ReleaseINT(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_CRQ,LOW_LEVEL);
}


/***********************************************************************
* Name        	: Comm_Driver_Start
* Description	: Start IIC comunication
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void Comm_Driver_Start(void)
{
	smbI2cIsOpen = TRUE;
	IIC_Adapt_Start(E_COMM_MODE_SLAVE,E_COMM_BAUD_400K,subI2cSlaveAddr);
}

/***********************************************************************
* Name        	: Comm_Driver_Stop
* Description	: Stop IIC communication
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void Comm_Driver_Stop(void)
{
	smbI2cIsOpen = FALSE;
	IIC_Adapt_Stop();
}

#endif

