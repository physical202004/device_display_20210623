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
* Version       Date      Name          Changes and comments
*-----------------------------------------------------------------------
* 0.1    Aug 11,2016                       Initial version
*----------------------------------------------------------------------
* 0.2    Jan 5,2017     ShangQiuju    change receive function parameter from U16* to U8*
*----------------------------------------------------------------------
* 0.3	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*=====================================================================*/

#ifndef SIGNAL_H
#define SIGNAL_H

#define  SIGNAL_PASS_1
#include "Signal_def.h"

#define  SIGNAL_PASS_2
#include "Signal_def.h"

typedef unsigned short SIGNAL_NAME_T;

typedef enum
{
    E_SIG_MYBOOL = 0,
    E_SIG_CHAR,
    E_SIG_SHORT,
    E_SIG_FLOAT,
    E_SIG_ENUM,
    E_SIG_INTEGER,
    E_SIG_TYPE_NUM
}Sig_SignalType;

typedef struct
{
    SIGNAL_NAME_T sourename;
	Sig_SignalType type;
}Signal_Info;

/**** Definition of macros ****/
typedef  void( *const V_syncreceive )( SIGNAL_NAME_T sigName,const void* value);
typedef  void( *const V_syncsend )( SIGNAL_NAME_T sigName,const void* value);


/**** Declaration of constants ****/
/**** Declaration of variables ****/

extern Signal_Info AllSignal[E_SIG_SIGNALS_NUM];
#endif /*_SIGNAL_H_*/

/****************************** END OF FILE ***************************/

