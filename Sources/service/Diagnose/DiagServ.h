/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : diagnose module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/09/18
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2020/09/18  Huang Haoxiong      Initial
*************************************************************************************************************/

#ifndef DIAGSERV_H
#define DIAGSERV_H
#include "Eep_Adapt_if.h"
#include "DiagServ_if.h"

#define DIAGSERV_IFC_OFFSET_ADDR 			(56U)										/*Offset addr of IFC in eeprom*/
#define DIAGSERV_IFC_SIZE					(EEP_MAX_SIZE - DIAGSERV_IFC_OFFSET_ADDR)	/*ifc size */
#define DIAGSERV_IFC_LEN					(DIAGSERV_NUM * DIAGSERV_IFC_DATA_LEN)

#define DIAGSERV_IFC_HISTORY_CNT_MAX		(0xFFU)

enum{
	E_DIAGSERV_IFC_NAME_INDEX = 0,
	E_DIAGSERV_IFC_HISTORY_CNT_INDEX,
};


#define DIAGSERV_PASS_2
#include "DiagServ_Def.h"



#endif

