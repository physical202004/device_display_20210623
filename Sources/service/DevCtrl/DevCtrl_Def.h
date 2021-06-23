/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : device controller
* Instance      :
* Description   :  
*----------------------------------------------------------------------------
* Version       : 0.1
* Date          : 2019/03/08
* Author        : Huang Haoxiong
*************************************************************************************************************/
/*-Histor---------------------------------------------------------------------
* Version   Date         Name        Changes and comments
*----------------------------------------------------------------------------
*  0.1    2019/03/08  Huang Haoxiong      Initial
*************************************************************************************************************/


#ifdef DEV_POWER_PASS_1
#define BEGIN_POWERDOWN_CTRL            static const S_POWER_SEQ_INFO gsDevPowerOffSeq[] = {
#define DEVCTRL_DECS(x,y)               {x,y},
#define END_POWERDOWN_CTRL              };
#define BEGIN_POWERON_CTRL              static const S_POWER_SEQ_INFO gsDevPowerOnSeq[] = {
#define END_POWERON_CTRL                };
#include "DevCtrl_Cfg.h"
#endif

#ifdef DEV_FUNC_PASS_1
#define BEGIN_DEVFUNC                    		enum DevBaseFunc{
#define DEVFUNC_DESC(x)              		    x,
#define END_DEVFUNC                      		DEV_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif

#ifdef COMM_DEV_FUNC_PASS_1
#define BEGIN_COMMFUNC                    		enum CommBaseFunc{
#define COMMFUNC_DESC(x)              		    x,
#define END_COMMFUNC                      		COMM_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif

#ifdef BL_DEV_FUNC_PASS_1
#define BEGIN_BLFUNC                    		enum BLBaseFunc{
#define BLFUNC_DESC(x)              		    x,
#define END_BLFUNC                      		BL_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif

#ifdef FOG_DEV_FUNC_PASS_1
#define BEGIN_FOGFUNC              				enum FOGBaseFunc{
#define FOGFUNC_DESC(x)                  		x,
#define END_FOGFUNC                      		FOG_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif

#ifdef LVDS_DEV_FUNC_PASS_1
#define BEGIN_LVDSFUNC              			enum LVDSBaseFunc{
#define LVDSFUNC_DESC(x)                  		x,
#define END_LVDSFUNC                      		LVDS_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif

#ifdef TP_DEV_FUNC_PASS_1
#define BEGIN_TPFUNC              				enum TPBaseFunc{
#define TPFUNC_DESC(x)                  		x,
#define END_TPFUNC                      		TP_BASE_FUNC_NUM};
#include "DevCtrl_Cfg.h"
#endif


