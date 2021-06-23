/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : communication module
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/11/15
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/11/15  Huang Haoxiong      Initial
*************************************************************************************************************/
#ifndef COMM_SERVICE_IF_H
#define COMM_SERVICE_IF_H

#include "Global_Definition.h"

typedef enum{
	E_EOL_REPLY_OK = 0,
	E_EOL_REPLY_ERR,
	E_EOL_REPLY_TIMEOUT,
	E_EOL_REPLY_DATA_ERR,
	E_EOL_REPLY_LENGTH_ERR,//reserved
	E_EOL_REPLY_WRITE_ERR,
	E_EOL_REPLY_READ_ERR,
	E_EOL_REPLY_PARAMETER_ERR,
	E_EOL_REPLY_OPEN_ERR,
	E_EOL_REPLY_SHORT_ERR,
}E_EOL_REPLY_STATE;//reply eol frame status

typedef struct
{
	U8 deviceType;						/* device type */
	U8 deviceCmd;						/* device command */
	U8 dirPara; 						/* direction parameter bit,use for reply eol message */
}S_COMM_EOL_FRAME;//eol frame structure

extern void CommServ_EOLEntryHandler(void);
extern void CommServ_NormalEntryHandler(void);
extern void CommServ_NormalActiveHandler(void);
extern void CommServ_EOLActiveHandler(void);
extern MYBOOL CommServ_ReplyEolMessage(E_EOL_REPLY_STATE result,U8 *data,U8 dataLen);
extern U8 CommServ_EOL_GetDeviceType(void);
extern U8 CommServ_EOL_GetDeviceCmd(void);
extern U8* CommServ_EOL_GetFrameData(void);

#endif /* COMM_ENGINE_IF_H */

