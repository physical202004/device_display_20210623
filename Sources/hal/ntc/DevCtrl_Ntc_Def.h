
#ifdef DEV_NTC_PASS_1
#undef DEV_NTC_PASS_1
#define BEGIN_NTC_CTRL                    		enum DevNtcCtrl{
#define NTC_CTRL_DESC(x,y)              		DEV_##x##_NTC,
#define END_NTC_CTRL                      		DEV_NTC_NUMBER};
#include "DevCtrl_Ntc_Cfg.h"
#endif


#ifdef DEV_NTC_PASS_2
#undef DEV_NTC_PASS_2
#define BEGIN_NTC_CTRL                    		static const U8 scubNtcTempVal[DEV_NTC_NUMBER] = {
#define NTC_CTRL_DESC(x,y)              		x,
#define END_NTC_CTRL                      		};
#include "DevCtrl_Ntc_Cfg.h"
#endif

#ifdef DEV_NTC_PASS_3
#undef DEV_NTC_PASS_3
#define BEGIN_NTC_CTRL                    		static const DEV_ADC_16BIT_T sctNTCAdcVal[DEV_NTC_NUMBER] = {
#define NTC_CTRL_DESC(x,y)              		y,
#define END_NTC_CTRL                      		};
#include "DevCtrl_Ntc_Cfg.h"
#endif



