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
* 0.2    Jan 04,2017   ShangQiuju  Change function HALBlock_DevPowerIsStatus to
                                                  HALBlock_DevGetPowerStatus
*------------------------------------------------------------------------
* 0.3    Jan 24,2017  ZhangJie  Fix Coverity warnings.
* ----------------------------------------------------------------------
* 0.4	Dec 19,2018		Huang,Haoxiong	improve HAL structure
*=====================================================================*/
#include "Hal_Block.h"


/***********************************************************************
* Name 			: HALBlock_Init
* Description	:
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void HALBlock_Init(U8 DeviceId)       //20150831
{
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
	    if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_Init !=  NULL))
	    {
			ssDevBlock[DeviceId]->hal_block_Init();
	    }
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
}


/***********************************************************************
* Name 			: HALBlock_DeInit
* Description	:
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void HALBlock_DeInit(U8 DeviceId)
{
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
	    if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_DeInit !=  NULL))
	    {
			ssDevBlock[DeviceId]->hal_block_DeInit();
	    }
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}

}


/***********************************************************************
* Name 			: HALBlock_Open
* Description	: open dev
* Parameter   	: DeviceId
* Returns     	: MYBOOL
***********************************************************************/
MYBOOL HALBlock_Open(U8 DeviceId)
{
	MYBOOL lbRet = FALSE;
	
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
		if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_open !=  NULL))
		{
			lbRet = ssDevBlock[DeviceId]->hal_block_open();
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
	
	return lbRet;
}

/***********************************************************************
* Name 			: HAL_Close
* Description	: close dev
* Parameter   	: DeviceId
* Returns     	: MYBOOL
***********************************************************************/
MYBOOL HALBlock_Close(U8 DeviceId)
{
	MYBOOL lbRet = FALSE;
		
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
		if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_close != NULL))
		{
			lbRet = ssDevBlock[DeviceId]->hal_block_close();
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
	
	return lbRet;
}

/***********************************************************************
* Name 			: HALBlock_Read
* Description	:
* Parameter   	: DeviceId,address,buffer,Len
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALBlock_Read(U8 DeviceId, U32 address,U8* buffer, U16 Len)
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
		
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
		if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_read != NULL))
		{
	    	leRet = ssDevBlock[DeviceId]->hal_block_read(address,buffer, Len);
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
	
	return leRet;
	
}

/***********************************************************************
* Name 			: HALBlock_Write
* Description	:
* Parameter   	: DeviceId,address,buffer,len
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALBlock_Write(U8 DeviceId, U32 address,U8* buffer, U16 Len)
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
			
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
		if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_write != NULL))
		{
	    	leRet = ssDevBlock[DeviceId]->hal_block_write(address,buffer, Len);
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
	
	return leRet;
}

/***********************************************************************
* Name 			: HALBlock_IoControl
* Description	:
* Parameter   	: DeviceId,Type,buffer
* Returns     	: E_HAL_STATE
***********************************************************************/
E_HAL_STATE HALBlock_IoControl(U8 DeviceId, U16 type, void* buffer)
{
	E_HAL_STATE leRet = E_HAL_STATE_FAIL;
			
	if(DeviceId > (U8)E_DEVBLOCK_START)
	{
		DeviceId = DeviceId - ((U8)E_DEVBLOCK_START + 1u);
		if((DeviceId < HAL_DEVBLOCK_MAX) && (ssDevBlock[DeviceId]->hal_block_iocontrol != NULL))
		{
	    	leRet = ssDevBlock[DeviceId]->hal_block_iocontrol(type,buffer);
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		/* do nothing */
	}
	
	return leRet;
}

/**************** END OF FILE *****************************************/

