/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : communication module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "CommServ.h"

static E_COMM_FRAME_STATE seCommRecvFrameState = E_COMM_FRAME_STATE_IDLE;//Show which mode data was received
static E_COMM_FRAME_STATE seCommSendFrameState = E_COMM_FRAME_STATE_IDLE;//send state
static MYBOOL smbCommTxFinishState = TRUE;//send state
static MYBOOL smbCommRxFinishState = FALSE;//receive state
static MYBOOL smbIsRequestToEol = FALSE;//Is there an instruction to receive a request to enter EOL mode
static U8 subCommRecvLen = 0U;//reveive buffer length
static U8 subEolSysCmdByte = COMM_EOL_SYS_CMD_NONE;//system comand state
static U16 suhwEolEnterTimeoutCnt = 0U;//timeout counter of enter to eol from app

/* eol frame structure*/
static S_COMM_EOL_FRAME ssEolFrame = {
	.deviceType = COMM_EOL_FRAME_DEVICE_TYPE_NONE,
	.deviceCmd = 0,
	.dirPara = 0,
};

/* receive buffer */
static U8 saubCommRecvBuf[COMM_RX_BUFFER_MAX] = {0};
/* send buffer */
static U8 saubCommSendBuf[COMM_TX_BUFFER_MAX] = {0};

static MYBOOL CommServ_SendMessage(E_COMM_FRAME_STATE sendState,U8* ptr,U8 length);
static void CommServ_ClearRecvData(void);
static void CommServ_RxHandler(E_COMM_MODE mode,U8 index,U8 data);
static void CommServ_ErrHandler(E_COMM_MODE mode,ERR_TYPE errType);
static void CommServ_FinishedHandler(E_COMM_MODE mode,E_COMM_DIR rw_dir);
static void CommServ_PairedHandler(E_COMM_DIR dir,void *data);
static void CommServ_Slave_APP_RxEndHandler(void);
static void CommServ_Slave_APP_TxEndHandler(void);
static void CommServ_APP_RecvFrameHandler(void);
static void CommServ_APP_SendFrameHandler(void);
static void CommServ_APP_SysCmdHandler(void);
static void CommServ_ReqToEOLTimer(void);
static void CommServ_EOL_RecvFrameHandler(void);
static MYBOOL CommServ_ReplySysMessage(void);

/***********************************************************************
*  Name        	: CommServ_SendMessage
*  Description 	: Send frame processing
*  Parameter   	: ptr:buffer,length:length of buffer
*  Returns     	: TRUE:send successful,FALSE:send failed
***********************************************************************/
MYBOOL CommServ_SendMessage(E_COMM_FRAME_STATE sendState,U8* ptr,U8 length)
{
	MYBOOL lmbRet = TRUE;
	
	if(E_HAL_STATE_OK == HALBlock_Write(COMM_DRIVER_ID,0,ptr,length))
	{
		if(E_COMM_FRAME_STATE_EOL == sendState)
		{
			(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_REQUEST_CRQ,NULL);
		}
		else
		{
			/* do nothing... */
		}
		seCommSendFrameState = sendState; //change state
		smbCommTxFinishState = FALSE;//change state
	}
	else
	{
		lmbRet = FALSE;
	}
	return lmbRet;
}

