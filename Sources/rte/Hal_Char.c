/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        :
* Module        :
* Instance      :
* Description   :
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    Jun 26,2016                      Initial version
* ----------------------------------------------------------------------
* 0.2    Jan 04,2017   ShangQiuju  Change function HALChar_DevPowerIsStatus to HALChar_DevGetPowerStatus
*------------------------------------------------------------------------
* 0.3    Jan 24,2017      ZhangJie   Fix Coverity warnings.
* ----------------------------------------------------------------------
* 0.4	Dec 19,2018		Huang,Haoxiong	improve HAL structure
*=====================================================================*/
#include "Hal_Char.h"

/***********************************************************************
* Name        	: HALChar_Init
* Description 	: Initial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void HALChar_Init(U8 DeviceId)
{
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_Init != NULL))
	{
	    ssDevChar[DeviceId]->hal_Init();
	}
}


/***********************************************************************
* Name        	: HALChar_DeInit
* Description 	: Deinitial
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void HALChar_DeInit(U8 DeviceId)
{
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_DeInit != NULL))
	{
	    ssDevChar[DeviceId]->hal_DeInit();
	}
}


/***********************************************************************
* Name        	: HALChar_Open
* Description 	: open dev
* Parameter   	: DeviceId
* Returns     	: MYBOOL
***********************************************************************/
MYBOOL HALChar_Open(U8 DeviceId)
{
	MYBOOL lbRet = FALSE;

	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_open != NULL))
	{
	    lbRet = ssDevChar[DeviceId]->hal_open();
	}
	return lbRet;
}


/***********************************************************************
* Name        	: HALChar_Close
* Description 	: close dev
* Parameter   	: DeviceId
* Returns     	: MYBOOL
***********************************************************************/
MYBOOL HALChar_Close(U8 DeviceId)
{
	MYBOOL lbRet = FALSE;
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_close != NULL))
	{
	    lbRet = ssDevChar[DeviceId]->hal_close();
	}
	return lbRet;
}


/***********************************************************************
* Name        	: HALChar_Read
* Description 	: read dev
* Parameter   	: DeviceId,buffer,len
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALChar_Read(U8 DeviceId, U8* buffer, U16 Len)
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_read != NULL))
	{
    	leRet = ssDevChar[DeviceId]->hal_read(buffer,Len);
	}
	return leRet;
}


/***********************************************************************
* Name        	: HALChar_Write
* Description 	: write dev
* Parameter   	: DeviceId,buffer,len
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALChar_Write(U8 DeviceId, U8* buffer, U16 Len )
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_write != NULL))
	{
    	leRet = ssDevChar[DeviceId]->hal_write(buffer,Len);
	}
	return leRet;
}


/***********************************************************************
* Name        	: HALChar_IoControl
* Description 	:
* Parameter   	: DeviceId,type,buffer
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALChar_IoControl(U8 DeviceId, U16 type, void* buffer)
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
	if((E_DEVCHAR_NUM > 0) && (DeviceId < E_DEVCHAR_NUM) && (ssDevChar[DeviceId]->hal_iocontrol != NULL))
	{
    	leRet = ssDevChar[DeviceId]->hal_iocontrol(type,buffer);
	}
	return leRet;
}

/**************** END OF FILE *****************************************/

