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
* Version       Date    Name            Changes and comments
*-----------------------------------------------------------------------
* 0.1    Aug 11,2016   LIU Chenxi    Initial version
*----------------------------------------------------------------------
* 0.2    Dec 20,2016   ShangQiuju   Remove unused include files.
*----------------------------------------------------------------------
* 0.3    Dec 21,2016   Peijingli        Change file include path
*----------------------------------------------------------------------
* 0.4    Jan 05,2017   ShangQiuju        Update sync and async function.
*----------------------------------------------------------------------
* 0.5    Jan 24,2017   ZhangJie        FIX Coverity warnings.
*----------------------------------------------------------------------
* 0.6	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type

*=====================================================================*/
#include "Sig_Combine.h"

SIGNAL_MYBOOL_8BIT_T gtSigBool[E_SIG_MYBOOL_NUM] = {FALSE};
SIGNAL_CHAR_8BIT_T gtSigChar[E_SIG_CHAR_NUM] = {0};
SIGNAL_SHORT_16BIT_T gtSigShort[E_SIG_SHORT_NUM] = {0};
SIGNAL_FLOAT_32BIT_T gtSigFloat[E_SIG_FLOAT_NUM] = {0};
SIGNAL_ENUM_32BIT_T gtSigEnum[E_SIG_ENUM_NUM] = {0};
SIGNAL_INT_32BIT_T gtSigInteger[E_SIG_INT_NUM] = {0};


/****************************************** boolean ******************************************/
/***********************************************************************
* Name        	: Sig_BoolGetValue
* Description 	: Get type of bool signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_BoolGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_MYBOOL_NUM)
    {
        /* do nothing */
    }
	else
	{
		*(SIGNAL_MYBOOL_8BIT_T *)value = gtSigBool[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_BoolSetValue
* Description 	: Set type of bool signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_BoolSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_MYBOOL_NUM)
    {
        /* do nothing */
    }
	else
	{
    	gtSigBool[sigName] = *(SIGNAL_MYBOOL_8BIT_T *)value;
	}
}

/****************************************** character ******************************************/
/***********************************************************************
* Name        	: Sig_CharGetValue
* Description 	: Get type of char signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_CharGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_CHAR_NUM)
    {
	    /* do nothing */
    }
	else
	{
    	*(SIGNAL_CHAR_8BIT_T *)value = gtSigChar[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_CharSetValue
* Description 	: Set type of char signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_CharSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_CHAR_NUM)
    {
	    /* do nothing */
    }
	else
	{
    	gtSigChar[sigName] = *(SIGNAL_CHAR_8BIT_T *)value;
	}
}

/****************************************** short ******************************************/
/***********************************************************************
* Name        	: Sig_ShortGetValue
* Description 	: Get type of short signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_ShortGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_SHORT_NUM)
    {
	    /* do nothing */
    }
	else
	{
		*(SIGNAL_SHORT_16BIT_T *)value = gtSigShort[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_ShortSetValue
* Description 	: Set type of short signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_ShortSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_SHORT_NUM)
    {
	    /* do nothing */
    }
	else
	{
    	gtSigShort[sigName] = *(SIGNAL_SHORT_16BIT_T *)value;
	}
}

/****************************************** float ******************************************/
/***********************************************************************
* Name        	: Sig_FloatGetValue
* Description 	: Get type of float signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_FloatGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_FLOAT_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	*(SIGNAL_FLOAT_32BIT_T *)value = gtSigFloat[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_FloatSetValue
* Description 	: Set type of float signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_FloatSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_FLOAT_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	gtSigFloat[sigName] = *(SIGNAL_FLOAT_32BIT_T *)value;
	}
}

/****************************************** enumeration ******************************************/

/***********************************************************************
* Name        	: Sig_EnumGetValue
* Description 	: Get type of enum signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_EnumGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_ENUM_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	*(SIGNAL_ENUM_32BIT_T *)value = gtSigEnum[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_EnumSetValue
* Description 	: Set type of enum signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_EnumSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_ENUM_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	gtSigEnum[sigName] = *(SIGNAL_ENUM_32BIT_T *)value;
	}
}

/****************************************** integer ******************************************/
/***********************************************************************
* Name        	: Sig_IntegerGetValue
* Description 	: Set type of integer signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_IntegerGetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_INT_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	*(SIGNAL_INT_32BIT_T *)value = gtSigInteger[sigName];
	}
}

/***********************************************************************
* Name        	: Sig_IntegerSetValue
* Description 	: Set type of integer signal value
* Description	: sigName,value
* Returns     	: None
***********************************************************************/
void Sig_IntegerSetValue(SIGNAL_NAME_T sigName,const void* value)
{
    if(sigName >= E_SIG_INT_NUM)
	{
	    /* do nothing */
    }
	else
	{
    	gtSigInteger[sigName] = *(SIGNAL_INT_32BIT_T *)value;
	}
}

