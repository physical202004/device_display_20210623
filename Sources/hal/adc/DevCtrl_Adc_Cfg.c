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

#include "DevCtrl_Adc.h"

DEV_ADC_TYPE_S gsAdcPort[E_DEV_ADC_NUM] = {{0}};

/***********************************************************************
* Name          : DevCtrl_ADC_Register
* Description   : Register ADC signal
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_ADC_Register(void)
{
	
	/* After modifying the filter time, you need to modify the macro definition DEV_ADC_ALL_FILTER_TIME in the DevCtrl_Adc_Cfg.h file. */
	/*****************adc name,				ad group,ad channel,filter time,can't set to 0,unit by 10ms ************/
	
    REGISTER_ADC(ADC_BL_VOL_DET, 				0,	3,	10); 	//100ms
    REGISTER_ADC(ADC_LVDS_VDD_DET, 				0,	4,	10);	//100ms
    REGISTER_ADC(ADC_LVDS_VDDIO_DET, 			0,	5,	10); 	//100ms
    REGISTER_ADC(ADC_BL_TEMP_DET, 				0,	6,	10); 	//100ms
    REGISTER_ADC(ADC_TFT_VDD_DET, 				0,	7,	10);	//100ms
    REGISTER_ADC(ADC_LVDS_1V2_DET, 				0,	8,	10);	//100ms
    REGISTER_ADC(ADC_3V3_SW_DET, 				0,	9,	10);	//100ms
  	REGISTER_ADC(ADC_TFT_VSP_DET, 				0,	10,	10); 	//100ms
	REGISTER_ADC(ADC_TFT_VGH_DET, 				0,	11,	10); 	//100ms
    REGISTER_ADC(ADC_BAT_DET, 					0,	12,	20); 	//200ms
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
	REGISTER_ADC(ADC_CTP_AVDD_DET,			  	0,  13, 10);	//100ms
	REGISTER_ADC(ADC_CTP_VDD_DET,				0,	14, 10);	//100ms
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
	REGISTER_ADC(ADC_5V_SW_DET,			  		0,  13, 10);	//100ms
	REGISTER_ADC(ADC_DAC_3V3_DET,				0,	14, 10);	//100ms
#endif
	REGISTER_ADC(ADC_TFT_VGL_DET,				0,	15, 10);	//100ms


}