/***********************************************************************
*  Name        	: CommServ_ClearRecvData
*  Description 	: reset receive frame status
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_ClearRecvData(void)
{
    seCommRecvFrameState = E_COMM_FRAME_STATE_IDLE;
	subCommRecvLen = 0U;
}

/***********************************************************************
*  Name        	: CommServ_EOLEntryHandler
*  Description 	: entery eol task handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void CommServ_EOLEntryHandler(void)
{
	
	smbCommRxFinishState = FALSE;
	subCommRecvLen = 0;
	memset(saubCommRecvBuf,0,sizeof(saubCommRecvBuf));
	
}

/***********************************************************************
*  Name        	: CommServ_NormalEntryHandler
*  Description 	: Entry normal mode handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void CommServ_NormalEntryHandler(void)
{
	MYBOOL lmbCommCanOpenState = FALSE;
	CS_SIG_SyncReceive(SIG_BOOL_COMM_CAN_OPEN,&lmbCommCanOpenState);
	/* register callback function */
	if(lmbCommCanOpenState == TRUE)
	{
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_RECV_CALLBACK_REGIST,CommServ_RxHandler);
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_ERR_CALLBACK_REGIST,CommServ_ErrHandler);
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_FINISH_CALLBACK_REGIST,CommServ_FinishedHandler);
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_PAIRED_CALLBACK_REGIST,CommServ_PairedHandler);

		/* data initial */
		smbCommRxFinishState = FALSE;
		smbCommTxFinishState = TRUE;
		subCommRecvLen = 0;
		memset(saubCommRecvBuf,0,sizeof(saubCommRecvBuf));
	}
	else
	{
		//waiting...
	}
}
/***********************************************************************
*  Name        	: CommServ_NormalActiveHandler
*  Description 	: 10ms running task
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void CommServ_NormalActiveHandler(void)
{
	MYBOOL lmbCommCanOpenState = FALSE;
	U8 lubSlaveAddr = COMM_I2C_SLAVE_ADDRESS;
	CS_SIG_SyncReceive(SIG_BOOL_COMM_CAN_OPEN,&lmbCommCanOpenState);
	if(lmbCommCanOpenState == TRUE)
	{
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_START,&lubSlaveAddr);//need not care result
		if(smbCommRxFinishState == TRUE)
		{
			smbCommRxFinishState = FALSE;
			if(E_COMM_FRAME_STATE_EOL == seCommRecvFrameState)
			{
				CommServ_EOL_RecvFrameHandler();//deal with the EOL frame
			}
			else if(E_COMM_FRAME_STATE_APP == seCommRecvFrameState)
			{
				CommServ_APP_RecvFrameHandler();//deal with the app frame

			}
			else
			{
				/* do nothing... */
			}
			CommServ_ClearRecvData();
		}
		else
		{
			/* do nothing... */
		}
		if(smbCommTxFinishState == TRUE)
		{
			CommServ_APP_SendFrameHandler();
		}
		else
		{
			/* do nothing... */
		}
		CommServ_APP_SysCmdHandler();//system command handler,place 10ms cycle
	}
	else
	{
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_STOP,NULL);//need not care result
	}
}



/***********************************************************************
*  Name        	: CommServ_EOLActiveHandler
*  Description 	: Cycle task in EOL active state
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void CommServ_EOLActiveHandler(void)
{
	MYBOOL lmbCommCanOpenState = FALSE;
	U8 lubSysReqChange = SYS_CMD_REQUEST_TO_APP;
	U8 lubSlaveAddr = COMM_I2C_SLAVE_ADDRESS;
	CS_SIG_SyncReceive(SIG_BOOL_COMM_CAN_OPEN,&lmbCommCanOpenState);
	if(lmbCommCanOpenState == TRUE)
	{
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_START,&lubSlaveAddr);//need not care result
		if(smbCommRxFinishState == TRUE)
		{
			if(E_COMM_FRAME_STATE_EOL == seCommRecvFrameState)
			{
				CommServ_EOL_RecvFrameHandler();//deal with the eol frame
				switch(saubCommRecvBuf[COMM_EOL_CMD_BYTE])
				{
					case COMM_EOL_CMD_SYSTEM:

						if(COMM_EOL_SYS_CMD_EXIT_EOL == saubCommRecvBuf[COMM_EOL_SYS_CMD_BYTE])
						{
							subEolSysCmdByte = COMM_EOL_SYS_CMD_EXIT_EOL;
							if(CommServ_ReplySysMessage() == TRUE)//reply message
							{
								lubSysReqChange = SYS_CMD_REQUEST_TO_APP;
								CS_SIG_SyncSend(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
							}
							else
							{
								/* do nothing... */
							}
						}
						else if(COMM_EOL_SYS_CMD_NONE == saubCommRecvBuf[COMM_EOL_SYS_CMD_BYTE])
						{
							smbCommRxFinishState = FALSE;
							seCommRecvFrameState = E_COMM_FRAME_STATE_IDLE;
						}
						else
						{
							subEolSysCmdByte = COMM_EOL_SYS_CMD_ENTER_EOL;
							(void)CommServ_ReplySysMessage();//not care result
						}
						break;
					case COMM_EOL_CMD_EOL:
						ssEolFrame.deviceType = saubCommRecvBuf[COMM_EOL_DEVICE_TYPE_BYTE];//set device type
						ssEolFrame.deviceCmd = saubCommRecvBuf[COMM_EOL_DEVICE_CMD_BYTE];//set device command
						break;
					default:
						CommServ_ClearRecvData();//invalid frame
						smbCommRxFinishState = FALSE;
						break;
				}	
			}
			else
			{
				/* other frame,quit... */
			}
		}
		else
		{
			/* do nothing... */
		}

	}
	else
	{
		(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_STOP,NULL);//need not care result
	}
}


