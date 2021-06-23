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




#include "DS90UB948_Driver.h"
#ifdef USE_DS90UB948_DRIVER

static S_SEQUENCE_LOG ssDS90UB948PowerSeqLog = {0};
static S_SEQUENCE_PROPERTIME ssDS90UB948PowerOnSeqTime[DS90UB948_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERTIME ssDS90UB948PowerOffSeqTime[DS90UB948_CTRL_ACT_NUMBER] = {0};
static S_SEQUENCE_PROPERACTION ssDS90UB948PowerOnSeqAction[DS90UB948_CTRL_ACT_NUMBER] = {{0}};
static S_SEQUENCE_PROPERACTION ssDS90UB948PowerOffSeqAction[DS90UB948_CTRL_ACT_NUMBER] = {{0}};

#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
static MYBOOL DS90UB948_Driver_SetDACOnOffPin(void);
static MYBOOL DS90UB948_Driver_ResetDACOnOffPin(void);
static MYBOOL DS90UB948_Driver_Set3V3SWPin(void);
static MYBOOL DS90UB948_Driver_Reset3V3SWPin(void);
#endif
static MYBOOL DS90UB948_Driver_SetBLFaultToHuPin(void);
static MYBOOL DS90UB948_Driver_ResetBLFaultToHuPin(void);
static MYBOOL DS90UB948_Driver_ResetTFTFaultToHuPin(void);
static MYBOOL DS90UB948_Driver_Set1V2Pin(void);
static MYBOOL DS90UB948_Driver_Reset1V2Pin(void);
static MYBOOL DS90UB948_Driver_SetPDBPin(void);
static MYBOOL DS90UB948_Driver_ResetPDBPin(void);

	
static const S_DEV_POWER_CTRL ssDS90UB948PowerCtrl = {
	.powerOnFunc		= DS90UB948_Driver_ExecutePowerOnSequence,
	.shutDownFunc		= DS90UB948_Driver_ExecutePowerOffSequence,
	.lowPowerFunc		= NULL,
};

S_DEV_INFO gsDS90UB948Dev = {
	.name				= DS90UB948_DRIVER_NAME,
	.id 				= DS90UB948_DRIVER_ID,
	.faultState 		= E_DEVCTRL_FAULT_STATE_NONE,
	.devState			= E_DEVCTRL_STATE_CLOSE,
	.powerReq			= E_DEVCTRL_POWER_REQ_NONE,
	.powerCtrl			= &ssDS90UB948PowerCtrl,
	.running10msTask	= DS90UB948_Driver_CycleTask10ms,
	.next				= NULL,
};


/***********************************************************************
* Name          : DS90UB948_Driver_PowerSequenceInit
* Description 	: DS90UB948 power sequence initial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void DS90UB948_Driver_PowerSequenceInit(void)
{
	INIT_SEQUENCE_LOG(ssDS90UB948PowerSeqLog);
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
	/*Add Control Action*/
	ADD_CONTROL_ACTION(DS90UB948_1V2,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_Set1V2Pin, 			1);
	ADD_CONTROL_ACTION(DS90UB948_BL_FAULT_TO_HU,	ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		0,	DS90UB948_Driver_SetBLFaultToHuPin, 	2);
	ADD_CONTROL_ACTION(DS90UB948_TFT_FAULT_TO_HU,	ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_ResetTFTFaultToHuPin,	3);
	ADD_CONTROL_ACTION(DS90UB948_PDB,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		0,	DS90UB948_Driver_SetPDBPin, 			4);
	ADD_CONTROL_ACTION(DS90UB948_CFG,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_DataInit,				5);

	ADD_CONTROL_ACTION(DS90UB948_CFG,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_DataDeinit,			1);
	ADD_CONTROL_ACTION(DS90UB948_PDB,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_ResetPDBPin,			2);
	ADD_CONTROL_ACTION(DS90UB948_TFT_FAULT_TO_HU,	ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		0,	DS90UB948_Driver_ResetTFTFaultToHuPin,	3);
	ADD_CONTROL_ACTION(DS90UB948_BL_FAULT_TO_HU,	ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_ResetBLFaultToHuPin,	4);
	ADD_CONTROL_ACTION(DS90UB948_1V2,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_Reset1V2Pin,			5);
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
	/*Add Control Action*/
	ADD_CONTROL_ACTION(MCU_3V3SW,					ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_Set3V3SWPin,			1);
    ADD_CONTROL_ACTION(DAC_ONOFF,					ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_SetDACOnOffPin,		2);
	ADD_CONTROL_ACTION(DS90UB948_1V2,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_Set1V2Pin, 			3);
	ADD_CONTROL_ACTION(DS90UB948_BL_FAULT_TO_HU,	ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		0,	DS90UB948_Driver_SetBLFaultToHuPin, 	4);
	ADD_CONTROL_ACTION(DS90UB948_TFT_FAULT_TO_HU,	ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_ResetTFTFaultToHuPin,	5);
	ADD_CONTROL_ACTION(DS90UB948_PDB,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		0,	DS90UB948_Driver_SetPDBPin, 			6);
	ADD_CONTROL_ACTION(DS90UB948_CFG,				ssDS90UB948PowerOnSeqTime,			ssDS90UB948PowerOnSeqAction,		1,	DS90UB948_Driver_DataInit,				7);

	ADD_CONTROL_ACTION(DS90UB948_CFG,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_DataDeinit,			1);
	ADD_CONTROL_ACTION(DS90UB948_PDB,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_ResetPDBPin,			2);
	ADD_CONTROL_ACTION(DS90UB948_TFT_FAULT_TO_HU,	ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		0,	DS90UB948_Driver_ResetTFTFaultToHuPin,	3);
	ADD_CONTROL_ACTION(DS90UB948_BL_FAULT_TO_HU,	ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_ResetBLFaultToHuPin,	4);
	ADD_CONTROL_ACTION(DS90UB948_1V2,				ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_Reset1V2Pin,			5);
	ADD_CONTROL_ACTION(DAC_ONOFF,					ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_ResetDACOnOffPin,		6);
	ADD_CONTROL_ACTION(MCU_3V3SW,					ssDS90UB948PowerOffSeqTime, 		ssDS90UB948PowerOffSeqAction,		1,	DS90UB948_Driver_Reset3V3SWPin,		    7);	
#endif
}


