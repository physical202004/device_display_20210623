/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : eeprom service
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/06/24
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2020/06/24  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "EepServ.h"
#include "Eep_Adapt_if.h"
#include "Global_Definition.h"


static void EepServ_InitAgingInfo(void);
static void EepServ_InitChecksumInfo(void);
static void EepServ_InitHardwareVer(void);
static void EepServ_InitAgingInfo(void);
static void EepServ_InitBrightnessRatio(void);

static MYBOOL smbInitState = FALSE;

/***********************************************************************
* Name        	: EepServ_Init
* Description	: Initial data   
* Parameter		: None
* Returns     	: None
***********************************************************************/
void EepServ_Init(void)
{
	smbInitState = Eep_Adapt_Init();
	if(TRUE == smbInitState)
	{
		EepServ_InitAgingInfo();
		EepServ_InitChecksumInfo();
		EepServ_InitHardwareVer();
		EepServ_InitAgingInfo();
		EepServ_InitBrightnessRatio();
	}
	else
	{
		/* initial failed */
	}
}

/***********************************************************************
* Name        	: EepServ_InitAgingInfo
* Description	: Initial about aging information from dataflash
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void EepServ_InitAgingInfo(void)
{
	
	U8 lubAgingData = E_AGING_STATE_FINISHED;
	MYBOOL lmbAgingState = FALSE;
	/* read aging state */
	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,&lubAgingData,1) == TRUE)
	{
		if((lubAgingData == E_AGING_STATE_FINISHED) || (lubAgingData == E_AGING_STATE_FINISHED_BY_CMD))
		{
			lmbAgingState = FALSE;
		}
		else
		{
			lmbAgingState = TRUE;
		}
		CS_SIG_SyncSend(SIG_BOOL_IS_EXEC_AGING,&lmbAgingState);//update state
	}
	else
	{
		lmbAgingState = TRUE;
		CS_SIG_SyncSend(SIG_BOOL_IS_EXEC_AGING,&lmbAgingState);//update state
	}
	
}

/***********************************************************************
* Name        	: EepServ_InitChecksumInfo
* Description	: Initial about checksum information from dataflash
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void EepServ_InitChecksumInfo(void)
{
	U32 luwChecksum = 0U;

	/* read checksum from eeprom */
	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_CHECKSUM_ADDR,gaubSysCheckSum,2) == TRUE)
	{
		if(((gaubSysCheckSum[0] == 0xFFU) && (gaubSysCheckSum[1] == 0xFFU)) || \
			((gaubSysCheckSum[0] == 0U) && (gaubSysCheckSum[1] == 0U)))
		{
			luwChecksum = CalChecksumForAPPFlash();
			gaubSysCheckSum[0] = luwChecksum & 0xFFU;
	        gaubSysCheckSum[1] = (luwChecksum >> 8u) & 0xFFU;	
			if(EepServ_WriteStaticData(E_BLOCK_0,EEP_CHECKSUM_ADDR,gaubSysCheckSum,2) == TRUE)
			{
				/* do nothing... */
			}
			else
			{
				/* operation failed */
			}
		}
		else
		{
			/* operation failed */
		}
	}
	else
	{
		/* operation failed */
	}

}

/***********************************************************************
* Name        	: EepServ_InitHardwareVer
* Description	: Initial about hardware version information from dataflash
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void EepServ_InitHardwareVer(void)
{
	U8 laubGetHWVer[2] = {0U};
	
	/* read hardware version from eeprom */
	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_MAIN_HW_ADDR,laubGetHWVer,2) == TRUE)
	{
		if(((laubGetHWVer[0] == 0xFFU) && (laubGetHWVer[1] == 0xFFU)) || \
			((laubGetHWVer[0] == 0U) && (laubGetHWVer[1] == 0U)))
		{
			//no write hardware version in eeprom,use default value
		}
		else
		{
			memcpy(gcHWVersion,laubGetHWVer,2); 		  //set new hardware version
		}
	}
	else
	{
		/* failed,use default setting */
	}
	
	/* read internal hardware version from eeprom */
	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_INTERNAL_HW_ADDR,laubGetHWVer,2) == TRUE)
	{
		if((laubGetHWVer[0] == 0xFFU) && (laubGetHWVer[1] == 0xFFU))
		{
			//no write hardware version in eeprom,use default value
		}
		else
		{
			memcpy(gcHWVersion + 3,laubGetHWVer,2); 	//set hardware internal version
		}
	}
	else
	{
		/* failed,use default value */
	}
}