/***************************************************************** APP ********************************************************************************/
/***********************************************************************
*  Name        	: CommServ_APP_SendFrameHandler
*  Description 	: All send frame processing in 10ms task
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_APP_SendFrameHandler(void)
{

}

/***********************************************************************
*  Name        	: CommServ_APP_RecvFrameHandler
*  Description 	: All receive frame processing in 10ms task
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_APP_RecvFrameHandler(void)
{

}


/***********************************************************************
*  Name        	: CommServ_Slave_APP_TxEndHandler
*  Description 	: Send complete handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_Slave_APP_TxEndHandler(void)
{
	
}

/***********************************************************************
*  Name        	: CommServ_Slave_APP_RxEndHandler
*  Description 	: receive complete handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_Slave_APP_RxEndHandler(void)
{

}

/***********************************************************************
*  Name        	: CommServ_APP_SysCmdHandler
*  Description 	: Counter for request to eol mode from app
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_APP_SysCmdHandler(void)
{
	U8 laubTempBuffer[2] = {0};
	U8 lubSysReqChange = SYS_CMD_REQUEST_TO_APP;

	switch(subEolSysCmdByte)
	{
		case COMM_EOL_SYS_CMD_REQ_EOL:
			if(CommServ_ReplySysMessage() == TRUE)//reply message
			{
				suhwEolEnterTimeoutCnt = 0;									//clear counter
				smbIsRequestToEol = TRUE;									//received request enter to the eol mode
			}
			else
			{
				/* do nothing... */
			}
			break;
		case COMM_EOL_SYS_CMD_ENTER_EOL:
			/* if EOL is allowed and reply message sucessful */
			if(smbIsRequestToEol == TRUE)
            {
				if(CommServ_ReplySysMessage() == TRUE)
				{
					smbIsRequestToEol = FALSE;
					lubSysReqChange = SYS_CMD_REQUEST_TO_EOL;
					CS_SIG_SyncSend(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
				}
				else
	            {
	                /* do nothing... */
	            }
            }
            else
            {
                /* do nothing... */
            }
			break;
		case COMM_EOL_SYS_CMD_EXIT_EOL:
			/* do nothing... */
			break;
		case COMM_EOL_SYS_CMD_ACCESS_KEY1:
			if(smbIsRequestToEol == TRUE)
			{
				laubTempBuffer[0] = COMM_EOL_ACCESS_KEY1_VALUE >> 8;
				laubTempBuffer[1] = COMM_EOL_ACCESS_KEY1_VALUE & 0xFFU;
	            if(EepServ_WriteStaticData(E_BLOCK_0,EEP_ACCESS_KEY1_ADDR,laubTempBuffer,2) == TRUE)
	            {
	                (void)CommServ_ReplySysMessage();
	            }
				else
				{
					/* do nothing... */
				}
			}
			else
            {
                /* do nothing... */
            }
			break;
		case COMM_EOL_SYS_CMD_ACCESS_KEY2:
			if(smbIsRequestToEol == TRUE)
            {
                laubTempBuffer[0] = COMM_EOL_ACCESS_KEY2_VALUE >> 8;
				laubTempBuffer[1] = COMM_EOL_ACCESS_KEY2_VALUE & 0xFFU;
                if(EepServ_WriteStaticData(E_BLOCK_0,EEP_ACCESS_KEY2_ADDR,laubTempBuffer,2) == TRUE)
                {
                    if(CommServ_ReplySysMessage() == TRUE)
                    {
						lubSysReqChange = SYS_CMD_REQUEST_TO_BOOT;
						CS_SIG_SyncSend(SIG_CHAR_SYSTEM_MODE_CHANGE,&lubSysReqChange);
                    }
					else
					{
						/* do nothing... */
					}
                }
				else
				{
					/* do nothing... */
				}
            }
			else
			{
				/* do nothing... */
			}
			break;
		default:
			/* do nothing... */
			break;
	}
	CommServ_ReqToEOLTimer();
}

