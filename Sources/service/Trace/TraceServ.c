/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           *
*				  All Rights Reserved.             *
*   Department : ADS R&D SW		                   *
*   AUTHOR	   : Huang Haoxiong                    *
************************************************************************
* Object        :
* Module        : Trace Manager
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version       : 0.2
* Date          : 2018/05/24
* Author        : Huang Haoxiong
***********************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name               Changes and comments
*------------------------------------------------------------------------
*  0.1    2018/05/24  Huang Haoxiong            Init
*  0.2    2018/11/08  Huang Haoxiong        Transplant to DP2.5
************************************************************************/

#include "TraceServ.h"


#ifdef TRACE_DEBUG

static MYBOOL TraceServ_SendMessage(void *lubData,U16 len,E_TRACE_SQUENCE squence);
static void TraceServ_ResetRecvData(void);
static void TraceServ_OpenChannel(void);
static void TraceServ_CloseChannel(void);
static void TraceServ_RecvHandler(void);
static void TraceServ_ResponseEventMsg(void);
static void TraceServ_ResponseSystemMsg(void);
static void TraceServ_ResponseSWVerMsg(void);
static void TraceServ_ResponseHWVerMsg(void);
static void TraceServ_ResponsePlatformMsg(void);
static void TraceServ_ResponseResetMsg(void);
static void TraceServ_ResponseIFCMsg(void);
static void TraceServ_ResponseRegisterMsg(void);
static void TraceServ_SendEventMsg(void);
static void TraceServ_SendIOSignalMsg(void);
static void TraceServ_SendBatteryMsg(void);
static void TraceServ_SendADCMsg(void);
static void TraceServ_SendCheckSumMsg(void);
static void TraceServ_NotifyMsgHandler(void);
static void TraceServ_RxHandler(E_COMM_MODE mode,U8 index,U8 lubData);
/* receive buffer structure */
static TRACE_RECV ssTraceRecvInfo = {E_TRACE_RECV_STATE_FRAME_HEADER,0,0,{0}};

static U8 subTraceSendBuffer[TRACE_STR_FRAME_MAX_LEN] = {0};				//send buffer
static U8 subTraceSendStrBuffer[TRACE_STR_FRAME_MAX_LEN] = {0};				//about string buffer

/********************************* local function **************************************/
/***********************************************************************
* Name          : TraceServ_SendMessage
* Description   : send trace message
* Parameter     : lubData: data, len:data length, squence: squence number
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
static MYBOOL TraceServ_SendMessage(void *lubData,U16 len,E_TRACE_SQUENCE squence)
{
    MYBOOL lmbRet = FALSE;
	U8 cnt = 0u;
	U8 lubCheckSum = 0u;
	if((len + TRACE_NODATA_LEN) > TRACE_FRAME_MAX_LEN)
	{
		return lmbRet;
	}
	else
	{
	    subTraceSendBuffer[cnt++] = TRACE_FRAME_HEADER;                     	    /* set frame header */
	    subTraceSendBuffer[cnt++] = (squence >> 8) & 0xFFu;                 	    /* set squence number MSB */
	    subTraceSendBuffer[cnt++] = squence & 0xFFu;                        	    /* set squence number LSB*/
	    subTraceSendBuffer[cnt++] = len;                                        	/* set lubData length */
	    memcpy(subTraceSendBuffer + cnt,lubData,len);                            		/* copy lubData */
		cnt += len;
	    lubCheckSum = FrameChecksum(subTraceSendBuffer,cnt);						/* calculate the checksum */
		subTraceSendBuffer[cnt++] = lubCheckSum;
	    lmbRet = TraceServ_List_AppendData(subTraceSendBuffer,cnt);
	}
    return lmbRet;

}

/***********************************************************************
* Name          : TraceServ_ResetRecvData
* Description   : reset the structrue variable
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResetRecvData(void)
{
    ssTraceRecvInfo.status = E_TRACE_RECV_STATE_FRAME_HEADER;
    ssTraceRecvInfo.len = 0;
    ssTraceRecvInfo.cnt = 0;
}

/***********************************************************************
* Name          : TraceServ_OpenChannel
* Description   : Open Print Channel
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_OpenChannel(void)
{
    Uart_Adapt_Start(TRACE_CHANNEL,TRACE_RATE);         //open uart
    Uart_Adapt_SetSendCallback(TRACE_CHANNEL,TraceServ_List_TxHandler);
	Uart_Adapt_SetRecvCallback(TRACE_CHANNEL,TraceServ_RxHandler);
}

/***********************************************************************
* Name          : TraceServ_CloseChannel
* Description   : Close Print Channel
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_CloseChannel(void)
{
    Uart_Adapt_Stop(TRACE_CHANNEL);                         //close uart
}

/***********************************************************************
* Name          :TraceServ_RecvHandler
* Description   : processing lubData
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_RecvHandler(void)
{

    //get squence number
    U16 squence = (ssTraceRecvInfo.buffer[E_TRACE_RECV_STATE_SQUENCE_MSB] << 8u) + ssTraceRecvInfo.buffer[E_TRACE_RECV_STATE_SQUENCE_LSB];
    switch(squence)
    {
        case E_TRACE_SQUENCE_EVENT:							/* event type */
            TraceServ_ResponseEventMsg();

        break;
        case E_TRACE_SQUENCE_SYS:							/* see about the system information */
            TraceServ_ResponseSystemMsg();

        break;
        case E_TRACE_SQUENCE_RESET_REASON:					/* cause of the reset */
            TraceServ_ResponseResetMsg();

        break;
        case E_TRACE_SQUENCE_HISTORY_IFC:			/* see about the diagnosetic history */
            TraceServ_ResponseIFCMsg();

        break;
        case E_TRACE_SQUENCE_REG:							/* see about the value of register */
            TraceServ_ResponseRegisterMsg();

        break;
        case E_TRACE_SQUENCE_NOTIFY:						/* upper computer connect */
            TraceServ_NotifyMsgHandler();

        break;
        default:

        break;
    }
}

