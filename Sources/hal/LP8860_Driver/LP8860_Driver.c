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
#include "LP8860_Driver_Eep_Cfg.h"

static void LP8860_Driver_Init(void);
static E_HAL_STATE LP8860_Driver_IOControl(U16 type, void* buffer);

static E_LP8860_CONFIG gubLP8860ConfigStep = E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK;
static DEV_BRN_16BIT_T ltPrevBrnVal = DEV_BL_BRN_MAX;
static U16 suhwEepromCtrlDelayCnt = 0u;
static U16 suhwReadFaultCnt = 0u;
static U16 suhwStartupFaultFilterCnt = 0u;
static U8 subRecvLen = 0U;
static U8 subLP8860FaultValue = 0u;
static U8 subLP8860LedFaultValue = 0u;
static MYBOOL smbCommRWState = FALSE;
static MYBOOL smbFaultFlag = FALSE;
static U8* subpRecvBuf = NULL;
static U8 saubEepConfigFlag[2] = {LP8860_EEP_CONFIG_INVALID,LP8860_EEP_CONFIG_INVALID};

static MYBOOL LP8860_Driver_SetBrn(DEV_BRN_16BIT_T brightness);
static DEV_BRN_16BIT_T LP8860_Driver_GetBrn(void);
static MYBOOL LP8860_Driver_ReadData(U32 address,U8* buffer, U16 Len);
static MYBOOL LP8860_Driver_WriteData(U32 address,U8* buffer, U16 Len);
static MYBOOL LP8860_Driver_CheckEepromIsUnlock(void);
static void LP8860_Driver_UnlockEeprom(void);
static MYBOOL LP8860_Driver_FlashEeprom(void);
static void LP8860_Driver_LockEeprom(void);
static U8 LP8860_Driver_ReadFaultInfo(void);
static void LP8860_Driver_LedOpenHandler(U8 data);
static void LP8860_Driver_LedShortHandler(U8 data);
static U8 LP8860_Driver_ReadLedFaultInfo(void);
static U8 LP8860_Driver_ReadFaultInfo(void);
static U8 LP8860_Driver_ReadLedFaultInfo(void);
static void LP8860_Driver_ClearFaultInfo(void);
static void LP8860_Driver_FaultHandler(void);
static void LP8860_Driver_ReadConfigFlag(void);
static void LP8860_Driver_WriteConfigFlag(void);
static MYBOOL LP8860_CheckConfigState(void);
static void LP8860_Driver_ConfigHandler(void);
static void LP8860_Driver_RxHandler(E_COMM_MODE mode,U8 index,U8 data);
static void LP8860_Driver_TxHandler(E_COMM_MODE mode);

bdev gsLP8860Drv =
{
	.hal_block_Init 		= LP8860_Driver_Init,
	.hal_block_DeInit	 	= NULL,
	.hal_block_open			= NULL,
	.hal_block_close 		= NULL,
	.hal_block_read 		= NULL,
	.hal_block_write		= NULL,
	.hal_block_iocontrol 	= LP8860_Driver_IOControl,
};

/***********************************************************************
* Name      	: LP8860_Driver_Init
* Description	: HAL Initial
* Parameter   	: None
* Returns     	: BOOL
***********************************************************************/
static void LP8860_Driver_Init(void)
{
	LP8860_Driver_ReadConfigFlag();
	LP8860_Driver_PowerSequenceInit();
	DevCtrl_Register_Dev(&gsLP8860Dev);
	Spi_Adapt_SetRecvCallback(LP8860_COMM_CHANNEL,LP8860_Driver_RxHandler);
	Spi_Adapt_SetSendCallback(LP8860_COMM_CHANNEL,LP8860_Driver_TxHandler);
	Spi_Adapt_Start(E_COMM_MODE_MASTER,LP8860_COMM_CHANNEL,LP8860_COMM_BAUDRATE);
}


