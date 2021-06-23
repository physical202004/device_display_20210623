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
/******************************************** register input signal  ************************************************/
BEGIN_IO_IN
IODESC(IO_IN_BL_FAULT)
IODESC(IO_IN_LVDS_LOCK)
IODESC(IO_IN_TFT_FAULT)
IODESC(IO_IN_BL_EN_FR_HU)
IODESC(IO_IN_TFT_EN_FR_HU)
IODESC(IO_IN_TFT_STBYB_FR_HU)
IODESC(IO_IN_TFT_RST_FR_HU)
IODESC(IO_IN_BL_PWM_FR_HU)
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
IODESC(IO_IN_CTP_RST_FR_HU)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif
END_IO_IN

/******************************************** register output signal  ************************************************/
BEGIN_IO_OUT
IODESC(IO_OUT_TFT_VDD)
IODESC(IO_OUT_LVDS_1V2)
IODESC(IO_OUT_DCDC_ENABLE)
IODESC(IO_OUT_TFT_VGH)
IODESC(IO_OUT_TFT_VGL)
IODESC(IO_OUT_TFT_BIAS)
IODESC(IO_OUT_TFT_SCL)
IODESC(IO_OUT_TFT_SDA)
IODESC(IO_OUT_TFT_STBYB)
IODESC(IO_OUT_LVDS_PDB)
IODESC(IO_OUT_BL_ENABLE)
IODESC(IO_OUT_BL_VDD)
IODESC(IO_OUT_BL_ONOFF)
IODESC(IO_OUT_BL_NSS)
IODESC(IO_OUT_BL_FAULT_TO_HU)
IODESC(IO_OUT_TFT_FAULT_TO_HU)
IODESC(IO_OUT_TFT_RST)
IODESC(IO_OUT_LVDS_CRQ)
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
IODESC(IO_OUT_CTP_VDD)
IODESC(IO_OUT_CTP_RST)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
IODESC(IO_OUT_3V3_SW_EN)
IODESC(IO_OUT_DAC_ONOFF)
#endif
END_IO_OUT


#undef BEGIN_IO_IN
#undef IODESC
#undef END_IO_IN
#undef BEGIN_IO_OUT
#undef END_IO_OUT


