/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : CT AD     									   *
*   AUTHOR	   : 									   *
************************************************************************
* Object        : 
* Module        : 
* Instance      : 
* Description   : 
*-----------------------------------------------------------------------
* Version: 
* Date: 
* Author: 
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date      Name    Changes and comments
 ----------------------------------------------------------------------
* 0.1    Jun 26,2016                    Initial version
 ----------------------------------------------------------------------
* 0.2    Dec 21,2016    PEI Jingli  add file patch according using hw
 ----------------------------------------------------------------------
* 0.3    Jan 5,2017       ShangQiuju   using signal_cfg.h Pacth configed at config.h
----------------------------------------------------------------------
* 0.4	Dec,18,2018		Huang,Haoxiong	1.Change function parameter from U16* to void*
										2.Define new types for signal
										3.Add function for classified by type
*------------------------------------------------------------------------
* 0.5	Mar,18,2019		Huang,Haoxiong	1.Removal of hardware-related functions

*=====================================================================*/

#ifdef SIGNAL_PASS_1
    #undef SIGNAL_PASS_1
    #define BEGIN_SIG        			enum Sig_SignalName {
    #define END_SIG          			E_SIG_SIGNALS_NUM };
    #define SIGDESC(x)          		x,
    #define BEGIN_SIG_MYBOOL
    #define END_SIG_MYBOOL
	#define BEGIN_SIG_CHAR
	#define END_SIG_CHAR
	#define BEGIN_SIG_SHORT
	#define END_SIG_SHORT
	#define BEGIN_SIG_FLOAT
	#define END_SIG_FLOAT
	#define BEGIN_SIG_ENUM
	#define END_SIG_ENUM
	#define BEGIN_SIG_INTEGER
	#define END_SIG_INTEGER
    #include "Signal_cfg.h"
#endif


#ifdef SIGNAL_PASS_2
    #undef SIGNAL_PASS_2
    #define BEGIN_SIG
    #define END_SIG
    #define SIGDESC(x)         			_##x##_,
    #define BEGIN_SIG_MYBOOL	  		enum Sig_SigBoolName {
    #define END_SIG_MYBOOL	  			E_SIG_MYBOOL_NUM };
	#define BEGIN_SIG_CHAR	  			enum Sig_SigCharName {
	#define END_SIG_CHAR	  			E_SIG_CHAR_NUM };
	#define BEGIN_SIG_SHORT	  			enum Sig_SigShortName {
	#define END_SIG_SHORT	  	  		E_SIG_SHORT_NUM };
	#define BEGIN_SIG_FLOAT	  			enum Sig_SigFloatName {
	#define END_SIG_FLOAT		  		E_SIG_FLOAT_NUM };
	#define BEGIN_SIG_ENUM 				enum Sig_SigEnumName {
	#define END_SIG_ENUM	  			E_SIG_ENUM_NUM };
	#define BEGIN_SIG_INTEGER 			enum Sig_SigIntegerName {
	#define END_SIG_INTEGER	  			E_SIG_INT_NUM };
    #include "Signal_cfg.h"
#endif

#define SIG_REGISTER_MYBOOL_TYPE(SIG_NAME,INITIAL_STATE)                                   	\
do{                                                                                      	\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0)                          	\
		&&(_##SIG_NAME##_ < E_SIG_MYBOOL_NUM)) 												\
    {                                                                                    	\
        AllSignal[SIG_NAME].type = E_SIG_MYBOOL;                                        	\
        AllSignal[SIG_NAME].sourename = _##SIG_NAME##_;        	                         	\
        gtSigBool[_##SIG_NAME##_] = INITIAL_STATE;                                     		\
    }                                                                                    	\
}while(0)

#define SIG_REGISTER_CHAR_TYPE(SIG_NAME,INITIAL_STATE)                                   	\
do{                                                                                      	\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0)                          	\
		&&(_##SIG_NAME##_ < E_SIG_CHAR_NUM)) 												\
	{                                                                                    	\
		AllSignal[SIG_NAME].type = E_SIG_CHAR;                                        		\
		AllSignal[SIG_NAME].sourename = _##SIG_NAME##_;        	                         	\
		gtSigChar[_##SIG_NAME##_] = INITIAL_STATE;                                     		\
	}                                                                                    	\
}while(0)

#define SIG_REGISTER_SHORT_TYPE(SIG_NAME,INITIAL_STATE)                                   	\
do{ 																						\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0) 							\
		&&(_##SIG_NAME##_ < E_SIG_SHORT_NUM))												\
	{																						\
		AllSignal[SIG_NAME].type = E_SIG_SHORT;												\
		AllSignal[SIG_NAME].sourename = _##SIG_NAME##_; 									\
		gtSigShort[_##SIG_NAME##_] = INITIAL_STATE;											\
	}																						\
}while(0)

#define SIG_REGISTER_FLOAT_TYPE(SIG_NAME,INITIAL_STATE)                                   	\
do{ 																						\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0) 							\
		&&(_##SIG_NAME##_ < E_SIG_FLOAT_NUM)) 												\
	{																						\
		AllSignal[SIG_NAME].type = E_SIG_FLOAT;												\
		AllSignal[SIG_NAME].sourename = _##SIG_NAME##_; 									\
		gtSigFloat[_##SIG_NAME##_] = INITIAL_STATE;											\
	}																						\
}while(0)

#define SIG_REGISTER_ENUM_TYPE(SIG_NAME,INITIAL_STATE)                                	 	\
do{ 																						\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0) 							\
				&&(_##SIG_NAME##_ < E_SIG_ENUM_NUM)) 										\
	{																						\
		AllSignal[SIG_NAME].type = E_SIG_ENUM;												\
		AllSignal[SIG_NAME].sourename = _##SIG_NAME##_; 									\
		gtSigEnum[_##SIG_NAME##_] = (SIGNAL_ENUM_32BIT_T)INITIAL_STATE;						\
	}																						\
}while(0)

#define SIG_REGISTER_INTEGER_TYPE(SIG_NAME,INITIAL_STATE)                                 	\
do{ 																						\
    if((SIG_NAME < E_SIG_SIGNALS_NUM) && (E_SIG_SIGNALS_NUM > 0)							\
		&&(_##SIG_NAME##_ < E_SIG_INT_NUM)) 												\
	{																						\
		AllSignal[SIG_NAME].type = E_SIG_INTEGER;											\
		AllSignal[SIG_NAME].sourename = _##SIG_NAME##_; 									\
		gtSigInteger[_##SIG_NAME##_] = INITIAL_STATE;										\
	}																						\
}while(0)

/****************************** END OF FILE ***************************/

