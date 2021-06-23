/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : Analog i2c module
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

#include "AI2c_Adapt.h"

static E_AI2C_ERR AI2c_Adapt_Send_Byte(unsigned char txd);
static void AI2c_Adapt_NAck(void);
static void AI2c_Adapt_Ack(void);
static E_AI2C_ERR AI2c_Adapt_Wait_Ack(void);
static E_AI2C_ERR AI2c_Adapt_Wait_Clk_Streching(void);
static void AI2c_Adapt_Stop(void);
static void AI2c_Adapt_Start(void);

//static void  AI2c_Adapt_Delay(int time);

/***********************************************************************
*  Name         : AI2c_Adapt_Init
*  Description  : initial I2C Port
*  Parameter    : None
*  Returns      : None
***********************************************************************/
void AI2c_Adapt_Init(void)
{
	AI2C_SCL_OUT();
	AI2C_SDA_OUT();
	
    AI2C_SET_SCL();
	AI2C_SET_SDA();
}

/***********************************************************************
*  Name         : AI2c_Adapt_Deinit
*  Description  : Deinitial I2C Port
*  Parameter    : None
*  Returns      : None
***********************************************************************/
void AI2c_Adapt_Deinit(void)
{

	AI2C_SCL_OUT();
	AI2C_SDA_OUT();

    AI2C_CLR_SCL();
	AI2C_CLR_SDA();
}


/***********************************************************************
* Name   		: AI2c_Adapt_Start
* Description  	: Start communication
* Parameter    	: None
* Returns      	: None
***********************************************************************/
static void AI2c_Adapt_Start(void)
{
	int liTimeout = AI2C_STRECHING_MAX_TIME;

	AI2C_SDA_OUT();

	AI2C_SET_SCL();
	while((AI2C_READ_SCL() == 0) && (liTimeout--)) 		//read SCL pin
	{
		;//waiting for scl to high
	}
	AI2C_SET_SDA();

    //AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
	AI2C_CLR_SDA();		//START:when CLK is high,DATA change from high to low

    //AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
	AI2C_CLR_SCL();
}

/***********************************************************************
* Name   		: AI2c_Adapt_Stop
* Description  	: Stop communication,when CLK is high DATA change from low to high
* Parameter    	: None
* Returns      	: None
***********************************************************************/
static void AI2c_Adapt_Stop(void)
{
	int liTimeout = AI2C_STRECHING_MAX_TIME;

	AI2C_SDA_OUT();
	
	AI2C_CLR_SCL();	
	AI2C_CLR_SDA();	
	
    //AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
	AI2C_SET_SCL();
	while((AI2C_READ_SCL() == 0) && (liTimeout--)) 		//read SCL pin
	{
		;//waiting for scl to high

	}
	AI2C_SET_SDA();

}


/***********************************************************************
* Name   		: AI2c_Adapt_Wait_Clk_Streching
* Description  	: wait streching
* Parameter    	: time: maximum waiting time
* Returns      	: successful: E_AI2C_ERR_OK, fail: E_AI2C_ERR_TIMEOUT
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_Wait_Clk_Streching(void)
{
	E_AI2C_ERR leState = E_AI2C_ERR_OK;
	U16 luhwStrechingTime = AI2C_STRECHING_MAX_TIME;
	//AI2c_Adapt_SCL_IN();
    while((AI2C_READ_SCL() == AI2C_LOW_LEVEL) && (luhwStrechingTime--))	//read SCL pin
    {
		;//waiting
    }
	//AI2C_SCL_OUT();
    if(luhwStrechingTime == 0)
    {
        leState = E_AI2C_ERR_TIMEOUT;
    }
    else
    {
        leState = E_AI2C_ERR_OK;
    }

	return (leState);
}

/***********************************************************************
* Name   		: AI2c_Adapt_Wait_Ack
* Description  	: wait ack
* Parameter    	: None
* Returns      	: successful: E_AI2C_ERR_OK, fail: E_AI2C_ERR_NACK
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_Wait_Ack(void)
{
	E_AI2C_ERR ret = E_AI2C_ERR_OK;

	
	AI2C_SET_SDA();//release sda
	AI2C_SET_SCL();

	AI2C_SDA_IN();

    ret = AI2c_Adapt_Wait_Clk_Streching();      //wait clk set to high level
    if(ret == E_AI2C_ERR_OK)
    {
		if(AI2C_READ_SDA() == AI2C_HIGH_LEVEL)
		{
			ret = E_AI2C_ERR_NACK;
		}
		else
		{
			AI2C_CLR_SCL();
			AI2C_SDA_OUT();
		}
    }
	else
	{
		/* do nothing */
	}

	return ret;
}

/***********************************************************************
* Name   		: AI2c_Adapt_Ack
* Description  	: send ack
* Parameter    	: None
* Returns      	: None
***********************************************************************/
static void AI2c_Adapt_Ack(void)
{

	AI2C_SDA_OUT();

	AI2C_CLR_SDA();
	AI2C_SET_SCL();
    //AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
	AI2C_CLR_SCL();
	AI2C_SET_SDA();	//release sda
	
}