/***********************************************************************
* Name      	: LP8860_Driver_ReadData
* Description	: Read data
* Parameter   	: address:register address
				  buffer:buffer pointer for read
				  Len:length of buffer
* Returns     	: TRUE:Read sucessful,FALSE:read failed
***********************************************************************/
static MYBOOL LP8860_Driver_ReadData(U32 address,U8* buffer, U16 Len)
{
	
	U16 luhwCommTimeCnt = 0U;
	U16 luhwCommTimeCnt1 = 0U;
	U8 lubAddr = (U8)address << 1;
	
	smbCommRWState = FALSE;
	subRecvLen = Len;				//set length
	subpRecvBuf = buffer;			//set receive buffer
	DEV_IO_WRITE(IO_OUT_BL_NSS,LOW_LEVEL);//active NSS pin
	if(Spi_Adapt_Write(LP8860_COMM_CHANNEL,&lubAddr,1) == TRUE)	//send address first
	{
		for(luhwCommTimeCnt = 0;luhwCommTimeCnt < LP8860_COMM_TIMEOUT;luhwCommTimeCnt++)
		{
			if(smbCommRWState == TRUE)
			{
				smbCommRWState = FALSE;			//reset state
				if(Spi_Adapt_Read(LP8860_COMM_CHANNEL,NULL,subRecvLen) == TRUE)	//read data
				{
					for(luhwCommTimeCnt1 = 0U;luhwCommTimeCnt1 < LP8860_COMM_TIMEOUT;luhwCommTimeCnt1++)
					{
						if(smbCommRWState == TRUE)
						{
							break;
						}
						else
						{
							DelayUs(1); 		//delay 1us
						}
					}
				}
				else
				{
					/* do nothing... */
				}
				break;
			}
			else
			{
				DelayUs(1); 				//delay 1us
			}
		}
	}
	else
	{
		/* do nothing... */
	}
	DEV_IO_WRITE(IO_OUT_BL_NSS,HIGH_LEVEL); //inactive NSS pin
	subpRecvBuf = NULL;						//reset receive buffer pointer
    return smbCommRWState;
}
/***********************************************************************
* Name      	: LP8860_Driver_WriteData
* Description	: Write data
* Parameter   	: address:register address
				  buffer:buffer pointer for read
				  Len:length of buffer
* Returns     	: TRUE:Read sucessful,FALSE:read failed
***********************************************************************/
static MYBOOL LP8860_Driver_WriteData(U32 address,U8* buffer, U16 Len)
{
	U16 i = 0;
	U16 luhwCommTimeCnt = 0U;
	U8 laubSendBuf[2] = {0};
	/*The transmission consists of 16-bit write and read cycles. One cycle consists of 7 address bits, 1 read/write (R/W) bit, and 8 data bits.*/
	for(i = 0;i < Len;i++)
	{
		laubSendBuf[0] = ((U8)(address + i) << 1) | 0x01;//set address
		laubSendBuf[1] = buffer[i];//set data
		smbCommRWState = FALSE;
		DEV_IO_WRITE(IO_OUT_BL_NSS,LOW_LEVEL);//active NSS pin
		if(Spi_Adapt_Write(LP8860_COMM_CHANNEL,laubSendBuf,2) == TRUE)	//send data
		{
			for(luhwCommTimeCnt = 0;luhwCommTimeCnt < LP8860_COMM_TIMEOUT;luhwCommTimeCnt++)
		    {
		        if(smbCommRWState == TRUE)
		        {
		            break;
		        }
				else
				{
					DelayUs(1);					//delay 1us
				}
		    }
		}
		else
		{
			DEV_IO_WRITE(IO_OUT_BL_NSS,HIGH_LEVEL);//inactive NSS pin
			break;
		}
		DEV_IO_WRITE(IO_OUT_BL_NSS,HIGH_LEVEL);//inactive NSS pin
	}
    return smbCommRWState;
}

