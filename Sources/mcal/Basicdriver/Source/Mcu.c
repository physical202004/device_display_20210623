/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        : Mcu.c
* Module        :
* Instance      :
* Description   :
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comment__DSB__DSBs
** ----------------------------------------------------------------------
* 0.1    Feb 10,2013    NI Rujin    Initial version
* ----------------------------------------------------------------------
* 0.2    Mar 11,2013    LIU Chenxi  Fix issues found on entering LP
* ----------------------------------------------------------------------
* 0.3    Jun 09,2013    LIU Chenxi  Update P2 setting for switch on
         battery power supply when main power lost
------------------------------------------------------------------------
* 0.4	 July 07,2016  PEI Jingli  Adapt 64pin MCU
------------------------------------------------------------------------
* 0.5	 July 07,2016  LIU Chenxi  Refine code structure
------------------------------------------------------------------------
* 0.6	 July 07,2016  LIU Chenxi  merge with remotes/origin/ui4959_bs1
------------------------------------------------------------------------
* 0.7	 July 21,2016  PEI Jingli  Implement code review comments.
* ----------------------------------------------------------------------
* 0.8    Aug  01,2016  PEI Jingli  Change interface by MCU_PIN
* ----------------------------------------------------------------------
* 0.9    Aug  26,2016  PEI Jingli  Modify IO_Init inf for filtering noise
                       Modify mcu reset reason micro
* ----------------------------------------------------------------------
* 1.0    Nov 17,2016  PEI Jingli  Add default value setting of IO
* ----------------------------------------------------------------------
* 1.1    Dec 14,2016  Ren Weimin		Add PFCAE11,PFCE11 for IO config.
* ----------------------------------------------------------------------
* 1.2    Dec 20,2016  ShangQiuju		Remove unused include files.
* ----------------------------------------------------------------------
* 1.3    Dec 29,2016  PEI Jingli  Change for codeReview result
* ----------------------------------------------------------------------
* 1.4    Jan 06,2016  PEI Jingli  Add parameter checking
* ----------------------------------------------------------------------
* 1.5    Jan 06,2016  PEI Jingli  Delete pwmd code
*------------------------------------------------------------------------
* 1.6    Jan 13,2017  ZhangJie  Fix Coverity warning.
 ----------------------------------------------------------------------
* 1.7    Jan 16,2017  RenWeimin  Add test code. Add interface to get reset reason.
*-----------------------------------------------------------------------
* 1.8    Jan 24,2017   ZhangJie   Remove unused include file.
*-----------------------------------------------------------------------
* 1.9    Feb 09,2017   ZhangJie   Using micro to call app function.
* ----------------------------------------------------------------------
* 2.0    Feb 20,2017  PEI Jingli  Rename interface
* ----------------------------------------------------------------------
* 2.1    Mar 23,2017  PEI Jingli  Fix enter-stop mode processing.
*=====================================================================*/

#include "Mcu.h"
#include "Mcu_if.h"
#include "ics.h"
#include "nvic.h"
#include "Global_Definition.h"


MCU_RESET_TYPE sResetReason = MCU_RESET_BY_NONE;



MCU_RESET_TYPE mcu_get_reset_flag( void )
{
    return sResetReason;
}

/***********************************************************************
* Name        	: mcu_setup_high_clock
* Description	: setup mcu clock in high speed mode, and set cloclk domains related.
* Description	: None
* Returns     	: None
***********************************************************************/
void mcu_setup_high_clock( void )
{

}

/***********************************************************************
* Name        	: mcu_setup_low_clock
* Description	: setup mcu clock in low speed mode,
*  				 after this function the mcu speed to LSI frequecy
* Description	: None
* Returns     	: None
***********************************************************************/
void mcu_setup_low_clock( void )
{

}

/***********************************************************************
* Name        	: mcu_io_init_active
*  Description :always called after wakeup from sleep mode
*  Parameter   :None
* Returns     	: None
***********************************************************************/
void mcu_io_init_active( void )
{

}

/***********************************************************************
* Name        	: mcu_io_init_reset
* Description	: called after mcu reset
*  Parameter   :None
* Returns     	: None
***********************************************************************/
void mcu_io_init_reset( void )
{
    return;
}

/***********************************************************************
* Name        	: mcu_io_init_standby
* Description	: IO setting for standby mode
*  Parameter   :None
* Returns     	: None
***********************************************************************/
void mcu_io_init_standby( void )
{

}

/***********************************************************************
* Name        	: mcu_disable_all_interrupt
*  Description :
*  Parameter   :None
* Returns     	: None
***********************************************************************/
void mcu_disable_all_interrupt(void)
{

}

/***********************************************************************
* Name        	: mcu_enable_all_interrupt
*  Description :
*  Parameter   :None
* Returns     	: None
***********************************************************************/
void mcu_enable_all_interrupt(void)
{

}

