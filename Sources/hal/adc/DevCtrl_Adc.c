/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : ADC Device
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
#include "Tools.h"
#include "Adc_Adapt_if.h"


/* Own headerfiles */
#include "DevCtrl_Adc.h"


static U16 suhwAllAdcFilterTime = 0U;

static DEV_ADC_DATA_S  ssAdcData =
{
	{0U},	//RawValue
	{0U},	//ResultValue
	{0U},	//FilterCounter
	{0U},	//CurrentOffset
};

/***********************************************************************
* Name        	: DevCtrl_ADC_Init
* Description	: Initializes the DevCtrl_ADC module, and calls AD driver initial.
* Parameter		: None
* Returns     	: None
***********************************************************************/
void DevCtrl_ADC_Init(void)
{
	U16 i = 0U;
	U16 luhwIndex = 0U;
	
	for(i = 0U;i < (U16)E_DEV_ADC_NUM;i++)
	{
		suhwAllAdcFilterTime += gsAdcPort[i].FilterTime;				//calculate all active channel's filter time
	}
	
	for(i = 0U,luhwIndex = 0U;i < (U16)E_DEV_ADC_NUM; i++)
	{
		if(luhwIndex < suhwAllAdcFilterTime)			//avoid range out of array
		{
			ssAdcData.CurrentOffset[i] = luhwIndex;		//get offset for signal name
		}
		else
		{
			break;										//range out of array
		}
		luhwIndex += gsAdcPort[i].FilterTime;
	}

	ADC_Adapt_Init();
}

/***********************************************************************
* Name        	: DevCtrl_ADC_Deinit
* Description 	: de-Initializes the DevCtrl_ADC module.
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void DevCtrl_ADC_Deinit(void)
{
#if 0
	U16 luhwIndex = 0u;
	/* clear the raw and filter value buffer */
	for( luhwIndex = 0u; luhwIndex < (U16)E_DEV_ADC_NUM; luhwIndex++)
	{
		ssAdcData.ResultValue[luhwIndex] = 0u;
		ssAdcData.FilterCounter[luhwIndex] = 0;
		ssAdcData.CurrentValue[luhwIndex] = 0u;
		ssAdcData.FilterTime[luhwIndex] = 0u;
		ssAdcData.CurrentOffset[luhwIndex] = 0u;
	}
	
	for( luhwIndex = 0u; luhwIndex < (U16)suhwAllAdcFilterTime; luhwIndex++)
	{
		ssAdcData.RawValue[luhwIndex] = 0u;
	}

	suhwAllAdcFilterTime = 0u;
	ADC_Adapt_Deinit();
#endif
}

/***********************************************************************
* Name     		: DevCtrl_ADC_CycleTask_10ms
* Description 	: Restart A/D conversion by tick time.
* Parameter   	: None
* Returns     	: None
***********************************************************************/
void DevCtrl_ADC_CycleTask_10ms(void)
{
    U16 luhwIndex = 0U;
	U16 luhwPosition = 0U;
	DEV_ADC_16BIT_T l16tAdValue = 0;
	DEV_ADC_16BIT_T latAdTmpBuffer[DEV_ADC_MAX_FILTER_TIME] = {0};
	
	ADC_Adapt_Conversion();//conversion used channel first

	for(luhwIndex = 0U; luhwIndex< E_DEV_ADC_NUM; luhwIndex++)
    {
        l16tAdValue = AD_READ(gsAdcPort[luhwIndex].Group,gsAdcPort[luhwIndex].Channel);																//read real time value
		if(ssAdcData.FilterCounter[luhwIndex] >= gsAdcPort[luhwIndex].FilterTime)//counter filter position
		{
			ssAdcData.FilterCounter[luhwIndex] = 0;
		}
		else
		{
			/* do nothing... */
		}
		luhwPosition = ssAdcData.FilterCounter[luhwIndex] + ssAdcData.CurrentOffset[luhwIndex];							//get current position
		ssAdcData.RawValue[luhwPosition] = l16tAdValue;																	//add current value in array
		if(gsAdcPort[luhwIndex].FilterTime <= DEV_ADC_MAX_FILTER_TIME)
		{
			MemcpyU16Array(latAdTmpBuffer,ssAdcData.RawValue + ssAdcData.CurrentOffset[luhwIndex],gsAdcPort[luhwIndex].FilterTime);	//copy data
			SortU16ArrayMinToMax(latAdTmpBuffer,gsAdcPort[luhwIndex].FilterTime);//sort of array
			ssAdcData.ResultValue[luhwIndex] = latAdTmpBuffer[gsAdcPort[luhwIndex].FilterTime/2];//find middle value
			memset(latAdTmpBuffer,0,DEV_ADC_MAX_FILTER_TIME);//clear array data
		}
		else
		{
			/* err,skip */
		}
		ssAdcData.FilterCounter[luhwIndex]++;
	}

}

/***********************************************************************
* Name        	: DevCtrl_ADC_GetFilterValue
* Description 	: Get AD value after debounce handler
* Parameter   	: None
* Returns     	: None
***********************************************************************/
DEV_ADC_16BIT_T DevCtrl_ADC_GetFilterValue(DEV_ADC_NAME_T sigName)
{
	DEV_ADC_16BIT_T ltValue = DEV_ADC_INVALID;
	
	if(sigName >= E_DEV_ADC_NUM)
	{
		/* do nothing... */
	}
	else
	{
		ltValue = ssAdcData.ResultValue[sigName];
	}
	return ltValue;
}

/***********************************************************************
* Name        	: DevCtrl_ADC_GetRawValue
* Description	: Get current value
* Parameter   	: SignalName
* Returns     	: None
***********************************************************************/
DEV_ADC_16BIT_T DevCtrl_ADC_GetRawValue(DEV_ADC_NAME_T sigName)
{
	DEV_ADC_16BIT_T ltValue = DEV_ADC_INVALID;
	
	if(sigName >= E_DEV_ADC_NUM)
	{
		/* do nothing... */
	}
	else
	{
		ltValue = AD_READ(gsAdcPort[sigName].Group,gsAdcPort[sigName].Channel);
	}
	
	return ltValue;
}

