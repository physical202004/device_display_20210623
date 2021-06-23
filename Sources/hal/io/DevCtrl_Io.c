/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : IO Device
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/08/21  Huang Haoxiong      Initial
*************************************************************************************************************/
#include "DevCtrl_Io.h"
#include "Global_Definition.h"


static IO_IN_DATA_S  ssInData =
{
    {0U},
    {0U},
    {0U},
};


/***********************************************************************
* Name			: DevCtrl_IO_Init
* Description	: Initial
* Parameter 	: None
* Returns		: None
***********************************************************************/
void DevCtrl_IO_Init(void)
{

    DEV_IO_NAME_T ltLoops = 0U;
	
	/* initial input port */
	for(ltLoops = 0U;ltLoops < (U16)E_IO_IN_NUM;ltLoops++)
    {
		ssInData.RawValue[ltLoops] = gsIoInPort[ltLoops].InitialValue;
		ssInData.ResultValue[ltLoops] = gsIoInPort[ltLoops].InitialValue;
		ssInData.FilterCounter[ltLoops] = 0;
		DIO_SETDIRIN(gsIoInPort[ltLoops].PinConfig.Port,gsIoInPort[ltLoops].PinConfig.Pin);
    }
	
	for(ltLoops = 0U;ltLoops < (U16)E_IO_OUT_NUM;ltLoops++)
    {
		DIO_SETDIROUT(gsIoOutPort[ltLoops].PinConfig.Port,gsIoOutPort[ltLoops].PinConfig.Pin);
		DevCtrl_IO_WriteValue(ltLoops,gsIoOutPort[ltLoops].initValue);
    }

}


/***********************************************************************
* Name			: DevCtrl_IO_Deinit
* Description	: Deinit
* Parameter 	: None
* Returns		: None
***********************************************************************/
void DevCtrl_IO_Deinit(void)
{
#if 0
    U16 Loops = 0U;

	for( Loops = 0U; Loops < (U16)E_IO_IN_NUM; Loops++)
    {
		ssInData.RawValue[Loops] = gsIoInPort[Loops].InitialValue;
        ssInData.ResultCounter[Loops] = 0U;
		ssInData.ResultValue[Loops] = gsIoInPort[Loops].InitialValue;
    }

#endif
}


/***********************************************************************
* Name			: DevCtrl_IO_CycleTask_10ms
* Description	: Cycle task for 10ms
* Parameter 	: None
* Returns		: None
***********************************************************************/
void DevCtrl_IO_CycleTask_10ms(void)
{

    U16 Index = 0U;
    DEV_IO_8BIT_T ltValue = LOW_LEVEL;

	for(Index = 0U; Index< E_IO_IN_NUM; Index++)
    {
        ltValue = DevCtrl_IO_GetRawValue(Index);	//get value
        if(ltValue != DEV_IO_INVALID)
        {
            if(ltValue != ssInData.RawValue[Index])			//update
            {
            	ssInData.FilterCounter[Index] = 1U;
                ssInData.RawValue[Index] = ltValue;
            }
            else
            {
	            if(ssInData.FilterCounter[Index] >= gsIoInPort[Index].FilterTime)
	            {
	                ssInData.ResultValue[Index] = ltValue;
	            }
	            else
	            {
	            	ssInData.FilterCounter[Index]++;
	            }
            }
        }
        else
        {
            /*nothing to do*/
        }
    }

}

/***********************************************************************
* Name			: DevCtrl_IO_GetRawValue
* Description	: IO Read from stroe
* Parameter 	: None
* Returns		: DEV_IO_8BIT_T
***********************************************************************/
DEV_IO_8BIT_T DevCtrl_IO_GetRawValue(DEV_IO_NAME_T ioName)
{
	DEV_IO_8BIT_T ltValue = DEV_IO_INVALID;
	if(ioName >= E_IO_IN_NUM)
	{
		/* do nothing... */
	}
	else
	{
		ltValue = DIO_READ(gsIoInPort[ioName].PinConfig.Port,gsIoInPort[ioName].PinConfig.Pin);	//read level
		ltValue = ltValue == LOW_LEVEL ? LOW_LEVEL : HIGH_LEVEL;
	}
	return ltValue;
}

/***********************************************************************
* Name			: DevCtrl_IO_GetFilterValue
* Description	: IO Read
* Parameter 	: None
* Returns		: DEV_IO_8BIT_T
***********************************************************************/
DEV_IO_8BIT_T DevCtrl_IO_GetFilterValue(DEV_IO_NAME_T ioName)
{

	DEV_IO_8BIT_T ltValue = DEV_IO_INVALID;

    if(ioName >= E_IO_IN_NUM)
    {
		/* do nothing... */
    }
	else
	{
		ltValue = ssInData.ResultValue[ioName];
	}
	
	return ltValue;

}

/***********************************************************************
* Name			: DevCtrl_IO_WriteValue
* Description	: IO Write
* Parameter 	: None
* Returns		: None
***********************************************************************/
void DevCtrl_IO_WriteValue(DEV_IO_NAME_T ioName,DEV_IO_8BIT_T value)
{
    if(ioName >= E_IO_OUT_NUM)
    {
        /* do nothing... */
    }
	else
	{

		if(value != LOW_LEVEL)
	    {
	        DIO_SET(gsIoOutPort[ioName].PinConfig.Port,gsIoOutPort[ioName].PinConfig.Pin); 
	    }
	    else
	    {
	        DIO_CLEAR(gsIoOutPort[ioName].PinConfig.Port,gsIoOutPort[ioName].PinConfig.Pin); 
	    }
	}
}




