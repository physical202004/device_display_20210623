/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           *
*				  All Rights Reserved.             *
*   Department : ADS R&D SW		                   *
*   AUTHOR	   : Huang Haoxiong                    *
************************************************************************
* Object        :
* Module        : Backlight
* Instance      :
* Description   : Backlight Modules
*-----------------------------------------------------------------------
* Version       : 0.1
* Date          : 2018/08/22
* Author        : Huang Haoxiong
***********************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1    2018/08/22  Huang Haoxiong      Init
************************************************************************/
#ifndef LP8860_DRIVER_H
#define LP8860_DRIVER_H

#include "LP8860_Driver_if.h"


#ifdef USE_LP8860_DRIVER
#include "DevCtrl_if.h"
#include "Spi_Adapt_if.h"

#define LP8860_CTRL_PASS_1
#include "LP8860_Driver_Def.h"



#define LP8860_COMM_CHANNEL					(SPI_CH0)
#define LP8860_COMM_BAUDRATE				(1000000U)			//baudrate
#define LP8860_COMM_TIMEOUT					(1000U)				//comunication timeout value,unit by 1us

#define LP8860_EEP_CONFIG_OK                (0xA5u)
#define LP8860_EEP_CONFIG_INVALID           (0xFFu)
#define LP8860_EEP_REG_MAP_MAX        		ARRAY_LENGTH(gcaubEepromMapData)

#define LP8860_BRIGHTNESS_MAX               (0xFFFFu)
#define LP8860_LEFT_SHIFT_MAX               (16u)

#define LP8860_LED_FAULT_OPEN               (0x20u)
#define LP8860_LED_FAULT_SHORT              (0x10u)
#define LP8860_LED_FAULT_RESTART_MARK       (0x0Fu)


#define LP8860_FAULT_CIRCLE_TIME           	SYS_CYCLE_1S_TIME   //1s
#define LP8860_LOCK_EEPROM_TIME             (25u)            	//delay time for lock eeprom,unit by 10ms
#define LP8860_CHECK_EEPROM_UNLOCK_TIME     (5u)            	//delay time for check unlock eeprom,unit by 10ms
#define LP8860_FAULT_FILTER_TIME            (20U)               //200Ms
#define LP8860_RESTART_FAULT_FILTER_TIME    (SYS_CYCLE_500MS_TIME - LP8860_FAULT_FILTER_TIME)   //300Ms


/*restart enable pin,restart time least at 10ms
first cycle lock eeprom,
second cycle pull down enable pin,
third cycle pull up enable pin*/
#define LP8860_EEPROM_RESTART_EN_TIME       (3u)  				

/* operation steps */
typedef enum {
	E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK,
    E_LP8860_CFG_STEP1_UPDATE_EEPROM,
    E_LP8860_CFG_STEP2_FLASH_EEPROM,
    E_LP8860_CFG_STEP3_LOCK_EEPROM,
    E_LP8860_CFG_STEP4_FINISHED,
    E_LP8860_CFG_STEP5_POWEROFF,
    E_LP8860_CFG_STEP6_POWERON,
}E_LP8860_CONFIG;

/* register addr */
enum
{
    E_LP8860_ADDR_SET_BRIGHTNESS = 0,
    E_LP8860_ADDR_PIN_FAULT = 0x0F,
    E_LP8860_ADDR_LED_FAULT,
    E_LP8860_ADDR_CLEAR_FAULT,
    E_LP8860_ADDR_EEP_CTRL = 0x19,
    E_LP8860_ADDR_EEP_UNLOCK,
    E_LP8860_ADDR_EEP_REG_0 = 0x60,
};

extern S_DEV_INFO gsLP8860Dev;

extern MYBOOL LP8860_Driver_ExecutePowerOnSequence(void);
extern MYBOOL LP8860_Driver_ExecutePowerOffSequence(void);
extern void LP8860_Driver_PowerSequenceInit(void);
extern MYBOOL LP8860_Driver_InitData(void);
extern MYBOOL LP8860_Driver_ClearData(void);
extern void LP8860_Driver_CycleTask10ms(void);
#endif  //USE_LP8860_DRIVER


#endif


