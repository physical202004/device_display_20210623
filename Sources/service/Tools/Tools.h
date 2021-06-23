/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Tools
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/08/21
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/21  Huang Haoxiong      Porting from DP2 platform
*************************************************************************************************************/
#ifndef TOOLS_H
#define TOOLS_H
#include "Define.h"
#include <string.h>

/**** Definition of constants ****/

/**** Definiion of types ****/

/**** Definition of macros ****/

#define HIGH_MASK			0xF0u
#define LOW_MASK			0x0Fu
#define BCD_OFFSET			0x04u

#define MIN_NUM				0x00u
#define MAX_NUM				0x09u
#define MIN_ALBA			0x0Au
#define MAX_ALBA			0x0Fu

#define NUM_BASE			0x30u
#define ALBA_BASE			0x41u

#define APP_START_ADDRESS  	0x0000
#define APP_END_ADDRESS  	0xFFFF


typedef unsigned char S_SEQUENCE_PROPERTIME;

#define EXE_POWER_OFF       (0u)
#define EXE_POWER_ON        (1u)

typedef struct
{
	U8 PowerOnCycleCnt;
	U8 PowerOnStepCnt;
	U8 PowerOffCycleCnt;
	U8 PowerOffStepCnt;
}S_SEQUENCE_LOG;


typedef struct
{
	MYBOOL (*Action)(void);
}S_SEQUENCE_PROPERACTION;

#define ADD_CONTROL_ACTION(ActionName,DelaySeqTime,DelaySeqAction,DelayTime,Handle,ExecuteSeqNum)     	\
do{                                                                               			\
		 (DelaySeqAction + (ExecuteSeqNum -1))->Action = Handle;  						\
	     *(DelaySeqTime + (ExecuteSeqNum -1)) = DelayTime;								\
}while(0)

#define INIT_SEQUENCE_LOG(x)	do{ 	\
	x.PowerOnStepCnt = 0U;				\
	x.PowerOffStepCnt = 0U;				\
	x.PowerOnCycleCnt = 0U; 			\
	x.PowerOffCycleCnt = 0U; 			\
}while(0)

extern U8 GetMaxValue(U8 *src,U8 size);
extern U8 GetMinValue(U8 *src,U8 size);
extern U8 NumToASC(U8 data);
extern U8 ASCToNum(U8 data);
extern void BCDEncode(U8 *src, U8 len, U8* tar);
extern void BCDDecode(U8 *src, U8 len, U8 *tar);
extern void DelayUs(U16 period);
extern void DelayMs(U16 period);
extern void MemcpyU16Array(U16* dest, U16* src, U16 len);
extern void SortU16ArrayMinToMax(U16* ArrayName,U16 Len);
extern MYBOOL SequenceHandler(S_SEQUENCE_LOG* Log, S_SEQUENCE_PROPERTIME *TimeInfo, S_SEQUENCE_PROPERACTION *ActionInfo, U8 PowerState, U16 Len);
extern U8 FrameChecksum(U8* frame,U16 len);
extern U32 CalChecksumForAPPFlash(void);

#endif /* TOOLS_H */

/****************************** END OF FILE ***************************/

