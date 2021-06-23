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
#ifndef COMM_SERVICE_H
#define COMM_SERVICE_H
#include "CommServ_if.h"

#define COMM_RX_BUFFER_MAX						(64U)
#define COMM_TX_BUFFER_MAX						(64U)

/***************************************************************** APP ********************************************************************************/


/***************************************************************** EOL ********************************************************************************/
#define COMM_I2C_SLAVE_ADDRESS					(0x90U)	//8bit address

#define COMM_EOL_ACCESS_KEY1_VALUE				(0x5AA5U)		//key1
#define COMM_EOL_ACCESS_KEY2_VALUE				(0x3CC3U)		//key2
#define COMM_EOL_CHANGE_MODE_TIMEOUT			(500U)			//5s,unit by 10ms
#define COMM_EOL_FRAME_HEADER					(0x5AU)			//frame header
#define COMM_EOL_NODATA_LENGTH					(5U)			//sync head + dir byte + para + length + cmd type
#define COMM_EOL_DATA_LENGTH_LIMIT     			(COMM_RX_BUFFER_MAX - COMM_EOL_NODATA_LENGTH)//Maximum length of data section length of receiving EOL frame
#define COMM_EOL_CMD_BYTE						(4U)			//position of command byte
#define COMM_EOL_DEVICE_TYPE_BYTE				(5U)			//position of device type byte
#define COMM_EOL_DEVICE_CMD_BYTE				(6U)			//position of device command byte
#define COMM_EOL_SYS_CMD_BYTE					(5U)			//position of command byte
#define COMM_EOL_FRAME_DEVICE_TYPE_NONE			(0xFFU)			//reset EOL frame device type
#define COMM_EOL_CMD_EOL						(0x30U)			//eol value in command byte
#define COMM_EOL_CMD_SYSTEM						(0x31U)			//system value in command byte
#define COMM_EOL_REPLY_DIR_BYTE					(0x06U)			//direction byte value
#define COMM_EOL_REPLY_DIR_RESERVED_BYTE		(0U)			//direction reserved byte value
#define COMM_EOL_REPLY_SYS_LENGTH				(0x02U)			//Reply system frame length is fixed,2 byte
#define COMM_EOL_REPLY_FRAME_NODATA_LENGTH		(9U)			//sync head + dir byte + dir para + length + cmd type + device type + device command + result + checksum
#define COMM_EOL_REPLY_CMD_LENGTH				(4U)			//cmd type + device type + device command + result

#define COMM_EOL_SYS_CMD_NONE           		0x00U
#define COMM_EOL_SYS_CMD_REQ_EOL          		0x01U
#define COMM_EOL_SYS_CMD_ENTER_EOL        		0x02U
#define COMM_EOL_SYS_CMD_ACCESS_KEY1    		0x03U
#define COMM_EOL_SYS_CMD_ACCESS_KEY2  			0x04U
#define COMM_EOL_SYS_CMD_EXIT_EOL      			0xFFU

typedef enum
{
	E_COMM_FRAME_STATE_IDLE = 0,
	E_COMM_FRAME_STATE_APP,
	E_COMM_FRAME_STATE_EOL,
}E_COMM_FRAME_STATE;//communication state

typedef enum
{
     E_COMM_EOL_STATE_HEADER = 0
    ,E_COMM_EOL_STATE_DIR
    ,E_COMM_EOL_STATE_DIR_RESERVED
    ,E_COMM_EOL_STATE_LENGTH
    ,E_COMM_EOL_STATE_DATA_SECTION
    ,E_COMM_EOL_STATE_CHECKSUM
}E_COMM_EOL_STATE;//eol frame format



#endif /* COMM_ENGINE_H */




