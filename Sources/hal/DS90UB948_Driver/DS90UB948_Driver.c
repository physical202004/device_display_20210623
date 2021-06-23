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


static void DS90UB948_Driver_Init(void);
static E_HAL_STATE DS90UB948_Driver_Write(U32 address, U8 * buffer, U16 Len);
static E_HAL_STATE DS90UB948_Driver_Read(U32 address, U8 * buffer, U16 Len);
static E_HAL_STATE DS90UB948_Driver_IOControl(U16 type, void* buffer);

#ifdef DS90UB948_USE_REGISTER
static MYBOOL DS90UB948_Driver_CfgGPIOReg(U8 reg,U8 data);
static MYBOOL DS90UB948_Driver_CfgSCLTime(void);
static MYBOOL DS90UB948_Driver_InitConfig(void);
static void DS90UB948_Driver_FinishedHandler(E_COMM_MODE mode,E_COMM_DIR rw_dir);
static void DS90UB948_Driver_ErrHandler(E_COMM_MODE mode,ERR_TYPE errType);
static MYBOOL smbRWFinishState = FALSE;
#endif

static void DS90UB948_Driver_IFCTimeCntInit(void);
static void DS90UB948_Driver_IFCRecordLockHandler(void);
//static void DS90UB948_Driver_IFCRecordBLOnOffHandler(void);
//static void DS90UB948_Driver_IFCRecordBLPwmHandler(void);
static void DS90UB948_Driver_IFCRecord1v2Handler(void);
//static void DS90UB948_Driver_IFCRecordVddHandler(void);
//static void DS90UB948_Driver_IFCRecordVddioHandler(void);
static void DS90UB948_Driver_IFCRecordVolHandler(void);

static U16 suhwCheckLockCnt = 0;
static U16 suhwCheckBLOnOffCnt = 0;
static U16 suhwCheckBLPWMCnt = 0;
static U16 suhwCheck948VolCnt = 0;


bdev gsDS90UB948Drv =
{
	.hal_block_Init 		= DS90UB948_Driver_Init,
	.hal_block_DeInit	 	= NULL,
	.hal_block_open			= NULL,
	.hal_block_close 		= NULL,
	.hal_block_read 		= DS90UB948_Driver_Read,
	.hal_block_write		= DS90UB948_Driver_Write,
	.hal_block_iocontrol 	= DS90UB948_Driver_IOControl,
};

/***********************************************************************
* Name      	: DS90UB948_Driver_Init
* Description	: HAL Initial
* Parameter   	: None
* Returns     	: MYBOOL
***********************************************************************/
static void DS90UB948_Driver_Init(void)
{
	DS90UB948_Driver_PowerSequenceInit();
	DevCtrl_Register_Dev(&gsDS90UB948Dev);
}

