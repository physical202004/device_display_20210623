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
/* variable of signal list */
TRACE_IO_8BIT_T gtTracePrevIOList[E_IO_IN_NUM] = {LOW_LEVEL};

/* previous state of battery voltage */
TRACE_BAT_16BIT_T gtPrevBatValue = 0u;

/* previous of pwm value */
TRACE_PWM_32BIT_T gtPrevPWMDutyValue[E_TRACE_PWM_MAX] = {0u};

/* adc previous status value */
TRACE_ADC_16BIT_T gtPrevADCValue[E_DEV_ADC_NUM] = {0u};

/* send diagnosetic message flag */
MYBOOL gmbSendIfcMsgFlag = FALSE;


/* event switch, TRUE: send belong to this type of event data, FALSE: do nothing */
MYBOOL gmbTraceEventSwitch[E_TRACE_EVENT_MAX] = {
    TRUE,         	/* trace on-off */
    FALSE,          /* general information */
    FALSE,          /* touch information */
    FALSE,          /* signal information */
    FALSE,          /* all event */
};

#endif