/***********************************************************************
* Name        	: EepServ_InitBrightnessRatio
* Description	: Initial birghtness ratio from dataflash   
* Parameter		: None
* Returns     	: None
***********************************************************************/
static void EepServ_InitBrightnessRatio(void)
{
	U8 lubBLRatioValue = 0;
	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_BRIGHTNESS_RATIO_ADDR,&lubBLRatioValue,1) == TRUE)
	{
		if((lubBLRatioValue < BRIGHTNESS_RATIO_MIN) || (lubBLRatioValue > BRIGHTNESS_RATIO_MAX))
		{
			lubBLRatioValue = BRIGHTNESS_RATIO_TYPE;
		}
		else
		{
			/* do nothing... */
		}
	}
	else
	{
		lubBLRatioValue = BRIGHTNESS_RATIO_TYPE;
	}

	CS_SIG_SyncSend(SIG_CHAR_BRIGHTNESS_RATIO,&lubBLRatioValue);

}

/***********************************************************************
* Name        	: EepServ_ReadStaticData
* Description	: Read directly from flash 
* Parameter		: BlockId:Compatible platforms,
*				  offset:offset of the eeprom start address,
*				  data:start address of data to write,
*				  len:the length of buffer.
* Returns     	: None
***********************************************************************/
MYBOOL EepServ_ReadStaticData(U8 BlockId, U8 offset, U8* data, U8 len)
{
	MYBOOL lmbRet =FALSE;
	if(TRUE == smbInitState)//check initial
	{
		lmbRet = Eep_Adapt_Read(BlockId,offset,data,len);
	}
	else
	{
		smbInitState = Eep_Adapt_Init();
		lmbRet = smbInitState == TRUE ? Eep_Adapt_Read(BlockId,offset,data,len) : FALSE;
	}

	return lmbRet;
}

/***********************************************************************
* Name        	: EepServ_ReadStaticData
* Description	: Write directly to flash 
* Parameter		: BlockId:Compatible platforms,
*				  offset:offset of the eeprom start address,
*				  data:start address of data to read,
*				  len:the length of buffer.
* Returns     	: None
***********************************************************************/
MYBOOL EepServ_WriteStaticData(U8 BlockId, U8 offset, U8* data, U8 len)
{
	MYBOOL lmbRet =FALSE;
	if(TRUE == smbInitState)//check initial
	{
		lmbRet = Eep_Adapt_Write(BlockId,offset,data,len);
	}
	else
	{
		smbInitState = Eep_Adapt_Init();
		lmbRet = smbInitState == TRUE ? Eep_Adapt_Write(BlockId,offset,data,len) : FALSE;
	}
	
	return lmbRet;
}

/***********************************************************************
* Name        	: EepServ_SaveDynamicData
* Description	: Save Dynamic data to flash
* Parameter		: None
* Returns     	: None
***********************************************************************/
void EepServ_SaveDynamicData(void)
{
	/* do nothing... */
}


/***********************************************************************
* Name        	: EepServ_ReadDynamicData
* Description	: Write directly to flash 
* Parameter		: id:PD id,
*				  data:start address of data to read,
*				  len:the length of buffer.
* Returns     	: None
***********************************************************************/
MYBOOL EepServ_ReadDynamicData(U8 id, U8* data, U8 len)
{
	/* unused */
	UNUSED(id);
	UNUSED(data);
	UNUSED(len);

	return TRUE;
}

/***********************************************************************
* Name        	: EepServ_WriteDynamicData
* Description	: Write directly to flash 
* Parameter		: BlockId:PD id,
*				  data:start address of data to read,
*				  len:the length of buffer.
* Returns     	: None
***********************************************************************/
MYBOOL EepServ_WriteDynamicData(U8 id, U8* data, U8 len)
{
	/* unused */
	UNUSED(id);
	UNUSED(data);
	UNUSED(len);

	return TRUE;
}

