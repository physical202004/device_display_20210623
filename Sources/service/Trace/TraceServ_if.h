/***********************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           		*
* All Rights Reserved.             										*
* Department : ADS R&D SW		                   						*
* AUTHOR	   : Huang Haoxiong                    						*
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

#ifndef TRACESERV_IF_H
#define TRACESERV_IF_H

typedef unsigned char TRACE_IO_8BIT_T;
typedef unsigned short TRACE_ADC_16BIT_T;
typedef unsigned short TRACE_BAT_16BIT_T;
typedef unsigned int TRACE_PWM_32BIT_T;

#include "Global_Definition.h"

//#define TRACE_DEBUG					//trace function switch

#ifdef TRACE_DEBUG
#define TRACE_USE_ABRIDGED_FEATURE		//open abridged version function

#define TRACE_SIGNAL
#define TRACE_ADC_FUNC
#define TRACE_IFC_FUNC

#define TRACE_CHANNEL	UART_CH1		/* channel */

#endif

/* event interface */
typedef enum{
    E_TRACE_EVENT_TRACE = 0,        /* trace on-off */
    E_TRACE_EVENT_GENERAL,          /* general information */
    E_TRACE_EVENT_TOUCH,            /* touch information */
    E_TRACE_EVENT_SIGNAL,           /* signal information */
    E_TRACE_EVENT_ONOFF,            /* control all event */
    E_TRACE_EVENT_MAX
}E_TRACE_EVENT;

typedef enum{
	E_TRACE_PWM_CH0 = 0,
	E_TRACE_PWM_CH1,
	E_TRACE_PWM_CH2,
	E_TRACE_PWM_CH3,
	E_TRACE_PWM_CH4,
	E_TRACE_PWM_CH5,
	E_TRACE_PWM_CH6,
	E_TRACE_PWM_CH7,
	E_TRACE_PWM_MAX,
}E_TRACE_PWM_CH;


extern MYBOOL TraceServ_SendStrMsg(const char *str,E_TRACE_EVENT event);                         			      	/* send type of string message */
extern MYBOOL TraceServ_SendDStrMsg(const char *str1,const char *str2,E_TRACE_EVENT event);							/* send double string message */
extern MYBOOL TraceServ_SendStrWithIntMsg(const char *label,int* num,U16 numLen,E_TRACE_EVENT event);       		/* send type of integer messages */
extern MYBOOL TraceServ_SendStrWithHexMsg(const char *label,U8* num,U16 numLen,E_TRACE_EVENT event);        		/* send type of hex messages */
extern MYBOOL TraceServ_SendStrWithUShortMsg(const char *label,U16* num,U16 numLen,E_TRACE_EVENT event);   			/* send type of unsigned short messages */
extern MYBOOL TraceServ_SendStrWithShortMsg(const char *label,short* num,U16 numLen,E_TRACE_EVENT event);  			/* send type of short messages */
extern MYBOOL TraceServ_SendStrWithFloatMsg(const char *label,float* num,U16 numLen,E_TRACE_EVENT event);  			/* send type of float messages */
extern MYBOOL TraceServ_SendSysStateMsg(E_SYS_STATE sysState,E_WORKING_STATE workingState);							/* send system state */
extern MYBOOL TraceServ_SendDevStateMsg(DEV_ID_T id,E_DEVCTRL_STATE state);											/* send device state */
extern MYBOOL TraceServ_SendDevPMMsg(E_DEVCTRL_SM_STATE currState,E_DEVCTRL_SM_STATE nextState);					/* send memory value */
extern MYBOOL TraceServ_SendRegMsg(const char *str,U32 addr,E_TRACE_EVENT event);                            		/* send register addr information message */
extern MYBOOL TraceServ_SendPwmMsg(E_TRACE_PWM_CH ch,U32 duty,U32 period);											/* send pwm message */
extern MYBOOL TraceServ_SendTPMsg(U8 *data);                                              							/* send tp message */
extern MYBOOL TraceServ_GetSendData(U8* data);                                                         				/* get send data */
extern void TraceServ_Destroy(void);                                                                 				/* destroy all trace data */
extern void TraceServ_Init(void);																					/* initial */
extern void TraceServ_CycleTask10ms(void);																			/* Cycle task function */

/* check error information */
#define ISERR(_label,_condition,_ret)   						\
do{ 															\
	if(_condition)												\
	{															\
		TraceServ_SendStrMsg(_label,E_TRACE_EVENT_GENERAL);		\
		_ret;													\
	}															\
}while(0)
		
		/* General log print */
#define PRINTOUT(_msg)											\
	TraceServ_SendStrMsg(_msg,E_TRACE_EVENT_GENERAL);
			
		/* Output log with important information */
#define PRINT_IMPORTANT_MSG(_msg)								\
	TraceServ_SendStrMsg(_msg,E_TRACE_EVENT_TRACE);
		


#endif