/***************************************************************** EOL ********************************************************************************/
/***********************************************************************
*  Name        	: CommServ_EOL_GetDeviceType
*  Description 	: Return frame type of device byte
*  Parameter   	: None
*  Returns     	: Type of frame
***********************************************************************/
U8 CommServ_EOL_GetDeviceType(void)
{
	return ssEolFrame.deviceType;
}

/***********************************************************************
*  Name        	: CommServ_EOL_GetDeviceCmd
*  Description 	: Return frame device cmd byte
*  Parameter   	: None
*  Returns     	: Command of frame
***********************************************************************/
U8 CommServ_EOL_GetDeviceCmd(void)
{
	return ssEolFrame.deviceCmd;
}

/***********************************************************************
*  Name        	: CommServ_EOL_GetFrameData
*  Description 	: Return frame data
*  Parameter   	: None
*  Returns     	: Pointer of buffer
***********************************************************************/
U8* CommServ_EOL_GetFrameData(void)
{
	return saubCommRecvBuf;
}


/***********************************************************************
*  Name        	: CommServ_ReqToEOLTimer
*  Description 	: Counter for request to eol mode from app
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_ReqToEOLTimer(void)
{
	if(smbIsRequestToEol == TRUE)
	{
		suhwEolEnterTimeoutCnt++;
		if(suhwEolEnterTimeoutCnt >= COMM_EOL_CHANGE_MODE_TIMEOUT)    //5s
		{
			suhwEolEnterTimeoutCnt = 0U;//clear timer
			smbIsRequestToEol = FALSE;//reset state
		}
	}
    else
    {
        suhwEolEnterTimeoutCnt = 0U;
    }
}

/***********************************************************************
*  Name        	: CommServ_EOL_RecvFrameHandler
*  Description 	: EOL receive frame handler in 10ms task
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void CommServ_EOL_RecvFrameHandler(void)
{
	U8 lubchecksum = 0U;
	U8 lubDataSectionLen = saubCommRecvBuf[E_COMM_EOL_STATE_LENGTH];//set data length

	if((lubDataSectionLen == (subCommRecvLen - COMM_EOL_NODATA_LENGTH)) && \
		(lubDataSectionLen < COMM_EOL_DATA_LENGTH_LIMIT) && (lubDataSectionLen > 0))//check data length invalid
	{
		lubchecksum = FrameChecksum(saubCommRecvBuf,subCommRecvLen - 1);//calculation
		if(saubCommRecvBuf[subCommRecvLen - 1] == lubchecksum)
		{
			ssEolFrame.dirPara = saubCommRecvBuf[E_COMM_EOL_STATE_DIR_RESERVED];//set parameter value
			if(saubCommRecvBuf[COMM_EOL_CMD_BYTE] == COMM_EOL_CMD_SYSTEM)
			{
				subEolSysCmdByte = saubCommRecvBuf[COMM_EOL_SYS_CMD_BYTE];//update system command status
			}
			else if(saubCommRecvBuf[COMM_EOL_CMD_BYTE] == COMM_EOL_CMD_EOL)
			{
				ssEolFrame.deviceType = saubCommRecvBuf[COMM_EOL_DEVICE_TYPE_BYTE];//set device type
				ssEolFrame.deviceCmd = saubCommRecvBuf[COMM_EOL_DEVICE_CMD_BYTE];//set device command
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			/* do nothing... */
		}
	}
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
*  Name        	: CommServ_ReplySysMessage
*  Description 	: Reply system command message
*  Parameter   	: None
*  Returns     	: TRUE:sucessful,FALSE:failed
***********************************************************************/
static MYBOOL CommServ_ReplySysMessage(void)
{
	U8 lubCnt = 0U;
	MYBOOL lmbRet = TRUE;
	
	saubCommSendBuf[lubCnt++] = COMM_EOL_FRAME_HEADER;							//header
	saubCommSendBuf[lubCnt++] = COMM_EOL_REPLY_DIR_BYTE;						//direction
	saubCommSendBuf[lubCnt++] = ssEolFrame.dirPara;;							//direction parameter
	saubCommSendBuf[lubCnt++] = COMM_EOL_REPLY_SYS_LENGTH;						//length
	saubCommSendBuf[lubCnt++] = COMM_EOL_CMD_SYSTEM;							//system command in data section
	saubCommSendBuf[lubCnt++] = subEolSysCmdByte;								//system state in data section
	saubCommSendBuf[lubCnt] = FrameChecksum(saubCommSendBuf,lubCnt);//calculation checksum
	if(CommServ_SendMessage(E_COMM_FRAME_STATE_EOL,saubCommSendBuf,lubCnt + 1) == TRUE)
	{
		/* update status */
		subEolSysCmdByte = COMM_EOL_SYS_CMD_NONE;
		smbCommRxFinishState = FALSE;
	}
	else
	{
		lmbRet = FALSE;//operation failed
	}

	return lmbRet;
}


