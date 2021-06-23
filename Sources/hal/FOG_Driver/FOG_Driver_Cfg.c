/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : FOG
* Instance      :
* Description   : This driver is based on 10.25 inch FOG of ShenChao DE103IA-01A model
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/04
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2019/09/04  Huang Haoxiong      Initial
*************************************************************************************************************/



#include "FOG_Driver.h"
#ifdef USE_FOG_DRIVER

static S_SEQUENCE_LOG ssFOGPowerSeqLog = {0};
static S_SEQUENCE_PROPERTIME ssFOGPowerOnSeqTime[FOG_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERTIME ssFOGPowerOffSeqTime[FOG_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERACTION ssFOGPowerOnSeqAction[FOG_CTRL_ACT_NUMBER] = {{0}};
static S_SEQUENCE_PROPERACTION ssFOGPowerOffSeqAction[FOG_CTRL_ACT_NUMBER] = {{0}};

static MYBOOL FOG_Driver_SetVddPin(void);
static MYBOOL FOG_Driver_ResetVddPin(void);
static MYBOOL FOG_Driver_SetRstPin(void);
static MYBOOL FOG_Driver_ResetRstPin(void);
static MYBOOL FOG_Driver_SetStandbyPin(void);
static MYBOOL FOG_Driver_ResetStandbyPin(void);
static MYBOOL FOG_Driver_SetBiasPin(void);
static MYBOOL FOG_Driver_ResetBiasPin(void);
static MYBOOL FOG_Driver_SetVGHPin(void);
static MYBOOL FOG_Driver_ResetVGHPin(void);
static MYBOOL FOG_Driver_SetVGLPin(void);
static MYBOOL FOG_Driver_ResetVGLPin(void);
static MYBOOL FOG_Driver_OpenVcom(void);
static MYBOOL FOG_Driver_CloseVcom(void);
	
static const S_DEV_POWER_CTRL ssFOGPowerCtrl = {
	.powerOnFunc		= FOG_Driver_ExecutePowerOnSequence,
	.shutDownFunc		= FOG_Driver_ExecutePowerOffSequence,
	.lowPowerFunc		= NULL,
};

S_DEV_INFO gsFOGDev = {
	.name				= FOG_DRIVER_NAME,
	.id 				= FOG_DRIVER_ID,
	.faultState 		= E_DEVCTRL_FAULT_STATE_NONE,
	.devState			= E_DEVCTRL_STATE_CLOSE,
	.powerReq			= E_DEVCTRL_POWER_REQ_NONE,
	.powerCtrl			= &ssFOGPowerCtrl,
	.running10msTask	= FOG_Driver_CycleTask10ms,
	.next				= NULL,
};


/***********************************************************************
* Name          : FOG_Driver_PowerSequenceInit
* Description 	: FOG power sequence initial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void FOG_Driver_PowerSequenceInit(void)
{
	INIT_SEQUENCE_LOG(ssFOGPowerSeqLog);

    /*Add Control Action*/
    ADD_CONTROL_ACTION(FOG_VDD,			ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		0,	FOG_Driver_SetVddPin,				1);
	ADD_CONTROL_ACTION(FOG_BIAS,		ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		0,	FOG_Driver_SetBiasPin,				2);
	ADD_CONTROL_ACTION(FOG_VGL, 		ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		0,	FOG_Driver_SetVGLPin,				3);
	ADD_CONTROL_ACTION(FOG_VGH,			ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		1,	FOG_Driver_SetVGHPin,				4);
	ADD_CONTROL_ACTION(FOG_RST, 		ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		2,	FOG_Driver_SetRstPin,				5);
	ADD_CONTROL_ACTION(FOG_STBYB,		ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		1,	FOG_Driver_SetStandbyPin,			6);
	ADD_CONTROL_ACTION(FOG_VCOM,	  	ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		0,  FOG_Driver_OpenVcom, 	  			7);
	ADD_CONTROL_ACTION(FOG_CONFIG,	  	ssFOGPowerOnSeqTime,			ssFOGPowerOnSeqAction,		0,  FOG_Driver_DataInit, 	  			8);	
		

	ADD_CONTROL_ACTION(FOG_CONFIG,	  	ssFOGPowerOffSeqTime,			ssFOGPowerOffSeqAction,		0,  FOG_Driver_DataDeinit, 	  			1);
	ADD_CONTROL_ACTION(FOG_VCOM,	  	ssFOGPowerOffSeqTime,			ssFOGPowerOffSeqAction,		1, 	FOG_Driver_CloseVcom,	  			2);
	ADD_CONTROL_ACTION(FOG_STBYB,		ssFOGPowerOffSeqTime,			ssFOGPowerOffSeqAction,		1,	FOG_Driver_ResetStandbyPin,			3);
	ADD_CONTROL_ACTION(FOG_VGH,			ssFOGPowerOffSeqTime, 			ssFOGPowerOffSeqAction,		0,	FOG_Driver_ResetVGHPin,				4);
	ADD_CONTROL_ACTION(FOG_VGL,			ssFOGPowerOffSeqTime, 			ssFOGPowerOffSeqAction,		0,	FOG_Driver_ResetVGLPin, 			5);
	ADD_CONTROL_ACTION(FOG_BIAS,		ssFOGPowerOffSeqTime, 			ssFOGPowerOffSeqAction,		1,	FOG_Driver_ResetBiasPin,			6);
	ADD_CONTROL_ACTION(FOG_RST,			ssFOGPowerOffSeqTime, 			ssFOGPowerOffSeqAction,		1, 	FOG_Driver_ResetRstPin, 			7);
	ADD_CONTROL_ACTION(FOG_VDD,			ssFOGPowerOffSeqTime, 			ssFOGPowerOffSeqAction,		0,	FOG_Driver_ResetVddPin, 			8);

}


/***********************************************************************
* Name          : FOG_Driver_ExecutePowerOnSequence
* Description 	: Execute FOG power up sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL FOG_Driver_ExecutePowerOnSequence(void)
{
    return SequenceHandler(&ssFOGPowerSeqLog,ssFOGPowerOnSeqTime,ssFOGPowerOnSeqAction,EXE_POWER_ON,FOG_CTRL_ACT_NUMBER);
}


/***********************************************************************
* Name          : FOG_Driver_ExecutePowerOffSequence
* Description 	: execute FOG power down sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL FOG_Driver_ExecutePowerOffSequence(void)
{
    return SequenceHandler(&ssFOGPowerSeqLog,ssFOGPowerOffSeqTime,ssFOGPowerOffSeqAction,EXE_POWER_OFF,FOG_CTRL_ACT_NUMBER);
}

/***********************************************************************
* Name        	: FOG_Driver_SetVddPin
* Description	: Set vdd pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetVddPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VDD,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_ResetVddPin
* Description	: Set vdd pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetVddPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VDD,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_SetRstPin
* Description	: Set reset pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetRstPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_RST,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_ResetRstPin
* Description	: Set reset pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetRstPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_RST,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_SetStandbyPin
* Description	: Set standby pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetStandbyPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_STBYB,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_ResetStandbyPin
* Description	: Set standby pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetStandbyPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_STBYB,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_SetBiasPin
* Description	: Set bias pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetBiasPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_BIAS,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_ResetBiasPin
* Description	: Set bias pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetBiasPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_BIAS,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_SetVGHPin
* Description	: Set VGH pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetVGHPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VGH,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_Close_Vcom
* Description	: Set VGH pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetVGHPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VGH,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_SetVGLPin
* Description	: Set VGL pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_SetVGLPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VGL,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_ResetVGLPin
* Description	: Set VGL pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_ResetVGLPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_VGL,LOW_LEVEL);
	return TRUE;
}


/***********************************************************************
* Name        	: FOG_Driver_OpenVcom
* Description	: open vcom
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_OpenVcom(void)
{
	Pwm_Adapt_SetDuty(FOG_VCOM_TIMER_UNIT,FOG_VCOM_TIMER_CHANNEL,guhwVcomVal);
	return TRUE;
}

/***********************************************************************
* Name        	: FOG_Driver_CloseVcom
* Description	: close vcom
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL FOG_Driver_CloseVcom(void)
{

	Pwm_Adapt_SetDuty(FOG_VCOM_TIMER_UNIT,FOG_VCOM_TIMER_CHANNEL,0);
	return TRUE;
}

#endif