/***********************************************************************
* Name          :TraceServ_ResponseEventMsg
* Description   : response event type
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseEventMsg(void)
{

    U8 lubData[2] = {0u};
    U16 i = 0u;
    MYBOOL state = TRUE;
	MYBOOL lbPrevSignalEventState = gmbTraceEventSwitch[E_TRACE_EVENT_SIGNAL];		//set current state
    //change all event state
    if(ssTraceRecvInfo.buffer[4] == E_TRACE_EVENT_ONOFF)
    {
        if(ssTraceRecvInfo.buffer[5] == 0u)
        {
            state = FALSE;
        }
        for(i = ((U16)E_TRACE_EVENT_TRACE + 1u);i < (U16)E_TRACE_EVENT_MAX;i++)
        {
            gmbTraceEventSwitch[i] = state;
        }
    }
    else
    {
        /* change event state */
        if(ssTraceRecvInfo.buffer[5] == 0u)
        {
            gmbTraceEventSwitch[ssTraceRecvInfo.buffer[4]] = FALSE;
        }
        else
        {
            gmbTraceEventSwitch[ssTraceRecvInfo.buffer[4]] = TRUE;
        }
    }
    /* check if the status has changed and reset lubData */
    if((gmbTraceEventSwitch[E_TRACE_EVENT_SIGNAL] == TRUE) && (lbPrevSignalEventState == FALSE))
    {
        gtPrevBatValue = 0u;										//clear battery information
        memset(gtTracePrevIOList,0u,sizeof(gtTracePrevIOList));		//clear the signal information
        memset(gtPrevADCValue,0u,sizeof(gtPrevADCValue));        	//clear adc value
        memset(gtPrevPWMDutyValue,0u,sizeof(gtPrevPWMDutyValue));   //clear pwm value
    }
	else
	{
		/* do nothing... */
	}

    lubData[0] = ssTraceRecvInfo.buffer[4];	    //set type of event
    lubData[1] = ssTraceRecvInfo.buffer[5];	    //set event state
    TraceServ_SendMessage(lubData,sizeof(lubData),E_TRACE_SQUENCE_EVENT);

}

/***********************************************************************
* Name          :TraceServ_ResponseSystemMsg
* Description   : response system information
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseSystemMsg(void)
{
	
    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
		/* do nothing... */
    }
	else
	{
	    TraceServ_ResponseResetMsg();    		//send reset the reason

	    TraceServ_SendEventMsg();			    //send event status

	    TraceServ_SendCheckSumMsg();			//send checksum message

	    TraceServ_ResponseSWVerMsg();    		//send software information message

	    TraceServ_ResponseHWVerMsg();    		//send hardware information message

	    TraceServ_ResponsePlatformMsg();    	//send platform information

		SystemMgr_UpdateState();				//update system state
		
	    DevCtrl_UpdateAllDevsState();			//update device state
	}

}

/***********************************************************************
* Name          : TraceServ_ResponseSoftwareMsg
* Description   : response software version information
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseSWVerMsg(void)
{

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
        /* do nothing... */
    }
	else
	{
		TraceServ_SendMessage((U8 *)gccSysVersion,strlen(gccSysVersion) + 1u,E_TRACE_SQUENCE_SYS);
	}
}

/***********************************************************************
* Name          : TraceServ_ResponseHWVerMsg
* Description   : response hardware version information
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseHWVerMsg(void)
{

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
        /* do nothing... */
    }
	else
	{
		TraceServ_SendMessage((U8 *)gcHWVersion,strlen(gcHWVersion) + 1u,E_TRACE_SQUENCE_HW_VERSION);
	}
}


/***********************************************************************
* Name          :TraceServ_ResponsePlatformMsg
* Description   : response platform information
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponsePlatformMsg(void)
{

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
        /* do nothing... */
    }
	else
	{
		TraceServ_SendMessage((U8 *)gccPlatformVersion,strlen(gccPlatformVersion) + 1u,E_TRACE_SQUENCE_PLATFORM);
	}
}


