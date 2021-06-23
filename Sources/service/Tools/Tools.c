/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Tools
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/08/21
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/21  Huang Haoxiong      Porting from DP2 platform
*************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Tools.h"
#include "Global_Definition.h"

/***********************************************************************
* Name        	: GetMaxValue
* Description 	: Get maximum value in array
* Parameter   	: None
* Returns     	: max value
***********************************************************************/
U8 GetMaxValue(U8 *src,U8 size)
{
    U8 i = 0U;
    U8 lubMax = src[0];
    for(i=1U;i<size;i++)
    {
		if(lubMax < src[i])
		{
		   lubMax = src[i];
		}
		else
		{
			continue;
		}
    }
    return lubMax;
}

/***********************************************************************
* Name        	: GetMinValue
* Description 	: Get minimum value in array
* Parameter   	: None
* Returns     	: minimum value
***********************************************************************/
U8 GetMinValue(U8 *src,U8 size)
{
    U32 i = 0U;
    U8 lubMin = src[0];
	for(i = 1U;i < size;i++)
	{
		if(lubMin > src[i])
		{
		   lubMin = src[i];
		}
		else
		{
			continue;
		}
	}
    return lubMin;
}

/***********************************************************************
* Name        	: NumToASC
* Description 	: Number change to ascii
* Parameter   	: data:number
* Returns     	: converted value
***********************************************************************/
U8 NumToASC(U8 data)
{
	U8 lubRet = 0U;
	if(data <= MAX_NUM)
	{
		lubRet = data + NUM_BASE;
	}
	else
	{
		lubRet = (data-MIN_ALBA) + ALBA_BASE;
	}
	return lubRet;
}

/***********************************************************************
* Name        	: ASCToNum
* Description 	: Ascii change to number
* Parameter   	: data:number
* Returns     	: converted value
***********************************************************************/
U8 ASCToNum(U8 data)
{
	U8 lubRet = 0U;
	if((data >= (MIN_NUM + NUM_BASE)) && (data <= (MAX_NUM + NUM_BASE)))
	{
		lubRet = data - NUM_BASE;
	}
	else
	{
		lubRet = (data- ALBA_BASE) + MIN_ALBA;
	}
	
	return lubRet;
}

/***********************************************************************
* Name        	: BCDEncode
* Description 	: Encode BCD
* Parameter   	: src:source data
				  len:length of data
				  tar:target data
* Returns     	: None
***********************************************************************/
void BCDEncode(U8 *src, U8 len, U8* tar)
{
	U8 i = 0U;
	U8 lubTemp = 0U;
	if((src == NULL) || (len == 0U))
	{
		/* do nothing... */
	}
	else
	{
		for(i = 0U; i < len; i++)
		{
			lubTemp = (src[i] & HIGH_MASK) >> BCD_OFFSET;
			tar[i * 2U] = NumToASC(lubTemp);

			lubTemp = src[i] & LOW_MASK;
			tar[i * 2 + 1U] = NumToASC(lubTemp);
		}
	}
}

/***********************************************************************
* Name        	: BCDDecode
* Description 	: Decode BCD
* Parameter   	: src:source data
				  len:length of data
				  tar:target data
* Returns     	: None
***********************************************************************/
void BCDDecode(U8 *src, U8 len, U8 *tar)
{
	U8 i = 0U;
	U8 lubTemp = 0U;
	if((src == NULL) || (len == 0U))
		return;

	for(i = 0U; i < len; i = i + 2U)
	{
		lubTemp = ASCToNum(src[i]);
		tar[i/2U] = lubTemp << BCD_OFFSET;

		lubTemp = ASCToNum(src[i + 1U]);
		tar[i/2U] += lubTemp;
	}

}

/***********************************************************************
* Name        	: DelayUs
* Description 	: delay us
* Parameter   	: period:unit by 1us
* Returns     	: None
***********************************************************************/
void DelayUs(U16 period)
{
    U32 i = (U32)period * 3;

    for(;i > 0;i--)
    {
		;
    }
}

/***********************************************************************
* Name        	: DelayMs
* Description 	: delay ms
* Parameter   	: period:unit by 1ms
* Returns     	: None
***********************************************************************/
void DelayMs(U16 period)
{
    U16 i = 0;
	for(i = 0;i < period;i++)
	{
    	DelayUs(1000);
	}
}

/***********************************************************************
* Name        	: MemcpyU16Array
* Description 	: Memcpy for U16 array
* Parameter   	: dest:destination addr
				  src:srouce addr
				  len:trans length
* Returns     	: None
***********************************************************************/
void MemcpyU16Array(U16* dest, U16* src, U16 len)
{
	U16 i = 0U;

	for(i = 0U;i < len;i++)
	{
		*dest = *src;
		dest++;
		src++;
	}
}

/***********************************************************************
* Name        	: SortU16ArrayMinToMax
* Description 	: Sort min to max for U16 array
* Parameter   	: ArrayName:array addr
				  Len:length of array
* Returns     	: None
***********************************************************************/
void SortU16ArrayMinToMax(U16* ArrayName,U16 Len)
{
    U16 Loops = 0U;
    U16 Index = 0U;
    U16 temp = 0U;

    if(NULL != ArrayName)
    {
        for(Loops = 0U; Loops < (Len - 1U); Loops++ )
        {
            for(Index = Loops + 1U; Index < Len; Index++ )
            {
                if(ArrayName[Loops] > ArrayName[Index])
                {
                    temp = ArrayName[Index];
                    ArrayName[Index] = ArrayName[Loops];
                    ArrayName[Loops] = temp;
                }
            }
        }
    }
}