/***********************************************************************
* Name      	: DS90UB948_Driver_Write
* Description	: HAL write data
* Parameter   	: address:register address,
*				  buffer:data pointer,
*				  Len:data length
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE DS90UB948_Driver_Write(U32 address,U8* buffer, U16 Len)
{
#ifdef DS90UB948_USE_REGISTER
	int i = 0;
	E_HAL_STATE leState = E_HAL_STATE_OK;
	U8 laubBuf[DS90UB948_PACKAGE_MAX + 1] = {0};
	if(Len <= DS90UB948_PACKAGE_MAX)
	{
		laubBuf[0] = address;
		memcpy(laubBuf + 1,buffer,Len);
		smbRWFinishState = FALSE;
		if(TRUE == IIC_Adapt_Send((U8)address,laubBuf,(U8)(Len + 1)))//send data out
		{
			for(i = 0;i < DS90UB948_COMM_TIMEOUT;i++)//waiting communication finished
			{
				if(smbRWFinishState == TRUE)
				{
					break;
				}
				else
				{
					DelayUs(10);
				}
			}
			if(i >= DS90UB948_COMM_TIMEOUT)//check communication state
			{
				leState = E_HAL_STATE_FAIL;
			}
			else
			{
				leState = E_HAL_STATE_OK;
			}
		}
		else
		{
			leState = E_HAL_STATE_FAIL;
		}
	}
	else
	{
		leState = E_HAL_STATE_INVALID_PARA;
	}
	return leState;
#else
	return E_HAL_STATE_OK;
#endif
}

/***********************************************************************
* Name      	: DS90UB948_Driver_Read
* Description	: HAL read data
* Parameter   	: address:register address,
*				  buffer:data pointer,
*				  Len:data length
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE DS90UB948_Driver_Read(U32 address,U8* buffer, U16 Len)
{
#ifdef DS90UB948_USE_REGISTER
	int i = 0;
	E_HAL_STATE leState = E_HAL_STATE_OK;
	smbRWFinishState = FALSE;
	if(IIC_Adapt_Read((U8)address,buffer,(U8)Len) == TRUE)//send data out
	{
		for(i = 0;i < DS90UB948_COMM_TIMEOUT;i++)//waiting communication finished
		{
			if(smbRWFinishState == TRUE)
			{
				break;
			}
			else
			{
				DelayUs(10);
			}
		}
		if(i >= DS90UB948_COMM_TIMEOUT)//check communication state
		{
			leState = E_HAL_STATE_FAIL;
		}
		else
		{
			leState = E_HAL_STATE_OK;
		}
	}
	else
	{
		leState = E_HAL_STATE_FAIL;
	}
	return leState;
#else
	return E_HAL_STATE_OK;
#endif

}


/***********************************************************************
* Name      	: DS90UB948_Driver_IOControl
* Description	: HAL IO Control
* Parameter   	: None
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE DS90UB948_Driver_IOControl(U16 type, void* buffer)
{

	E_HAL_STATE leState = E_HAL_STATE_OK;
	if(gsDS90UB948Dev.devState != E_DEVCTRL_STATE_OPEN)
	{
		leState = E_HAL_STATE_FAIL;
	}
	else
	{
		switch(type)
		{
			case LVDS_FUNC_LOCK_STATE:
				if(HIGH_LEVEL == DEV_IO_ASYNC_READ(IO_IN_LVDS_LOCK))
				{
					leState = E_HAL_STATE_OK;
				}
				else
				{
					leState = E_HAL_STATE_FAIL;
				}
				break;
			case LVDS_FUNC_TFT_ENABLE_STATE:
				if(HIGH_LEVEL == DEV_IO_ASYNC_READ(IO_IN_TFT_EN_FR_HU))
				{
					leState = E_HAL_STATE_OK;
				}
				else
				{
					leState = E_HAL_STATE_FAIL;
				}
				break;
			case LVDS_FUNC_BL_ENABLE_STATE:
				if(HIGH_LEVEL == DEV_IO_ASYNC_READ(IO_IN_BL_EN_FR_HU))
				{
					leState = E_HAL_STATE_OK;
				}
				else
				{
					leState = E_HAL_STATE_FAIL;
				}
				break;
			/*case LVDS_FUNC_GET_FAULT_STATE:
				if(buffer == NULL)
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
				else
				{
					*(E_DEVCTRL_FAULT_STATE *)buffer = gsDS90UB948Dev.faultState;
				}
				break;*/
			default:
				leState = E_HAL_STATE_NO_FUNC;
				break;
		}
	}

	return leState;
}

#ifdef DS90UB948_USE_REGISTER

