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


static void TP_Driver_Init(void);
//static E_HAL_STATE TP_Driver_IoControl(U16 type, void* buffer);
static void TP_Driver_IFCRecordVddHandler(void);
//static void TP_Driver_IFCRecordAvddHandler(void);
static void TP_Driver_IFCRecordVolHandler(void);

static U8 subCheckTPvddCnt = 0;

static MYBOOL lmbHuRstLowFlag = FALSE;

bdev gsSynapticsTPDrv =
{
	.hal_block_Init 		= TP_Driver_Init,
	.hal_block_DeInit	 	= NULL,
	.hal_block_open			= NULL,
	.hal_block_close 		= NULL,
	.hal_block_read 		= NULL,
	.hal_block_write		= NULL,
	.hal_block_iocontrol 	= NULL,//TP_Driver_IoControl,
};

/***********************************************************************
* Name      	: TP_Driver_Init
* Description	: HAL Initial
* Parameter   	: None
* Returns     	: MYBOOL
***********************************************************************/
static void TP_Driver_Init(void)
{
	TP_Driver_PowerSequenceInit();
	DevCtrl_Register_Dev(&gsSynapticsTPDev);
}
#if 0
/***********************************************************************
* Name      	: TP_Driver_IoControl
* Description	: HAL IO Control
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static E_HAL_STATE TP_Driver_IoControl(U16 type, void* buffer)
{
	E_HAL_STATE leState = E_HAL_STATE_OK;
	switch(type)
	{
		case TP_FUNC_GET_FAULT_STATE:
			if(buffer == NULL)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				*(E_DEVCTRL_FAULT_STATE *)buffer = gsSynapticsTPDev.faultState;
			}
			break;
		default:
			leState = E_HAL_STATE_NO_FUNC;
			break;
	}
	
	return leState;
}
#endif

/***********************************************************************
* Name      	: TP_Driver_DataInit
* Description	: Data Initialization
* Parameter   	: None
* Returns     	: None
***********************************************************************/
MYBOOL TP_Driver_DataInit(void)
{
	lmbHuRstLowFlag = FALSE;

	return TRUE;
}

/***********************************************************************
*  Name        	: TP_Driver_IFCRecordVolHandler
*  Description 	: record TP abnormal voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void TP_Driver_IFCRecordVolHandler(void)
{
	if(subCheckTPvddCnt < DIAGSERV_IFC_TIME_500MS)
	{
		subCheckTPvddCnt++;
	}
	else
	{
		TP_Driver_IFCRecordVddHandler();
		//TP_Driver_IFCRecordAvddHandler();
	}
}

/***********************************************************************
*  Name        	: TP_Driver_IFCRecordVddHandler
*  Description 	: record TP abnormal vdd voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void TP_Driver_IFCRecordVddHandler(void)
{
	DEV_ADC_16BIT_T ltADCvdd = DEV_ADC_ASYNC_READ(ADC_CTP_VDD_DET);
	if((ltADCvdd > TP_VDD_AD_MIN) && (ltADCvdd < TP_VDD_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsSynapticsTPDev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		DiagServ_AddIfcInfo(IFC_CTP_VDD);  // record TP VDD abnormality
	}
}

/***********************************************************************
*  Name        	: TP_Driver_IFCRecordAvddHandler
*  Description 	: record TP abnormal avdd voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#if 0
static void TP_Driver_IFCRecordAvddHandler(void)
{
	DEV_ADC_16BIT_T ltADCavdd = DEV_ADC_ASYNC_READ(ADC_CTP_AVDD_DET);
	if((ltADCavdd > TP_AVDD_AD_MIN) && (ltADCavdd < TP_AVDD_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		DiagServ_AddIfcInfo(DIAG_CTP_AVDD); // record TP AVDD abnormality
	}
}
#endif
/***********************************************************************
* Name      	: TP_Driver_CycleTask10ms
* Description	: Cycle task entry
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void TP_Driver_CycleTask10ms(void)
{
	DEV_IO_8BIT_T l8tCtpRstFrHuLevel = LOW_LEVEL;
	l8tCtpRstFrHuLevel = DEV_IO_SYNC_READ(IO_IN_CTP_RST_FR_HU);//read TP reset level from HU
	if(l8tCtpRstFrHuLevel == LOW_LEVEL)
	{
		if(lmbHuRstLowFlag == FALSE)
		{
			lmbHuRstLowFlag = TRUE;
			DEV_IO_WRITE(IO_OUT_CTP_RST, LOW_LEVEL);
			DelayMs(1);
			DEV_IO_WRITE(IO_OUT_CTP_RST, HIGH_LEVEL);
		}
		else
		{
			/* do nothing... */
		}
	}
	else
	{
		lmbHuRstLowFlag = FALSE;
		DEV_IO_WRITE(IO_OUT_CTP_RST, l8tCtpRstFrHuLevel);
	}

	TP_Driver_IFCRecordVolHandler();
}


#endif



