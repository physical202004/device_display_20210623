/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
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
* ----------------------------------------------------------------------
* 0.1    Feb 10,2013    NI Rujin    Initial version
* ----------------------------------------------------------------------
* 0.2    Mar 11,2013    LIU Chenxi  Correct function signature
*----------------------------------------------------------------------
* 0.3    Jan 5,2017     ShangQiuju    change CS_SIG_AsyncReceive /CS_SIG_SyncReceive
                                                    parameter plocalSignalCopy from u16* to U8*
*----------------------------------------------------------------------
* 0.4	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*=====================================================================*/

#ifndef SIGNAL_IF_H
#define SIGNAL_IF_H
#include "Signal.h"
#include "Sig_Combine.h"

/**** Definition of macros ****/
/* Macro definitions*/
extern void CS_SIG_RegisterSignal(void);
extern void CS_SIG_SyncSend(SIGNAL_NAME_T SigName,const void* value);
extern void CS_SIG_SyncReceive(SIGNAL_NAME_T SigName,const void* value);

/**** Declaration of constants ****/
/**** Declaration of variables ****/

/**** Declaration of functions ****/



#endif /*_SIGNAL_IF_H_*/

/****************************** END OF FILE ***************************/