/***********************************************************************
* Name      	: LP8860_Driver_IOControl
* Description	: HAL IO Control
* Parameter   	: None
* Returns     	: E_HAL_STATE
***********************************************************************/
static E_HAL_STATE LP8860_Driver_IOControl(U16 type, void* buffer)
{
	E_HAL_STATE leState = E_HAL_STATE_OK;
	DEV_BRN_16BIT_T ltTargetBrnVal = 0U;

	if((LP8860_CheckConfigState() == FALSE) || (gsLP8860Dev.devState != E_DEVCTRL_STATE_OPEN))
	{
		leState = E_HAL_STATE_INACTIVE;
	}
	else
	{
		switch(type)
		{
			case BL_FUNC_SET_BRN:
				if(buffer == NULL)
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
				else
				{
					ltTargetBrnVal = *(DEV_BRN_16BIT_T *)buffer;
					if(ltTargetBrnVal == DEV_BL_BRN_MIN)
					{
						if(LP8860_Driver_SetBrn(ltTargetBrnVal) == TRUE)
						{
							ltPrevBrnVal = DEV_BL_BRN_MIN;
							DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);//close enable
							leState = E_HAL_STATE_OK;
						}
						else
						{
							leState = E_HAL_STATE_FAIL;
						}
					}
					else
					{
						if(ltPrevBrnVal == DEV_BL_BRN_MIN)
						{
							ltPrevBrnVal = ltTargetBrnVal;
							DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);//open enable first
							leState = E_HAL_STATE_WAITING;
						}
						else
						{
							leState = LP8860_Driver_SetBrn(ltTargetBrnVal) == TRUE ? E_HAL_STATE_OK : E_HAL_STATE_FAIL;
						}
					}
					
				}
	        	break;
			case BL_FUNC_READ_BRN:
				if(buffer == NULL)
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
				else
				{
					*(DEV_BRN_16BIT_T *)buffer = LP8860_Driver_GetBrn();
				}
				break;
			/*case BL_FUNC_GET_FAULT_STATE:
				if(buffer == NULL)
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
				else
				{
					*(E_DEVCTRL_FAULT_STATE *)buffer = gsLP8860Dev.faultState;
				}
				break;
			case BL_FUNC_CTRL_ENABLE:
				if(buffer == NULL)
				{
					leState = E_HAL_STATE_INVALID_PARA;
				}
				else
				{
					if(*(MYBOOL *)buffer == TRUE)
					{
						DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);
					}
					else
					{
						DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);
					}
				}
				break;*/
			default:
				leState = E_HAL_STATE_NO_FUNC;
			break;
		}
	}
	return leState;
}

static void LP8860_Driver_ReadConfigFlag(void)
{
	int i = 0;
	for(i = 0;i < 3;i++)					//if read eeprom fail,retry 3 times
	{
		if(EepServ_ReadStaticData(E_BLOCK_0,EEP_BL_CONFIG_FLAG_ADDR,saubEepConfigFlag,2) == TRUE)//read data from eeprom
		{
			break;
		}
		else
		{
			continue;
		}
	}
}

static void LP8860_Driver_WriteConfigFlag(void)
{
	
	int i = 0;
	for(i = 0;i < 3;i++)					//if write eeprom fail,retry 3 times
	{
		if(EepServ_WriteStaticData(E_BLOCK_0,EEP_BL_CONFIG_FLAG_ADDR,saubEepConfigFlag,2) == TRUE)//write data to eeprom
		{
			break;
		}
		else
		{
			continue;
		}
	}
}

/***********************************************************************
* Name      	: LP8860_CheckConfigState
* Description	: Check config value whether or not was  writing in eeprom
* Parameter   	: None
* Returns     	: TRUE:data is written,FALSE:no data in eeprom
***********************************************************************/
static MYBOOL LP8860_CheckConfigState(void)
{
	MYBOOL lbRet = FALSE;
	
	if((saubEepConfigFlag[0] != LP8860_EEP_CONFIG_OK) || (saubEepConfigFlag[1] != LP8860_EEP_CONFIG_OK))
	{
		lbRet = FALSE;
	}
	else
	{
		lbRet = TRUE;
	}

	return (lbRet);
}

