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
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/21  Huang Haoxiong      Initial
*************************************************************************************************************/

#include "Gpio_Adapt.h"

/***********************************************************************
* Name        	: GPIO_Adapt_SetInputMode
* Description	: Set GPIO to input mode
* Description	: port:GPIO port
				  pin:GPIO pin
				  Mode:mode for enum E_GPIO_IN_TYPE
* Returns     	: None
***********************************************************************/
void GPIO_Adapt_SetInputMode(GPIO_Type *port,U8 pin,E_GPIO_IN_TYPE Mode)
{
    CONFIG_PIN_AS_GPIO(port, pin, INPUT);
    if(Mode == E_GPIO_IN_PLLUP)
    {
        ENABLE_PULLUP(port, pin);
    }
    ENABLE_INPUT(port, pin);
}

/***********************************************************************
* Name        	: GPIO_Adapt_SetOutputMode
* Description	: Set GPIO to output mode
* Description	: port:GPIO port
				  pin:GPIO pin
				  Mode:mode for enum E_GPIO_IN_TYPE
* Returns     	: None
***********************************************************************/
void GPIO_Adapt_SetOutputMode(GPIO_Type *port,U8 pin,E_GPIO_OUT_TYPE Mode)
{
    CONFIG_PIN_AS_GPIO(port, pin, OUTPUT);
    OUTPUT_CLEAR(port, pin);
}


/***********************************************************************
* Name        	: GPIO_Adapt_GetVal
* Description	: Get value of the GPIO Pin
* Description	: port:GPIO port
				  pin:GPIO pin
* Returns     	: pin level
***********************************************************************/
U8 GPIO_Adapt_GetVal(GPIO_Type *port, U8 pin)
{
    U8 subRet = 0;
    E_GPIO_DIR_TYPE leMode = E_GPIO_DIR_IN;
    
    if(GET_MODE(port, pin))
    {
        leMode = E_GPIO_DIR_OUT;
    }
    else
    {
        leMode = E_GPIO_DIR_IN;
    }
    
    if(leMode == E_GPIO_DIR_OUT)
    {
		if(READ_OUTPUT(port, pin) == 0)
        {
            subRet = 0;
        }
        else
        {
            subRet = 1;
        }
    }
    else
    {
		if(READ_INPUT(port, pin) == 0)
        {
            subRet = 0;
        }
        else
        {
            subRet = 1;
        }
    }
    return subRet;
}

/***********************************************************************
* Name        	: GPIO_Adapt_SetVal
* Description	: Set value of the GPIO Pin
* Description	: port:GPIO port
				  pin:GPIO pin
				  level:IO level
* Returns     	: None
***********************************************************************/
void GPIO_Adapt_SetVal(GPIO_Type *port, U8 pin,U8 level)
{
    if(level == 0)
    {
    	OUTPUT_CLEAR(port, pin);
    }
    else
    {
        OUTPUT_SET(port, pin);
    }
}


