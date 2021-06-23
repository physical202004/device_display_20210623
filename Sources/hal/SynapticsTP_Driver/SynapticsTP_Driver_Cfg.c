/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Synaptics TP Driver
* Instance      :
* Description   : Direct reading
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/08/22
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2020/08/22  Huang Haoxiong      Initial
*************************************************************************************************************/



#include "SynapticsTP_Driver.h"
#ifdef USE_SYNAPTICS_TP_DRIVER

static S_SEQUENCE_LOG ssTPPowerSeqLog = {0};
static S_SEQUENCE_PROPERTIME ssTPPowerOnSeqTime[TP_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERTIME ssTPPowerOffSeqTime[TP_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERACTION ssTPPowerOnSeqAction[TP_CTRL_ACT_NUMBER] = {{0}};
static S_SEQUENCE_PROPERACTION ssTPPowerOffSeqAction[TP_CTRL_ACT_NUMBER] = {{0}};

static MYBOOL TP_Driver_SetVddPin(void);
static MYBOOL TP_Driver_ResetVddPin(void);
//static MYBOOL TP_Driver_SetRstPin(void);
static MYBOOL TP_Driver_ResetRstPin(void);

	
static const S_DEV_POWER_CTRL ssTPPowerCtrl = {
	.powerOnFunc		= TP_Driver_ExecutePowerOnSequence,
	.shutDownFunc		= TP_Driver_ExecutePowerOffSequence,
	.lowPowerFunc		= NULL,
};

S_DEV_INFO gsSynapticsTPDev = {
	.name				= SYNAPTICS_TP_DRIVER_NAME,
	.id 				= SYNAPTICS_TP_DRIVER_ID,
	.faultState 		= E_DEVCTRL_FAULT_STATE_NONE,
	.devState			= E_DEVCTRL_STATE_CLOSE,
	.powerReq			= E_DEVCTRL_POWER_REQ_NONE,
	.powerCtrl			= &ssTPPowerCtrl,
	.running10msTask	= TP_Driver_CycleTask10ms,
	.next				= NULL,
};


/***********************************************************************
* Name          : TP_Driver_PowerSequenceInit
* Description 	: TP power sequence initial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void TP_Driver_PowerSequenceInit(void)
{
	INIT_SEQUENCE_LOG(ssTPPowerSeqLog);

    /*Add Control Action*/
    ADD_CONTROL_ACTION(TP_VDD,			ssTPPowerOnSeqTime,			ssTPPowerOnSeqAction,		1,	TP_Driver_SetVddPin,				1);
	//ADD_CONTROL_ACTION(TP_RST,			ssTPPowerOnSeqTime,			ssTPPowerOnSeqAction,		0,	TP_Driver_SetRstPin,				2);
	ADD_CONTROL_ACTION(TP_CONFIG,		ssTPPowerOnSeqTime,			ssTPPowerOnSeqAction,		0,	TP_Driver_DataInit,				    2);


	ADD_CONTROL_ACTION(TP_RST,	  		ssTPPowerOffSeqTime,		ssTPPowerOffSeqAction,		1, 	TP_Driver_ResetRstPin,	  			1);
	ADD_CONTROL_ACTION(TP_VDD,			ssTPPowerOffSeqTime,		ssTPPowerOffSeqAction,		1,	TP_Driver_ResetVddPin,				2);

}


/***********************************************************************
* Name          : TP_Driver_ExecutePowerOnSequence
* Description 	: Execute TP power up sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL TP_Driver_ExecutePowerOnSequence(void)
{
	return SequenceHandler(&ssTPPowerSeqLog,ssTPPowerOnSeqTime,ssTPPowerOnSeqAction,EXE_POWER_ON,TP_CTRL_ACT_NUMBER);
}


/***********************************************************************
* Name          : TP_Driver_ExecutePowerOffSequence
* Description 	: execute TP power down sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL TP_Driver_ExecutePowerOffSequence(void)
{
	MYBOOL lmbRet = FALSE;
	MYBOOL lmbAdcDetectState = FALSE;
	CS_SIG_SyncReceive(SIG_BOOL_POWER_ADC_STATE,&lmbAdcDetectState);//receive adc interrupt detect state
	if(lmbAdcDetectState == FALSE)
	{
		lmbRet = TRUE;
	}
	else
	{
		lmbRet = SequenceHandler(&ssTPPowerSeqLog,ssTPPowerOffSeqTime,ssTPPowerOffSeqAction,EXE_POWER_OFF,TP_CTRL_ACT_NUMBER);
	}
    return lmbRet;
}

/***********************************************************************
* Name        	: TP_Driver_SetVddPin
* Description	: Set vdd pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL TP_Driver_SetVddPin(void)
{
	DEV_IO_WRITE(IO_OUT_CTP_VDD,HIGH_LEVEL);
	return TRUE;
}
/***********************************************************************
* Name        	: TP_Driver_ResetVddPin
* Description	: Set vdd pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL TP_Driver_ResetVddPin(void)
{
	DEV_IO_WRITE(IO_OUT_CTP_VDD,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: TP_Driver_SetRstPin
* Description	: Set reset pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
/*static MYBOOL TP_Driver_SetRstPin(void)
{
	DEV_IO_WRITE(IO_OUT_CTP_RST,HIGH_LEVEL);
	return TRUE;
}*/

/***********************************************************************
* Name        	: TP_Driver_ResetRstPin
* Description	: Set reset pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL TP_Driver_ResetRstPin(void)
{
	DEV_IO_WRITE(IO_OUT_CTP_RST,LOW_LEVEL);
	return TRUE;
}

#endif



