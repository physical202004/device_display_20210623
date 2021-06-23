/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : device controller
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
*  0.1    2019/03/08  Huang Haoxiong      Initial
*************************************************************************************************************/
/*####################################### devices function #######################################*/
#ifdef DEV_POWER_PASS_1
#undef DEV_POWER_PASS_1

BEGIN_POWERDOWN_CTRL		//power down/sleep execution sequence
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
DEVCTRL_DECS(LVDS_DEVICE_ID,					3)
DEVCTRL_DECS(TP_DEVICE_ID, 						2)
DEVCTRL_DECS(FOG_DEVICE_ID,						1)
DEVCTRL_DECS(BACKLIGHT_DEVICE_ID,				0)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
DEVCTRL_DECS(LVDS_DEVICE_ID,					2)
DEVCTRL_DECS(FOG_DEVICE_ID,						1)
DEVCTRL_DECS(BACKLIGHT_DEVICE_ID,				0)
#endif
END_POWERDOWN_CTRL

BEGIN_POWERON_CTRL			//power on execution sequence
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
DEVCTRL_DECS(LVDS_DEVICE_ID,					0)
DEVCTRL_DECS(TP_DEVICE_ID,						1)
DEVCTRL_DECS(FOG_DEVICE_ID,						2)
DEVCTRL_DECS(BACKLIGHT_DEVICE_ID,				3)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
DEVCTRL_DECS(LVDS_DEVICE_ID,					0)
DEVCTRL_DECS(FOG_DEVICE_ID,						1)
DEVCTRL_DECS(BACKLIGHT_DEVICE_ID,				2)
#endif
END_POWERON_CTRL


#undef BEGIN_POWERDOWN_CTRL
#undef END_POWERDOWN_CTRL
#undef DEVCTRL_DECS
#undef BEGIN_POWERON_CTRL
#undef END_POWERON_CTRL
#endif

/******************************************************** device controller ********************************************************/
#ifdef DEV_FUNC_PASS_1
#undef DEV_FUNC_PASS_1
BEGIN_DEVFUNC
DEVFUNC_DESC(DEV_FUNC_REQUEST_CHANGE_PM_STATE)			//Request change device controller state
DEVFUNC_DESC(DEV_FUNC_REQ_OPEN_DEVICE) 					//Request open device from device id
DEVFUNC_DESC(DEV_FUNC_REQ_CLOSE_DEVICE)					//Request close device from device id
DEVFUNC_DESC(DEV_FUNC_RELEASE_DEVICE) 					//Release control device from device id
//DEVFUNC_DESC(DEV_FUNC_RESTART_DEVICE)					//Restart control device from device id
DEVFUNC_DESC(DEV_FUNC_DEVICE_IS_OPEN) 					//Check the device is open and available
DEVFUNC_DESC(DEV_FUNC_DEVICE_IS_CLOSE)					//Check the device is closed
DEVFUNC_DESC(DEV_FUNC_GET_ALL_ADC_CHANNEL) 				//Get all adc channel voltage in register
DEVFUNC_DESC(DEV_FUNC_GET_NTC_ADC_VALUE)				//Get NTC voltage for ADC register
DEVFUNC_DESC(DEV_FUNC_CHECK_VOLT_DISCHARGE) 			//Check whether all power supply voltage have been discharged
DEVFUNC_DESC(DEV_FUNC_SOFT_RESET)						//execute soft reset
//DEVFUNC_DESC(DEV_FUNC_DEEP_SLEEP)						//go to deep sleep
END_DEVFUNC

#undef BEGIN_DEVFUNC
#undef DEVFUNC_DESC
#undef END_DEVFUNC
#endif

/******************************************************** communication function ********************************************************/
#ifdef COMM_DEV_FUNC_PASS_1
#undef COMM_DEV_FUNC_PASS_1
BEGIN_COMMFUNC
COMMFUNC_DESC(COMM_FUNC_SNED_CALLBACK_REGIST)
COMMFUNC_DESC(COMM_FUNC_RECV_CALLBACK_REGIST)
COMMFUNC_DESC(COMM_FUNC_ERR_CALLBACK_REGIST)
COMMFUNC_DESC(COMM_FUNC_FINISH_CALLBACK_REGIST)
COMMFUNC_DESC(COMM_FUNC_PAIRED_CALLBACK_REGIST)			//This function is only available in IIC communication.
COMMFUNC_DESC(COMM_FUNC_START)
COMMFUNC_DESC(COMM_FUNC_STOP)
COMMFUNC_DESC(COMM_FUNC_RESTART)
COMMFUNC_DESC(COMM_FUNC_REQUEST_CRQ)
COMMFUNC_DESC(COMM_FUNC_RELEASE_CRQ)
END_COMMFUNC

