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

#include "DevCtrl_Io.h"

IO_IN_TYPE_S gsIoInPort[E_IO_IN_NUM] = {{0}};
IO_OUT_TYPE_S gsIoOutPort[E_IO_OUT_NUM] = {{0}};

/***********************************************************************
* Name          : DevCtrl_IO_Register
* Description   : Register IO signal
* Parameter     : None
* Returns       : None
***********************************************************************/
void DevCtrl_IO_Register(void)
{
	/* PWM configuration */
	SIM_PINSEL |= SIM_PINSEL_FTM2PS0_MASK | SIM_PINSEL_SPI0PS_MASK; /* Select Pins corresponds to the PTH0 for pwm output,and PTE0~2 for spi */
	
	/*############################################## input ####################################################*/
	/*****************name,					initial state,	port,	pin,		filter time(unit by 10ms)******************/
	IO_REGISTER_IN(IO_IN_BL_FAULT,			HIGH_LEVEL,		PTG,	PTG1,		20);
	IO_REGISTER_IN(IO_IN_LVDS_LOCK,			LOW_LEVEL,		PTE,	PTE7,		10);
	IO_REGISTER_IN(IO_IN_TFT_FAULT,			LOW_LEVEL,		PTA,	PTA1,		10);
	IO_REGISTER_IN(IO_IN_BL_EN_FR_HU, 		LOW_LEVEL,		PTE,	PTE6,		10);
	IO_REGISTER_IN(IO_IN_TFT_EN_FR_HU,		LOW_LEVEL,		PTE,	PTE5,		10);
	IO_REGISTER_IN(IO_IN_TFT_STBYB_FR_HU,	LOW_LEVEL,		PTD,	PTD2,		10);
	IO_REGISTER_IN(IO_IN_TFT_RST_FR_HU,		LOW_LEVEL,		PTD,	PTD3,		10);
	IO_REGISTER_IN(IO_IN_BL_PWM_FR_HU,		LOW_LEVEL,		PTH,	PTH0,		10);
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
	IO_REGISTER_IN(IO_IN_CTP_RST_FR_HU, 	LOW_LEVEL,		PTA,	PTA6,		10);
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif

	/*############################################## output ####################################################*/
	/******************name, 				initial value,	port, 	pin ************************/
	IO_REGISTER_OUT(IO_OUT_TFT_VDD,			LOW_LEVEL,		PTD,	PTD0);
	IO_REGISTER_OUT(IO_OUT_LVDS_1V2,		LOW_LEVEL,		PTH,	PTH7);
	IO_REGISTER_OUT(IO_OUT_DCDC_ENABLE,		LOW_LEVEL,		PTH,	PTH6);
	IO_REGISTER_OUT(IO_OUT_TFT_VGH,			LOW_LEVEL,		PTF,	PTF0);
	IO_REGISTER_OUT(IO_OUT_TFT_VGL,			LOW_LEVEL,		PTF,	PTF1);
	IO_REGISTER_OUT(IO_OUT_TFT_BIAS,		LOW_LEVEL,		PTD,	PTD4);
	IO_REGISTER_OUT(IO_OUT_TFT_SCL,			LOW_LEVEL,		PTF,	PTF2);
	IO_REGISTER_OUT(IO_OUT_TFT_SDA,			LOW_LEVEL,		PTF,    PTF3);
	IO_REGISTER_OUT(IO_OUT_TFT_STBYB,		LOW_LEVEL,		PTG,	PTG3);
	IO_REGISTER_OUT(IO_OUT_LVDS_PDB,		LOW_LEVEL,		PTB,	PTB5);
	IO_REGISTER_OUT(IO_OUT_BL_ENABLE,	  	LOW_LEVEL,		PTG,	PTG0);
	IO_REGISTER_OUT(IO_OUT_BL_VDD,	  		LOW_LEVEL,		PTH,	PTH1);
	IO_REGISTER_OUT(IO_OUT_BL_ONOFF,	  	LOW_LEVEL,		PTC,	PTC5);
	IO_REGISTER_OUT(IO_OUT_BL_NSS,	  		LOW_LEVEL,		PTE,	PTE3);
	IO_REGISTER_OUT(IO_OUT_BL_FAULT_TO_HU, 	LOW_LEVEL,		PTD,	PTD7);
	IO_REGISTER_OUT(IO_OUT_TFT_FAULT_TO_HU,	LOW_LEVEL,		PTD,	PTD6);
	IO_REGISTER_OUT(IO_OUT_TFT_RST, 		LOW_LEVEL,		PTG,	PTG2);
	IO_REGISTER_OUT(IO_OUT_LVDS_CRQ,		LOW_LEVEL,		PTD,	PTD6);
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
	IO_REGISTER_OUT(IO_OUT_CTP_VDD,	        LOW_LEVEL,		PTD,	PTD1);
	IO_REGISTER_OUT(IO_OUT_CTP_RST,	        LOW_LEVEL,		PTE,	PTE4);
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
	IO_REGISTER_OUT(IO_OUT_3V3_SW_EN,		LOW_LEVEL,		PTE,	PTE4);
	IO_REGISTER_OUT(IO_OUT_DAC_ONOFF,		LOW_LEVEL,		PTD,	PTD1);
#endif	
}


