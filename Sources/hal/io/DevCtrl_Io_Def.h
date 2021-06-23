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

#ifdef DEV_IO_PASS_1
#undef DEV_IO_PASS_1


#define BEGIN_IO_IN				enum IO_InName{
#define END_IO_IN				E_IO_IN_NUM};
#define BEGIN_IO_OUT			enum IO_OutName{
#define END_IO_OUT				E_IO_OUT_NUM};
#define IODESC(x)				x,

#include "DevCtrl_Io_Cfg.h"

#endif

#define IO_REGISTER_IN(IO_NAME,INITIAL_VALUE,PORT,PIN,FILTER_COUNT)			                \
do{                                                                                      	\
	if((IO_NAME < E_IO_IN_NUM) && (E_IO_IN_NUM > 0))                               	 		\
	{                                                                                    	\
		gsIoInPort[IO_NAME].InitialValue = INITIAL_VALUE;                       			\
		gsIoInPort[IO_NAME].PinConfig.Port = PORT;	                             			\
		gsIoInPort[IO_NAME].PinConfig.Pin = PIN;	                             			\
		gsIoInPort[IO_NAME].FilterTime = FILTER_COUNT;                         				\
	}                                                                                    	\
}while(0)


#define IO_REGISTER_OUT(IO_NAME,INITIAL_VALUE,PORT,PIN)			                            \
do{                                                                                         \
	if((IO_NAME < E_IO_OUT_NUM) && (E_IO_OUT_NUM > 0))                                      \
	{                                                                                       \
		gsIoOutPort[IO_NAME].PinConfig.Port = PORT;	     									\
		gsIoOutPort[IO_NAME].PinConfig.Pin = PIN;	                             			\
		gsIoOutPort[IO_NAME].initValue = INITIAL_VALUE;										\
	}                                                                                    	\
}while(0)


