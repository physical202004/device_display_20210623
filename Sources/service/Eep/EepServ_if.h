/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : eeprom service
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
#ifndef EEPSERV_IF_H
#define EEPSERV_IF_H
#include "Define.h"

typedef enum{
	E_BLOCK_0 = 0,
	E_BLOCK_1,
	E_BLOCK_2,
	E_BLOCK_3,
	E_BLOCK_4,
	E_BLOCK_5,
	E_BLOCK_6,
	E_BLOCK_7,
	E_BLOCK_MAX,
}E_BLOCK_NUM;


#define EEP_UNUSE_BLOCK				(0xFFU)

extern void EepServ_Init(void);
extern MYBOOL EepServ_ReadStaticData(U8 BlockId, U8 offset, U8* data, U8 len);
extern MYBOOL EepServ_WriteStaticData(U8 BlockId, U8 offset, U8* data, U8 len);
extern MYBOOL EepServ_ReadDynamicData(U8 id, U8* data, U8 len);
extern MYBOOL EepServ_WriteDynamicData(U8 id, U8* data, U8 len);
extern void EepServ_SaveDynamicData(void);

#endif

