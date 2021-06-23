/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : GPIO
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

#ifndef GPIO_ADAPT_H
#define GPIO_ADAPT_H
#include "Gpio_Adapt_if.h"
#include "SKEAZN642.h"

/**** Definition of contanst ****/
/**** Definition of macros ****/
#define READ_OUTPUT(port,port_pin)					XREAD_OUTPUT(port,port_pin)
#define XREAD_OUTPUT(port,port_pin)					(port->PDOR & GPIO_PDOR_PDO(1<<port_pin))

#define GET_MODE(port,port_pin)						XGET_MODE(port,port_pin)
#define XGET_MODE(port,port_pin)					(port->PDDR & (1 << port_pin))

#endif