/***********************************************************************
*  Name        	: CommServ_ReplyEolMessage
*  Description 	: Reply eol command message
*  Parameter   	: result:result in COMM_EOL_CMD_BYTE byte,data:reply buffer,dataLen:length of buffer
*  Returns     	: TRUE:sucessful,FALSE:failed
***********************************************************************/
MYBOOL CommServ_ReplyEolMessage(E_EOL_REPLY_STATE result,U8 *data,U8 dataLen)
{
	U8 lubCnt = 0U;
	MYBOOL lmbRet = TRUE;
	if(((dataLen + COMM_EOL_REPLY_FRAME_NODATA_LENGTH) <= COMM_TX_BUFFER_MAX) && (E_COMM_FRAME_STATE_EOL == seCommRecvFrameState))
	{
		saubCommSendBuf[lubCnt++] = COMM_EOL_FRAME_HEADER;							//header
		saubCommSendBuf[lubCnt++] = COMM_EOL_REPLY_DIR_BYTE;						//direction
		saubCommSendBuf[lubCnt++] = ssEolFrame.dirPara;								//direction parameter byte
		saubCommSendBuf[lubCnt++] = COMM_EOL_REPLY_CMD_LENGTH + dataLen;			//calculation data section length
		saubCommSendBuf[lubCnt++] = COMM_EOL_CMD_EOL;								//eol command in data section
		saubCommSendBuf[lubCnt++] = ssEolFrame.deviceType;							//device type in data section
		saubCommSendBuf[lubCnt++] = ssEolFrame.deviceCmd;							//device command in data section
		saubCommSendBuf[lubCnt++] = result;											//result in data section
		if(dataLen > 0)
		{
			memcpy(saubCommSendBuf + lubCnt,data,dataLen);							//copy data to buffer
			lubCnt += dataLen;														//add data length
		}
		else
		{
			/* do nothing... */
		}
		saubCommSendBuf[lubCnt] = FrameChecksum(saubCommSendBuf,lubCnt);//calculation checksum
		if(CommServ_SendMessage(E_COMM_FRAME_STATE_EOL,saubCommSendBuf,lubCnt + 1) == TRUE)//send message
		{
			smbCommRxFinishState = FALSE;//reset
			ssEolFrame.deviceType = COMM_EOL_FRAME_DEVICE_TYPE_NONE;//reset type
		}
		else
		{
			lmbRet = FALSE;//operation failed
		}
	}
	else
	{
		/* invalid,return*/
		lmbRet = FALSE;
	}

	return lmbRet;
}

