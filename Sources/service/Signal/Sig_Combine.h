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
*----------------------------------------------------------------------
* 0.1   Aug 31,2016             Re-init
*----------------------------------------------------------------------
* 0.2    Dec 20,2016   ShangQiuju Remove unused include files.
*----------------------------------------------------------------------
* 0.3   Jan 05,2017   ShangQiuju  Add functions.          
*----------------------------------------------------------------------
* 0.4    Jan 24,2017   ZhangJie     Fix Coverity warnings.   
*----------------------------------------------------------------------
* 0.5	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type

*=====================================================================*/

#ifndef _SIG_COMBINE_H_
#define _SIG_COMBINE_H_

#include "define.h"
#include "Signal.h"

/**** Definition of constants ****/

/**** Definition of macros ****/
typedef MYBOOL SIGNAL_MYBOOL_8BIT_T;
typedef unsigned char SIGNAL_CHAR_8BIT_T;
typedef unsigned short SIGNAL_SHORT_16BIT_T;
typedef float SIGNAL_FLOAT_32BIT_T;
typedef unsigned int SIGNAL_ENUM_32BIT_T;
typedef unsigned int SIGNAL_INT_32BIT_T;
/* Signal definition  and please check the hardware channel with the comment*/

/**** Declaration of variables ****/
extern SIGNAL_MYBOOL_8BIT_T gtSigBool[E_SIG_MYBOOL_NUM];
extern SIGNAL_CHAR_8BIT_T gtSigChar[E_SIG_CHAR_NUM];
extern SIGNAL_SHORT_16BIT_T gtSigShort[E_SIG_SHORT_NUM];
extern SIGNAL_FLOAT_32BIT_T gtSigFloat[E_SIG_FLOAT_NUM];
extern SIGNAL_ENUM_32BIT_T gtSigEnum[E_SIG_ENUM_NUM];
extern SIGNAL_INT_32BIT_T gtSigInteger[E_SIG_INT_NUM];



/**** Declaration of functions ****/
extern void Sig_BoolGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_BoolSetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_CharGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_CharSetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_ShortGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_ShortSetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_FloatGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_FloatSetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_EnumGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_EnumSetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_IntegerGetValue(SIGNAL_NAME_T sigName,const void* value);
extern void Sig_IntegerSetValue(SIGNAL_NAME_T sigName,const void* value);


#endif /*_SIG_COMBINE_H_*/

/****************************** END OF FILE ***************************/