/***********************************************************************
* Name   		: AI2c_Adapt_NAck
* Description  	: send NACK
* Parameter    	: None
* Returns      	: None
***********************************************************************/
static void AI2c_Adapt_NAck(void)
{


	AI2C_SDA_OUT();

	AI2C_SET_SDA();

	AI2C_SET_SCL();

    //AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
	AI2C_CLR_SCL();
}

/***********************************************************************
* Name   		: AI2c_Adapt_Send_Byte
* Description  	: Send I2C data
* Parameter    	: txd:send buffer
* Returns      	: None
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_Send_Byte(unsigned char txd)
{
    unsigned char t = 0u;
    E_AI2C_ERR leRet = E_AI2C_ERR_OK;
	
	AI2C_CLR_SCL();
    for(t=0;t<8;t++)
    {
		if((txd >> 7) == 0x01u)
		{
			AI2C_SET_SDA();
		}
		else
		{
			AI2C_CLR_SDA();
		}
        txd<<=1;
		AI2C_SET_SCL();
        leRet = AI2c_Adapt_Wait_Clk_Streching();	 	    //wait clk set to high level
        if(leRet != E_AI2C_ERR_OK)                 		//timeout
        {
			
			AI2C_SDA_OUT();
            break;
        }
		else
		{
			//AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
			AI2C_CLR_SCL();
		}
		//AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
    }
 	if(leRet == E_AI2C_ERR_OK)
 	{
		leRet = AI2c_Adapt_Wait_Ack();
 	}
	else
	{
		/*do nothing */
	}
	AI2C_SDA_OUT();	
    return leRet;
}


/***********************************************************************
* Name   		: AI2c_Adapt_Read_Byte
* Description  	: Read I2C data
* Parameter    	: receive:receive buffer,ack:0 means ACK, 1 means NACK
* Returns      	: receive data
***********************************************************************/
E_AI2C_ERR AI2c_Adapt_Read_Byte(unsigned char* receive,E_AI2C_ACK_TYPE ack)
{
	unsigned char i = 0;
    E_AI2C_ERR leRet = E_AI2C_ERR_OK;

	AI2C_SDA_IN();
    for(i=0;i<8;i++ )
	{
		AI2C_SET_SCL();
        leRet = AI2c_Adapt_Wait_Clk_Streching();     //wait clk set to high level
        if(leRet != E_AI2C_ERR_OK)                 //timeout
        {
           break;

		}
		else
		{
	        *receive <<= 1;
	        if(AI2C_READ_SDA() != AI2C_LOW_LEVEL)
				*receive += 1;
			//AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
			AI2C_CLR_SCL();
		}
		//AI2c_Adapt_Delay(AI2c_Adapt_DELAY_TIME);
    }
	if(leRet == E_AI2C_ERR_OK)
	{
		if(ack == E_AI2C_ACK)
		{
			AI2c_Adapt_Ack(); //ACK
		}
		else
		{
			AI2c_Adapt_NAck();//NACK		
		}
	}
	else
	{
		/*do nothing */
	}

    return leRet;
}

#ifdef AI2C_SEQUENCE_RW
/***********************************************************************
* Name   		: AI2c_Adapt_SequenceRead
* Description  	: Sequence Read
* Parameter    	: address: read reg addr, buffer:read data for register,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_SequenceRead(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	unsigned short i = 0u;
	E_AI2C_ERR ret = E_AI2C_ERR_OK;

	AI2c_Adapt_Start();
	ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_WRITE_BIT);
	if(ret != E_AI2C_ERR_OK)				//write mode
	{
		AI2c_Adapt_Stop();

	}
	else
	{
		ret = AI2c_Adapt_Send_Byte(address);//select address
		AI2c_Adapt_Stop();																//stop
		if(ret != E_AI2C_ERR_OK)
		{
			/* exit */
		}
		else
		{
			AI2c_Adapt_Start();																//restart
			ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_READ_BIT);
			if(ret != E_AI2C_ERR_OK) 	//read mode
			{
				AI2c_Adapt_Stop();
			}
			else
			{

				for(i = 0;i < (Len - 1u);i++)
				{
					ret = AI2c_Adapt_Read_Byte(buffer,E_AI2C_ACK);
					if(ret != E_AI2C_ERR_OK)	//send register and data
					{
						AI2c_Adapt_Stop();
						break;
					}
					else
					{
						buffer++;
					}
				}
				if(ret == E_AI2C_ERR_OK)
				{
					ret = AI2c_Adapt_Read_Byte(buffer,E_AI2C_NACK);//send last data and NACK
					AI2c_Adapt_Stop();
				}
				else
				{
					/* do nothing... */
				}
			}
		}
	}
	return (ret);
}