/***********************************************************************
* Name          :TraceServ_ResponseResetMsg
* Description   : response cause of the reset message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseResetMsg(void)
{
    U8 lubResetValue = 0u;
    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
		/* do nothing... */
    }
	else
	{
	    lubResetValue = mcu_get_reset_flag();  //get reset the reason
	    TraceServ_SendMessage(&lubResetValue,1u,E_TRACE_SQUENCE_RESET_REASON);
	}
	
}

/***********************************************************************
* Name          :TraceServ_ResponseIFCMsg
* Description   : response IFC message
* Parameter     : uart: uart channel
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseIFCMsg(void)
{

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close,return
    {
		/* do nothing... */
    }
	else
	{
    	gmbSendIfcMsgFlag = TRUE;
	}
}


/***********************************************************************
* Name          :TraceServ_ResponseRegisterMsg
* Description   : response the value of register
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_ResponseRegisterMsg(void)
{
#ifndef TRACE_USE_ABRIDGED_FEATURE	
    U8 i = 0u;
    U32 reg = 0u;
    volatile U32 value = 0u;
    U8 lubData[4] = {0u};
    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //trace close,return
    {
		/* do nothing... */
	}
	else
	{
	    for(i = 0u;i < 4u; i++)   /* get register address */
	    {
	        reg += ssTraceRecvInfo.buffer[4 + i] << (24u - (i * 8u));

	    }
	    value = *((volatile U32 *)reg); /* get the value from the register address */

	    for(i = 0u;i < 4u;i++)   /* set the register value */
	    {
	        lubData[i] = value >> (24u - (i * 8u));
	    }
	    TraceServ_SendMessage(lubData,sizeof(lubData),E_TRACE_SQUENCE_REG); /* send lubData of register to uart */
	}
#endif
}


/***********************************************************************
* Name          :TraceServ_SendEventMsg
* Description   : send all of event state message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_SendEventMsg(void)
{
    U8 lubData[2] = {0u};
    U16 i = 0u;
    for(i = 0;i < E_TRACE_EVENT_MAX;i++)
    {
        //event index
        lubData[0] = i;
        //event state
        if(gmbTraceEventSwitch[i] == FALSE)
        {
            lubData[1] = 0u;
        }
        else
        {
            lubData[1] = 1u;
        }
        //send type of event message to uart
        TraceServ_SendMessage(lubData,sizeof(lubData),E_TRACE_SQUENCE_EVENT);

    }
}

/***********************************************************************
* Name          :TraceServ_SendIOSignalMsg
* Description   : send io signal message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_SendIOSignalMsg(void)
{
#ifdef TRACE_SIGNAL
    U16 i = 0u;
    U8 lubData[2] = {0u};
    DEV_IO_8BIT_T ltIOSignal = LOW_LEVEL;

    for(i = 0u; i < E_IO_IN_NUM;i++)
    {
		ltIOSignal = DEV_IO_SYNC_READ(i);
        if(gtTracePrevIOList[i] != ltIOSignal)                   		/* different  status */
        {
            gtTracePrevIOList[i] = ltIOSignal;                   		/* update signal status */
            lubData[0] = i;                                            /* signal index */
            lubData[1] = gtTracePrevIOList[i];                       	/* set signal status  */
            TraceServ_SendMessage(lubData,2u,E_TRACE_SQUENCE_SIGNAL); 	/* send type of signal message to uart */
        }
		else
		{
			/* do nothing... */
		}
    }
#endif  //TRACE_SIGNAL
}

/***********************************************************************
* Name          :TraceServ_SendBatteryMsg
* Description   : send adc value of system battery voltage
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_SendBatteryMsg(void)
{
    U8 lubData[4] = {0u};
    DEV_ADC_16BIT_T ltBatADvalue = 0u;
    //received battery value from ADC register
    ltBatADvalue = DEV_ADC_ASYNC_READ(ADC_BAT_DET);
    //judge deviation
    if(abs(gtPrevBatValue - ltBatADvalue) > TRACE_ADC_FILTER)
    {
        gtPrevBatValue = ltBatADvalue;
        lubData[0] = TRACE_ADC_REFER_VOLT;                 	//reference voltage
        lubData[1] = TRACE_ADC_ACCURACY;                   	//accuracy
        lubData[2] = (ltBatADvalue >> 8) & 0xFFu;         	//MSB
        lubData[3] = ltBatADvalue & 0xFFu;                	//LSB
        /* send type of battery message to uart */
        TraceServ_SendMessage(lubData,sizeof(lubData),E_TRACE_SQUENCE_BAT);
    }
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
* Name          :TraceServ_SendADCMsg
* Description   : send adc value
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_SendADCMsg(void)
{
#ifdef TRACE_ADC_FUNC
    DEV_ADC_16BIT_T ltADCValue = 0u;
    U16 i = 0u;
    U8 lubData[5] = {0u};
    //received adc value from ADC register

    for(i = 0u;i < E_DEV_ADC_NUM;i++)
    {
		ltADCValue = DEV_ADC_ASYNC_READ(i);
        //judge deviation
        if(abs(gtPrevADCValue[i] - ltADCValue) > TRACE_ADC_FILTER)
        {

            gtPrevADCValue[i] = ltADCValue;
            lubData[0] = i;                                    	//index of adc list
            lubData[1] = TRACE_ADC_REFER_VOLT;                 	//reference voltage
            lubData[2] = TRACE_ADC_ACCURACY;                   	//accuracy
            lubData[3] = (ltADCValue >> 8) & 0xFFu;         	//MSB
            lubData[4] = ltADCValue & 0xFFu;                	//LSB
            TraceServ_SendMessage(lubData,sizeof(lubData),E_TRACE_SQUENCE_ADC);
        }
		else
		{
			/* do nothing... */
		}
    }
#endif
}