/***********************************************************************
* Name      	: LP8860_Driver_ConfigHandler
* Description	: write data to LP8860 eeprom
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_ConfigHandler(void)
{

    U16 i = 0u;
	U8 laubEepromRegData[LP8860_EEP_REG_MAP_MAX] = {0u};
    switch(gubLP8860ConfigStep)
    {
		case E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK:
			if(suhwEepromCtrlDelayCnt < LP8860_CHECK_EEPROM_UNLOCK_TIME)
			{
				suhwEepromCtrlDelayCnt++;
			}
			else
			{
				suhwEepromCtrlDelayCnt = 0U;
				if(LP8860_Driver_CheckEepromIsUnlock() == TRUE)
				{
					/* unlock,do nothing... */
				}
				else
				{
	        		LP8860_Driver_UnlockEeprom();     //lock,unlock eeprom
				}
				gubLP8860ConfigStep = E_LP8860_CFG_STEP1_UPDATE_EEPROM;
			}
			break;
        case E_LP8860_CFG_STEP1_UPDATE_EEPROM:
            for(i = 0;i < LP8860_EEP_REG_MAP_MAX;i++)
            {
                LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_REG_0 + i,(U8 *)&gcaubEepromMapData[i],1);       //Write data to EEPROM
                DelayUs(200);
            }
            gubLP8860ConfigStep = E_LP8860_CFG_STEP2_FLASH_EEPROM;
        break;
        case E_LP8860_CFG_STEP2_FLASH_EEPROM:
            if(LP8860_Driver_FlashEeprom() == TRUE)
            {
                gubLP8860ConfigStep = E_LP8860_CFG_STEP3_LOCK_EEPROM;
            }
        break;
        case E_LP8860_CFG_STEP3_LOCK_EEPROM:
			suhwEepromCtrlDelayCnt++;
			if(suhwEepromCtrlDelayCnt == 1)//first cycle
			{
				LP8860_Driver_LockEeprom(); 											//lock
			}	
			else if(suhwEepromCtrlDelayCnt < LP8860_EEPROM_RESTART_EN_TIME)
			{
				DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);               				//pull down bl_enable pin after lock eeprom
			}
			else
			{
				suhwEepromCtrlDelayCnt = 0;
				gubLP8860ConfigStep = E_LP8860_CFG_STEP4_FINISHED;
				DelayMs(3);																//delay to ensure enough time
				DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);								//pull up bl_enable pin wait at least 10ms
			}
        break;
        case E_LP8860_CFG_STEP4_FINISHED:
            LP8860_Driver_ReadData(E_LP8860_ADDR_EEP_REG_0,laubEepromRegData,LP8860_EEP_REG_MAP_MAX);//Read 8860 EEPROM
            if(!memcmp(gcaubEepromMapData,laubEepromRegData,LP8860_EEP_REG_MAP_MAX))//check eeprom
            {
				saubEepConfigFlag[0] = LP8860_EEP_CONFIG_OK;
				saubEepConfigFlag[1] = LP8860_EEP_CONFIG_OK;
				gubLP8860ConfigStep = E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK;
				LP8860_Driver_WriteConfigFlag();
            }
			else
			{
				saubEepConfigFlag[0] = LP8860_EEP_CONFIG_INVALID;
				saubEepConfigFlag[1] = LP8860_EEP_CONFIG_INVALID;
				//DiagServ_AddIfcInfo(IFC_BL_CONFIG_FAILED);
				gubLP8860ConfigStep = E_LP8860_CFG_STEP5_POWEROFF;		//if configure data of eeprom failed,restart device
			}
        break;
		case E_LP8860_CFG_STEP5_POWEROFF:
			DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);					//pull down bl_enable pin
			gubLP8860ConfigStep = E_LP8860_CFG_STEP6_POWERON;
	
		break;
		case E_LP8860_CFG_STEP6_POWERON:
			DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);					//pull up bl_enable pin
			gubLP8860ConfigStep = E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK;
		break;
        default :
            gubLP8860ConfigStep = E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK;
        break;
    }

}


/***********************************************************************
* Name      	: LP8860_Driver_InitData
* Description	: initial data
* Parameter   	: None
* Returns     	: None
***********************************************************************/
MYBOOL LP8860_Driver_InitData(void)
{
	gubLP8860ConfigStep = E_LP8860_CFG_STEP0_EEP_UNLOCK_CHECK;
	suhwEepromCtrlDelayCnt = 0u;
	suhwReadFaultCnt = 0u;
	suhwStartupFaultFilterCnt = 0u;
	subRecvLen = 0U;
	subLP8860FaultValue = 0u;
	subLP8860LedFaultValue = 0u;
	smbCommRWState = FALSE;
	smbFaultFlag = FALSE;
	subpRecvBuf = NULL;
	ltPrevBrnVal = DEV_BL_BRN_MAX;
	return (TRUE);
}


