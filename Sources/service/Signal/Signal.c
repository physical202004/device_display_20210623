/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*-----------------------------------------------------------------------
* 0.1    Aug 11,2016   LIU Chenxi    Initial version
*----------------------------------------------------------------------
* 0.2    Jan 4,2017     ShangQiuju    change CS_SIG_AsyncReceive /CS_SIG_SyncReceive
                                                    parameter plocalSignalCopy from u16* to U8*
*------------------------------------------------------------------------
* 0.3    Jan 24,2017  ZhangJie  Fix Coverity warnings.
*------------------------------------------------------------------------
* 0.4	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*=====================================================================*/
/* Foreign headerfiles */
/* Own headerfiles */
#include "Signal_if.h"


Signal_Info AllSignal[E_SIG_SIGNALS_NUM] = {{0}};

extern V_syncreceive SignalRevProc[E_SIG_TYPE_NUM];
extern V_syncsend  SignalSndProc[E_SIG_TYPE_NUM];


/***********************************************************************
* Name        	: CS_SIG_SyncSend
* Description	 : SigName, plocalSignalCopy
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void CS_SIG_SyncSend(SIGNAL_NAME_T SigName,const void* value)
{
    Sig_SignalType sigtype = E_SIG_TYPE_NUM;
    if(((SIGNAL_NAME_T)E_SIG_SIGNALS_NUM > 0u) && (SigName < (SIGNAL_NAME_T)E_SIG_SIGNALS_NUM))
    {
        sigtype = AllSignal[SigName].type;
        SigName = AllSignal[SigName].sourename;
        if((SignalSndProc[sigtype] != NULL) && (sigtype < E_SIG_TYPE_NUM))
        {
            SignalSndProc[sigtype](SigName,value);
        }
		else
	    {
	        /*nothing to do*/
	    }
    }
    else
    {
        /*nothing to do*/
    }
}


/***********************************************************************
* Name        	: CS_SIG_SyncReceive
* Description	:
* Parameter   	: SigName
* Returns     	: SIGNAL_NAME_T
***********************************************************************/
void CS_SIG_SyncReceive(SIGNAL_NAME_T SigName,const void* value)
{

    Sig_SignalType sigtype = E_SIG_TYPE_NUM;
    if(((SIGNAL_NAME_T)E_SIG_SIGNALS_NUM > 0u) && (SigName < (SIGNAL_NAME_T)E_SIG_SIGNALS_NUM))
    {
        sigtype = AllSignal[SigName].type;
        SigName = AllSignal[SigName].sourename;
        if((SignalRevProc[sigtype] != NULL) && (sigtype < E_SIG_TYPE_NUM))
        {
            SignalRevProc[sigtype](SigName,value);
        }
		else
	    {
	        /*nothing to do*/
	    }
    }
    else
    {
        /*nothing to do*/
    }
}

/**************** END OF FILE *****************************************/