/***********************************************************************
* Name          :TraceServ_SendEventMsg
* Description   : send checksum message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_SendCheckSumMsg(void)
{

    TraceServ_SendMessage(gaubSysCheckSum,2u, E_TRACE_SQUENCE_CHECKSUM);

}

/***********************************************************************
* Name          :TraceServ_NotifyMsgHandler
* Description   : upper computer connect message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_NotifyMsgHandler(void)
{

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)   //Trace close
    {
		/* do nothing... */
    }
	else
	{
	    TraceServ_ResponseSystemMsg();                      					//send systemInfo

	    gtPrevBatValue = 0u;                                				//reset the battery value
	    memset(gtTracePrevIOList,0u,sizeof(gtTracePrevIOList));				//clear the signal information
	    memset(gtPrevPWMDutyValue,0u,sizeof(gtPrevPWMDutyValue));
		memset(gtPrevADCValue,0u,sizeof(gtPrevADCValue));
	}
}

/***********************************************************************
* Name          :TraceServ_NotifyMsgHandler
* Description   : upper computer connect message
* Parameter     : None
* Returns       : None
***********************************************************************/
static void TraceServ_StartupHandler(void)
{

    TraceServ_SendStrMsg("System startup!", E_TRACE_EVENT_TRACE);
	TraceServ_NotifyMsgHandler();			//notify info to trace tool
}

#endif

/***********************************************************************
* Name          : TraceServ_Init
* Description   : initialization trace
* Parameter     : None
* Returns       : None
***********************************************************************/
void TraceServ_Init(void)
{

#ifdef TRACE_DEBUG
    TraceServ_OpenChannel();                                 //open print channel
    TraceServ_List_Init();                                   //init list
	TraceServ_StartupHandler();								//send startup message

#endif

}

/***********************************************************************
* Name          : TraceServ_CycleTask10ms
* Description   : Trace Task Entry
* Parameter     : None
* Returns       : None
***********************************************************************/
void TraceServ_CycleTask10ms(void)
{
#ifdef TRACE_DEBUG
    if(ssTraceRecvInfo.status == E_TRACE_RECV_STATE_FINISH)
    {
        if(ssTraceRecvInfo.len > 0)
        {
            TraceServ_RecvHandler(); /* processing lubData */
        }
		else
		{
			/* do nothing */
		}
		ssTraceRecvInfo.status = E_TRACE_RECV_STATE_FRAME_HEADER;
    }
	else
	{
		/* do nothing... */
	}

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == TRUE)
    {
        if(gmbTraceEventSwitch[E_TRACE_EVENT_SIGNAL] == TRUE)
        {
            TraceServ_SendIOSignalMsg();    //signal message
            TraceServ_SendBatteryMsg();     //battery information
            TraceServ_SendADCMsg();         //adc information
        }
		else
		{
			/* do nothing... */
		}
#ifdef TRACE_IFC_FUNC
        if(gmbSendIfcMsgFlag == TRUE)
        {
            int i = 0;
            U8 lubData[DIAGSERV_IFC_DATA_LEN] = {0};
            for(i = 0;i < DiagServ_GetIfcQuantity(); i++)
            {
                if(DiagServ_ReadIfcInfo(i, lubData) == TRUE)
                {
                	TraceServ_SendMessage(lubData,DIAGSERV_IFC_DATA_LEN, E_TRACE_SQUENCE_HISTORY_IFC);
                }
				else
				{
					/* do nothing... */
				}
            }
			
            gmbSendIfcMsgFlag = FALSE;
        }
#endif  //TRACE_IFC_FUNC
    }
	else
	{
		/* do nothing... */
	}
#endif  //TRACE_DEBUG
}