/***********************************************************************
* Name      	: LP8860_Driver_ClearData
* Description	: clear data
* Parameter   	: None
* Returns     	: None
***********************************************************************/
MYBOOL LP8860_Driver_ClearData(void)
{
	LP8860_Driver_SetBrn(0);
	return (TRUE);
}

/***********************************************************************
* Name      	: LP8860_Driver_SetBrn
* Description	: Set brightness
* Parameter   	: brightness:brightness value
* Returns     	: TRUE:OK,FALSE:fail
***********************************************************************/
static MYBOOL LP8860_Driver_SetBrn(DEV_BRN_16BIT_T brightness)
{
	U8 laubTempBuffer[2] = {0};
	laubTempBuffer[0] = (brightness >> 8) & 0xFFU;//MSB first
	laubTempBuffer[1] = brightness & 0xFFU;
	
	return LP8860_Driver_WriteData(E_LP8860_ADDR_SET_BRIGHTNESS,laubTempBuffer,2);
}

/***********************************************************************
* Name      	: LP8860_Driver_GetBrn
* Description	: Get brightness value
* Parameter   	: None
* Returns     	: return current brightness value
***********************************************************************/
static DEV_BRN_16BIT_T LP8860_Driver_GetBrn(void)
{
	DEV_BRN_16BIT_T luhwCurrBrn = 0U;
	U8 laubTempBuffer[2] = {0};
	if(LP8860_Driver_ReadData(E_LP8860_ADDR_SET_BRIGHTNESS,laubTempBuffer,2) == TRUE)//read brightness
	{
		luhwCurrBrn = (laubTempBuffer[0] << 8) + laubTempBuffer[1];//MSB first
	}
	else
	{
		/* do nothing... */
	}
	return luhwCurrBrn;
}

/***********************************************************************
* Name      	: LP8860_Driver_CheckEepromIsUnlock
* Description	: Check eeprom whether is unlock
* Parameter   	: None
* Returns     	: TRUE:unlock,FALSE:lock
***********************************************************************/
static MYBOOL LP8860_Driver_CheckEepromIsUnlock(void)
{
	U8 lubEepromRefVal1 = 0U;
	U8 lubEepromRefVal2 = 0U;
	U8 lubEepromWriteVal = 1U;
	MYBOOL lmbRet = FALSE;
	
	(void)LP8860_Driver_ReadData(E_LP8860_ADDR_EEP_REG_0,&lubEepromRefVal1,1u);//read first time
	(void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_REG_0,&lubEepromWriteVal,1u);//write value
	(void)LP8860_Driver_ReadData(E_LP8860_ADDR_EEP_REG_0,&lubEepromRefVal2,1u);//read new value
	if(lubEepromRefVal2 ==  lubEepromRefVal1)
	{
		lmbRet = FALSE;
	}
	else
	{
		lubEepromWriteVal = lubEepromRefVal1;//write initial value
		(void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_REG_0,&lubEepromRefVal1,1u);//write value
		lmbRet = TRUE;
	}
	return lmbRet;
}

/***********************************************************************
* Name      	: LP8860_Driver_UnlockEeprom
* Description	: unlock eeprom
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_UnlockEeprom(void)
{
    U8 lubRegValue[3] = {0x08u,0xBAu,0xEFu};
    int i = 0;
    for(i = 0;i < sizeof(lubRegValue);i++)
    {
        (void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_UNLOCK,lubRegValue + i,1u);
    }

}

/***********************************************************************
* Name      	: LP8860_Driver_FlashEeprom
* Description	: lock eeprom
* Parameter   	: None
* Returns     	: FALSE:execute flash eeprom,TRUE:flash eeprom completed
***********************************************************************/
static MYBOOL LP8860_Driver_FlashEeprom(void)
{
    MYBOOL lmbRet = FALSE;
	U8 lubUpdateFlashVal = 0x02U;
	U8 lubFlashNormalVal = 0U;
    if(suhwEepromCtrlDelayCnt == 0u)								//first cyclic
    {
        (void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_CTRL,&lubUpdateFlashVal,1u); //control eeprom
        lmbRet = FALSE;
    }
    else if(suhwEepromCtrlDelayCnt == LP8860_LOCK_EEPROM_TIME)	//delay time
    {
        suhwEepromCtrlDelayCnt = 0u;
        (void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_CTRL,&lubFlashNormalVal,1u);
        lmbRet = TRUE;
    }
	else
	{
		/* waiting... */
		lmbRet = FALSE;
	}
    suhwEepromCtrlDelayCnt++;
    return lmbRet;
}