/***************************************************************** Callback *****************************************************************************/
/***********************************************************************
*  Name        	: CommServ_RxHandler
*  Description 	: Communication Rx callback
*  Parameter   	: mode:communication mode,index:current received position,data:receive buffer
*  Returns     	: None
***********************************************************************/
static void CommServ_RxHandler(E_COMM_MODE mode,U8 index,U8 data)
{
	if(E_COMM_MODE_SLAVE == mode)
	{
		/* Detect whether the last received data frame has been processed in the application. */
		if(FALSE == smbCommRxFinishState)
		{
			if(0U == index)
			{
				if(COMM_EOL_FRAME_HEADER == data)//0x5A
				{
					seCommRecvFrameState = E_COMM_FRAME_STATE_EOL;
					saubCommRecvBuf[0] = data;
				}
				else
				{
					seCommRecvFrameState = E_COMM_FRAME_STATE_APP;
					saubCommRecvBuf[0] = data;
				}
				subCommRecvLen = 1;	//reset length
			}
			else if(index >= COMM_RX_BUFFER_MAX)
			{
				TraceServ_SendStrMsg("data is range out of receive buffer", E_TRACE_EVENT_GENERAL);
			}
			else
			{
				if(E_COMM_FRAME_STATE_IDLE == seCommRecvFrameState)
				{
					/* give up data */
					return;
				}
				else
				{
					saubCommRecvBuf[index] = data;//receive data
					subCommRecvLen++;//length increase
				}
			}
		}
		else
		{
			/* Waiting for the previous frame of data processing to complete */
		}
	}
	else
	{
		/* in master mode */
	}
}

/***********************************************************************
*  Name        	: CommServ_ErrHandler
*  Description 	: Communication error callback
*  Parameter   	: mode:communication mode,errType:type of error
*  Returns     	: None
***********************************************************************/
static void CommServ_ErrHandler(E_COMM_MODE mode,ERR_TYPE errType)
{
	/* restart communication module */
	(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_RESTART,NULL);
}

/***********************************************************************
*  Name 		: CommServ_FinishedHandler
*  Description	: Handler when one frame session finishes(Stop received).
*  Parameter	: mode:communication mode,rw_dir:Tx or Rx
*  Returns		: None
***********************************************************************/
static void CommServ_FinishedHandler(E_COMM_MODE mode,E_COMM_DIR rw_dir)
{
	if(E_COMM_MODE_MASTER == mode)			//master mode
	{
		// do nothing
	}
	else
	{

		if(rw_dir == E_COMM_DIR_R)
		{
			if(subCommRecvLen > 0)
			{
				smbCommRxFinishState = TRUE;//set state first
				if(seCommRecvFrameState == E_COMM_FRAME_STATE_APP)
				{
					/* APP RX END handler */
					CommServ_Slave_APP_RxEndHandler();
				}
				else if(seCommRecvFrameState == E_COMM_FRAME_STATE_EOL)
				{
					/* EOL RX END handler */
				}
				else
				{
					/* do nothing... */
				}
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			smbCommTxFinishState = TRUE;
			if(seCommSendFrameState == E_COMM_FRAME_STATE_APP)
			{
				CommServ_Slave_APP_TxEndHandler();
			}
			else if(seCommSendFrameState == E_COMM_FRAME_STATE_EOL)
			{
				/* EOL TX END handler */
			}
			else
			{
				/* do nothing... */
			}
		}
	}
}


/***********************************************************************
*  Name 		: CommServ_PairHandler
*  Description	: Communication match callback, only valid in slave mode
*  Parameter	: dir:read/write direction
*				  data:U8 device address
*  Returns		: None
***********************************************************************/
static void CommServ_PairedHandler(E_COMM_DIR dir,void *data)
{
	U8 lubDevAddr = *(U8 *)data;

	(void)HALBlock_IoControl(COMM_DRIVER_ID,COMM_FUNC_RELEASE_CRQ,NULL);//slave address paired, release the CRQ signal

	if((E_COMM_DIR_R == dir) && (lubDevAddr == (COMM_I2C_SLAVE_ADDRESS)))
	{
		saubCommSendBuf[0] = 0x33;
		saubCommSendBuf[1] = 0xbb;
		CommServ_SendMessage(E_COMM_FRAME_STATE_APP,saubCommSendBuf,2);
	}
	else
	{
		/* do nothing... */
	}

}

