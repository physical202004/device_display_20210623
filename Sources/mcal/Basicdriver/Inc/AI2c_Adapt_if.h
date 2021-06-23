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

#ifndef AI2C_ADAPT_IF_H
#define AI2C_ADAPT_IF_H

typedef enum{
  E_AI2C_ERR_OK = 0,   		//OK
  E_AI2C_ERR_NACK,			//NO ACK response
  E_AI2C_ERR_TIMEOUT,    	//Response timeout
}E_AI2C_ERR;

extern void AI2c_Adapt_Init(void);
extern void AI2c_Adapt_Deinit(void);
extern E_AI2C_ERR AI2c_Adapt_Write(unsigned char address,unsigned char* buffer,unsigned short Len);
extern E_AI2C_ERR AI2c_Adapt_Read(unsigned char address,unsigned char* buffer,unsigned short Len);


#endif

