/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file flash.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing APIs for configuring FLASH module (FLASH). 
*
*******************************************************************************
*
* provide APIs for configuring FLASH module (FLASH)
******************************************************************************/
#include "flash.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* define NVM APIs
*
*//*! @addtogroup nvm_api_list
* @{
*******************************************************************************/
uint16_t EEPROM_EraseVerifySection(uint32_t u32NVMTargetAddress, uint16_t u16ByteCount)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	// Check address to see if it is aligned to 4 bytes
	// Global address [1:0] must be 00.
	if(u32NVMTargetAddress & 0x03)
	{
		u16Err = FLASH_ERR_INVALID_PARAM;
		return (u16Err);
	}
	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_ERASE_VERIFY_SECTION;// erase verify FLASH section command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;
	// Write index to specify the # of longwords to be verified
	FTMRH->FCCOBIX = 0x2;
	// Write the # of longwords 
	FTMRH->FCCOBLO = u16ByteCount;
	FTMRH->FCCOBHI = u16ByteCount>>8;
	
	// Launch the command
	FLASH_LaunchCMD(TRUE);
	
	// Check error status
	if(FTMRH->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
	{
		u16Err |= FLASH_ERR_ACCESS;
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT0;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT1;		
	}
#if 0	
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_SFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_SINGLE_BIT_FAULT;
	}
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_DFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_DOUBLE_BIT_FAULT;
	}	
	
	if(FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
	{
		u16Err |= FLASH_ERR_PROTECTION;		
	}
#endif	
	return (u16Err);
}
/*****************************************************************************//*!
  *
  * @brief program EEPROM routine, each program operation supports up to 4 bytes
  * 		 programming.
  *        
  * @param[in]   u32NVMTargetAddress programed EEPROM address.
  * @param[in]   pData programming data pointer.
  * @param[in]   u16SizeBytes programming data length.
  *
  * @return error status.
  *
  * @ Pass/ Fail criteria: none
*****************************************************************************/

uint16_t EEPROM_Program(uint32_t u32NVMTargetAddress, uint8_t *pData, uint16_t u16SizeBytes)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	uint8_t  u8WrLeftBytes = (u16SizeBytes & 0x3);
	uint16_t u16WrLeftLongWords = u16SizeBytes>>2;
	uint32_t u32WrTargetAddress = u32NVMTargetAddress;
	int  i;
	

	// Loop for 4 bytes programming
	for(i = 0; i < u16WrLeftLongWords; i++)
	{
		u16Err = EEPROM_ProgramUpto4Bytes(u32WrTargetAddress, pData, 4);
		if(u16Err)
		{
			goto EndP;
			//break;
		}
		u32WrTargetAddress += 4;
		pData += 4;
	}
	if(u8WrLeftBytes>0)
	{
		u16Err = EEPROM_ProgramUpto4Bytes(u32WrTargetAddress, pData, u8WrLeftBytes);	
	}
EndP:	
	return (u16Err);
}
/*****************************************************************************//*!
  *
  * @brief program one byte to EEPROM.
  *        
  * @param[in] u32NVMTargetAddress: target address in EEPROM.  
  * @param[in] u8Data:   programming data.
  *	
  * @return error status.
  *
  * @ Pass/ Fail criteria: none
*****************************************************************************/
uint16_t EEPROM_Program1Byte(uint32_t u32NVMTargetAddress, uint8_t u8Data)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	
	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_PROGRAM;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;
	// Write index to specify the byte0 (MSB word) to be programmed
	FTMRH->FCCOBIX = 0x2;
	// Write the byte 0
	FTMRH->FCCOBLO = u8Data;
	// Launch the command
	FLASH_LaunchCMD(TRUE);

	
	// Check error status
	if(FTMRH->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
	{
		u16Err |= FLASH_ERR_ACCESS;
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
	{
		u16Err |= FLASH_ERR_PROTECTION;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT0;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT1;		
	}	
#if 0	
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_SFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_SINGLE_BIT_FAULT;
	}
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_DFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_DOUBLE_BIT_FAULT;
	}
#endif	
	
	return (u16Err);
}
/*****************************************************************************//*!
  *
  * @brief program up to 4 bytes to EEPROM.
  *        
  * @param[in] u32NVMTargetAddress: target address in EEPROM.  
  * @param[in] pData:   programming data pointer.
  * @param[in] u8ByteCount number of programming data.
  *	
  * @return error status.
  *
  * @ Pass/ Fail criteria: none
*****************************************************************************/
uint16_t EEPROM_ProgramUpto4Bytes(uint32_t u32NVMTargetAddress, uint8_t *pData, uint8_t u8ByteCount)
{
	int i;
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	if(u8ByteCount >4 || u8ByteCount == 0)
	{
		u16Err = FLASH_ERR_INVALID_PARAM;
		return (u16Err);		
	}
	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_PROGRAM;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;
	
	for (i = 0; i < u8ByteCount; i++)
	{
		// Write index to specify the byte0 (MSB word) to be programmed
		FTMRH->FCCOBIX = 0x2+i;
		// Write the byte 0
		FTMRH->FCCOBLO = *pData++;
	}
	// Launch the command
	FLASH_LaunchCMD(TRUE);
	
	// Check error status
	if(FTMRH->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
	{
		u16Err |= FLASH_ERR_ACCESS;
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
	{
		u16Err |= FLASH_ERR_PROTECTION;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT0;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT1;		
	}	
#if 0	
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_SFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_SINGLE_BIT_FAULT;
	}
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_DFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_DOUBLE_BIT_FAULT;
	}
#endif	
	return (u16Err);
}