/***********************************************************************
* Name      	: LP8860_Driver_LockEeprom
* Description	: lock eeprom
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_LockEeprom(void)
{
	U8 lubLockEepromVal = 0U;
	(void)LP8860_Driver_WriteData(E_LP8860_ADDR_EEP_UNLOCK,&lubLockEepromVal,1u); //unlock
}


/***********************************************************************
* Name      	: LP8860_Driver_ReadFaultInfo
* Description	: read fault information from register
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static U8 LP8860_Driver_ReadFaultInfo(void)
{
    U8 lubFaultData = 0u;
	U8 i = 0u;
    (void)LP8860_Driver_ReadData(E_LP8860_ADDR_PIN_FAULT,&lubFaultData,1u);
	if(lubFaultData != 0)
	{
		LP8860_Driver_ClearFaultInfo();             //clear the pin fault register
		suhwStartupFaultFilterCnt = LP8860_RESTART_FAULT_FILTER_TIME;
		for(;i < 8;i++)
		{
			if((lubFaultData >> i) & 0x01u)
			{
				switch(i)
				{
					case 0:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_CP_2X);
						break;
					case 1:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_PL_FET);
						break;
					case 2:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_BOOST_OVP);
						break;
					case 3:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_BOOST_OCP);
						break;
					case 4:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_TSD_REASON);
						break;
					case 5:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_VIN_UVLO);
						break;
					case 6:
						DiagServ_AddIfcInfo(IFC_BL_FAULT_VIN_OVP);
					break;
					default:
						/*do nothing*/
						break;
				}
			}
			else
			{
				/*do nothing...*/
			}
		}
	}
	else
	{
		/* do nothing... */
	}
    return lubFaultData;
}

/***********************************************************************
* Name      	: LP8860_Driver_LedOpenHandler
* Description	: Record led open fault information
* Parameter   	: data:register data
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_LedOpenHandler(U8 data)
{
	U8 i = 0U;
	for(i = 0;i < 4;i++)
	{
		if((data >> i) & 0x01u)
		{
			switch(i)
			{
				case 0:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_OPEN_LED1);
                    break;
                case 1:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_OPEN_LED2);
                	break;
                case 2:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_OPEN_LED3);
                    break;
                case 3:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_OPEN_LED4);
                    break;
                default: 
                    /*do nothing*/
                    break;
			}
		}
		else
		{
			/*do nothing...*/
		}
	}
}

/***********************************************************************
* Name      	: LP8860_Driver_LedShortHandler
* Description	: Record led short fault information
* Parameter   	: data:register data
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_LedShortHandler(U8 data)
{
	U8 i = 0U;
	for(i = 0;i < 4;i++)
	{
		if((data >> i) & 0x01u)
		{
			switch(i)
			{
				case 0:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_SHORT_LED1);
                    break;
                case 1:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_SHORT_LED2);
                	break;
                case 2:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_SHORT_LED3);
                    break;
                case 3:
                    DiagServ_AddIfcInfo(IFC_BL_FAULT_SHORT_LED4);
                    break;
                default: 
                    /*do nothing*/
                    break;
			}
		}
		else
		{
			/*do nothing...*/
		}
	}
}


