/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Deserializer
* Instance      :
* Description   : 
*------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/09/27
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor-----------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1     2019/09/27  Huang Haoxiong      Initial
*************************************************************************************************************/



#ifdef DS90UB948_CTRL_PASS_1
#undef DS90UB948_CTRL_PASS_1

BEGIN_DS90UB948_CTRL
#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)
ADD_DS90UB948_CTRL(MCU_3V3SW)
ADD_DS90UB948_CTRL(DAC_ONOFF)
#endif
ADD_DS90UB948_CTRL(DS90UB948_1V2)
ADD_DS90UB948_CTRL(DS90UB948_PDB)
ADD_DS90UB948_CTRL(DS90UB948_BL_FAULT_TO_HU)
ADD_DS90UB948_CTRL(DS90UB948_TFT_FAULT_TO_HU)
ADD_DS90UB948_CTRL(DS90UB948_CFG)
END_DS90UB948_CTRL


#undef BEGIN_DS90UB948_CTRL
#undef ADD_DS90UB948_CTRL
#undef END_DS90UB948_CTRL
#endif