/*****************************************************************************//*!
  *
  * @brief erase EEPROM sector, each EEPROM sector is of 2 bytes long.
  * 		 
  * @param[in]  u32NVMTargetAddress target EEPROM address. 
  *
  * @return error status.
  *
  * @ Pass/ Fail criteria: none.
*****************************************************************************/
uint16_t EEPROM_EraseSector(uint32_t u32NVMTargetAddress)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;

	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_ERASE_SECTOR;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;        
	
	// Launch the command
        FLASH_LaunchCMD(TRUE);
	
	// Check error status
	if(FTMRH->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
	{
		u16Err |= FLASH_ERR_ACCESS;
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
	{
		u16Err |= FLASH_ERR_PROTECTION;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT0;		
	}
	if(FTMRH->FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
	{
		u16Err |= FLASH_ERR_MGSTAT1;		
	}	
#if 0	
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_SFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_SINGLE_BIT_FAULT;
	}
	if(FTMRH->FERSTAT & (FTMRH_FERSTAT_DFDIF_MASK))
	{
		u16Err |= EEPROM_ERR_DOUBLE_BIT_FAULT;
	}
#endif	
	return (u16Err);
}

/*****************************************************************************//*!
  *
  * @brief program one byte to EEPROM, don't wait complete flag.
  *        
  * @param[in] u32NVMTargetAddress: target address in EEPROM.  
  * @param[in] u8Data:   programming data.
  *	
  * @return error status.
  *
  * @ Pass/ Fail criteria: none
*****************************************************************************/
uint16_t EEPROM_Program1Byte_NoWait(uint32_t u32NVMTargetAddress, uint8_t u8Data)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	
	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_PROGRAM;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;
	// Write index to specify the byte0 (MSB word) to be programmed
	FTMRH->FCCOBIX = 0x2;
	// Write the byte 0
	FTMRH->FCCOBLO = u8Data;
	// Launch the command
	FTMRH->FSTAT = 0x80;

	return (u16Err);
}
/*****************************************************************************//*!
  *
  * @brief program up to 4 bytes to EEPROM, don't wait complete flag.
  *        
  * @param[in] u32NVMTargetAddress: target address in EEPROM.  
  * @param[in] pData:   programming data pointer.
  * @param[in] u8ByteCount number of programming data.
  *	
  * @return error status.
  *
  * @ Pass/ Fail criteria: none
*****************************************************************************/
uint16_t EEPROM_ProgramUpto4Bytes_NoWait(uint32_t u32NVMTargetAddress, uint8_t *pData, uint8_t u8ByteCount)
{
	int i;
	uint16_t u16Err = FLASH_ERR_SUCCESS;
	if(u8ByteCount >4 || u8ByteCount == 0)
	{
		u16Err = FLASH_ERR_INVALID_PARAM;
		return (u16Err);		
	}
	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_PROGRAM;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;
	
	for (i = 0; i < u8ByteCount; i++)
	{
		// Write index to specify the byte0 (MSB word) to be programmed
		FTMRH->FCCOBIX = 0x2+i;
		// Write the byte 0
		FTMRH->FCCOBLO = *pData++;
	}
	// Launch the command
	FTMRH->FSTAT = 0x80;
      
	return (u16Err);
}
/*****************************************************************************//*!
  *
  * @brief erase EEPROM sector, each EEPROM sector is of 2 bytes long, don't wait complete flag.
  * 		 
  * @param[in]  u32NVMTargetAddress target EEPROM address. 
  *
  * @return error status.
  *
  * @ Pass/ Fail criteria: none.
*****************************************************************************/
uint16_t EEPROM_EraseSector_NoWait(uint32_t u32NVMTargetAddress)
{
	uint16_t u16Err = FLASH_ERR_SUCCESS;

	// Clear error flags
	FTMRH->FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	FTMRH->FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	FTMRH->FCCOBHI = EEPROM_CMD_ERASE_SECTOR;// EEPROM FLASH command
	FTMRH->FCCOBLO = u32NVMTargetAddress>>16;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	FTMRH->FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	FTMRH->FCCOBHI = u32NVMTargetAddress>>8;
	FTMRH->FCCOBLO = u32NVMTargetAddress;        
	
	// Launch the command
	FTMRH->FSTAT = 0x80;
	
	return (u16Err);
}
