/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        :	GPIO
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***************************************************************************************************/
/*-History--------------------------------------------------------------
* Version       Date        Name    Changes and comments
* 0.1	   April 15,2020   Huang, Haoxiong  Initial version
****************************************************************************************************/

#ifndef   GPIO_ADAPT_IF_H
#define   GPIO_ADAPT_IF_H
#include "Define.h"
#include "gpio.h"

typedef enum
{
    E_GPIO_DIR_IN = INPUT,
    E_GPIO_DIR_OUT = OUTPUT,
}E_GPIO_DIR_TYPE;

typedef enum
{
    E_GPIO_IN_NO_PULLUP = 0,     	/*default*/
    E_GPIO_IN_PLLUP,        		/*internal pull-up*/
    E_GPIO_IN_INPUT_HIZ   			/*high impedance*/
}E_GPIO_IN_TYPE;

/*output type*/
typedef enum
{
    E_GPIO_OUT_PUSH_PULL = 0,  		/*default*/
}E_GPIO_OUT_TYPE;


extern void GPIO_Adapt_SetInputMode(GPIO_Type *port,U8 pin,E_GPIO_IN_TYPE Mode);
extern void GPIO_Adapt_SetOutputMode(GPIO_Type *port,U8 pin,E_GPIO_OUT_TYPE Mode);
extern U8 GPIO_Adapt_GetVal(GPIO_Type *port, U8 pin);
extern void GPIO_Adapt_SetVal(GPIO_Type *port, U8 pin,U8 level);

#define DIO_SETDIRIN(x, y) GPIO_Adapt_SetInputMode(x,y,E_GPIO_IN_NO_PULLUP)
#define DIO_SETDIROUT(x, y) GPIO_Adapt_SetOutputMode(x,y,E_GPIO_OUT_PUSH_PULL)

#endif