/***********************************************************************
* Name          : TraceServ_RxHandler
* Description   : receive lubData from uart rx handler
* Parameter     : lubData: received from uart
* Returns       : None
***********************************************************************/
void TraceServ_RxHandler(E_COMM_MODE mode,U8 index,U8 lubData)
{
#ifdef TRACE_DEBUG
    U8 lubCheckSum = 0u;
    switch(ssTraceRecvInfo.status)
    {

        case E_TRACE_RECV_STATE_FRAME_HEADER:
            if(lubData == TRACE_FRAME_HEADER)  /* frame header */
            {
				ssTraceRecvInfo.cnt = 0;
				ssTraceRecvInfo.len = 0;
                ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
                ssTraceRecvInfo.status = E_TRACE_RECV_STATE_SQUENCE_MSB;
            }
			else
			{
				/* do nothing... */
			}
            break;
        case E_TRACE_RECV_STATE_SQUENCE_MSB:            /* squence number MSB */
            ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
            ssTraceRecvInfo.status = E_TRACE_RECV_STATE_SQUENCE_LSB;
            break;
        case E_TRACE_RECV_STATE_SQUENCE_LSB:			/* squence number LSB */

            ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
            ssTraceRecvInfo.status = E_TRACE_RECV_STATE_DATA_LENGTH;

            break;
        case E_TRACE_RECV_STATE_DATA_LENGTH:            /* lubData length */
            ssTraceRecvInfo.len = lubData;
            if((ssTraceRecvInfo.len < (TRACE_RECV_LENGTH - TRACE_NODATA_LEN)) && (ssTraceRecvInfo.len > 0))
            {
                ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
                ssTraceRecvInfo.status = E_TRACE_RECV_STATE_DATA_SECTION;
            }
            else
            {
                TraceServ_ResetRecvData();               /* length invalid ,reset structure */
            }
            break;
        case E_TRACE_RECV_STATE_DATA_SECTION:           /* lubData section */

            ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
            if((ssTraceRecvInfo.cnt) >= (ssTraceRecvInfo.len + (TRACE_NODATA_LEN - 1)))
            {
                ssTraceRecvInfo.status = E_TRACE_RECV_STATE_CHECKSUM;
            }
            break;
        case E_TRACE_RECV_STATE_CHECKSUM:               /* checksum */
            ssTraceRecvInfo.buffer[ssTraceRecvInfo.cnt++] = lubData;
            lubCheckSum = FrameChecksum(ssTraceRecvInfo.buffer,(ssTraceRecvInfo.len + (TRACE_NODATA_LEN - 1))); /*calculate the checksum*/
            /* lubData receive finish */
            if(lubData == lubCheckSum)
            {
                ssTraceRecvInfo.status = E_TRACE_RECV_STATE_FINISH;
            }
			else
			{
				TraceServ_ResetRecvData();	 /* reset the structure */
			}

            break;
        default:
            break; 
	}
#else
	/*avoid static code scanning error reporting*/
	UNUSED(index);
    UNUSED(lubData);		
#endif
	UNUSED(mode);
}

/***********************************************************************
* Name          : TraceServ_SendStrMsg
* Description   : send type of string message
* Parameter     : str: string, event:event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrMsg(const char *str,E_TRACE_EVENT event)
{
#ifdef TRACE_DEBUG
    U16 len = strlen(str);
    MYBOOL lmbRet = FALSE;

    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || (gmbTraceEventSwitch[event] == FALSE) || (len < 1u))
    {
		/* do nothing... */
    }
	else
	{
		memcpy(subTraceSendStrBuffer,str,len + 1);
    	lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,len + 1u,E_TRACE_SQUENCE_STRING); /* send string type message to uart */
	}
    return lmbRet;
#else
    UNUSED(str);		//avoid static code scanning error reporting
    UNUSED(event);
    return FALSE;
#endif
}

/***********************************************************************
* Name          : TraceServ_SendDStrMsg
* Description   : send double string message
* Parameter     : str1: first string ,str2:second string, event:event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendDStrMsg(const char *str1,const char *str2,E_TRACE_EVENT event)
{
#ifdef TRACE_DEBUG
	U16 len = 0u;
    U16 len1 = strlen(str1);
    U16 len2 = strlen(str2);
	MYBOOL lmbRet = FALSE;
	
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || (gmbTraceEventSwitch[event] == FALSE)
		|| (len1 < 1u) || (len2 < 1u) || ((len1 + len2 + 3u) > (TRACE_STR_FRAME_MAX_LEN)))
    {
		/* do nothing... */
    }
	else
	{
		len = len1;
		memcpy(subTraceSendStrBuffer,str1,len);
		subTraceSendStrBuffer[len++] = ':';
		memcpy(subTraceSendStrBuffer + len,str2,len2);
		len += len2;
		subTraceSendStrBuffer[len++] = '\0';

    	lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,len,E_TRACE_SQUENCE_STRING); /* send string type message to uart */
	}
    return lmbRet;
#else
    UNUSED(str1);		//avoid static code scanning error reporting
    UNUSED(str2);
    UNUSED(event);
    return FALSE;
#endif
}


