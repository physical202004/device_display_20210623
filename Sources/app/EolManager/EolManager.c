/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : EOL manager
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
#include "EolManager.h"
#include "Global_Definition.h"

static void EOLMgr_ProductCmdHandler(void);
static void EOLMgr_DisplayCmdHandler(void);


/***********************************************************************
*  Name        	: EOLMgr_EOLActiveHandler
*  Description 	: EOL frame handler in 10ms task
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
void EOLMgr_EOLActiveHandler(void)
{
	U8 lubFrameDeviceType = CommServ_EOL_GetDeviceType();

	switch(lubFrameDeviceType)
	{
		case E_EOL_DEVICE_TYPE_PRODUCT:
			EOLMgr_ProductCmdHandler();
			break;
		case E_EOL_DEVICE_TYPE_DISPLAY:
			EOLMgr_DisplayCmdHandler();
			break;
	    case E_EOL_DEVICE_TYPE_SLIDER:
			break;
	    case E_EOL_DEVICE_TYPE_TP:
			break;
	    case E_EOL_DEVICE_TYPE_TFT:
	    	break;
	    case E_EOL_DEVICE_TYPE_BACKLIGHT:
			break;
	    case E_EOL_DEVICE_TYPE_KEY:
			break;
		default:
			/* do nothing... */
			break;
	}
}

