/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Aging manager
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/11/02
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2020/11/02  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "AgingManager.h"
#include "Global_Definition.h"

static U32 suwAgingCounter = 0U;
static U8 subAgingTime = AGING_DEFAULT_TIME;
static MYBOOL smbExecAgingState = FALSE;

/***********************************************************************
*  Name        	: AgingMgr_NormalEntryHandler
*  Description 	: Entry Handler in normal mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void AgingMgr_NormalEntryHandler(void)
{

	suwAgingCounter = 0U;//clear counter

	CS_SIG_SyncReceive(SIG_BOOL_IS_EXEC_AGING,&smbExecAgingState);//update state
	if(smbExecAgingState == FALSE)
	{
		/* do nothing... */
	}
	else
	{
		if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_TIME_ADDR,&subAgingTime,1) == TRUE)
		{
			if((subAgingTime == 0U) || (subAgingTime == 0xFFU))
			{
				subAgingTime = AGING_DEFAULT_TIME;
			}
			else
			{
				/* do nothing... */
			}
		}
		else
		{
			subAgingTime = AGING_DEFAULT_TIME;
		}
	}
}

/***********************************************************************
*  Name        	: AgingMgr_NormalActiveHandler
*  Description 	: 10ms task in normal mode
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void AgingMgr_NormalActiveHandler(void)
{
	U8 luabAgingState[2] = {0};
	if(smbExecAgingState == TRUE)
	{
	    if(suwAgingCounter < (subAgingTime * AGING_TIME_ONE_MINUTE))//counter
	    {
	        suwAgingCounter++;
	    }
	    else
	    {
		   	luabAgingState[0] = E_AGING_STATE_FINISHED;
            if(EepServ_WriteStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,luabAgingState,2) == TRUE)
            {
	       		smbExecAgingState = FALSE;
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




