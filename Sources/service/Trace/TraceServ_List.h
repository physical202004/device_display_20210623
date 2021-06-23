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
************************************************************************/
#include "TraceServ_if.h"

#ifdef TRACE_DEBUG
#ifndef TRACESERV_LIST_H
#define TRACESERV_LIST_H

#include <string.h>
#include <stdlib.h>


#define TRACE_LIST_QUEUE_MAX 				512u			//queue buffer
#define TRACE_LIST_FRAME_MAX 				255u			//frame length

typedef U16 TRACE_LIST_INDEX_T;



extern void TraceServ_List_Init(void);
extern MYBOOL TraceServ_List_AppendData(void *data, U16 len);
extern void TraceServ_List_TxHandler(E_COMM_MODE mode);
extern void TraceServ_List_Destroy(void);

#endif      //_TRACESERV_LIST_H_

#endif      //TRACE_DEBUG