/***********************************************************************
* Name   		: AI2c_Adapt_Addr8_SequenceWrite
* Description  	: Sequence write
* Parameter    	: address: read reg addr, buffer:read data for register,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_SequenceWrite(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	unsigned short i = 0u;
	E_AI2C_ERR ret = E_AI2C_ERR_OK;
	AI2c_Adapt_Start();
	ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_WRITE_BIT);
	if(ret != E_AI2C_ERR_OK)//select address
	{
		AI2c_Adapt_Stop();
	}
	else
	{
		ret = AI2c_Adapt_Send_Byte(address);//select address
		if(ret != E_AI2C_ERR_OK)
		{
			AI2c_Adapt_Stop();
		}
		else
		{
			for(i = 0;i < Len;i++)
			{
				ret = AI2c_Adapt_Send_Byte(*buffer);
				if(ret != E_AI2C_ERR_OK)
				{
					break;
				}
				else
				{
					buffer++;
				}

			}
			AI2c_Adapt_Stop();
		}
	}

	return (ret);

}

#else
/***********************************************************************
* Name   		: AI2c_Adapt_Addr8_SignalRead
* Description  	: Signal read
* Parameter    	: address: read reg addr, buffer:read data for register,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_SignalRead(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	unsigned short i = 0u;
	E_AI2C_ERR ret = E_AI2C_ERR_OK;
	for(i = 0;i < Len;i++)
	{
		AI2c_Adapt_Start();
		ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_WRITE_BIT);
		if(ret != E_AI2C_ERR_OK)			//write mode
		{
			AI2c_Adapt_Stop();
			break;
		}
		else
		{
			ret = AI2c_Adapt_Send_Byte(address);//select address
			AI2c_Adapt_Stop();
			if(ret != E_AI2C_ERR_OK)
			{
				break;
			}
			else
			{
				AI2c_Adapt_Start();																//restart
				ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_READ_BIT);
				if(ret != E_AI2C_ERR_OK) 	//read mode
				{
					AI2c_Adapt_Stop();
					break;
				}
				else
				{	
					ret = AI2c_Adapt_Read_Byte(buffer,E_AI2C_NACK);//send last data and NACK
					AI2c_Adapt_Stop();
					if(ret != E_AI2C_ERR_OK)
					{
						break;
					}
					else
					{
						buffer++;
						address++;
					}	
				}
			}
		}
	}
	return (ret);

}



/***********************************************************************
* Name   		: AI2c_Adapt_Addr8_SignalWrite
* Description  	: Signal write
* Parameter    	: address: read reg addr, buffer:read data for register,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
static E_AI2C_ERR AI2c_Adapt_SignalWrite(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	unsigned short i = 0u;
	E_AI2C_ERR ret = E_AI2C_ERR_OK;
	for(i = 0;i < Len;i++)
	{
		AI2c_Adapt_Start();
		ret = AI2c_Adapt_Send_Byte(AI2C_SLAVE_ADDR | AI2C_WRITE_BIT);
		if(ret != E_AI2C_ERR_OK)//select address
		{
			AI2c_Adapt_Stop();
			break;
		}
		else
		{
			ret = AI2c_Adapt_Send_Byte(address);//select address
			if(ret != E_AI2C_ERR_OK)
			{
				AI2c_Adapt_Stop();
				break;
			}
			else
			{
				ret = AI2c_Adapt_Send_Byte(*buffer);
				if(ret != E_AI2C_ERR_OK)
				{
					AI2c_Adapt_Stop();
					break;
				}
				else
				{
					AI2c_Adapt_Stop();
				}

			}
			buffer++;
			address++;
		}
	}

	return (ret);
}
#endif


/***********************************************************************
* Name   		: AI2c_Adapt_Read
* Description  	: Read I2C basis on 8bit address
* Parameter    	: address: read reg addr, buffer:read data for register,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
E_AI2C_ERR AI2c_Adapt_Read(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	E_AI2C_ERR ret = E_AI2C_ERR_OK;
#ifdef AI2C_SEQUENCE_RW
	ret = AI2c_Adapt_SequenceRead(address,buffer,Len);
#else
	ret = AI2c_Adapt_SignalRead(address,buffer,Len);
#endif
	return (ret);
}

/***********************************************************************
* Name     		: AI2c_Adapt_Write
* Description  	: Write I2C basis on 8bit address
* Parameter    	: address: send reg addr, buffer:send data,Len: read data length
* Returns      	: E_AI2C_ERR_OK is successed,else failed
***********************************************************************/
E_AI2C_ERR AI2c_Adapt_Write(unsigned char address,unsigned char* buffer,unsigned short Len)
{
	E_AI2C_ERR ret = E_AI2C_ERR_OK;
#ifdef AI2C_SEQUENCE_RW
	ret = AI2c_Adapt_SequenceWrite(address,buffer,Len);
#else
	ret = AI2c_Adapt_SignalWrite(address,buffer,Len);
#endif
	return (ret);
}
#if 0
/***********************************************************************
* Name   		: AI2c_Adapt_Delay
* Description  	: Adjust I2C frequence
* Parameter    	: time:delay counter
* Returns      	: None
***********************************************************************/
static void AI2c_Adapt_Delay(int time)
{
	if(time > 0)
	{
    	while(time--);
	}
	else
	{
		/* do nothing... */
	}

}
#endif