/***********************************************************************
* Name      	: DS90UB948_Driver_CfgGPIOReg
* Description	: configuration GPIO register
* Parameter   	: reg:register address,data:register data
* Returns     	: TRUE:sucessful:FALSE:failed 
***********************************************************************/
static MYBOOL DS90UB948_Driver_CfgGPIOReg(U8 reg,U8 data)
{
	MYBOOL lmbRet = TRUE;
	U8 lubRecvData = 0;

	if(E_HAL_STATE_OK == DS90UB948_Driver_Write(reg,&data,1))
	{
		if(E_HAL_STATE_OK == DS90UB948_Driver_Read(reg,&lubRecvData,1))
		{
			if(data == lubRecvData)//check the validity of the data
			{
				lmbRet = TRUE;
			}
			else
			{
				TraceServ_SendStrWithHexMsg("Config LVDS GPIO failed,reg",&reg,1,E_TRACE_EVENT_GENERAL);
				TraceServ_SendStrWithHexMsg("Config LVDS GPIO failed,data",&lubRecvData,1,E_TRACE_EVENT_GENERAL);
				lmbRet = FALSE;
			}
		}
		else
		{
			TraceServ_SendStrMsg("Config LVDS GPIO failed",E_TRACE_EVENT_GENERAL);
			lmbRet = FALSE;
		}
	}
	else
	{
		TraceServ_SendStrMsg("Config LVDS GPIO failed",E_TRACE_EVENT_GENERAL);
		lmbRet = FALSE;
	}
	return (lmbRet);
}

/***********************************************************************
* Name      	: DS90UB948_Driver_CfgSCLTime
* Description	: configuration SCL time
* Parameter   	: data:time data
* Returns     	: TRUE:sucessful:FALSE:failed 
***********************************************************************/
static MYBOOL DS90UB948_Driver_CfgSCLTime(void)
{
	MYBOOL lmbRet = TRUE;
	U8 laubSCLTimeVal[2] = {0x22,0x22};//high time + low time,400KHz
	U8 laubRecvBuffer[2] = {0};
	if(E_HAL_STATE_OK == DS90UB948_Driver_Write(E_DS90UB948_REG_ADDR_SCL_HIGH_TIME,laubSCLTimeVal,2))
	{
		if(E_HAL_STATE_OK == DS90UB948_Driver_Read(E_DS90UB948_REG_ADDR_SCL_HIGH_TIME,laubRecvBuffer,2))
		{
			if(memcmp(laubRecvBuffer,laubSCLTimeVal,2) == 0)//check the validity of the data
			{
				lmbRet = TRUE;
			}
			else
			{
				TraceServ_SendStrWithHexMsg("Config LVDS SCL failed",laubRecvBuffer,2, E_TRACE_EVENT_GENERAL);
				lmbRet = FALSE;
			}
		}
		else
		{
			TraceServ_SendStrMsg("Config LVDS SCL failed",E_TRACE_EVENT_GENERAL);
			lmbRet = FALSE;
		}
	}
	else
	{
		TraceServ_SendStrMsg("Config LVDS SCL failed",E_TRACE_EVENT_GENERAL);
		lmbRet = FALSE;
	}
	return (lmbRet);
}

/***********************************************************************
* Name      	: DS90UB948_Driver_InitConfig
* Description	: configuration handler
* Parameter   	: None
* Returns     	: TRUE:sucessful:FALSE:failed 
***********************************************************************/
static MYBOOL DS90UB948_Driver_InitConfig(void)
{
	MYBOOL lmbRet = TRUE;

	if(TRUE == DS90UB948_Driver_CfgSCLTime())//configuration SCL time
	{
		if(TRUE == DS90UB948_Driver_CfgGPIOReg(E_DS90UB948_REG_ADDR_GPIO0_CFG,0x07))//configuration GPIO0
		{
			if(TRUE == DS90UB948_Driver_CfgGPIOReg(E_DS90UB948_REG_ADDR_GPIO1_2_CFG,0x99))//configuration GPIO3
			{
				if(TRUE == DS90UB948_Driver_CfgGPIOReg(E_DS90UB948_REG_ADDR_GPIO3_CFG,0x07))//configuration GPIO2
				{
					if(TRUE == DS90UB948_Driver_CfgGPIOReg(E_DS90UB948_REG_ADDR_GPIO5_6_CFG,0x07))//configuration GPIO3
					{
						lmbRet = TRUE;
					}
					else
					{
						lmbRet = FALSE;
					}
				}
				else
				{
					lmbRet = FALSE;
				}
			}
			else
			{
				lmbRet = FALSE;
			}
		}
		else
		{
			lmbRet = FALSE;
		}
	}
	else
	{
		lmbRet = FALSE;
	}

	return (lmbRet);
}