/***********************************************************************
* Name        	: mcu_sw_reset
*  Description :set mcu reset register to software reset
*  Parameter   :None
*  called: app functions
* Returns     	: None
***********************************************************************/
void mcu_sw_reset( void )
{
    SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);
}

/***********************************************************************
* Name        	: mcu_get_reset_reason
* Description	: Get mcu reset reason of reset register.
*    If a deepstop caused reset, release IO hold state.
* Description	: None
* Returns     	: MCU_RESET_TYPE mcu reset type
***********************************************************************/
MCU_RESET_TYPE mcu_get_reset_reason( void )
{
    U32 ret_val = SIM_SRSID;

	if(ret_val == 0x02U)
	{
		sResetReason = MCU_RESET_BY_LVI;
	}
	else if(ret_val == 0x04U)
	{
		sResetReason = MCU_RESET_BY_ICS;
	}
	else if(ret_val == 0x20U)
	{
		sResetReason = MCU_RESET_BY_WDTA0;
	}
	else if(ret_val == 0x40U)
	{
		sResetReason = MCU_RESET_BY_EXTERNAL;
	}
	else if(ret_val == 0x80U)
	{
		sResetReason = MCU_RESET_BY_POWERUP;
	}
	else if(ret_val == 0x200U)
	{
		sResetReason = MCU_RESET_BY_CVM;
	}
	else if(ret_val == 0x400U)
	{
		sResetReason = MCU_RESET_BY_SW;
	}
	else if(ret_val == 0x800U)
	{
		sResetReason = MCU_RESET_BY_MDM_AP;
	}
	else if(ret_val == 0x2000U)
	{
		sResetReason = MCU_RESET_BY_STOP_ACK_ERR;
	}
	else
	{
		sResetReason = MCU_RESET_BY_NONE;
	}
    return sResetReason;
}


/***********************************************************************
* Name        	: mcu_io_init_sleep
*  Description :IO setting for sleep mode.
* Description	: None
* Returns     	: None
***********************************************************************/
void mcu_io_init_sleep( void )
{
    mcu_io_init_standby();
}

/***********************************************************************
* Name        	: mcu_io_init_sleep
*  Description :IO setting when power on.
* Description	: None
* Returns     	: None
***********************************************************************/
void mcu_io_init_poweron(void)
{
//    mcu_io_init_reset();
}

/***********************************************************************
* Name        	: mcu_io_init_wakeup
*  Description :IO initializing after wakeup from sleep.
* Description	: None
* Returns     	: None
***********************************************************************/
void mcu_io_init_wakeup(void)
{
    mcu_io_init_active();
}
/***********************************************************************
* Name        	: mcu_to_sleep_mode
*  Description :this function to make mcu to stop/ mode
* Description	: type MCU_STOP_MODE,MCU_DEEPSTOP_MODE
* Returns     	: None
***********************************************************************/
void mcu_to_sleep_mode( MCU_SLEEP_TYPE type )
{
    /*if((MCU_STOP_MODE==type)||(MCU_DEEPSTOP_MODE==type))
    {
        PowerMode_SetMode(PMC, type);
    }*/
}

/***********************************************************************
* Name        	: mcu_to_wakup_mode
* Description	: mcu handler afer wakeup from stand-by mode.If from deepstop mode,
                    release IO buffer hold-state first
* Description	: type always be MCU_FULL_MODE
* Returns     	: None
***********************************************************************/
void mcu_to_wakup_mode(MCU_WAKEUP_TYPE type)
{

}

/***********************************************************************
* Name        	: mcu_set_interrupt_table_address
* Description	:  Not used vector table.
* Description	: addr value
* Returns     	: None
***********************************************************************/
void mcu_set_irq_table_address(U32 addr)
{
    /*Set vector register*/
}

/***********************************************************************
* Name        	: mcu_init_clock
* Description	: init the mcu clock.
* Description	: 
* Returns     	: None
***********************************************************************/
void mcu_init_clock(void)
{
    /* Perform processor initialization */
	SIM_SOPT &= ~(SIM_SOPT_NMIE_MASK);	//disable NMI function
    ICS_ConfigType ICS_set={0};		/* Declaration of ICS_setup structure */
    ICS_set.u8ClkMode=ICS_CLK_MODE_FEE; /* ICS in FLL engaged internal mode*/
    ICS_set.bdiv=0;					/* bdiv bit equals 0, prescaler=1*/
    ICS_set.oscConfig.bEnable=1;	/* Osc enabled*/
    ICS_set.oscConfig.bRange=1;		/* High frequency range 4-24 MHz*/
    ICS_set.oscConfig.bIsCryst=1;	/* Oscillator clock source is selected*/
    ICS_set.oscConfig.bGain = 1;
    ICS_set.oscConfig.u32OscFreq=16000 ; /* 16MHz crystal, crytal is in KHz*/
    ICS_Init(&ICS_set);            		/*Initialization of core clock at 32MHz, bus clock 16MHz*/
	
	SysTick_Config(320000);      //bus clock = 16mhz,10ms
}

/**************** END OF FILE *****************************************/

