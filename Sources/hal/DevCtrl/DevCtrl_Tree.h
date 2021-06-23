/************************************************************************************************************
* Copyright (C) Huizhou Desay SV Automotive Co., Ltd.  
* All Rights Reserved
* Department 	: ADS R&D SW
* Author	   : Huang Haoxiong
*************************************************************************************************************
* Object        :
* Module        : device tree
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

#ifndef DEVCTRL_TREE_H
#define DEVCTRL_TREE_H
#include "DevCtrl_if.h"

typedef unsigned char DEV_TREE_NUM_T;
typedef int (*const DEV_TREE_CMP_T)(const void *,S_DEV_INFO *);

#define DEV_TREE_NUM_MAX 20

/* head orderly unidirection linked list */
typedef struct{
    DEV_TREE_NUM_T num;
    DEV_POWER_SQUENCE_T powerOnCnt;
    DEV_POWER_SQUENCE_T powerOffCnt;
    S_DEV_INFO* head;
}S_DEV_TREE;


extern void DevCtrl_Tree_Init(void);
extern void DevCtrl_Tree_Deinit(void);
extern S_DEV_TREE* DevCtrl_Tree_GetDevicesManager(void);
extern void DevCtrl_Tree_AddDevice(S_DEV_INFO *dev);
extern BOOL DevCtrl_Tree_RemoveDevice(const void *key, DEV_TREE_CMP_T cmp);
extern void DevCtrl_Tree_DelAllDeviceForKey(const void *key, DEV_TREE_CMP_T cmp);
extern S_DEV_INFO *DevCtrl_Tree_FindDeviceForKey(const void *key, DEV_TREE_CMP_T cmp);
extern DEV_ID_T DevCtrl_Tree_GetDeviceNum(void);
extern DEV_POWER_SQUENCE_T DevCtrl_Tree_GetPowerOnIndex(void);
extern DEV_POWER_SQUENCE_T DevCtrl_Tree_GetPowerOffIndex(void);
extern void DevCtrl_Tree_SetPowerOnIndex(DEV_POWER_SQUENCE_T index);
extern void DevCtrl_Tree_SetPowerOffIndex(DEV_POWER_SQUENCE_T index);
extern void DevCtrl_Tree_SortDeviceByID(void);

#endif

