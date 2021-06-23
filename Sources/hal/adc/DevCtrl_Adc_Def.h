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

#ifdef DEV_ADC_PASS_1
#undef DEV_ADC_PASS_1

#define BEGIN_ADC				enum ADC_Name{
#define END_ADC					E_DEV_ADC_NUM};
#define ADCDESC(x)				x,
#include "DevCtrl_Adc_Cfg.h"

#endif

#define REGISTER_ADC(ADC_NAME,GROUP,CHANNEL,FILTER_TIME)                     		    \
do{                                                                                   	\
	if((ADC_NAME < E_DEV_ADC_NUM) && (E_DEV_ADC_NUM > 0))                               \
	{                                                                                   \
		gsAdcPort[ADC_NAME].Group = GROUP;												\
		gsAdcPort[ADC_NAME].Channel = CHANNEL;                                   		\
		gsAdcPort[ADC_NAME].FilterTime = FILTER_TIME;                                	\
	}                                                                                   \
}while(0)






