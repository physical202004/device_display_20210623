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
const unsigned char gcaubEepromMapData[] = {
	0xEE,//EEPROM_REG_0
	0x14,//EEPROM_REG_1,limit the max currrent to 88mA every LED
	0xD0,//EEPROM_REG_2,DC
	0xD0,//EEPROM_REG_3,LED string (1+2+3+4). Tied strings with same phase,100mA
	0xDF,//EEPROM_REG_4
	0xE5,//EEPROM_REG_5
	0xF2,//EEPROM_REG_6
	0x77,//EEPROM_REG_7
	0x77,//EEPROM_REG_8
	0x71,//EEPROM_REG_9
	0x3F,//EEPROM_REG_10
	0xB7,//EEPROM_REG_11
	0x17,//EEPROM_REG_12
	0xEF,//EEPROM_REG_13
	0xB0,//EEPROM_REG_14
	0x84,//EEPROM_REG_15
	0xC5,//EEPROM_REG_16,Max Current Limit = 4A
	0xF3,//EEPROM_REG_17,400Khz
	0xE5,//EEPROM_REG_18
	0x28,//EEPROM_REG_19,Max voltage output = 36V
	0x35,//EEPROM_REG_20
	0x06,//EEPROM_REG_21
	0xDF,//EEPROM_REG_22
	0xFF,//EEPROM_REG_23
	0x3E,//EEPROM_REG_24
};