/***********************************************************************
* Name          : TraceServ_SendStrWithHexMsg
* Description   : send type of hex message
* Parameter     : lubData: lubData,num: array,numLen: array length,event: event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrWithHexMsg(const char *label,U8* num,U16 numLen,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
    U16 i = 0u;
    U16 cnt = 0u;
	MYBOOL lmbRet = FALSE;
    U8 len = strlen(label);		//get string length
    
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || ((len + numLen + 1) >= (TRACE_STR_FRAME_MAX_LEN))
		|| (len < 1u) || (numLen < 1u) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		len++; 														//include '\0'
		subTraceSendStrBuffer[cnt++] = len;                  		//string length
	    memcpy(subTraceSendStrBuffer + cnt,label,len);				//copy string
	    cnt += len;
	    subTraceSendStrBuffer[cnt++] = E_TRACE_HEX_TYPE;			//type of array
	    for(i = 0; i < numLen;i++)
	    {
	        subTraceSendStrBuffer[cnt + i] = num[i] & 0xFFu;
	    }
	    cnt += numLen;												//add length
	    lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,cnt,E_TRACE_SQUENCE_LABEL); /* send message to uart */
	}
    return lmbRet;
#else
    UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif

}


/***********************************************************************
* Name          : TraceServ_SendStrWithUShortMsg
* Description   : send type of unsigned short message
* Parameter     : lubData: lubData,num: array,numLen: array length,event: event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrWithUShortMsg(const char *label,U16* num,U16 numLen,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
    U16 i = 0u;
    U16 cnt = 0u;
    MYBOOL lmbRet = FALSE;
    U8 len = strlen(label);		//get string length
    
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || ((len + (numLen << 1) + 1) >= (TRACE_STR_FRAME_MAX_LEN))
		|| (len < 1u) || (numLen < 1u) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		len++; 																//include '\0'
	    subTraceSendStrBuffer[cnt++] = len;									//string length
	    memcpy(subTraceSendStrBuffer + cnt,label,len);      				//copy string
	    cnt += len;
	    subTraceSendStrBuffer[cnt++] = E_TRACE_USHORT_TYPE;					//type of array
	    for(i = 0; i < numLen;i++)
	    {
	        subTraceSendStrBuffer[cnt + (i << 1)] = (num[i] >> 8u) & 0xFFu;
	        subTraceSendStrBuffer[cnt + ((i << 1)+ 1u)] = num[i] & 0xFFu;
	    }
	    cnt = cnt + (numLen << 1);											//add length
	    lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,cnt,E_TRACE_SQUENCE_LABEL);					//send message to uart
	}
    return lmbRet;
#else
    UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif

}
/***********************************************************************
* Name          : TraceServ_SendStrWithShortMsg
* Description   : send type of short message
* Parameter     : lubData: lubData,num: array,numLen: array length,event: event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrWithShortMsg(const char *label,short* num,U16 numLen,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
    U16 i = 0u;
    U16 cnt = 0u;
    MYBOOL lmbRet = FALSE;
    U8 len = strlen(label);		//get string length

    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || ((len + (numLen << 1) + 1) >= (TRACE_STR_FRAME_MAX_LEN))
		|| (len < 1u) || (numLen < 1u) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		len++;																	//include '\0'
	    subTraceSendStrBuffer[cnt++] = len;                  					//string length
	    memcpy(subTraceSendStrBuffer + cnt,label,len); 							//copy string
	    cnt += len;
	    subTraceSendStrBuffer[cnt++] = E_TRACE_SHORT_TYPE;   					//type of array
	    for(i = 0; i < numLen;i++)
	    {
	        subTraceSendStrBuffer[cnt + (i << 1)] = (num[i] >> 8u) & 0xFFu;
	        subTraceSendStrBuffer[cnt + ((i << 1)+ 1u)] = num[i] & 0xFFu;
	    }
	    cnt = cnt + (numLen << 1);												//add length
	    lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,cnt,E_TRACE_SQUENCE_LABEL); //send message to uart
	}
    return lmbRet;
#else
    UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif

}


/***********************************************************************
* Name          : TraceServ_SendStrWithIntMsg
* Description   : send type of integer message
* Parameter     : lubData: lubData,num: array,numLen: array length,event: event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrWithIntMsg(const char *label,int* num,U16 numLen,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
    U16 i = 0u;
    U16 cnt = 0u;
    MYBOOL lmbRet = FALSE;
    U8 len = strlen(label);		//get string length
    
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || ((len + (numLen << 2) + 1) >= (TRACE_STR_FRAME_MAX_LEN))
		|| (len < 1u) || (numLen < 1u) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		len++;																		//include '\0'
	    subTraceSendStrBuffer[cnt++] = len;              							//string length
	    memcpy(subTraceSendStrBuffer + cnt,label,len);    							//copy string
	    cnt += len;
	    subTraceSendStrBuffer[cnt++] = E_TRACE_INT_TYPE; 							//type of array
	    for(i = 0; i < numLen;i++)
	    {
	        subTraceSendStrBuffer[cnt + (i << 2)] = (num[i] >> 24u) & 0xFFu;
	        subTraceSendStrBuffer[cnt + ((i << 2) + 1u)] = (num[i] >> 16u) & 0xFFu;
	        subTraceSendStrBuffer[cnt + ((i << 2) + 2u)] = (num[i] >> 8u) & 0xFFu;
	        subTraceSendStrBuffer[cnt + ((i << 2) + 3u)] = num[i] & 0xFFu;
	    }
	    cnt = cnt + (numLen << 2);						//add length
	    lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,cnt,E_TRACE_SQUENCE_LABEL); /* send message to uart */
	}
    return lmbRet;