/***********************************************************************
* Name      	: LP8860_Driver_ReadLedFaultInfo
* Description	: read led fault information from register
* Parameter   	: None
* Returns     	: Led data
***********************************************************************/
static U8 LP8860_Driver_ReadLedFaultInfo(void)
{
    U8 lubLedFaultData = 0u;

    (void)LP8860_Driver_ReadData(E_LP8860_ADDR_LED_FAULT,&lubLedFaultData,1u);
	if(lubLedFaultData & LP8860_LED_FAULT_OPEN)
	{
		LP8860_Driver_LedOpenHandler(lubLedFaultData);//record open IFC
	}
	else
	{
		/* do nothing... */
	}
	if(lubLedFaultData & LP8860_LED_FAULT_SHORT)
	{
		LP8860_Driver_LedShortHandler(lubLedFaultData);//record short IFC
	}
	else
	{
		/* do nothing... */
	}
	if(lubLedFaultData & LP8860_LED_FAULT_RESTART_MARK)//whether need reset enable
	{
		DEV_IO_WRITE(IO_OUT_BL_ENABLE,LOW_LEVEL);
		DelayUs(100);
		DEV_IO_WRITE(IO_OUT_BL_ENABLE,HIGH_LEVEL);
	}
	else
	{
		/* do nothing... */
	}
    return lubLedFaultData;
}

/***********************************************************************
* Name      	: LP8860_Driver_ClearFaultInfo
* Description	: send clear fault command to register
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_ClearFaultInfo(void)
{
    U8 lubClrFaultValue = 0x01u;
    (void)LP8860_Driver_WriteData(E_LP8860_ADDR_CLEAR_FAULT,&lubClrFaultValue,1u);
}

/***********************************************************************
* Name      	: LP8860_Driver_FaultHandler
* Description	: Detect fault handler
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_FaultHandler(void)
{
    DEV_IO_8BIT_T ltFaultLevel = HIGH_LEVEL;
	if(suhwStartupFaultFilterCnt < SYS_CYCLE_500MS_TIME)//filter time
	{
		suhwStartupFaultFilterCnt++;
	}
	else
	{
	    ltFaultLevel = DEV_IO_ASYNC_READ(IO_IN_BL_FAULT);
	    if(ltFaultLevel == HIGH_LEVEL)
	    {
	        suhwReadFaultCnt = 0u;
	    }
	    else
	    {
	        if(suhwReadFaultCnt % LP8860_FAULT_CIRCLE_TIME == 0)
	        {
	            
                //read register
                subLP8860FaultValue = LP8860_Driver_ReadFaultInfo();
                subLP8860LedFaultValue = LP8860_Driver_ReadLedFaultInfo();
				/*if((subLP8860FaultValue == 0) && (subLP8860LedFaultValue == 0))
				{
					//DiagServ_AddIfcInfo(IFC_BL_FAULT_NONE);
					gsLP8860Dev.faultState = E_DEVCTRL_FAULT_STATE_NONE;
				}
				else
				{
					gsLP8860Dev.faultState = E_DEVCTRL_FAULT_STATE_MARK;
				}*/
	        }
			else
			{
	        	/* do nothing... */
			}
			suhwReadFaultCnt++;
	    }
	}
}

/***********************************************************************
* Name      	: LP8860_Driver_RxHandler
* Description	: Communication Rx handler
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_RxHandler(E_COMM_MODE mode,U8 index,U8 data)
{
	if((subpRecvBuf != NULL) && (index <subRecvLen))
	{
		subpRecvBuf[index] = data;
	}
	else
	{
		/* return */
	}
	UNUSED(mode);
}

/***********************************************************************
* Name      	: LP8860_Driver_TxHandler
* Description	: Communication Tx handler
* Parameter   	: None
* Returns     	: None
***********************************************************************/
static void LP8860_Driver_TxHandler(E_COMM_MODE mode)
{
	smbCommRWState = TRUE;
	UNUSED(mode);
}

/***********************************************************************
* Name      	: LP8860_Driver_CycleTask10ms
* Description	: Running task
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void LP8860_Driver_CycleTask10ms(void)
{
	if(LP8860_CheckConfigState() == FALSE)
	{
		LP8860_Driver_ConfigHandler();
	}
	else
	{
		LP8860_Driver_FaultHandler();
	}
}

#endif
/****************************** END OF FILE ***************************/