/***********************************************************************
*  Name        	: CommServ_ErrHandler
*  Description 	: Communication error callback
*  Parameter   	: mode:communication mode,errType:type of error
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_ErrHandler(E_COMM_MODE mode,ERR_TYPE errType)
{
	IIC_Adapt_Start(E_COMM_MODE_MASTER,E_COMM_BAUD_400K,DS90UB948_IIC_ADDR);//restart IIC
}

/***********************************************************************
*  Name 		: CommServ_FinishedHandler
*  Description	: Handler when one frame session finishes(Stop received).
*  Parameter	: mode:communication mode,rw_dir:Tx or Rx
*  Returns		: None
***********************************************************************/
static void DS90UB948_Driver_FinishedHandler(E_COMM_MODE mode,E_COMM_DIR rw_dir)
{
	smbRWFinishState = TRUE;
}
#endif

MYBOOL DS90UB948_Driver_DataInit(void)
{
	MYBOOL lmbRet = TRUE;
	DS90UB948_Driver_IFCTimeCntInit();
#ifdef DS90UB948_USE_REGISTER
	/* register callback function */
	IIC_Adapt_SetErrCallback(DS90UB948_Driver_ErrHandler);
	IIC_Adapt_SetStopCallback(DS90UB948_Driver_FinishedHandler);
	IIC_Adapt_Start(E_COMM_MODE_MASTER,E_COMM_BAUD_400K,DS90UB948_IIC_ADDR);
	
	lmbRet = DS90UB948_Driver_InitConfig();//initial register configuration
#endif
	CS_SIG_SyncSend(SIG_BOOL_COMM_CAN_OPEN,&lmbRet);
	
	return (lmbRet);
}

MYBOOL DS90UB948_Driver_DataDeinit(void)
{
	MYBOOL lmbState = FALSE;
	
	CS_SIG_SyncSend(SIG_BOOL_COMM_CAN_OPEN,&lmbState);

	return (TRUE);
}

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCTimeCntInit
*  Description 	: IFC Time Count Clear handler
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_IFCTimeCntInit(void)
{
	suhwCheckLockCnt = 0;
	suhwCheckBLOnOffCnt = 0;
	suhwCheckBLPWMCnt = 0;
	suhwCheck948VolCnt = 0;
}

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordLockHandler
*  Description 	: record lock signal loss processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_IFCRecordLockHandler(void)
{
	if(suhwCheckLockCnt < DIAGSERV_IFC_TIME_10S)  // lvds chip startup for 10s
	{
		suhwCheckLockCnt++;
	}
	else
	{
		if(HIGH_LEVEL == DEV_IO_ASYNC_READ(IO_IN_LVDS_LOCK))  // check lock signal
		{
			suhwCheckLockCnt = DIAGSERV_IFC_TIME_10S;
		}
		else
		{
			if(suhwCheckLockCnt < DIAGSERV_IFC_TIME_20S)  // lock signal has lost for 10s
			{
				suhwCheckLockCnt++;
			}
			else
			{
				DiagServ_AddIfcInfo(IFC_LOCK_SIGNAL);  // record lock signal has loss
			}
		}
	}
}

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordBLOnOffHandler
*  Description 	: record bl enable signal processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#if 0
static void DS90UB948_Driver_IFCRecordBLOnOffHandler(void)
{
	if(suhwCheckBLOnOffCnt < DIAGSERV_IFC_TIME_5S)  // lvds chip startup for 5s
	{
		suhwCheckBLOnOffCnt++;
	}
	else if(LOW_LEVEL == DEV_IO_ASYNC_READ(IO_IN_BL_EN_FR_HU))  // check backlight enable signal
	{
		//DiagServ_AddIfcInfo(IFC_BL_ON_OFF);  // record backlight enable signal invalid
	}
	else
	{
		/* do nothing... */
	}
}


