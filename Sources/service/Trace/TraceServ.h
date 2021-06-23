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

#ifndef TRACESERV_H
#define TRACESERV_H
#include "TraceServ_if.h"

#ifdef TRACE_DEBUG
#include "TraceServ_List.h"
#include "Uart_Adapt_if.h"
#include "Mcu_if.h"

/* set trace channel */
#define TRACE_RATE								115200u		// buadrate

#define TRACE_FRAME_MAX_LEN						128u			// Frame length
#define TRACE_NODATA_LEN						5u			// Length in frame except the data segment
#define TRACE_STR_FRAME_MAX_LEN					(TRACE_FRAME_MAX_LEN - TRACE_NODATA_LEN - 5u)	//Length of String frame

#define TRACE_RECV_LENGTH           			32u     	// receive buffer length

#define TRACE_FRAME_HEADER        				0x5Au   	// receive package frame header

#define TRACE_ADC_FILTER            			5u

#ifdef TRACE_ADC_FUNC

//adc reference voltage
#define TRACE_ADC_REFER_VOLT_3V3  				0u
#define TRACE_ADC_REFER_VOLT_5V     			1u
#define TRACE_ADC_REFER_VOLT        			TRACE_ADC_REFER_VOLT_3V3

//all adc precision,default 12bit
#define TRACE_ADC_ACCURACY_8BIT     			0u
#define TRACE_ADC_ACCURACY_10BIT    			1u
#define TRACE_ADC_ACCURACY_12BIT    			2u
#define TRACE_ADC_ACCURACY_16BIT    			3u
#define TRACE_ADC_ACCURACY          			TRACE_ADC_ACCURACY_12BIT

#endif

#define TRACE_RECV_FINISH          				1u      //receive frame finished
#define TRACE_RECV_NONE             			0u      //free status

enum TRACE_BYTE_TYPE
{
    E_TRACE_INT_TYPE = 0,                   	/* type of integer */
    E_TRACE_HEX_TYPE,                    		/* type of hex */
    E_TRACE_FLOAT_TYPE,                     	/* type of float */
    E_TRACE_SHORT_TYPE,                     	/* type of short */
    E_TRACE_USHORT_TYPE,                    	/* type of unsigned short */
    E_TRACE_DSTR_TYPE,							/* type of double string */

};

typedef enum{
    E_TRACE_RECV_STATE_FRAME_HEADER = 0, 		/* frame header */
    E_TRACE_RECV_STATE_SQUENCE_MSB,    			/* squence number MSB */
    E_TRACE_RECV_STATE_SQUENCE_LSB,      		/* squence number LSB */
    E_TRACE_RECV_STATE_DATA_LENGTH,    			/* data length */
    E_TRACE_RECV_STATE_DATA_SECTION,        	/* data */
    E_TRACE_RECV_STATE_CHECKSUM,            	/* check sum */
    E_TRACE_RECV_STATE_FINISH,              	/* receive finish */
}E_TRACE_RECV_STATE;                        	/* trace receive status */



typedef enum{
    E_TRACE_SQUENCE_EVENT = 0,              	/* trace event */
    E_TRACE_SQUENCE_SYS,                    	/* see about the system information */
    E_TRACE_SQUENCE_RESET_REASON,           	/* cause of the reset */
    E_TRACE_SQUENCE_HISTORY_IFC,        		/* see about the diagnosetic history */
    E_TRACE_SQUENCE_REG,                    	/* see about the value of register */
    E_TRACE_SQUENCE_DEV_STATE,             		/* send devices state */
    E_TRACE_SQUENCE_STRING,                 	/* send string message */
    E_TRACE_SQUENCE_LABEL,                  	/* send label message */
    E_TRACE_SQUENCE_NOTIFY,                 	/* uart connect notify */
    E_TRACE_SQUENCE_CHECKSUM,               	/* see about the checksum */
    E_TRACE_SQUENCE_TP_TOUCH,               	/* tp message */
    E_TRACE_SQUENCE_SIGNAL,                 	/* signal message */
    E_TRACE_SQUENCE_PWM,                    	/* pwm message */
    E_TRACE_SQUENCE_BAT,                    	/* battery message */
    E_TRACE_SQUENCE_PLATFORM,               	/* platform information*/
    E_TRACE_SQUENCE_ADC,                    	/* adc channel information */
    E_TRACE_SQUENCE_DEV_PM_STATE,              	/* power status of device controller */
    E_TRACE_SQUENCE_SYS_STATE,              	/* system state */
    E_TRACE_SQUENCE_HW_VERSION,              	/* hw version */
}E_TRACE_SQUENCE;                           	/* trace squence number */



typedef struct
{
    E_TRACE_RECV_STATE status;              	/* reveive status */
    U8 len;                                 	/* data section length */
    U8 cnt;                                 	/* reveive buffer count */
    U8 buffer[TRACE_RECV_LENGTH];           	/* reveive buffer array */
}TRACE_RECV;                       				/* trace receive data structure */


extern TRACE_IO_8BIT_T gtTracePrevIOList[E_IO_IN_NUM];
extern TRACE_BAT_16BIT_T gtPrevBatValue;
extern TRACE_PWM_32BIT_T gtPrevPWMDutyValue[E_TRACE_PWM_MAX];
extern TRACE_ADC_16BIT_T gtPrevADCValue[E_DEV_ADC_NUM];
extern MYBOOL gmbTraceEventSwitch[E_TRACE_EVENT_MAX];
extern MYBOOL gmbSendIfcMsgFlag;

#endif      //TRACE_DEBUG
#endif      //_TRACESERV_H_

