/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   : 										   *
************************************************************************
* Object        : DRIVER_IF.H
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:  0.1
* Date: 2011-12-20
* Author: Shao Guangxian
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
*-----------------------------------------------------------------------
* 0.1          Dec 21,2015    Init version.
*-----------------------------------------------------------------------
* 0.2          Dec 21,2016    ShangQiuju    Change include files.
*-----------------------------------------------------------------------
* 0.3          Jan 12,2017    ZhangJie    Remove unused enum and struct.
*-----------------------------------------------------------------------
* 0.4          Jan 13,2017    ShangQiuju    Remove E_COMCONTROL define here.
 ----------------------------------------------------------------------
* 0.5    	   Jan 16,2017    RenWeimin    Add define for test code.
*----------------------------------------------------------------------
* 0.6          Jan 24,2017    ZhangJie     Fix Coverity warning.
*----------------------------------------------------------------------
* 0.7          Feb 09,2017    ZhangJie     Remove E_EOL_COMCONTROL define here.
*----------------------------------------------------------------------
* 0.8          Feb 09,2017    Pei Jingli     Add pwmd IOTYPE.
* ----------------------------------------------------------------------
* 0.9          Feb 20,2017  PEI Jingli  Add tau-pwm,capture, hw-timer
*----------------------------------------------------------------------
* 1.0          Feb 20,2017    Pei Jingli     Add POWERS TYPE.
*----------------------------------------------------------------------
* 1.1          Feb 28,2017    ZhangJie     Add uart baudrate TYPE.
*----------------------------------------------------------------------
* 1.2          Mar 17,2017    ZhangJie    Add E_REGISTER_CALLBACK.
* ----------------------------------------------------------------------
* 1.3          Mar 21,2017    PEI Jingli  Fixed enum name.
*=====================================================================*/
#ifndef _DRIVER_IF_H_
#define _DRIVER_IF_H_
#include "Timer_if.h"
#include "Define.h"

/**** Definition of constants ****/
/*
typedef enum
{
	E_REGISTER_CALLBACK,
	E_MEM_MAP,
	E_RW,
	E_IOTYPE_NUM
}IoControlType;
*/

/**** Definition of types ****/
/*map high layer memory to low layer
typedef struct
{
 void* memadress;
 U16 memlen;
}IoMemMap;

typedef struct
{
 BOOL bRegister;
 IOCALLBACK_FUNC func;
}IoCallback;

typedef struct
{
 U32 param;
 void* pbufer;
 U16 buferlen;
}IoRW;
*/

/**** Definition of macros ****/

typedef enum
{
   E_DEV_POWERON,
   E_DEV_POWEROFF,   	
   E_DEV_WAKEUP,
   E_DEV_SLEEP,
   E_DEV_MAX_TYPE,
}E_DEV_POWER;


typedef enum
{
   E_DEV_SPOWERON,
   E_DEV_SPOWEROFF,   	
   E_DEV_SWAKEUP,
   E_DEV_SSLEEP,
   E_DEV_ERROR,
   E_DEV_MAX_STYPE,
}E_DEV_POWERS;

enum
{
	E_DEV_START,
	E_DEV_STOP,
	E_DEV_SETDUTY,
	E_DEV_SETBAUDRATE,
	E_REGISTER_CALLBACK,
	E_DEV_IOTYPE_NUM
};

/* TAU PWM function */
typedef struct{
//  TAU_UNIT un;          /* unit select, i.e TAUB0, D0, J0,J1*/
//  TAU_CHANNEL chm; /* master channel select, 0~15(0~3 for J0/1)*/
//  TAU_CHANNEL sl_chm; /* slave channel select, 0~15(0~3 for J0/1)*/
}ST_PWM_DEVICE_CHANNEL;
typedef struct{
  ST_PWM_DEVICE_CHANNEL pwm;
  U32 freHZ;   /* unit by HZ*/
  U16 ratioType;  /*precision*/
  U16 duty;           /* high level percentage is (duty/RatioType)*100 */
}ST_PWM_DEVICE_PARAM;

typedef struct{
  ST_PWM_DEVICE_CHANNEL pwm;
  U16 ratioType;  /*precision*/
  U16 duty;           /* high level percentage is (duty/RatioType)*100 */
}ST_PWM_DEVICE_CHANGEDUTY;

/* Capture function */
typedef struct{
//  TAU_UNIT un;          /* unit select, i.e TAUB0, D0, J0,J1*/
//  TAU_CHANNEL chm; /* master channel select, 0~15(0~3 for J0/1)*/
}ST_DEVICE_CHANNEL;
typedef struct{
//  ST_DEVICE_CHANNEL capChannel;
//  CAP_TYPE_ST edge;           /* edge detection */
}ST_CAP_DEVICE_PARAM;

/* Hardware timer function */
typedef struct{
  ST_DEVICE_CHANNEL timerChannel;
  U32 timerMs;           /* unit by micro second */
  BOOL bWaveOutput; /* output square ware or nor */
  BOOL bPeriodic;      /* if a cyclic timer */
}ST_HWTIMER_DEVICE_PARAM;

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


/***********end extern micro/type define ****************/

#endif /*_DRIVER_IF_H_*/
/****************************** END OF FILE ***************************/
