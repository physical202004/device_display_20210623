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

#ifndef DIAGSERV_IF_H
#define DIAGSERV_IF_H
#include "Define.h"

#define DIAGSERV_IFC_DATA_LEN				(2U)

#define DIAGSERV_IFC_TIME_500MS				(50U)
#define DIAGSERV_IFC_TIME_5S				(500U)
#define DIAGSERV_IFC_TIME_10S				(DIAGSERV_IFC_TIME_5S + DIAGSERV_IFC_TIME_5S)
#define DIAGSERV_IFC_TIME_20S				(DIAGSERV_IFC_TIME_10S + DIAGSERV_IFC_TIME_10S)
#define DIAGSERV_IFC_TIME_30S				(DIAGSERV_IFC_TIME_10S + DIAGSERV_IFC_TIME_20S)
#define DIAGSERV_IFC_TIME_60S				(DIAGSERV_IFC_TIME_30S + DIAGSERV_IFC_TIME_30S)


typedef unsigned char DIAGSERV_NAME_8BIT_T;


#define DIAGSERV_PASS_1
#include "DiagServ_Def.h"

extern void DiagServ_Init(void);
extern void DiagServ_CycleTask10ms(void);
extern MYBOOL DiagServ_AddIfcInfo(DIAGSERV_NAME_8BIT_T diagName);
extern MYBOOL DiagServ_ReadIfcInfo(DIAGSERV_NAME_8BIT_T diagName,U8 *buf);
extern MYBOOL DiagServ_ClearIfcInfo(DIAGSERV_NAME_8BIT_T diagName);
extern void DiagServ_ClearAllIfcInfo(void);
extern U8 DiagServ_GetIfcQuantity(void);
extern void DiagServ_SaveIfcToFlash(void);


#endif


