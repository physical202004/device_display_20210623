/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : eeprom adapter
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2020/06/24
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1     2020/06/24  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef EEP_ADAPT_IF_H
#define EEP_ADAPT_IF_H
#include "Define.h"

#define EEP_MAX_SIZE 			(256U)						//eeprom maximum size

extern MYBOOL Eep_Adapt_Init(void);
extern MYBOOL Eep_Adapt_DeInit(void);
extern MYBOOL Eep_Adapt_Write(U8 BlockId,U16 offset,U8* buf,U8 len);
extern MYBOOL Eep_Adapt_Read(U8 BlockId,U16 offset,U8* buf,U8 len);

#endif

