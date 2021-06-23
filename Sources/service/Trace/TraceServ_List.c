/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           *
*				  All Rights Reserved.             *
*   Department : ADS R&D SW		                   *
*   AUTHOR	   : Huang Haoxiong                    *
************************************************************************
* Object        :
* Module        : Trace
* Instance      :
* Description   : Trace list
*-----------------------------------------------------------------------
* Version       : 0.1
* Date          : 2017/07/21
* Author        : Huang Haoxiong
***********************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1    2017/07/21  Huang Haoxiong      Init
*  0.2	  2018/12/12  Huang Haoxiong	  change data structure from list to queue
************************************************************************/

#include "TraceServ_List.h"
#ifdef TRACE_DEBUG
#include "Uart_Adapt_if.h"

static MYBOOL TraceServ_List_IsFull(U16 len);
static MYBOOL TraceServ_List_IsEmpty(void);

/* trace list */
static U8 subTraceQueue[TRACE_LIST_QUEUE_MAX] = {0u};					//queue buffer
static TRACE_LIST_INDEX_T stTraceQueueEndIndex = 0u;				//The next element pointing to the last valid element of the queue
static TRACE_LIST_INDEX_T stTraceQueueCurrIndex = 0u;				//Point to the first element of the queue
static U16 luhwPrevFrameLen = 0u;

/***********************************************************************
* Name          : TraceServ_List_Init
* Description   : initial data
* Parameter     : None
* Returns       : None
***********************************************************************/
void TraceServ_List_Init(void)
{
	stTraceQueueCurrIndex = 0u;
	stTraceQueueEndIndex = 0u;
	luhwPrevFrameLen = 0u;
}

/***********************************************************************
* Name          : TraceServ_List_IsFull
* Description   : Check if the queue is full
* Parameter     : len:length of data
* Returns       : TRUE:queue full, FALSE:queue can load data continue
***********************************************************************/
static MYBOOL TraceServ_List_IsFull(U16 len)
{
	int i = len;
	TRACE_LIST_INDEX_T ltIndexMax = 0;
	TRACE_LIST_INDEX_T ltCurrIndex = 0;
	MYBOOL lbRet = FALSE;
	mcu_disable_interrupt();
	
	ltIndexMax = TRACE_LIST_QUEUE_MAX - 1;	//index is from zero start for array
	ltCurrIndex = stTraceQueueCurrIndex;
	for(i = len;i > 0;i--)
	{
		if(ltCurrIndex >= ltIndexMax)						//judge offset
		{
			ltCurrIndex = 0;
		}
		else
		{
			ltCurrIndex++;
		}
		
		if(ltCurrIndex == stTraceQueueEndIndex)			//check if the queue is full
		{
			lbRet = TRUE;
			break;
		}
		else
		{
			continue;
		}
	}
	mcu_enable_interrupt();
	return lbRet;
}

/***********************************************************************
* Name          : TraceServ_List_IsFull
* Description   : Check how many data in queue
* Parameter     : len:length of data
* Returns       : TRUE:queue full, FALSE:queue can load data continue
***********************************************************************/
static MYBOOL TraceServ_List_IsEmpty(void)
{
	MYBOOL lmbRet = FALSE;
	if(stTraceQueueCurrIndex == stTraceQueueEndIndex)
	{
		lmbRet = TRUE;
	}
	else
	{
		lmbRet = FALSE;
	}
	
	return lmbRet;
}


/***********************************************************************
* Name          : TraceServ_List_AppendData
* Description   : Add trace data
* Parameter     : data: send out data of uart, len:data length
* Returns       : TRUE: add successful ,FALSE: add fail
***********************************************************************/
MYBOOL TraceServ_List_AppendData(void *data,U16 len)
{
	MYBOOL lbCheckEmpty = FALSE;
	MYBOOL lmbRet = TRUE;
	U16 luhwTmpLen = 0;

	if(TraceServ_List_IsFull(len) == TRUE)
	{
		lmbRet = FALSE;
	}
	else
	{
		lbCheckEmpty = TraceServ_List_IsEmpty();//If it's empty,need to resend data.
		if((stTraceQueueCurrIndex + len) >= TRACE_LIST_QUEUE_MAX)
		{
			luhwTmpLen = TRACE_LIST_QUEUE_MAX - stTraceQueueCurrIndex;								//get the remaining length of the queue
			memcpy(subTraceQueue + stTraceQueueCurrIndex,data,luhwTmpLen);							//copy data to the tail of the queue 
			stTraceQueueCurrIndex = len - luhwTmpLen;												//Calculate the remaining data that needs to be copied
			memcpy(subTraceQueue,data + luhwTmpLen,stTraceQueueCurrIndex);							//Copy the remaining uncopied data,add it from the head of the queue
		}
		else
		{
			memcpy(subTraceQueue + stTraceQueueCurrIndex,data,len);									//copy data to the queue
			stTraceQueueCurrIndex += len;															//increase
		}
		
		if(lbCheckEmpty == TRUE)	//queue no data,call send function
		{
			/* send data to tx register first */
			luhwPrevFrameLen = len > TRACE_LIST_FRAME_MAX ? TRACE_LIST_FRAME_MAX : len;
			Uart_Adapt_Send(TRACE_CHANNEL,subTraceQueue + stTraceQueueEndIndex,luhwPrevFrameLen);
		}
		else
		{
			/* do nothing... */
		}
	}
	return lmbRet;
}


/***********************************************************************
* Name          : TraceServ_List_TxHandler
* Description   : The previous frame buffer is sent, ready to send the next frame of data.
* Parameter     : None
* Returns       : None
***********************************************************************/
void TraceServ_List_TxHandler(E_COMM_MODE mode)
{
	if((stTraceQueueEndIndex + luhwPrevFrameLen) >= TRACE_LIST_QUEUE_MAX)
	{
		stTraceQueueEndIndex = (stTraceQueueEndIndex + luhwPrevFrameLen) - TRACE_LIST_QUEUE_MAX;//Update the length of the last data sent
	}
	else
	{
		stTraceQueueEndIndex += luhwPrevFrameLen;//Update the length of the last data sent
	}
	
	/* Determine whether the length of this transmission exceeds the queue, and send the remaining data of the queue. */
	if(stTraceQueueEndIndex == stTraceQueueCurrIndex)
	{
		luhwPrevFrameLen = 0u;
	}
	else if(stTraceQueueEndIndex > stTraceQueueCurrIndex)
	{
		luhwPrevFrameLen = TRACE_LIST_QUEUE_MAX - stTraceQueueEndIndex;//calculation remain buffer
	}
	else
	{
		luhwPrevFrameLen = stTraceQueueCurrIndex - stTraceQueueEndIndex;
	}
	
	luhwPrevFrameLen = luhwPrevFrameLen > TRACE_LIST_FRAME_MAX ? TRACE_LIST_FRAME_MAX : luhwPrevFrameLen;
	(void)Uart_Adapt_Send(TRACE_CHANNEL,subTraceQueue+stTraceQueueEndIndex,luhwPrevFrameLen);
	UNUSED(mode);
}

/***********************************************************************
* Name          : TraceServ_List_Destroy
* Description   : destroy and reset all  data
* Parameter     : None
* Returns       : None
***********************************************************************/
void TraceServ_List_Destroy(void)
{
    TraceServ_List_Init();		//clear data
}
#endif

