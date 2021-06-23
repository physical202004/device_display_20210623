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
 ----------------------------------------------------------------------
* 0.1    Jun 26,2016    PEI Jingli  Initial version
 ----------------------------------------------------------------------
* 0.2    Aug 01,2016    PEI Jingli  add signal
 ----------------------------------------------------------------------
* 0.3    Jan 05,2017    ShangQiuju Remove unused include file.
*------------------------------------------------------------------------
* 0.4    Jan 24,2017  ZhangJie  Fix Coverity warnings.
*------------------------------------------------------------------------
* 0.5	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*------------------------------------------------------------------------
* 0.6	Mar,18,2019		Huang,Haoxiong	1.Removal of hardware-related functions

*=====================================================================*/
/**** Declaration of variables ****/

BEGIN_SIG
/****************************************** register bool type ******************************************/
BEGIN_SIG_MYBOOL
SIGDESC(SIG_BOOL_WAKEUP_IS_ACTIVE)//it can't delect if there is not other signal, Otherwise,it will occur error
SIGDESC(SIG_BOOL_IS_EXEC_AGING)
SIGDESC(SIG_BOOL_COMM_CAN_OPEN)
SIGDESC(SIG_BOOL_POWER_ADC_STATE)
END_SIG_MYBOOL

/****************************************** register character type ******************************************/
BEGIN_SIG_CHAR
SIGDESC(SIG_CHAR_SYSTEM_MODE_CHANGE)//it can't delect if there is not other signal, Otherwise,it will occur error
SIGDESC(SIG_CHAR_NTC_TEMPERATURE)
SIGDESC(SIG_CHAR_BRIGHTNESS_RATIO)
END_SIG_CHAR

/****************************************** register short type ******************************************/
BEGIN_SIG_SHORT
SIGDESC(SIG_SHORT_CURR_BRN_VAL)
SIGDESC(SIG_SHORT_COMM_REQ_BRN_VAL)
SIGDESC(SIG_SHORT_PWM_FROM_HU)
END_SIG_SHORT

/****************************************** register float type ******************************************/
BEGIN_SIG_FLOAT
SIGDESC(SIG_FLOAT_TEST_VAL)//it can't delect if there is not other signal, Otherwise,it will occur error
END_SIG_FLOAT

/****************************************** register enumeration  type ******************************************/
BEGIN_SIG_ENUM
SIGDESC(SIG_ENUM_SYSTEM_STATE)//it can't delect if there is not other signal, Otherwise,it will occur error
SIGDESC(SIG_ENUM_BATTERY_STATE)
SIGDESC(SIG_ENUM_TEMPERATURE_STATE)
END_SIG_ENUM

/****************************************** register integer type ******************************************/
BEGIN_SIG_INTEGER
SIGDESC(SIG_INTEGER_TEST_VAL)//it can't delect if there is not other signal, Otherwise,it will occur error
END_SIG_INTEGER

END_SIG

#undef BEGIN_SIG
#undef END_SIG
#undef SIGDESC
#undef BEGIN_SIG_MYBOOL
#undef END_SIG_MYBOOL
#undef BEGIN_SIG_CHAR
#undef END_SIG_CHAR
#undef BEGIN_SIG_SHORT
#undef END_SIG_SHORT
#undef BEGIN_SIG_FLOAT
#undef END_SIG_FLOAT
#undef BEGIN_SIG_ENUM
#undef END_SIG_ENUM
#undef BEGIN_SIG_INTEGER
#undef END_SIG_INTEGER