/***********************************************************************
* Name          : DS90UB948_Driver_ExecutePowerOnSequence
* Description 	: Execute DS90UB948 power up sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL DS90UB948_Driver_ExecutePowerOnSequence(void)
{
	return SequenceHandler(&ssDS90UB948PowerSeqLog,ssDS90UB948PowerOnSeqTime,ssDS90UB948PowerOnSeqAction,EXE_POWER_ON,DS90UB948_CTRL_ACT_NUMBER);

}


/***********************************************************************
* Name          : DS90UB948_Driver_ExecutePowerOffSequence
* Description 	: execute DS90UB948 power down sequence
* Parameter   	: None
* Returns     	: TRUE:execute finished,FALSE:executing
***********************************************************************/
MYBOOL DS90UB948_Driver_ExecutePowerOffSequence(void)
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
		lmbRet = SequenceHandler(&ssDS90UB948PowerSeqLog,ssDS90UB948PowerOffSeqTime,ssDS90UB948PowerOffSeqAction,EXE_POWER_OFF,DS90UB948_CTRL_ACT_NUMBER);
	}
    return lmbRet;
}

#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
/***********************************************************************
* Name        	: DS90UB948_Driver_SetDACOnOffPin
* Description	: Set DAC ONOFF pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_Set3V3SWPin(void)
{
	DEV_IO_WRITE(IO_OUT_3V3_SW_EN,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_ResetDACOnOffPin
* Description	: Set DAC ONOFF pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_Reset3V3SWPin(void)
{
	DEV_IO_WRITE(IO_OUT_3V3_SW_EN,LOW_LEVEL);
	return TRUE;
}
/***********************************************************************
* Name        	: DS90UB948_Driver_SetDACOnOffPin
* Description	: Set DAC ONOFF pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_SetDACOnOffPin(void)
{
	DEV_IO_WRITE(IO_OUT_DAC_ONOFF,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_ResetDACOnOffPin
* Description	: Set DAC ONOFF pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_ResetDACOnOffPin(void)
{
	DEV_IO_WRITE(IO_OUT_DAC_ONOFF,LOW_LEVEL);
	return TRUE;
}
#endif


/***********************************************************************
* Name        	: DS90UB948_Driver_SetBLFaultToHuPin
* Description	: Set IO_OUT_BL_FAULT_TO_HU pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_SetBLFaultToHuPin(void)
{
	DEV_IO_WRITE(IO_OUT_BL_FAULT_TO_HU,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_ResetDCDCPin
* Description	: Set IO_OUT_BL_FAULT_TO_HU pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_ResetBLFaultToHuPin(void)
{
	DEV_IO_WRITE(IO_OUT_BL_FAULT_TO_HU,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_ResetTFTFaultToHuPin
* Description	: Set IO_OUT_TFT_FAULT_TO_HU pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_ResetTFTFaultToHuPin(void)
{
	DEV_IO_WRITE(IO_OUT_TFT_FAULT_TO_HU,LOW_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_Set1V2Pin
* Description	: Set 1V2 pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_Set1V2Pin(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_1V2,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_Reset1V2Pin
* Description	: Set 1V2 pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_Reset1V2Pin(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_1V2,LOW_LEVEL);
	return TRUE;
}


/***********************************************************************
* Name        	: DS90UB948_Driver_SetPDBPin
* Description	: Set PDB pin to high level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_SetPDBPin(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_PDB,HIGH_LEVEL);
	return TRUE;
}

/***********************************************************************
* Name        	: DS90UB948_Driver_ResetPDBPin
* Description	: Set PDB pin to low level
* Description	: None
* Returns     	: None
***********************************************************************/
static MYBOOL DS90UB948_Driver_ResetPDBPin(void)
{
	DEV_IO_WRITE(IO_OUT_LVDS_PDB,LOW_LEVEL);
	return TRUE;
}

#endif