/***********************************************************************
*  Name        	: EOLMgr_ProductCmdHandler
*  Description 	: Product command for EOL frame processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void EOLMgr_ProductCmdHandler(void)
{
	U8 laubTempSendData[40] = {0};
	U8 lubCnt = 0;
	MYBOOL lmbAgingState = TRUE; 
	U8 lubFrameDeviceCmd = CommServ_EOL_GetDeviceCmd();
	U8 *lpubFrameData = CommServ_EOL_GetFrameData();
	switch(lubFrameDeviceCmd)
	{
		case E_EOL_PRODUCT_CMD_READ_EXT_VER:         	//read external version
            laubTempSendData[lubCnt++] = 'S';
            laubTempSendData[lubCnt++] = 'W';
            laubTempSendData[lubCnt++] = '0';
            laubTempSendData[lubCnt++] = gccSysVersion[0];
            laubTempSendData[lubCnt++] = gccSysVersion[1];
            CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,lubCnt);
			break;
	    case E_EOL_PRODUCT_CMD_READ_INT_VER:       	//read internal version
	    	lubCnt = strlen(gccSysVersion);
            memcpy(laubTempSendData,gccSysVersion,lubCnt); //copy string
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,lubCnt);
	    	break;
	    case E_EOL_PRODUCT_CMD_WRITE_HW_VER:     		//write hardware version
	    	if(EepServ_WriteStaticData(E_BLOCK_0,EEP_MAIN_HW_ADDR,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 3],2) == TRUE)
            {
                memcpy(gcHWVersion,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 3],2);
                CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
            }
            else
            {
                CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_HW_VER:     		//read hardware version
            laubTempSendData[lubCnt++] = 'H';
            laubTempSendData[lubCnt++] = 'W';
            laubTempSendData[lubCnt++] = '0';
            laubTempSendData[lubCnt++] = gcHWVersion[0];
            laubTempSendData[lubCnt++] = gcHWVersion[1];
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,lubCnt);
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_CHECKSUM:   		//read checksum
            laubTempSendData[lubCnt++] = gaubSysCheckSum[1];
            laubTempSendData[lubCnt++] = gaubSysCheckSum[0];
            CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,lubCnt);
	    	break;
	    case E_EOL_PRODUCT_CMD_SET_AGING_TIME:	   		//set aging time
	    	if((lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] == 0u) || (lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] == 0xFFu))//invalid value
		    {
				CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,0);

            }
            else
            {
                if(EepServ_WriteStaticData(E_BLOCK_0,EEP_AGING_TIME_ADDR,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE],1) == TRUE)
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
                }
                else
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
                }
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_AGING_TIME:		//read aging time
			if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_TIME_ADDR,laubTempSendData,1) == TRUE)
			{
                if(laubTempSendData[0] == 0xFF)  //no write aging time, reply failed
                {
					CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,1);
                }
                else
                {
					CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,1);
                }
			}
            else
            {
                laubTempSendData[0] = 0xFFu;
				CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,1);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_START_AGING:           	//start aging
            /* read aging time */
            if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_TIME_ADDR,laubTempSendData,1) == TRUE)
            {
                //invalid value for aging time
                if((laubTempSendData[0] == 0xFFu) || (laubTempSendData[0] == 0u))
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,0);
                }
                else
                {
                    /* write aging state */
					laubTempSendData[0] = E_AGING_STATE_RUNNING;
                    if(EepServ_WriteStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,laubTempSendData,1) == TRUE)
                    {
						lmbAgingState = TRUE;
						CS_SIG_SyncSend(SIG_BOOL_IS_EXEC_AGING,&lmbAgingState);//update state
                    	CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
                    }
                    else
                    {
                        CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
                    }
                }
            }
            else
            {
                CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,0);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_STOP_AGING:             //stop aging
	    	/* read aging time */
            if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,laubTempSendData,1) == TRUE)
            {
                //invalid value for aging time
                if(laubTempSendData[0] == E_AGING_STATE_RUNNING)
                {
                    /* write aging state */
					laubTempSendData[0] = E_AGING_STATE_FINISHED_BY_CMD;
        			if(EepServ_WriteStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,laubTempSendData,1) == TRUE)
        			{
						lmbAgingState = FALSE;
						CS_SIG_SyncSend(SIG_BOOL_IS_EXEC_AGING,&lmbAgingState);//update state
                        CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
        			}
                    else
                    {
                        CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
                    }
                }
                else
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
                }
            }
            else
            {
                CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,0);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_AGING_STATE:    	//read aging state
	    	if(EepServ_ReadStaticData(E_BLOCK_0,EEP_AGING_STATE_ADDR,laubTempSendData,1) == TRUE)
			{
                if(laubTempSendData[0] == 0xFF)
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,1);
                }
                else
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,1);
                }
			}
            else
            {
                laubTempSendData[0] = 0xFFu;
				CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,1);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_GET_IFC_TOTAL_QUANTITY:	//read ifc quantity
			laubTempSendData[0] = DiagServ_GetIfcQuantity();
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,1);
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_IFC:           	//read all ifc information
			if(DiagServ_ReadIfcInfo(lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE],laubTempSendData) == TRUE)
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,COMM_EOL_IFC_DATA_LENGTH);
			}
			else
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,0);
			}
	    	break;
	    case E_EOL_PRODUCT_CMD_CLEAR_ALL_IFC:              //clear all ifc information
			DiagServ_ClearAllIfcInfo();
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
	    	break;
	    case E_EOL_PRODUCT_CMD_WRITE_SERIAL_NUMBER:  	//write serial-number
	    	if(EepServ_WriteStaticData(E_BLOCK_0,EEP_SERIAL_NUMBER_ADDR,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE],COMM_EOL_SERIAL_NUM_LENGTH) == TRUE)
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
			}
			else
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
			}
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_SERIAL_NUMBER:  	//read serial-number
			if(EepServ_ReadStaticData(E_BLOCK_0,EEP_SERIAL_NUMBER_ADDR,laubTempSendData,COMM_EOL_SERIAL_NUM_LENGTH) == TRUE)
			{
 				CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,COMM_EOL_SERIAL_NUM_LENGTH);
			}
			else
			{
                memset(laubTempSendData,0xFFu,COMM_EOL_SERIAL_NUM_LENGTH);
				CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,COMM_EOL_SERIAL_NUM_LENGTH);
			}
	    	break;
	    /*case E_EOL_PRODUCT_CMD_WRITE_HW_INT_VER:		//write hardware internal version
	    	if(EepServ_WriteStaticData(E_BLOCK_0,EEP_INTERNAL_HW_ADDR,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 3],2) == TRUE)
            {
                memcpy(gcHWVersion + 3,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 3],2);
                CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
            }
            else
            {
                CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
            }
	    	break;
	    case E_EOL_PRODUCT_CMD_READ_HW_INT_VER:		//read hardware internal version
	    	laubTempSendData[lubCnt++] = 'I';
            laubTempSendData[lubCnt++] = 'H';
            laubTempSendData[lubCnt++] = 'W';
            laubTempSendData[lubCnt++] = gcHWVersion[3];
            laubTempSendData[lubCnt++] = gcHWVersion[4];
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,lubCnt);
	    	break;
		case E_EOL_PRODUCT_CMD_QUICK_ENTER_SLEEP:	    //quick enter sleep
			CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
			break;
		*/	
		case E_EOL_PRODUCT_CMD_READ_ADC_VALUE:     	//read ADC Value
			if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_GET_ALL_ADC_CHANNEL,laubTempSendData))
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,E_DEV_ADC_NUM * sizeof(DEV_ADC_16BIT_T));
	    	}
			else
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
			}
			break;
		default:
			CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
			break;
	}
}


