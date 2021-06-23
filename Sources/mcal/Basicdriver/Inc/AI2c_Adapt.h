/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Analog i2c module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/


#ifndef AI2C_ADAPT_H
#define AI2C_ADAPT_H
#include "AI2c_Adapt_if.h"
#include "GPIO_Adapt_if.h"

//Whether to use sequential read and write
#define AI2C_SEQUENCE_RW

#define AI2C_READ_BIT					(0x01u)
#define AI2C_WRITE_BIT					(0u)
#define AI2C_SLAVE_ADDR					(0x5A)	//8bit address

#define AI2C_BAUD_100K					(100)
#define AI2C_BAUD_400K					(0)
#define AI2C_DELAY_TIME					(AI2C_BAUD_400K)
#define AI2C_STRECHING_MAX_TIME 		(100)

typedef enum{
  E_AI2C_ACK     //send ACK
  ,E_AI2C_NACK   //send NACK
}E_AI2C_ACK_TYPE;


#define AI2C_SCL_PORT					PTD
#define AI2C_SCL_PIN					PTD0

#define AI2C_SDA_PORT					PTD
#define AI2C_SDA_PIN					PTD1
 
#define AI2C_LOW_LEVEL					0U
#define AI2C_HIGH_LEVEL					1U

#define AI2C_SCL_IN()					//GPIO_Adapt_SetInputMode(AI2C_SCL_PORT,AI2C_SCL_PIN,E_NO_PULLUP)
#define AI2C_SDA_IN()					GPIO_Adapt_SetInputMode(AI2C_SDA_PORT,AI2C_SDA_PIN,E_GPIO_IN_NO_PULLUP)

#define AI2C_SCL_OUT()					//GPIO_Adapt_SetOutputMode(AI2C_SCL_PORT,AI2C_SCL_PIN,E_PUSH_PULL)
#define AI2C_SDA_OUT()					GPIO_Adapt_SetOutputMode(AI2C_SDA_PORT,AI2C_SDA_PIN,E_GPIO_OUT_PUSH_PULL)

#define AI2C_READ_SCL()					GPIO_Adapt_GetVal(AI2C_SCL_PORT,AI2C_SCL_PIN)
#define AI2C_READ_SDA()					GPIO_Adapt_GetVal(AI2C_SDA_PORT,AI2C_SDA_PIN)

#define AI2C_SET_SCL()					GPIO_Adapt_SetVal(AI2C_SCL_PORT,AI2C_SCL_PIN,AI2C_HIGH_LEVEL)
#define AI2C_CLR_SCL()					GPIO_Adapt_SetVal(AI2C_SCL_PORT,AI2C_SCL_PIN,AI2C_LOW_LEVEL)

#define AI2C_SET_SDA()					GPIO_Adapt_SetVal(AI2C_SDA_PORT,AI2C_SDA_PIN,AI2C_HIGH_LEVEL)
#define AI2C_CLR_SDA()					GPIO_Adapt_SetVal(AI2C_SDA_PORT,AI2C_SDA_PIN,AI2C_LOW_LEVEL)

#endif


