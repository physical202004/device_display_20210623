/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : diagnose module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/09/18
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2020/09/18  Huang Haoxiong      Initial
*************************************************************************************************************/

/****************IFC name,						,ID *******************/
BEGIN_DIAGSERV
DIAGSERV_DECS(IFC_LOCK_SIGNAL,					0x00u)
//DIAGSERV_DECS(IFC_BL_FAULT,						0x01u)
DIAGSERV_DECS(IFC_FOG_FAULT,					0x02u)
//DIAGSERV_DECS(IFC_BL_ON_OFF,					0x04u)
//DIAGSERV_DECS(IFC_BL_PWM,					    0x05u)
DIAGSERV_DECS(IFC_BL_NTC_OPEN_CIRCUIT,			0x06u)
DIAGSERV_DECS(IFC_BL_NTC_SHORT_CIRCUIT, 		0x07u)
//DIAGSERV_DECS(IFC_BAT_OVER_VOLTAGE,    		0x08u)
//DIAGSERV_DECS(IFC_BAT_UNDER_VOLTAGE,			0x09u)
DIAGSERV_DECS(IFC_3V3_SW,		                0x0Eu)
DIAGSERV_DECS(IFC_LVDS_1V2,						0x0Fu)
//DIAGSERV_DECS(IFC_LVDS_VDD,					0x11u)
//DIAGSERV_DECS(IFC_BL_VOL,					    0x14u)
DIAGSERV_DECS(IFC_CTP_VDD,					    0x16u)
DIAGSERV_DECS(IFC_TFT_VDD, 				    	0x18u)
DIAGSERV_DECS(IFC_TFT_VGH, 				    	0x1Au)
//DIAGSERV_DECS(IFC_CTP_AVDD, 				    0x1Cu)
//DIAGSERV_DECS(IFC_TFT_VSP,					0x4Fu)
DIAGSERV_DECS(IFC_TFT_VGL, 						0x50u)
//DIAGSERV_DECS(IFC_LVDS_VDDIO, 				0x6Cu)
//DIAGSERV_DECS(IFC_BL_CONFIG_FAILED,			0x1Du)
DIAGSERV_DECS(IFC_BL_FAULT_VIN_OVP,				0x1Eu)
DIAGSERV_DECS(IFC_BL_FAULT_VIN_UVLO,			0x1Fu)
DIAGSERV_DECS(IFC_BL_FAULT_TSD_REASON,			0x20u)
DIAGSERV_DECS(IFC_BL_FAULT_BOOST_OCP, 			0x21u)
DIAGSERV_DECS(IFC_BL_FAULT_BOOST_OVP, 			0x22u)
DIAGSERV_DECS(IFC_BL_FAULT_PL_FET,				0x23u)
DIAGSERV_DECS(IFC_BL_FAULT_CP_2X, 				0x24u)
DIAGSERV_DECS(IFC_BL_FAULT_OPEN_LED1, 			0x25u)
DIAGSERV_DECS(IFC_BL_FAULT_OPEN_LED2, 			0x26u)
DIAGSERV_DECS(IFC_BL_FAULT_OPEN_LED3, 			0x27u)
DIAGSERV_DECS(IFC_BL_FAULT_OPEN_LED4, 			0x28u)
DIAGSERV_DECS(IFC_BL_FAULT_SHORT_LED1,			0x29u)
DIAGSERV_DECS(IFC_BL_FAULT_SHORT_LED2,			0x2Au)
DIAGSERV_DECS(IFC_BL_FAULT_SHORT_LED3,			0x2Bu)
DIAGSERV_DECS(IFC_BL_FAULT_SHORT_LED4,			0x2Cu)
//DIAGSERV_DECS(IFC_BL_FAULT_NONE,				0x2Du)


END_DIAGSERV


#undef BEGIN_DIAGSERV
#undef DIAGSERV_DECS
#undef END_DIAGSERV

