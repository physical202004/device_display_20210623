/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.                *
*                 All Rights Reserved.                                 *
*   Department : ADS AD                                                 *
*   AUTHOR     :                                                       *
************************************************************************
* Object        :
* Module        :	IIC
* Instance      :
* Description   :	
*-----------------------------------------------------------------------
* Version:
* Date:
* Author:
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* ----------------------------------------------------------------------
* 0.1    	April 16,2020 Huang Haoxiong	Initial
*=====================================================================*/
#ifndef I2C_ADAPT_H
#define I2C_ADAPT_H
#include "I2c_Adapt_if.h"
#include "I2c_Adapt_Cfg.h"



/* enable I2C module */
#define IIC_C1_IICEN_DISABLE				(0U)//disable I2C module operation
#define IIC_C1_IICEN_ENABLE					(1U)//enable I2C module operation

/* enable I2C interrupt */
#define IIC_C1_IICIE_DISABLE				(0U)//disable I2C interrupt
#define IIC_C1_IICIE_ENABLE					(1U)//enable I2C interrupt


/* select I2C mode */
#define IIC_C1_MAT_SLAVE_MODE				(0U)//slave mode
#define IIC_C1_MAT_MASTER_MODE				(1U)//master mode


/* I2C wake up enable */
#define IIC_C1_WUEN_DISABLE					(0U)//wakeup disable
#define IIC_C1_WUEN_ENABLE					(1U)//wakeup enable


/* I2C General call address enable */
#define IIC_C2_GCAEN_DISABLE				(0U)//disables general call address
#define IIC_C2_GCAEN_ENABLE					(1U)//enables general call address


/* I2C extension address selection */
#define IIC_C2_ADEXT_7BIT_ADDRESS			(0U)//7-bit address scheme
#define IIC_C2_ADEXT_10BIT_ADDRESS			(1U)//10-bit address scheme

/* enable range address */
#define IIC_C2_RMEN_DISABLE					(0U)//Range mode disabled. No address matching occurs for an address within the range of values of the A1 and RA registers.
#define IIC_C2_RMEN_ENABLE					(1U)//Range mode enabled. Address matching occurs when a slave receives an address within the range of values of the A1 and RA registers.


/* enable fast ack */
#define IIC_SMB_FACK_ENABLE					(0U)//An ACK or NACK is sent on the following receiving data byte.
#define IIC_SMB_FACK_DISABLE				(1U)//Writing 0 to TXAK after receiving a data byte generates an ACK. Writing 1 to TXAK after receiving adata byte generates a NACK.

/* SMB Alert enable */
#define IIC_SMB_ALERTEN_DISABLE				(0U)//SMBus alert response address matching is disabled
#define IIC_SMB_ALERTEN_ENABLE				(1U)//SMBus alert response address matching is enabled


/* enable the second address */
#define IIC_SMB_SIICAEN_DISABLE				(0U)//I2C address register 2 matching is disabled
#define IIC_SMB_SIICAEN_ENABLE				(1U)//I2C address register 2 matching is enabled



/* TimeoutCountClockSelect */
#define IIC_SMB_TCKSEL_DISABLE				(0U)//Timeout counter counts at the frequency of the I2C module clock / 64
#define IIC_SMB_TCKSEL_ENABLE				(1U)//Timeout counter counts at the frequency of the I2C module clock


/* SHTF2 interrupt enable */
#define IIC_SMB_SHTF2IE_DISABLE				(0U)//SHTF2 interrupt is disabled
#define IIC_SMB_SHTF2IE_ENABLE				(1U)//SHTF2 interrupt is enabled


/*Reserved */
#define IIC_RESERVED						(0U)//reserved

#define IIC_LOW_TIMEOUT_VALUE				(50000)//SCL low timeout of 200 ms,TCKSEL(0:iic clock/64,1:iic clock) * value,open interrupt need enable IIC_SMB_SHTF2IE bit


#endif /*I2C_ADAPT_H*/

