/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : diagnose module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/09/18
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2020/09/18  Huang Haoxiong      Initial
*************************************************************************************************************/

#include "DiagServ.h"
#include "EepServ_if.h"

/* ifc buffer */
static U8 saubDiagIfcData[DIAGSERV_IFC_LEN] = {0};
static MYBOOL smbDiagIfcRecordState[DIAGSERV_NUM] = {FALSE};

/***********************************************************************
* Name        	: DiagServ_Init
* Description	: Initial data   
* Parameter		: None
* Returns     	: None
***********************************************************************/
void DiagServ_Init(void)
{
	U16 i = 0;
	U16 luhwIfcOffset = 0U;
	U8 *lpubData = NULL;
	if(EepServ_ReadStaticData(E_BLOCK_0,DIAGSERV_IFC_OFFSET_ADDR,saubDiagIfcData,DIAGSERV_IFC_LEN) == TRUE)
	{
		for(;i < DIAGSERV_NUM;i++)//read IFC data
		{
			luhwIfcOffset = i * DIAGSERV_IFC_DATA_LEN;
			lpubData = saubDiagIfcData + luhwIfcOffset;//set IFC buffer offset
			if(lpubData[E_DIAGSERV_IFC_NAME_INDEX] == 0xFFU)//first startup
			{
				lpubData[E_DIAGSERV_IFC_NAME_INDEX] = scubDiagIfcId[i];//set id
				lpubData[E_DIAGSERV_IFC_HISTORY_CNT_INDEX] = 0U;
				(void)EepServ_WriteStaticData(E_BLOCK_0,luhwIfcOffset + DIAGSERV_IFC_OFFSET_ADDR,lpubData,DIAGSERV_IFC_DATA_LEN);//write to flash first times
			}
			else
			{
				/* do nothing... */
			}
		}
	}
	else
	{
		/* do nothing... */
	}
}

/***********************************************************************
* Name        	: DiagServ_CycleTask10ms
* Description	: Cyclic task
* Parameter		: None
* Returns     	: None
***********************************************************************/
void DiagServ_CycleTask10ms(void)
{
	/* reserved */
}

/***********************************************************************
* Name        	: DiagServ_AddIfcInfo
* Description	: Update IFC message for diagName
* Parameter		: diagName:diagnose name
* Returns     	: TRUE:ok,FALSE:failed
***********************************************************************/
MYBOOL DiagServ_AddIfcInfo(DIAGSERV_NAME_8BIT_T diagName)
{
	U8 *lpubData = NULL;
	MYBOOL lmbRet = FALSE;
	if(diagName >= DIAGSERV_NUM)
	{
		/* invalid,return */
	}
	else
	{
		if(smbDiagIfcRecordState[diagName] == TRUE)
		{
			/* It has been recorded */
			lmbRet = TRUE;
		}
		else
		{
			smbDiagIfcRecordState[diagName] = TRUE;
			lpubData = saubDiagIfcData + (diagName * DIAGSERV_IFC_DATA_LEN);//get IFC data
			if(lpubData[E_DIAGSERV_IFC_HISTORY_CNT_INDEX] >= DIAGSERV_IFC_HISTORY_CNT_MAX)
			{
				/* history counter full */
				lmbRet = TRUE;
			}
			else
			{
				lpubData[E_DIAGSERV_IFC_HISTORY_CNT_INDEX]++;//history counter
				/* save ifc data */
				lmbRet = EepServ_WriteStaticData(E_BLOCK_0,(diagName * DIAGSERV_IFC_DATA_LEN) + DIAGSERV_IFC_OFFSET_ADDR,\
											&saubDiagIfcData[diagName * DIAGSERV_IFC_DATA_LEN],DIAGSERV_IFC_DATA_LEN);
			}
		}
	}
	return lmbRet;
}


/***********************************************************************
* Name        	: DiagServ_ReadIfcInfo
* Description	: Read ifc message for diagnose name
* Parameter		: diagName:diagnose name,buf:buffer pointer
* Returns     	: TRUE:ok,FALSE:failed
***********************************************************************/
MYBOOL DiagServ_ReadIfcInfo(DIAGSERV_NAME_8BIT_T diagName,U8 *buf)
{
	MYBOOL lmbRet = FALSE;
	if(diagName >= DIAGSERV_NUM)
	{
		/* invalid,return */
	}
	else
	{
		lmbRet = TRUE;
		memcpy(buf,saubDiagIfcData + (diagName * DIAGSERV_IFC_DATA_LEN),DIAGSERV_IFC_DATA_LEN);
	}
	return lmbRet;
}

/***********************************************************************
* Name        	: DiagServ_ClearIfcInfo
* Description	: Clear ifc message for diagnose name
* Parameter		: diagName:diagnose name
* Returns     	: TRUE:ok,FALSE:failed
***********************************************************************/
MYBOOL DiagServ_ClearIfcInfo(DIAGSERV_NAME_8BIT_T diagName)
{
	U16 luhwIfcOffset = 0U;
	MYBOOL lmbRet = FALSE;
	if(diagName >= DIAGSERV_NUM)
	{
		/* invalid,return */
	}
	else
	{
		luhwIfcOffset = diagName * DIAGSERV_IFC_DATA_LEN;//calculation IFC offset
		memset(&saubDiagIfcData[luhwIfcOffset],0,DIAGSERV_IFC_DATA_LEN);//clear data
		saubDiagIfcData[luhwIfcOffset + E_DIAGSERV_IFC_NAME_INDEX] = scubDiagIfcId[diagName];//set id
		/* update */
		lmbRet = EepServ_WriteStaticData(E_BLOCK_0,luhwIfcOffset + DIAGSERV_IFC_OFFSET_ADDR,&saubDiagIfcData[luhwIfcOffset],DIAGSERV_IFC_DATA_LEN);
	}
	return lmbRet;
}

/***********************************************************************
* Name        	: DiagServ_ClearAllIfcInfo
* Description	: Clear all ifc message
* Parameter		: None
* Returns     	: None
***********************************************************************/
void DiagServ_ClearAllIfcInfo(void)
{
	U16 i = 0;
	for(;i < DIAGSERV_NUM;i++)
	{
		DiagServ_ClearIfcInfo(i);
	}
}

/***********************************************************************
* Name        	: DiagServ_GetIfcQuantity
* Description	: Get ifc quantity
* Parameter		: None
* Returns     	: Return ifc quantity
***********************************************************************/
U8 DiagServ_GetIfcQuantity(void)
{

	return DIAGSERV_NUM;
}

/***********************************************************************
* Name        	: DiagServ_SaveIfcToFlash
* Description	: Save IFC message to falsh
* Parameter		: None
* Returns     	: None
***********************************************************************/
void DiagServ_SaveIfcToFlash(void)
{
	/* do nothing... */	
}

