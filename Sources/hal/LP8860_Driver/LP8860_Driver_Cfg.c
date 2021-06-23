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
#include "LP8860_Driver.h"
#ifdef USE_LP8860_DRIVER
static S_SEQUENCE_LOG ssLP8860PowerSeqLog = {0};
static S_SEQUENCE_PROPERTIME ssLP8860PowerOnSeqTime[LP8860_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERTIME ssLP8860PowerOffSeqTime[LP8860_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERACTION ssLP8860PowerOnSeqAction[LP8860_CTRL_ACT_NUMBER] = {{0}};
static S_SEQUENCE_PROPERACTION ssLP8860PowerOffSeqAction[LP8860_CTRL_ACT_NUMBER] = {{0}};

static MYBOOL LP8860_Driver_SetOnOffPin(void);
static MYBOOL LP8860_Driver_ResetOnOffPin(void);
static MYBOOL LP8860_Driver_SetVddPin(void);
static MYBOOL LP8860_Driver_ResetVddPin(void);
static MYBOOL LP8860_Driver_SetEnablePin(void);
static MYBOOL LP8860_Driver_ResetEnablePin(void);
static MYBOOL LP8860_Driver_SetNssPin(void);
static MYBOOL LP8860_Driver_ResetNssPin(void);


static const S_DEV_POWER_CTRL ssLP8860PowerCtrl = {
    .powerOnFunc        = LP8860_Driver_ExecutePowerOnSequence,
    .shutDownFunc       = LP8860_Driver_ExecutePowerOffSequence,
    .lowPowerFunc       = NULL,
};

S_DEV_INFO gsLP8860Dev = {
    .name               = LP8860_DRIVER_NAME,
	.id                 = LP8860_DRIVER_ID,
	.faultState			= E_DEVCTRL_FAULT_STATE_NONE,
    .devState			= E_DEVCTRL_STATE_CLOSE,
    .powerReq			= E_DEVCTRL_POWER_REQ_NONE,
    .powerCtrl          = &ssLP8860PowerCtrl,
    .running10msTask    = LP8860_Driver_CycleTask10ms,
    .next               = NULL,
};


/***********************************************************************
* Name          : LP8860_Driver_PowerSequenceInit
* Description 	: LP8860 power sequence initial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void LP8860_Driver_PowerSequenceInit(void)
{
    INIT_SEQUENCE_LOG(ssLP8860PowerSeqLog);

    /*Add Control Action*/
	ADD_CONTROL_ACTION(LP8860_ONOFF,		ssLP8860PowerOnSeqTime,			ssLP8860PowerOnSeqAction,		1,	LP8860_Driver_SetOnOffPin,			1);
	ADD_CONTROL_ACTION(LP8860_VDD,			ssLP8860PowerOnSeqTime,			ssLP8860PowerOnSeqAction,		1,	LP8860_Driver_SetVddPin,			2);
    ADD_CONTROL_ACTION(LP8860_ENABLE,		ssLP8860PowerOnSeqTime,			ssLP8860PowerOnSeqAction,		1,	LP8860_Driver_SetEnablePin,			3);
	ADD_CONTROL_ACTION(LP8860_NSS,			ssLP8860PowerOnSeqTime,			ssLP8860PowerOnSeqAction,		0,	LP8860_Driver_SetNssPin,			4);
	ADD_CONTROL_ACTION(LP8860_CONFIG,		ssLP8860PowerOnSeqTime,			ssLP8860PowerOnSeqAction,		0,	LP8860_Driver_InitData,				5);
	
	ADD_CONTROL_ACTION(LP8860_CONFIG,		ssLP8860PowerOffSeqTime,		ssLP8860PowerOffSeqAction,		0,	LP8860_Driver_ClearData,			1);
    ADD_CONTROL_ACTION(LP8860_ENABLE,		ssLP8860PowerOffSeqTime,		ssLP8860PowerOffSeqAction, 		0, 	LP8860_Driver_ResetEnablePin, 		2);
	ADD_CONTROL_ACTION(LP8860_NSS,			ssLP8860PowerOffSeqTime,		ssLP8860PowerOffSeqAction,		1,	LP8860_Driver_ResetNssPin,			3);
	ADD_CONTROL_ACTION(LP8860_VDD,			ssLP8860PowerOffSeqTime,		ssLP8860PowerOffSeqAction,		1,	LP8860_Driver_ResetVddPin,			4);
	ADD_CONTROL_ACTION(LP8860_ONOFF,		ssLP8860PowerOffSeqTime,		ssLP8860PowerOffSeqAction,		10,	LP8860_Driver_ResetOnOffPin,		5);

}


/***********************************************************************
* Name          : LP8860_Driver_ExecutePowerOnSequence
* Description 	: Execute LP8860 power up sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL LP8860_Driver_ExecutePowerOnSequence(void)
{
    return SequenceHandler(&ssLP8860PowerSeqLog,ssLP8860PowerOnSeqTime,ssLP8860PowerOnSeqAction,EXE_POWER_ON,LP8860_CTRL_ACT_NUMBER);
}


/***********************************************************************
* Name          : LP8860_Driver_ExecutePowerOffSequence
* Description 	: execute LP8860 power down sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL LP8860_Driver_ExecutePowerOffSequence(void)
{
    return SequenceHandler(&ssLP8860PowerSeqLog,ssLP8860PowerOffSeqTime,ssLP8860PowerOffSeqAction,EXE_POWER_OFF,LP8860_CTRL_ACT_NUMBER);
}

/***********************************************************************
* Name          : LP8860_Driver_SetOnOffPin
* Description 	: Set chip on/off pin to high level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_SetOnOffPin(void)
{

    DEV_IO_WRITE(IO_OUT_BL_ONOFF,HIGH_LEVEL);

	return (TRUE);
}

/***********************************************************************
* Name          : LP8860_Driver_ResetOnOffPin
* Description 	: Set chip on/off pin to low level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_ResetOnOffPin(void)
{

    DEV_IO_WRITE(IO_OUT_BL_ONOFF,LOW_LEVEL);

	return (TRUE);
}

/***********************************************************************
* Name          : LP8860_Driver_SetVddPin
* Description 	: Set chip Vdd pin to high level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_SetVddPin(void)
{

    DEV_IO_WRITE(IO_OUT_BL_VDD,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name          : LP8860_Driver_ResetVddPin
* Description 	: Set chip Vdd pin to low level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_ResetVddPin(void)
{
    DEV_IO_WRITE(IO_OUT_BL_VDD,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name          : LP8860_Driver_SetEnablePin
* Description 	: Set chip enable pin to high level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_SetEnablePin(void)
{

    DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name          : LP8860_Driver_ResetEnablePin
* Description 	: Set chip enable pin to low level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_ResetEnablePin(void)
{
    DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name          : LP8860_Driver_SetNssPin
* Description 	: Set chip NSS pin to high level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_SetNssPin(void)
{

    DEV_IO_WRITE(IO_OUT_BL_NSS,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name          : LP8860_Driver_ResetNssPin
* Description 	: Set chip NSS pin to low level
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static MYBOOL LP8860_Driver_ResetNssPin(void)
{
    DEV_IO_WRITE(IO_OUT_BL_NSS,LOW_LEVEL);
	return TRUE;
}


#endif