/***********************************************************************
*  Name        	: EOLMgr_DisplayCmdHandler
*  Description 	: Display command for EOL frame processing
*  Parameter   	: None
*  Returns     	: None
***********************************************************************/
static void EOLMgr_DisplayCmdHandler(void)
{
	
	U8 laubTempSendData[32] = {0};
	U8 *lpubFrameData = CommServ_EOL_GetFrameData();
	U8 lubFrameDeviceCmd = CommServ_EOL_GetDeviceCmd();
	U8 lubCnt = 0;
	DEV_BRN_16BIT_T ltBrightnessVal = 0U;

	switch(lubFrameDeviceCmd)
	{
	    case E_EOL_DISPLAY_CMD_WRITE_VCOM:		    	//write vcom
			if((lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] == 0) || (lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] == 0xFF))
		   	{
			   CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,0);
		   	}
		   	else
		   	{
			   laubTempSendData[0] = lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE];
			   laubTempSendData[1] = lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 1];
			   laubTempSendData[2] = FrameChecksum(laubTempSendData,2);
			   if(EepServ_WriteStaticData(E_BLOCK_0,EEP_FOG_VCOM_ADDR,laubTempSendData,4) == TRUE)
			   {
					if(E_HAL_STATE_OK == HALBlock_IoControl(FOG_DEVICE_ID,FOG_FUNC_CHANGE_VCOM,laubTempSendData))
					{
						CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
					}
					else
					{
						CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
					}
			   }
			   else
			   {
				   CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
			   }
		   	}
	    	break;
	    case E_EOL_DISPLAY_CMD_READ_VCOM: 				//read vcom
            if(EepServ_ReadStaticData(E_BLOCK_0,EEP_FOG_VCOM_ADDR,laubTempSendData,4) == TRUE)
            {
                if((laubTempSendData[0] == 0xFFu) || (laubTempSendData[0] == 0u)
                    || (FrameChecksum(laubTempSendData,2) != laubTempSendData[2]))
                {
					CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,2);
                }
                else
                {
                    CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,2);
                }
            }
            else
            {
                laubTempSendData[lubCnt++] = 0xFFu;
                laubTempSendData[lubCnt++] = 0xFFu;
				CommServ_ReplyEolMessage(E_EOL_REPLY_READ_ERR,laubTempSendData,lubCnt);
            }
	    	break;
	    case E_EOL_DISPLAY_CMD_WTITE_BRIGHTNESS:		//write backlight brightness;
	    	ltBrightnessVal = (U16)(lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] << 8) + (U16)(lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE + 1]);
			CS_SIG_SyncSend(SIG_SHORT_COMM_REQ_BRN_VAL,&ltBrightnessVal);//update brightness signal
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
	    	break;
	    case E_EOL_DISPLAY_CMD_READ_BRIGHTNESS:   		//read backlight brightness
	    	CS_SIG_SyncReceive(SIG_SHORT_CURR_BRN_VAL,&ltBrightnessVal);
			laubTempSendData[0] = (U8)(ltBrightnessVal >> 8);
			laubTempSendData[1] = (U8)(ltBrightnessVal & 0xFFU);
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,2);
	    	break;
	    /*case E_EOL_DISPLAY_CMD_READ_BL_NTC_VALUE:   	//read NTC value
	    	if(E_HAL_STATE_OK == HALChar_IoControl(DEVICE_CONTROLLER_ID,DEV_FUNC_GET_NTC_ADC_VALUE,laubTempSendData))
	    	{
				CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,sizeof(DEV_ADC_16BIT_T));
	    	}
			else
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
			}
	    	break;*/
		case E_EOL_DISPLAY_CMD_WRITE_BL_RATIO:	//write brightness ratio
			if((lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] >= BRIGHTNESS_RATIO_MIN) &&\
				(lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] <= BRIGHTNESS_RATIO_MAX))
			{
					laubTempSendData[0] = lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE];
					if(EepServ_WriteStaticData(E_BLOCK_0, EEP_BRIGHTNESS_RATIO_ADDR, laubTempSendData, 1) == TRUE)
					{
						CS_SIG_SyncSend(SIG_CHAR_BRIGHTNESS_RATIO,&lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE]);
						ltBrightnessVal = DEV_BL_BRN_MAX * lpubFrameData[COMM_EOL_DEVICE_DATA_BYTE] / 100;
						HALBlock_IoControl(BACKLIGHT_DEVICE_ID,BL_FUNC_SET_BRN,&ltBrightnessVal);
						CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,0);
					}
					else
					{
						CommServ_ReplyEolMessage(E_EOL_REPLY_WRITE_ERR,laubTempSendData,0);
					}
			}
			else
			{
				CommServ_ReplyEolMessage(E_EOL_REPLY_DATA_ERR,laubTempSendData,0);
			}
			break;
		case E_EOL_DISPLAY_CMD_READ_BL_RATIO:  //read brightness ratio
			CS_SIG_SyncReceive(SIG_CHAR_BRIGHTNESS_RATIO,&laubTempSendData[0]);
			CommServ_ReplyEolMessage(E_EOL_REPLY_OK,laubTempSendData,1);
			break;
		default:
			CommServ_ReplyEolMessage(E_EOL_REPLY_ERR,laubTempSendData,0);
			break;
	}
}



