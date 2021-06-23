/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : ADC Device
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/21  Huang Haoxiong      Initial
*************************************************************************************************************/

#ifndef DEVCTRL_ADC_H
#define DEVCTRL_ADC_H

#include "DevCtrl_Io.h"
#include "Adc_Adapt_if.h"
/**** Definition of constants ****/

/**** Declaration of variables ****/
#define DEV_ADC_PASS_1
#include "DevCtrl_Adc_Def.h"

#define DEV_ADC_INVALID					0xFFFFU

typedef unsigned short DEV_ADC_NAME_T;
typedef unsigned short DEV_ADC_16BIT_T;

typedef struct
{
    U8  Port;
    U8  Pin;
}AD_PIN_CONFIG_S;

typedef struct
{
	DEV_ADC_16BIT_T RawValue[DEV_ADC_ALL_FILTER_TIME];				/* store the convert result that get from the ResultRegister */
    DEV_ADC_16BIT_T ResultValue[E_DEV_ADC_NUM];						/* store the convert result after filter counter */
	U16 FilterCounter[E_DEV_ADC_NUM];								/* filter counter */
	U16 CurrentOffset[E_DEV_ADC_NUM];								/* record current offset for origin */
}DEV_ADC_DATA_S;


typedef struct
{
	U8 Group;														//group
	U8 Channel;														//channel
	U16 FilterTime;													//set filter time for detection adc,unit by 10ms
}DEV_ADC_TYPE_S;


#define AD_READ(x,y)  	ADC_Adapt_GetValue(x,y)




extern  DEV_ADC_TYPE_S gsAdcPort[E_DEV_ADC_NUM];

extern void DevCtrl_ADC_Register(void);
extern void DevCtrl_ADC_Init(void);
extern void DevCtrl_ADC_Deinit(void);
extern void DevCtrl_ADC_CycleTask_10ms(void);

extern DEV_ADC_16BIT_T DevCtrl_ADC_GetFilterValue(DEV_ADC_NAME_T sigName);
extern DEV_ADC_16BIT_T DevCtrl_ADC_GetRawValue(DEV_ADC_NAME_T sigName);

#define DEV_ADC_SYNC_READ(name)					DevCtrl_ADC_GetRawValue(name)
#define DEV_ADC_ASYNC_READ(name)				DevCtrl_ADC_GetFilterValue(name)


#endif




