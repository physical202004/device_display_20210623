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


static void FOG_Driver_Init(void);
static E_HAL_STATE FOG_Driver_IOControl(U16 type, void* buffer);
static void FOG_Driver_ReadVcomFromFlash(void);
static void FOG_Driver_DetectFaultHandler(void);
//static void FOG_Driver_IFCRecordVspHandler(void);
static void FOG_Driver_IFCRecordVddHandler(void);
static void FOG_Driver_IFCRecordVghHandler(void);
static void FOG_Driver_IFCRecordVglHandler(void);
static void FOG_Driver_IFCRecordVolHandler(void);
U16 guhwVcomVal = FOG_VCOM_TIMER_DEFAULT_VAL;
static U16 suhwCheckTFTFaultCnt = 0;
static U8 subCheckFogVolCnt = 0;

bdev gsFOGDrv =
{
	.hal_block_Init 		= FOG_Driver_Init,
	.hal_block_DeInit	 	= NULL,
	.hal_block_open			= NULL,
	.hal_block_close 		= NULL,
	.hal_block_read 		= NULL,
	.hal_block_write		= NULL,
	.hal_block_iocontrol 	= FOG_Driver_IOControl,
};

/***********************************************************************
* Name      	: FOG_Driver_Init
* Description	: HAL Initial
* Parameter   	: None
* Returns     	: MYBOOL
***********************************************************************/
static void FOG_Driver_Init(void)
{
	FOG_Driver_PowerSequenceInit();
	DevCtrl_Register_Dev(&gsFOGDev);
	FOG_Driver_ReadVcomFromFlash();
	Pwm_Adapt_Init(FOG_VCOM_TIMER_UNIT,FOG_VCOM_TIMER_CHANNEL,FOG_VCOM_TIMER_FREQ,0);
}

/***********************************************************************
* Name      	: FOG_Driver_IOControl
* Description	: HAL IO Control
* Parameter   	: None
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE FOG_Driver_IOControl(U16 type, void* buffer)
{

	E_HAL_STATE leState = E_HAL_STATE_OK;
	switch(type)
	{
		case FOG_FUNC_CHANGE_VCOM:
			if(buffer != NULL)
			{
				guhwVcomVal = ((*(U8 *)buffer) << 8) + *(U8 *)(buffer + 1);
				Pwm_Adapt_SetDuty(FOG_VCOM_TIMER_UNIT,FOG_VCOM_TIMER_CHANNEL,guhwVcomVal);
			}
			else
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			break;
		/*case FOG_FUNC_GET_FAULT_STATE:
			if(buffer == NULL)
			{
				leState = E_HAL_STATE_INVALID_PARA;
			}
			else
			{
				*(E_DEVCTRL_FAULT_STATE *)buffer = gsFOGDev.faultState;
			}
			break;*/
		default:
			leState = E_HAL_STATE_NO_FUNC;
			break;
	}

	return leState;
}

/***********************************************************************
* Name      	: FOG_Driver_ReadVcomFromFlash
* Description	: Read vcom value from flash,failed will use default value and retry 3 times
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void FOG_Driver_ReadVcomFromFlash(void)
{
	int i = 0;
	U8 laubReadData[4] = {0};
	U8 lubCheckSum = 0;
	for(i = 0;i < 3;i++)
	{
		if(EepServ_ReadStaticData(E_BLOCK_0,EEP_FOG_VCOM_ADDR,laubReadData,4) == TRUE)
		{
			lubCheckSum = FrameChecksum(laubReadData,2);
			if(lubCheckSum == laubReadData[2])
			{
				guhwVcomVal = laubReadData[0] + (laubReadData[0] << 8);
				break;
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}
	}
}

/***********************************************************************
* Name      	: FOG_Driver_DataInit
* Description	: Data Initialization
* Parameter   	: None
* Returns     	: None
***********************************************************************/
MYBOOL FOG_Driver_DataInit(void)
{
	suhwCheckTFTFaultCnt = 0;
	subCheckFogVolCnt = 0;
	return TRUE;
}

/***********************************************************************
* Name      	: DS90UB948_Driver_DataDeinit
* Description	: Data Initialization
* Parameter   	: None
* Returns     	: None
***********************************************************************/
MYBOOL FOG_Driver_DataDeinit(void)
{
	return TRUE;
}

