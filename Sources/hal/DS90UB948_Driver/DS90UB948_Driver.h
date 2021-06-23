/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Deserializer
* Instance      :
* Description   : 
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/27
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2019/09/27  Huang Haoxiong      Initial
*************************************************************************************************************/

#ifndef DS90UB948_DRIVER_H
#define DS90UB948_DRIVER_H

#include "DS90UB948_Driver_if.h"


#ifdef USE_DS90UB948_DRIVER
#include "DevCtrl_if.h"
#include "I2c_Adapt_if.h"
#define DS90UB948_CTRL_PASS_1
#include "DS90UB948_Driver_Def.h"

//#define DS90UB948_USE_REGISTER

#define DS90UB948_IIC_ADDR				(0x2C)	//I2C address,7bit
#define DS90UB948_COMM_TIMEOUT			(50)	//timeout,unit by 10us
#define DS90UB948_PACKAGE_MAX			(3U)	//timeout,unit by 10us

#define DS90UB948_1V2_AD_MIN		    (1415U)   //1V2 ADC min value
#define DS90UB948_1V2_AD_MAX		    (1564U)   //1V2 ADC max value
#define DS90UB948_VDDIO_AD_MIN		    (1946U)   //VDDIO ADC min value
#define DS90UB948_VDDIO_AD_MAX		    (2150U)   //VDDIO ADC max value
#define DS90UB948_VDD_AD_MIN		    (1946U)   //VDD ADC min value
#define DS90UB948_VDD_AD_MAX		    (2150U)   //VDD ADC max value

typedef enum 
{
	E_DS90UB948_REG_ADDR_I2C_DEVICE_ID = 0,
	E_DS90UB948_REG_ADDR_RESET,
	E_DS90UB948_REG_ADDR_GENERAL_CFG,
	E_DS90UB948_REG_ADDR_GENERAL_CFG1,
	E_DS90UB948_REG_ADDR_BBC_WDG_CTRL,
	E_DS90UB948_REG_ADDR_I2C_CTRL1,
	E_DS90UB948_REG_ADDR_I2C_CTRL2,
	E_DS90UB948_REG_ADDR_REMOTE_ID,
	E_DS90UB948_REG_ADDR_SLAVEI_D0,
	E_DS90UB948_REG_ADDR_SLAVE_ID1,
	E_DS90UB948_REG_ADDR_SLVAE_ID2,
	E_DS90UB948_REG_ADDR_SLAVE_ID3,
	E_DS90UB948_REG_ADDR_SLAVE_ID4,
	E_DS90UB948_REG_ADDR_SLAVE_ID5,
	E_DS90UB948_REG_ADDR_SLAVE_ID6,
	E_DS90UB948_REG_ADDR_SLAVE_ID7,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS0,//0x10
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS1,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS2,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS3,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS4,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS5,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS6,
	E_DS90UB948_REG_ADDR_SLAVE_ALIAS7,
	E_DS90UB948_REG_ADDR_MAILBOX18,
	E_DS90UB948_REG_ADDR_MAILBOX19,
	E_DS90UB948_REG_ADDR_GPIO9_CFG,
	E_DS90UB948_REG_ADDR_FREQ_CNT,
	E_DS90UB948_REG_ADDR_GENERAL_STATUS,
	E_DS90UB948_REG_ADDR_GPIO0_CFG,
	E_DS90UB948_REG_ADDR_GPIO1_2_CFG,
	E_DS90UB948_REG_ADDR_GPIO3_CFG,
	E_DS90UB948_REG_ADDR_GPIO5_6_CFG,//0x20
	E_DS90UB948_REG_ADDR_GPIO7_8_CFG,
	E_DS90UB948_REG_ADDR_DATAPATH_CTRL,
	E_DS90UB948_REG_ADDR_RX_MODE_STATUS,
	E_DS90UB948_REG_ADDR_BIST_CTRL,
	E_DS90UB948_REG_ADDR_BIST_ERR_CNT,
	E_DS90UB948_REG_ADDR_SCL_HIGH_TIME,
	E_DS90UB948_REG_ADDR_SCL_LOW_TIME,
	E_DS90UB948_REG_ADDR_DATAPATH_CTRL2,
	E_DS90UB948_REG_ADDR_FRC_CTRL,
	E_DS90UB948_REG_ADDR_WHITE_BALANCE_CTRL,
	E_DS90UB948_REG_ADDR_I2S_CTRL,//0x2B
	E_DS90UB948_REG_ADDR_PCLK_TEST_MODE = 0x2E,
	E_DS90UB948_REG_ADDR_DUAL_RX_CTL = 0x34,
	E_DS90UB948_REG_ADDR_AEQ_TEST,
	E_DS90UB948_REG_ADDR_MODE_SEL = 0x37,
	E_DS90UB948_REG_ADDR_I2S_DIVSEL = 0x3A,
	E_DS90UB948_REG_ADDR_LINK_ERR_CNT = 0x41,
	E_DS90UB948_REG_ADDR_HSCC_CTRL = 0x43,
	E_DS90UB948_REG_ADDR_ADAPTIVE_EQ_BYPASS,
	E_DS90UB948_REG_ADDR_ADAPTIVE_EQ_MIN_MAX,
	E_DS90UB948_REG_ADDR_FPD_TX_MODE = 0x49,
	E_DS90UB948_REG_ADDR_LVDS_CTRL = 0x4B,
	E_DS90UB948_REG_ADDR_CML_OUTPUT_CTL1 = 0x52,
	E_DS90UB948_REG_ADDR_CML_OUTPUT_ENABLE = 0x56,
	E_DS90UB948_REG_ADDR_CML_OUTPUT_CTL2,
	E_DS90UB948_REG_ADDR_PGCTL = 0x64,
	E_DS90UB948_REG_ADDR_PGCFG,
	E_DS90UB948_REG_ADDR_PGIA,
	E_DS90UB948_REG_ADDR_PGID,
	E_DS90UB948_REG_ADDR_PGDBG,
	E_DS90UB948_REG_ADDR_PGTSTDAT,
	E_DS90UB948_REG_ADDR_GPI_PIN_STATUS1 = 0x6E,
	E_DS90UB948_REG_ADDR_GPI_PIN_STATUS2,
	E_DS90UB948_REG_ADDR_RX_ID = 0xF0,//6 byte
}E_DS90UB948_REG_ADDR;//address table

extern void DS90UB948_Driver_PowerSequenceInit(void);
extern MYBOOL DS90UB948_Driver_ExecutePowerOnSequence(void);
extern MYBOOL DS90UB948_Driver_ExecutePowerOffSequence(void);

extern S_DEV_INFO gsDS90UB948Dev;
extern void DS90UB948_Driver_CycleTask10ms(void);
extern MYBOOL DS90UB948_Driver_DataInit(void);
extern MYBOOL DS90UB948_Driver_DataDeinit(void);

#endif  //USE_DS90UB948_DRIVER


#endif