/***********************************************************************
* Name        	: SequenceHandler
* Description 	: power sequence management
* Parameter   	: Log:sequence info
				  TimeInfo:time info
				  ActionInfo:action info
				  PowerState:Need to execute power on/down state
				  Len:sequence length
* Returns     	: TRUE:sequence execute completed,FALSE:sequence executing
***********************************************************************/
MYBOOL SequenceHandler(S_SEQUENCE_LOG* Log, S_SEQUENCE_PROPERTIME *TimeInfo, S_SEQUENCE_PROPERACTION *ActionInfo, U8 PowerState, U16 Len)
{

	MYBOOL lbRet = FALSE;
	MYBOOL lbFuncRet = FALSE;
	U8 *lupbStepCnt = NULL;
	U8 *lupbCycleCnt = NULL;

	if(PowerState == EXE_POWER_ON )
	{
		lupbStepCnt = &Log->PowerOnStepCnt;
		lupbCycleCnt = &Log->PowerOnCycleCnt;
	}
	else
	{
		lupbStepCnt = &Log->PowerOffStepCnt;
		lupbCycleCnt = &Log->PowerOffCycleCnt;
	}
	if(((*lupbStepCnt) == 0U) && ((*lupbCycleCnt) == 0U))
	{

		CHECK_FUNC_POINTER(ActionInfo[*lupbStepCnt].Action,lbFuncRet);
		if(lbFuncRet == FALSE)
		{
			lbRet = FALSE;
		}
		else
		{
			(*lupbCycleCnt) += 1U;
			while(0U == TimeInfo[*lupbStepCnt])
			{
				//Is the last stage?
				if((*lupbStepCnt) < (Len - 1U))
				{
					*lupbCycleCnt = 0U;
					CHECK_FUNC_POINTER(ActionInfo[(*lupbStepCnt) + 1].Action,lbFuncRet);//if action is NULL,return TRUE.Otherwise return result of action
					if(lbFuncRet == TRUE)
					{
						(*lupbStepCnt) += 1U;
					}
					else
					{
						lbRet = FALSE;
						break;
					}
				}
				else
				{
					//finish and quit sequence and deinitialize
					INIT_SEQUENCE_LOG((*Log));
					lbRet = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		(*lupbCycleCnt) += 1U;
		if((*lupbCycleCnt) >= TimeInfo[*lupbStepCnt])
		{
			//Is the last stage?
			if((*lupbStepCnt) < (Len - 1U))
			{
				*lupbCycleCnt = 0U;
				CHECK_FUNC_POINTER(ActionInfo[(*lupbStepCnt) + 1].Action,lbFuncRet);
				if(lbFuncRet == FALSE)
				{
					lbRet = FALSE;
				}
				else
				{
					(*lupbStepCnt) += 1U;
					while(0U == TimeInfo[*lupbStepCnt])
					{
						//Is the last stage?
						if((*lupbStepCnt) < (Len - 1U))
						{

							*lupbCycleCnt = 0U;
							CHECK_FUNC_POINTER(ActionInfo[(*lupbStepCnt) + 1].Action,lbFuncRet);//if action is NULL,return TRUE.Otherwise return result of action
							if(lbFuncRet == TRUE)
							{
								(*lupbStepCnt) += 1U;
							}
							else
							{
								lbRet = FALSE;
								break;
							}
						}
						else
						{
							//finish and quit sequence and deinitialize
							INIT_SEQUENCE_LOG((*Log));
							lbRet = TRUE;
							break;
						}
					}
				}
			}
			else
			{
				//finish and quit sequence and deinitialize
				INIT_SEQUENCE_LOG((*Log));
				lbRet = TRUE;
			}
		}
		else
		{
			/* do nothing... */
		}
	}
	return lbRet;
	
}

/***********************************************************************
* Name        	: FrameChecksum
* Description 	: Calculation frame checksum
* Parameter   	: frame:frame array
				  len:leng of frame
* Returns     	: Frame checksum
***********************************************************************/
U8 FrameChecksum(U8* frame,U16 len)
{
    U8 sum = 0U;
	for(;len > 0U;len--)
	{
		sum += *frame;
		frame++;
	}
	
    return sum;
}

/***********************************************************************
* Name        	: CalChecksumForAPPFlash
* Description 	: Calculation checksum for APP section
* Parameter   	: None
* Returns     	: Checksum
***********************************************************************/
U32 CalChecksumForAPPFlash(void)
{

    U32 luwChecksumResult = 0U;
    U8* lpStartAddress = NULL;

	for(lpStartAddress = (U8*)(APP_START_ADDRESS); lpStartAddress <= (U8*)(APP_END_ADDRESS);lpStartAddress++)
	{
		luwChecksumResult += *(lpStartAddress);
	}
	
    return luwChecksumResult;
}

/**************** END OF FILE *****************************************/