/***********************************************************************
* Name      	: FOG_Driver_DetectFaultHandler
* Description	: Detect fault handler
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void FOG_Driver_DetectFaultHandler(void)
{
    DEV_IO_8BIT_T l8tTFTPinFaultLevel = DEV_IO_ASYNC_READ(IO_IN_TFT_FAULT);

	if(suhwCheckTFTFaultCnt < DIAGSERV_IFC_TIME_10S)
	{
		suhwCheckTFTFaultCnt++;
	}
	else if(l8tTFTPinFaultLevel == HIGH_LEVEL)
	{
		DiagServ_AddIfcInfo(IFC_FOG_FAULT);  // record tft fault signal valid
	}
	else
	{
	    /* do nothing... */
	}
}

/***********************************************************************
*  Name        	: FOG_Driver_IFCRecordVolHandler
*  Description 	: record FOG abnormal voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void FOG_Driver_IFCRecordVolHandler(void)
{
	if(subCheckFogVolCnt < DIAGSERV_IFC_TIME_500MS)
	{
		subCheckFogVolCnt++;
	}
	else
	{
		//FOG_Driver_IFCRecordVspHandler();
		FOG_Driver_IFCRecordVddHandler();
		FOG_Driver_IFCRecordVghHandler();
		FOG_Driver_IFCRecordVglHandler();
	}
}

/***********************************************************************
*  Name        	: FOG_Driver_IFCRecordVspHandler
*  Description 	: record FOG abnormal VSP voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#if 0
static void FOG_Driver_IFCRecordVspHandler(void)
{
	DEV_ADC_16BIT_T ltADCvsp = DEV_ADC_ASYNC_READ(ADC_TFT_VSP_DET);
	if((ltADCvsp > FOG_VSP_AD_MIN) && (ltADCvsp < FOG_VSP_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsFOGDev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		//DiagServ_AddIfcInfo(IFC_TFT_VSP);	// record LVDS VSP abnormality
	}
}
#endif
/***********************************************************************
*  Name        	: FOG_Driver_IFCRecordVddHandler
*  Description 	: record FOG abnormal Vdd voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void FOG_Driver_IFCRecordVddHandler(void)
{
	DEV_ADC_16BIT_T ltADCvdd = DEV_ADC_ASYNC_READ(ADC_TFT_VDD_DET);
	if((ltADCvdd > FOG_VDD_AD_MIN) && (ltADCvdd < FOG_VDD_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsFOGDev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		DiagServ_AddIfcInfo(IFC_TFT_VDD);	// record LVDS VDD abnormality
	}
}

/***********************************************************************
*  Name        	: FOG_Driver_IFCRecordVghHandler
*  Description 	: record FOG abnormal Vgh voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void FOG_Driver_IFCRecordVghHandler(void)
{
	DEV_ADC_16BIT_T ltADCvgh = DEV_ADC_ASYNC_READ(ADC_TFT_VGH_DET);
	if((ltADCvgh > FOG_VGH_AD_MIN) && (ltADCvgh < FOG_VGH_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsFOGDev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		DiagServ_AddIfcInfo(IFC_TFT_VGH);	// record LVDS VGH abnormality
	}
}

/***********************************************************************
*  Name        	: FOG_Driver_IFCRecordVglHandler
*  Description 	: record FOG abnormal Vgh voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void FOG_Driver_IFCRecordVglHandler(void)
{
	DEV_ADC_16BIT_T ltADCvgl = DEV_ADC_ASYNC_READ(ADC_TFT_VGL_DET);	
	if((ltADCvgl > FOG_VGL_AD_MIN) && (ltADCvgl < FOG_VGL_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsFOGDev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		DiagServ_AddIfcInfo(IFC_TFT_VGL);	// record LVDS VGL abnormality
	}
}
/***********************************************************************
* Name      	: FOG_Driver_CycleTask10ms
* Description	: Cycle task entry
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void FOG_Driver_CycleTask10ms(void)
{
	FOG_Driver_DetectFaultHandler();
	FOG_Driver_IFCRecordVolHandler();
}


#endif



