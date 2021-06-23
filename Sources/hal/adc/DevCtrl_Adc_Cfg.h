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
/*Define the maximum value of the filter time in the registered channel by DevCtrl_Adc_Cfg.c*/
#define DEV_ADC_MAX_FILTER_TIME	 		(20U)
/*The filtering time of all channels should correspond to the time registered by DevCtrl_Adc_Cfg.c*/
#define DEV_ADC_ALL_FILTER_TIME	 		(140U)

/******************************************** ADC Configuration ************************************************/
BEGIN_ADC
ADCDESC(ADC_BL_VOL_DET)
ADCDESC(ADC_LVDS_VDD_DET)
ADCDESC(ADC_LVDS_VDDIO_DET)
ADCDESC(ADC_BL_TEMP_DET)
ADCDESC(ADC_TFT_VDD_DET)
ADCDESC(ADC_LVDS_1V2_DET)
ADCDESC(ADC_3V3_SW_DET)
ADCDESC(ADC_TFT_VSP_DET)
ADCDESC(ADC_TFT_VGH_DET)
ADCDESC(ADC_BAT_DET)
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
ADCDESC(ADC_CTP_AVDD_DET)
ADCDESC(ADC_CTP_VDD_DET)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
ADCDESC(ADC_5V_SW_DET)
ADCDESC(ADC_DAC_3V3_DET)
#endif
ADCDESC(ADC_TFT_VGL_DET)
END_ADC

#undef BEGIN_ADC
#undef ADCDESC
#undef END_ADC


