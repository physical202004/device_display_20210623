/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.           *
*				  All Rights Reserved.             *
*   Department : ADS R&D SW		                   *
*   AUTHOR	   : Huang Haoxiong                    *
************************************************************************
* Object        :
* Module        : system
* Instance      :
* Description   : production version
*-----------------------------------------------------------------------
* Version       : 0.1
* Date          : 2018/01/08
* Author        : Huang Haoxiong
***********************************************************************/
/*-History--------------------------------------------------------------
* Version   Date         Name        Changes and comments
*------------------------------------------------------------------------
*  0.1    2018/01/08  Huang Haoxiong      Init
************************************************************************/
#include "ver.h"
#include "Define.h"

/*platform version: main version.internal version_release date_project*/
const char gccPlatformVersion[] = "02.02_200611_DP3L";

#if(FAW_HS5_PROJ_NAME == FAW_HS5_CSD_PROJ)//csd verison
/*software main version.software internal version_software release date__product stage*/
const char gccSysVersion[] = "04.01_210506___D";
#elif(FAW_HS5_PROJ_NAME == FAW_HS5_DIS_PROJ)//dis version
/*software main version.software internal version_software release date__product stage*/
const char gccSysVersion[] = "04.01_210506___D";
#endif

/*hardware main version.internal version*/
char gcHWVersion[] = "FF.FF";

//system checksum
unsigned char gaubSysCheckSum[2] = {0xFFu,0xFFu};