#else
    UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif
}

/***********************************************************************
* Name          : TraceServ_SendStrWithFloatMsg
* Description   : send type of float message
* Parameter     : lubData: lubData,num: array,numLen: array length,event: event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendStrWithFloatMsg(const char *label,float* num,U16 numLen,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
#ifndef TRACE_USE_ABRIDGED_FEATURE
    U16 i = 0u;
    U16 cnt = 0u;
	char lcTmpCBuffer[16] = {0};
	MYBOOL lmbRet = FALSE;
    U8 len = strlen(label);		//get string length
	U8 lubFStrLen = 0u;
	
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || ((len + 1) >= (TRACE_STR_FRAME_MAX_LEN))
		|| (len < 1u) || (numLen < 1u) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		len++;																//include '\0'
		subTraceSendStrBuffer[cnt++] = len;                          		//string length
	    memcpy(subTraceSendStrBuffer + cnt,label,len);              		//copy string
	    cnt += len;
	    subTraceSendStrBuffer[cnt++] = E_TRACE_FLOAT_TYPE;           		//type of array
	    for(i = 0u; i < numLen;i++)
	    {
			sprintf(lcTmpCBuffer,"%.3f ",num[i]);       					//float turn to string
			lubFStrLen = strlen(lcTmpCBuffer);								//exclude '\0'
	        if((cnt + lubFStrLen) > (TRACE_STR_FRAME_MAX_LEN))				//check length as add to float string
	        {
				break;
	        }
			else
			{
				memcpy(subTraceSendStrBuffer + cnt,lcTmpCBuffer,lubFStrLen);//copy string
				cnt += lubFStrLen;										 	//calculate total length,exclude '\0'
				memset(lcTmpCBuffer,0,lubFStrLen);							//clear string
			}
	    }
	    subTraceSendStrBuffer[cnt] = '\0';       							//add '\0'
	    lmbRet = TraceServ_SendMessage(subTraceSendStrBuffer,cnt,E_TRACE_SQUENCE_LABEL); /* send message to uart */
	}
    return lmbRet;
#else
	UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif
#else
    UNUSED(label);		//avoid static code scanning error reporting
    UNUSED(num);
    UNUSED(numLen);
    UNUSED(event);
    return FALSE;
#endif
}

/***********************************************************************
* Name          : TraceServ_SendRegMsg
* Description   : send register infotmation message
* Parameter     : str: string,addr:register addr, event:event type
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendRegMsg(const char *str,U32 addr,E_TRACE_EVENT event)
{

#ifdef TRACE_DEBUG
#ifndef TRACE_USE_ABRIDGED_FEATURE
    MYBOOL lmbRet = FALSE;
    volatile U32 value;
    U8 lubData[4] = {0};
    U8 i = 0;
	
    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || (gmbTraceEventSwitch[event] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		value = *((volatile U32 *)(addr));
	    for(i = 0;i < 4u;i++)   /* set the register value */
	    {
	        lubData[i] = value >> (24u - i * 8u);
	    }
	    lmbRet = TraceServ_SendStrWithHexMsg(str,lubData,4u, event);
	}
    return lmbRet;
#else	//TRACE_USE_ABRIDGED_FEATURE
	UNUSED(str);		//avoid static code scanning error reporting
    UNUSED(addr);
    UNUSED(event);
    return FALSE;
#endif
#else
    UNUSED(str);		//avoid static code scanning error reporting
    UNUSED(addr);
    UNUSED(event);
    return FALSE;
#endif
}

