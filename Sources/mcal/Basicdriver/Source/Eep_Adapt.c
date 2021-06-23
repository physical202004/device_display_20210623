/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : eeprom adapter
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
#include "Eep_Adapt.h"
#include "flash.h"


static MYBOOL Eep_Adapt_Erase(U16 offset,U8 len);

/***********************************************************************
* Name        	: Eep_Adapt_Init
* Description	: Init driver, do initialization and start-up.        
* Parameter		: None
* Returns     	: None
***********************************************************************/
MYBOOL Eep_Adapt_Init(void)
{
	MYBOOL lmbRet= FALSE;

	lmbRet = FLASH_Init(SYS_CLK_HZ) == FLASH_ERR_SUCCESS ? TRUE : FALSE;

	return lmbRet;
}

/***********************************************************************
* Name        	: Eep_Adapt_DeInit
* Description 	: Deinitial data
* Parameter		: None
* Returns     	: TRUE
***********************************************************************/
MYBOOL Eep_Adapt_DeInit(void)
{
    return TRUE;
}

/***********************************************************************
* Name        	: Eep_Adapt_Erase
* Description	: Erase data from eeprom        
* Parameter		: offset:offset of the eeprom start address,len:data length
* Returns     	: None
***********************************************************************/
static MYBOOL Eep_Adapt_Erase(U16 offset,U8 len)
{
	U8 i = 0;
	MYBOOL lmbRet = FALSE;
	
	if((len % EEP_RW_MIN_UNIT) != 0)//only can erase double size
	{
	    lmbRet = FALSE;
	}
	else
	{
	    /* do nothing... */
	}
	for(i = 0;i < len;i += EEP_RW_MIN_UNIT)
	{
	    if(FLASH_ERR_SUCCESS != EEPROM_EraseSector(offset + i))
	    {
			lmbRet = FALSE;
			break;
	    }
		else
		{
			lmbRet = TRUE;
		}
	}
	return lmbRet;
}


/***********************************************************************
* Name			: Eep_Adapt_Write
* Description	: Write the eeprom, in order to fix app interface, add the erase action before writing
* Parameter 	: BlockId:Compatible platforms,
*				  offset:offset of the eeprom start address,
*				  buf:start address of data to write,
*				  len:the length of buffer.
* Returns		: TRUE:sucessful,FALSE:failed
***********************************************************************/
MYBOOL Eep_Adapt_Write(U8 BlockId,U16 offset,U8* buf,U8 len)
{
	const U32 luwStartAddr = EEPROM_START_ADDRESS + offset;
    BOOL lmbRet =FALSE;

    if(buf == NULL)
    {
        lmbRet = FALSE;
    }
    else
    {  
        /*calculate offset address on the base of ID, no IFC ID use first 56 byte space, the remain spase is used to IFC*/
        if((offset + len) > (EEP_MAX_SIZE - 1))
        {
			lmbRet = FALSE;
        }
		else
		{
			if(TRUE == Eep_Adapt_Erase(luwStartAddr,len))
			{
				lmbRet = FLASH_ERR_SUCCESS == EEPROM_Program(luwStartAddr,buf,len) ? TRUE : FALSE;
			}
			else
			{
				lmbRet = FALSE;
			}
		}
    }
	UNUSED(BlockId);
    return lmbRet;
}

/***********************************************************************
* Name			: Eep_Adapt_Read
* Description	: Read data from eeprom
* Parameter 	: BlockId:Compatible platforms,
*				  offset:the offset of the eeprom start address,
*				  buf:start address of data to write,
*				  len:the length of buffer.
* Returns		: TRUE:sucessful,FALSE:failed
***********************************************************************/
MYBOOL Eep_Adapt_Read(U8 BlockId,U16 offset,U8* buf,U8 len)
{
    MYBOOL lmbRet =FALSE;
    U8 index = 0;
    
    if(buf == NULL)
    {
        lmbRet = FALSE;
    }
    else
    {  
        /*calculate offset address on the base of startadress*/
        if((offset + len) > EEP_MAX_SIZE)
        {
			lmbRet = FALSE;
        }
		else
		{
	        for(index = 0;index < len; index++)
	        {
	            buf[index] = *((uint8_t *)(EEPROM_START_ADDRESS + offset + index));
	        }
	        lmbRet = TRUE;
		}
    }
	UNUSED(BlockId);
    return lmbRet;
}

