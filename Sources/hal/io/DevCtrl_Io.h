/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : IO Device
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
#ifndef DEVCTRL_IO_H
#define DEVCTRL_IO_H
#include "Define.h"
/**** Declaration of variables ****/
#include "Gpio_Adapt_if.h"
#include "DevCtrl_if.h"

#define DEV_IO_PASS_1
#include "DevCtrl_Io_Def.h"


/**** Definition of macros ****/
#define DIO_READ(x,y)       GPIO_Adapt_GetVal(x,y)
#define DIO_SET(x,y)        GPIO_Adapt_SetVal(x,y,HIGH_LEVEL)
#define DIO_CLEAR(x,y)      GPIO_Adapt_SetVal(x,y,LOW_LEVEL)


#define DEV_IO_INVALID					0xFFU

/* hardware signal status */
#define LOW_LEVEL						0U
#define HIGH_LEVEL						1U


typedef unsigned short DEV_IO_NAME_T;
typedef unsigned char DEV_IO_8BIT_T;

typedef struct
{
    GPIO_Type * Port;
    U8 			Pin;
}IO_PIN_CONFIG_S;



typedef struct
{
	U8 initValue;
    IO_PIN_CONFIG_S PinConfig;
}IO_OUT_TYPE_S;


typedef struct
{
	DEV_IO_8BIT_T RawValue[E_IO_IN_NUM];				/*store the convert result that get from the ResultRegister*/
	DEV_IO_8BIT_T ResultValue[E_IO_IN_NUM];				/*store the convert result after 1 order filter*/
	U16 FilterCounter[E_IO_IN_NUM];
}IO_IN_DATA_S;


typedef struct
{
	U8 InitialValue;
	U8 FilterTime;
    IO_PIN_CONFIG_S PinConfig;
}IO_IN_TYPE_S;


extern IO_IN_TYPE_S gsIoInPort[E_IO_IN_NUM];
extern IO_OUT_TYPE_S gsIoOutPort[E_IO_OUT_NUM];

extern void DevCtrl_IO_Register(void);
extern void DevCtrl_IO_Init(void);
extern void DevCtrl_IO_Deinit(void);
extern void DevCtrl_IO_CycleTask_10ms(void);

extern DEV_IO_8BIT_T DevCtrl_IO_GetRawValue(DEV_IO_NAME_T ioName);
extern DEV_IO_8BIT_T DevCtrl_IO_GetFilterValue(DEV_IO_NAME_T ioName);
extern void DevCtrl_IO_WriteValue(DEV_IO_NAME_T ioName,DEV_IO_8BIT_T value);



#define DEV_IO_SYNC_READ(name)					DevCtrl_IO_GetRawValue(name)
#define DEV_IO_ASYNC_READ(name)					DevCtrl_IO_GetFilterValue(name)
#define DEV_IO_WRITE(name,val)					DevCtrl_IO_WriteValue(name,val)

#endif