/***********************************************************************
* Name          : TraceServ_SendPwmMsg
* Description   : Send pwm value message
* Parameter     : ch:pwm channel,duty:pwm duty,period:pwm period
* Returns       : TRUE: successful,FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendPwmMsg(E_TRACE_PWM_CH ch,U32 duty,U32 period)
{
#ifdef TRACE_DEBUG
	MYBOOL lmbRet = FALSE;
	U8 i = 0U,j = 0U;
	U8 lubData[12] = {0u};
	if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || (gmbTraceEventSwitch[E_TRACE_EVENT_SIGNAL] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		if(duty != gtPrevPWMDutyValue[ch])
		{
		    if(ch < E_TRACE_PWM_MAX)
		    {
				gtPrevPWMDutyValue[ch] = duty;//update value
				lubData[i++] = ch;//set channel
		        for(j = 0;j < 4;j++)//set duty
		        {
		            lubData[i + j] = (duty >> (24u - (j * 8u)));
		        }
				i += j;
		        for(j = 0;j < 4;j++)//set period
		        {
		            lubData[i + j] = (period >> (24u - (j * 8u)));
		        }
		        i += j;
		        lmbRet = TraceServ_SendMessage(lubData,i,E_TRACE_SQUENCE_PWM); //send type of pwm message to uart
		    }
			else
			{
				lmbRet = FALSE;
			}
		}
		else
		{
			lmbRet = TRUE;
		}
	}
	return lmbRet;
#else
	UNUSED(ch);
	UNUSED(duty);
	UNUSED(period);
	return FALSE;
#endif
}

/***********************************************************************
* Name          : TraceServ_SendTPMsg
* Description   : send tp information
* Parameter     : lubData: touch point information
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendTPMsg(U8* lubData)
{

#ifdef TRACE_DEBUG
#ifndef TRACE_USE_ABRIDGED_FEATURE
    MYBOOL lmbRet = FALSE;

    if((gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE) || (gmbTraceEventSwitch[E_TRACE_EVENT_TOUCH] == FALSE))
    {
		/* do nothing... */
    }
	else
	{
		lmbRet = TraceServ_SendMessage(lubData,6u,E_TRACE_SQUENCE_TP_TOUCH);         /* send tp message to uart */
	}
    return lmbRet;
#else	//TRACE_USE_ABRIDGED_FEATURE
	UNUSED(lubData);		//avoid static code scanning error reporting
    return FALSE;
#endif	
#else
    UNUSED(lubData);		//avoid static code scanning error reporting
    return FALSE;
#endif

}

/***********************************************************************
* Name          : TraceServ_SendSysStateMsg
* Description   : send system state
* Parameter     : state:system state
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendSysStateMsg(E_SYS_STATE sysState,E_WORKING_STATE workingState)
{

#ifdef TRACE_DEBUG
	U8 lubData[2] = {0};
    MYBOOL lmbRet = FALSE;

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)
    {
		/* do nothing... */
    }
	else
	{
		lubData[0] = (U8)sysState;
		lubData[1] = (U8)workingState;
		lmbRet = TraceServ_SendMessage(lubData,2u,E_TRACE_SQUENCE_SYS_STATE);         /* send message to uart */
	}
    return lmbRet;
#else
    UNUSED(sysState);			//avoid static code scanning error reporting
    UNUSED(workingState);		//avoid static code scanning error reporting
    //printf("system state:%d->%d\n",sysState,workingState);
    return FALSE;
#endif

}


/***********************************************************************
* Name          : TraceServ_SendDevStateMsg
* Description   : send device state
* Parameter     : id:device id,state:device current status
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendDevStateMsg(DEV_ID_T id,E_DEVCTRL_STATE state)
{

#ifdef TRACE_DEBUG
	U8 lubData[2] = {0};
    MYBOOL lmbRet = FALSE;

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)
    {
		/* do nothing... */
    }
	else
	{
		lubData[0] = (U8)id;
		lubData[1] = (U8)state;
		lmbRet = TraceServ_SendMessage(lubData,2u,E_TRACE_SQUENCE_DEV_STATE);         /* send message to uart */
	}
    return lmbRet;
#else
    UNUSED(id);			//avoid static code scanning error reporting
    UNUSED(state);		//avoid static code scanning error reporting
    //printf("device state:%d->%d\n",id,state);
    return FALSE;
#endif

}

/***********************************************************************
* Name          : TraceServ_SendDevPMMsg
* Description   : send power status of device controller
* Parameter     : currState:current status,nextState:next change status
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
MYBOOL TraceServ_SendDevPMMsg(E_DEVCTRL_SM_STATE currState,E_DEVCTRL_SM_STATE nextState)
{

#ifdef TRACE_DEBUG
	U8 lubData[2] = {0};
    MYBOOL lmbRet = FALSE;

    if(gmbTraceEventSwitch[E_TRACE_EVENT_TRACE] == FALSE)
    {
		/* do nothing... */
    }
	else
	{
		lubData[0] = (U8)currState;
		lubData[1] = (U8)nextState;
		lmbRet = TraceServ_SendMessage(lubData,2u,E_TRACE_SQUENCE_DEV_PM_STATE);         /* send tp message to uart */
	}
    return lmbRet;
#else
    UNUSED(currState);		//avoid static code scanning error reporting
    UNUSED(nextState);		//avoid static code scanning error reporting
    return FALSE;
#endif

}

/***********************************************************************
* Name          : TraceServ_Destroy
* Description   : List interface
* Parameter     : void
* Returns       : TRUE: successful, FALSE: failed
***********************************************************************/
void TraceServ_Destroy(void)
{

#ifdef TRACE_DEBUG
    TraceServ_CloseChannel();
    TraceServ_List_Destroy();
#endif

}

