/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    Jun 26,2016    PEI Jingli  Initial version
* ----------------------------------------------------------------------
* 0.2    Aug 01,2016   PEI Jingli  Add calling of IO/AD signal
* ----------------------------------------------------------------------
* 0.3    Dec 20,2016   ShangQiuju remove unused include files.
* ----------------------------------------------------------------------
* 0.4    Jan 05,2017   ShangQiuju  update Sig_CombineGetDirectValue,Sig_CombineSetDirectValue
                                                   in sync function.
*------------------------------------------------------------------------
* 0.5    Jan 24,2017  ZhangJie  Fix Coverity warnings.
*------------------------------------------------------------------------
* 0.6	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*------------------------------------------------------------------------
* 0.7	Mar,18,2019		Huang,Haoxiong	1.Removal of hardware-related functions
*=====================================================================*/

#include "Signal_if.h"
#include "Global_Definition.h"

void CS_SIG_RegisterSignal(void)
{

/****************************************** register bool type ******************************************/
	SIG_REGISTER_MYBOOL_TYPE(SIG_BOOL_WAKEUP_IS_ACTIVE,				TRUE);//waleup status,This is not necessarily a hard-wired signal, it may be an instruction in a communication frame.
	SIG_REGISTER_MYBOOL_TYPE(SIG_BOOL_IS_EXEC_AGING,				FALSE);//Check execute aging state.TRUE:execute aging,FALSE:do nothing
	SIG_REGISTER_MYBOOL_TYPE(SIG_BOOL_COMM_CAN_OPEN,				FALSE);//communication on/off control
	SIG_REGISTER_MYBOOL_TYPE(SIG_BOOL_POWER_ADC_STATE,				FALSE);//detect dc/dc power adc state,abnormal set TRUE
	
/****************************************** register unsigned character type ******************************************/
	SIG_REGISTER_CHAR_TYPE(SIG_CHAR_SYSTEM_MODE_CHANGE,				SYS_CMD_REQUEST_TO_APP);//system request from HU
	SIG_REGISTER_CHAR_TYPE(SIG_CHAR_NTC_TEMPERATURE,				0);//temperature for ntc
	SIG_REGISTER_CHAR_TYPE(SIG_CHAR_BRIGHTNESS_RATIO,				90U);//Brightness Ratio

/****************************************** register unsigned short type ******************************************/	
	SIG_REGISTER_SHORT_TYPE(SIG_SHORT_CURR_BRN_VAL, 				0U);//current brightness value
	SIG_REGISTER_SHORT_TYPE(SIG_SHORT_COMM_REQ_BRN_VAL,				0U);//request brightnes from HU in EOL or APP mode
	SIG_REGISTER_SHORT_TYPE(SIG_SHORT_PWM_FROM_HU,					0U);//pwm from HU
/****************************************** register float type ******************************************/
	SIG_REGISTER_FLOAT_TYPE(SIG_FLOAT_TEST_VAL,             		0.00f);//test value

/****************************************** register enumeration type ******************************************/
	SIG_REGISTER_ENUM_TYPE(SIG_ENUM_SYSTEM_STATE,					E_SYS_STATE_STANDBY);//system state
    SIG_REGISTER_ENUM_TYPE(SIG_ENUM_BATTERY_STATE,					E_BAT_STATE_LOW);//battery state
	SIG_REGISTER_ENUM_TYPE(SIG_ENUM_TEMPERATURE_STATE,				E_TEMPERATURE_STATE_NORMAL);//temperature for NTC
   
/****************************************** register unsigned integer type ******************************************/
	SIG_REGISTER_INTEGER_TYPE(SIG_INTEGER_TEST_VAL,        			0UL);//test value


}

/*---Global-----------------------------------------------------------*/

const V_syncreceive SignalRevProc[E_SIG_TYPE_NUM]=
{
    Sig_BoolGetValue,
	Sig_CharGetValue,
	Sig_ShortGetValue,
	Sig_FloatGetValue,
	Sig_EnumGetValue,
	Sig_IntegerGetValue,
};

const V_syncsend  SignalSndProc[E_SIG_TYPE_NUM]=
{
    Sig_BoolSetValue,
	Sig_CharSetValue,
	Sig_ShortSetValue,
	Sig_FloatSetValue,
	Sig_EnumSetValue,
    Sig_IntegerSetValue,
};
/**************** END OF FILE *****************************************/