/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordBLPwmHandler
*  Description 	: record bl pwm signal processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_IFCRecordBLPwmHandler(void)
{
	DEV_BRN_16BIT_T ltCurrBrightness = 0;

	CS_SIG_SyncReceive(SIG_SHORT_PWM_FROM_HU,&ltCurrBrightness);
	if(suhwCheckBLPWMCnt < DIAGSERV_IFC_TIME_500MS)  // lvds chip startup for 500ms
	{
		suhwCheckBLPWMCnt++;
	}
	else if(suhwCheckBLPWMCnt < (DIAGSERV_IFC_TIME_60S + DIAGSERV_IFC_TIME_500MS))
	{
		if(ltCurrBrightness == 0)  // pwm signal continues to be lost for 60s
		{
			suhwCheckBLPWMCnt++;
			if(suhwCheckBLPWMCnt == (DIAGSERV_IFC_TIME_60S + DIAGSERV_IFC_TIME_500MS))
			{
				DiagServ_AddIfcInfo(IFC_BL_PWM);  // record backlight pwm signal invalid
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			suhwCheckBLPWMCnt = DIAGSERV_IFC_TIME_500MS;  // timer reset
		}
	}
	else
	{
		/* do nothing... */
	}
}
#endif

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordVolHandler
*  Description 	: record LVDS abnormal voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_IFCRecordVolHandler(void)
{
	if(suhwCheck948VolCnt < DIAGSERV_IFC_TIME_500MS)
	{
		suhwCheck948VolCnt++;
	}
	else
	{
		DS90UB948_Driver_IFCRecord1v2Handler();
		//DS90UB948_Driver_IFCRecordVddHandler();
		//DS90UB948_Driver_IFCRecordVddioHandler();
	}
}

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecord1v2Handler
*  Description 	: record LVDS abnormal 1v2 voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void DS90UB948_Driver_IFCRecord1v2Handler(void)
{
	DEV_ADC_16BIT_T ltADC1v2 = DEV_ADC_ASYNC_READ(ADC_LVDS_1V2_DET);
	if((ltADC1v2 > DS90UB948_1V2_AD_MIN) && (ltADC1v2 < DS90UB948_1V2_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		//gsDS90UB948Dev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		DiagServ_AddIfcInfo(IFC_LVDS_1V2);  // record LVDS 1V2 abnormality
	}
}

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordVddHandler
*  Description 	: record LVDS abnormal vdd voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#if 0
static void DS90UB948_Driver_IFCRecordVddHandler(void)
{
	DEV_ADC_16BIT_T ltADCvdd = DEV_ADC_ASYNC_READ(ADC_LVDS_VDD_DET);
	if((ltADCvdd > DS90UB948_VDD_AD_MIN) && (ltADCvdd < DS90UB948_VDD_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsDS90UB948Dev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		//DiagServ_AddIfcInfo(IFC_LVDS_VDD);  // record LVDS VDD abnormality
	}
}
#endif

/***********************************************************************
*  Name        	: DS90UB948_Driver_IFCRecordVddioHandler
*  Description 	: record LVDS abnormal vddio voltage processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
#if 0
static void DS90UB948_Driver_IFCRecordVddioHandler(void)
{
	DEV_ADC_16BIT_T ltADCvddio = DEV_ADC_ASYNC_READ(ADC_LVDS_VDDIO_DET);
	if((ltADCvddio > DS90UB948_VDDIO_AD_MIN) && (ltADCvddio < DS90UB948_VDDIO_AD_MAX))
	{
		/* do nothing... */
	}
	else
	{
		gsDS90UB948Dev.faultState = E_DEVCTRL_FAULT_STATE_POWER;
		//DiagServ_AddIfcInfo(IFC_LVDS_VDDIO);  // record LVDS VDDIO abnormality
	}
}
#endif

/***********************************************************************
* Name      	: DS90UB948_Driver_CycleTask10ms
* Description	: Cycle task entry
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void DS90UB948_Driver_CycleTask10ms(void)
{
	DS90UB948_Driver_IFCRecordLockHandler();
	//DS90UB948_Driver_IFCRecordBLOnOffHandler();
	//DS90UB948_Driver_IFCRecordBLPwmHandler();
	DS90UB948_Driver_IFCRecordVolHandler();
}

#endif