#undef BEGIN_COMMFUNC
#undef COMMFUNC_DESC
#undef END_COMMFUNC
#endif

/******************************************************** backlight function ********************************************************/
#ifdef BL_DEV_FUNC_PASS_1
#undef BL_DEV_FUNC_PASS_1
BEGIN_BLFUNC
BLFUNC_DESC(BL_FUNC_GET_FAULT_STATE)					//return fault state
BLFUNC_DESC(BL_FUNC_SET_BRN)							//set brightness,0.00% ~ 100.00%
BLFUNC_DESC(BL_FUNC_READ_BRN)							//read current brightness
BLFUNC_DESC(BL_FUNC_CLEAR_FAULT)						//clear fault status
BLFUNC_DESC(BL_FUNC_CTRL_ENABLE)						//control enable signal,TRUE:enable,FALSE:disable
END_BLFUNC

#undef BEGIN_BLFUNC
#undef BLFUNC_DESC
#undef END_BLFUNC
#endif


/******************************************************** FOG function ************************************************************/
#ifdef FOG_DEV_FUNC_PASS_1
#undef FOG_DEV_FUNC_PASS_1
BEGIN_FOGFUNC
FOGFUNC_DESC(FOG_FUNC_GET_FAULT_STATE)					//return fault state
FOGFUNC_DESC(FOG_FUNC_CHANGE_VCOM)						//change vcom pwm value
END_FOGFUNC

#undef BEGIN_FOGFUNC
#undef FOGFUNC_DESC
#undef END_FOGFUNC
#endif

/******************************************************** LVDS function ************************************************************/
#ifdef LVDS_DEV_FUNC_PASS_1
#undef LVDS_DEV_FUNC_PASS_1
BEGIN_LVDSFUNC
LVDSFUNC_DESC(LVDS_FUNC_GET_FAULT_STATE)				//return fault state
LVDSFUNC_DESC(LVDS_FUNC_REQ_ENTER_BIST)					//request enter to bist mode
LVDSFUNC_DESC(LVDS_FUNC_LOCK_STATE) 					//read lock state
LVDSFUNC_DESC(LVDS_FUNC_TFT_ENABLE_STATE) 				//read FOG enable state
LVDSFUNC_DESC(LVDS_FUNC_BL_ENABLE_STATE)				//read BL enable state
END_LVDSFUNC

#undef BEGIN_LVDSFUNC
#undef LVDSFUNC_DESC
#undef END_LVDSFUNC
#endif


/******************************************************** TP function ************************************************************/
#ifdef TP_DEV_FUNC_PASS_1
#undef TP_DEV_FUNC_PASS_1
BEGIN_TPFUNC	
TPFUNC_DESC(TP_FUNC_GET_FAULT_STATE)					//return fault state
TPFUNC_DESC(TP_FUNC_SET_OBJECT)							//set touch data object
TPFUNC_DESC(TP_FUNC_READ_TOUCH) 						//Detect the touch data is updated or not
TPFUNC_DESC(TP_FUNC_READ_VER)							//read tp configuration version
TPFUNC_DESC(TP_FUNC_EXEC_SELFTEST) 						//execute selftest
TPFUNC_DESC(TP_FUNC_OPEN_TEST)							//running open test
TPFUNC_DESC(TP_FUNC_SHORT_TEST) 						//running short test
TPFUNC_DESC(TP_FUNC_EXEC_CALIBRATION) 					//execute calibration
TPFUNC_DESC(TP_FUNC_UPDATE_PREPARE) 					//Let the TP chip prepare for the upgrade
TPFUNC_DESC(TP_FUNC_UPDATE_RAWDATA) 					//Programing upgrade data
TPFUNC_DESC(TP_FUNC_UPDATE_RAWDATA_END) 				//Programing complete
END_TPFUNC

#undef BEGIN_TPFUNC
#undef TPFUNC_DESC
#undef END_TPFUNC
#endif

/*####################################### devices function #######################################*/



