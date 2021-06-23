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
*-----------------------------------------------------------------------
* 0.1  Aug 11,2016  LIU Chenxi  Initial version
*-----------------------------------------------------------------------
* 0.2  Jan 01,2017   ShangQiuju remove unused include file.
*------------------------------------------------------------------------
* 0.3  Jan 24,2017    ZhangJie  Fix Coverity warnings.
*------------------------------------------------------------------------
* 0.4  Feb 04,2017    Pei Jingli  Add device DEV_PWMD_HW,i.e hardware pwm function
* ----------------------------------------------------------------------
* 0.5  Feb 15,2017  ShangQiuju Remove devie fdl from dev to devblock.
* ----------------------------------------------------------------------
* 0.6    Feb 20,2017    PEI Jingli     Add device of TAU-Pwm,capture, timer
* ----------------------------------------------------------------------
* 0.7  Feb 20,2017    ZhangJie     Add device of CAN.
* ----------------------------------------------------------------------
* 0.8	Dec 19,2018		Huang,Haoxiong	improve HAL structure
*=====================================================================*/

/****************************************** register character devices ******************************************/
BEGIN_DEVCHAR
ADD_DEVCHAR(DEVICE_CONTROLLER_ID,gsDevCtrl)
END_DEVCHAR

/****************************************** register block devices ******************************************/
BEGIN_DEVBLOCK
ADD_DEVBLOCK(COMM_DEVICE_ID,gsCommDrv)
ADD_DEVBLOCK(BACKLIGHT_DEVICE_ID,gsLP8860Drv)
ADD_DEVBLOCK(FOG_DEVICE_ID,gsFOGDrv)
ADD_DEVBLOCK(LVDS_DEVICE_ID,gsDS90UB948Drv)
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
ADD_DEVBLOCK(TP_DEVICE_ID,gsSynapticsTPDrv)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
#endif
END_DEVBLOCK

#undef BEGIN_DEVCHAR
#undef ADD_DEVCHAR
#undef END_DEVCHAR
#undef BEGIN_DEVBLOCK
#undef ADD_DEVBLOCK
#undef END_DEVBLOCK

/****************************** END OF FILE ***************************/

