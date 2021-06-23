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


#ifdef DIAGSERV_PASS_1
#undef DIAGSERV_PASS_1
#define BEGIN_DIAGSERV 			enum DiagServName{
#define DIAGSERV_DECS(a,b)		a,
#define END_DIAGSERV			DIAGSERV_NUM};
#include "DiagServ_Cfg.h"
#endif


#ifdef DIAGSERV_PASS_2
#undef DIAGSERV_PASS_2
#define BEGIN_DIAGSERV 			static const U8 scubDiagIfcId[DIAGSERV_NUM] = {
#define DIAGSERV_DECS(a,b)		b,
#define END_DIAGSERV			};
#include "DiagServ_Cfg.h"
#endif


