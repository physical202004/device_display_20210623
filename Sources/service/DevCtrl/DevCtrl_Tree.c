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

#include "DevCtrl_Tree.h"
#include "DevCtrl.h"
#include "Global_Definition.h"
#include <stdlib.h>

/* device manager struct */
static S_DEV_TREE gsDeviceTree = {0u,0u,0u,NULL};

/***********************************************************************
* Name        :DevCtrl_Tree_Init
* Description :Initial devices list
* Parameter   :None
* Returns     :None
***********************************************************************/
void DevCtrl_Tree_Init(void)
{
    gsDeviceTree.num = 0u;
    gsDeviceTree.head = NULL;
}

/***********************************************************************
* Name        :DevCtrl_Tree_Deinit
* Description :Deinitial devices list
* Parameter   :None
* Returns     :None
***********************************************************************/
void DevCtrl_Tree_Deinit(void)
{
    if(gsDeviceTree.num < 1u)
    {
        return;
    }
	else
	{
		DevCtrl_Tree_Init();
	}
}

/***********************************************************************
* Name        :DevCtrl_Tree_GetDevicesManager
* Description :Get devices manager struct
* Parameter   :None
* Returns     :device manager struct pointer
***********************************************************************/
S_DEV_TREE* DevCtrl_Tree_GetDevicesManager(void)
{
    return &gsDeviceTree;
}

/***********************************************************************
* Name        :DevCtrl_Tree_AddDevice
* Description :Add device in list
* Parameter   :dev:device
* Returns     :None
***********************************************************************/
void DevCtrl_Tree_AddDevice(S_DEV_INFO *dev)
{

    S_DEV_INFO *lsTail = NULL;
    if(gsDeviceTree.head == NULL)
    {
        gsDeviceTree.head = dev;
    }
    else
    {
        //traversal list
        for(lsTail = gsDeviceTree.head;lsTail->next != NULL;lsTail = lsTail->next);
        lsTail->next = dev;
    }
    dev->next = NULL;
    gsDeviceTree.num++;
    PRINT_DEV_MSG("add device",dev->name);
}

/***********************************************************************
* Name        :DevCtrl_Tree_RemoveDevice
* Description :Remove device for key in list
* Parameter   :key:keyword,cmp:compare function
* Returns     :TRUE:delete successed,FALSE:delete failed
***********************************************************************/
MYBOOL DevCtrl_Tree_RemoveDevice(const void *key, DEV_TREE_CMP_T cmp)
{
    S_DEV_INFO *lsNext = gsDeviceTree.head;
    S_DEV_INFO *lsPrev = gsDeviceTree.head;
    CHECK_POINTER("device list head is NULL,can't remove device!",gsDeviceTree.head,return FALSE);
    for(;lsNext != NULL;lsNext = lsNext->next)
    {
        if(cmp(key,lsNext) == 0)
        {
            lsPrev->next = lsNext->next;
            lsNext->next = NULL;
            gsDeviceTree.num--;
            PRINT_DEV_MSG("remove device",lsNext->name);
            return TRUE;
        }
        lsPrev = lsNext;
    }
    return FALSE;
}

/***********************************************************************
* Name        :DevCtrl_Tree_RemoveAllDeviceForKey
* Description :Remove all devices containing keywords
* Parameter   :key:keyword,cmp:compare function
* Returns     :None
***********************************************************************/
void DevCtrl_Tree_RemoveAllDeviceForKey(const void *key, DEV_TREE_CMP_T cmp)
{

    S_DEV_INFO *lsNext = gsDeviceTree.head;
    S_DEV_INFO *lsPrev = gsDeviceTree.head;
    CHECK_POINTER("device list head is NULL,can't remove devices!",gsDeviceTree.head,return);
    for(;lsNext != NULL;lsNext = lsNext->next)
    {
        if(cmp(key,lsNext) == 0)
        {
            lsPrev->next = lsNext->next;
            lsNext->next = NULL;
            gsDeviceTree.num--;
            PRINT_DEV_MSG("remove device",lsNext->name);
        }
        lsPrev = lsNext;
    }

}

/***********************************************************************
* Name        :DevCtrl_Tree_FindDeviceForKey
* Description :Find the device by keyword
* Parameter   :Key:keyword,cmp:compare function
* Returns     :Device struct
***********************************************************************/
S_DEV_INFO *DevCtrl_Tree_FindDeviceForKey(const void *key, DEV_TREE_CMP_T cmp)
{
    S_DEV_INFO *lsTail = NULL;
    if(gsDeviceTree.num < 1u)
    {
        /* return */
    }
	else
	{
		lsTail = gsDeviceTree.head;
	    for(;lsTail != NULL;lsTail = lsTail->next)
	    {
	        if(cmp(key,lsTail) == 0)
	        {
	            break;
	        }
			else
			{
				continue;
			}
	    }
	}
    return lsTail;
}

/***********************************************************************
* Name        :DevCtrl_Tree_GetDeviceNum
* Description :Get the total number of devices
* Parameter   :None
* Returns     :Devicces number
***********************************************************************/
DEV_ID_T DevCtrl_Tree_GetDeviceNum(void)
{
    return gsDeviceTree.num;
}

/***********************************************************************
* Name        :DevCtrl_Tree_GetPowerOnIndex
* Description :Get the current index for power on sequence for devices
* Parameter   :None
* Returns     :Index
***********************************************************************/
DEV_POWER_SQUENCE_T DevCtrl_Tree_GetPowerOnIndex(void)
{
    return gsDeviceTree.powerOnCnt;
}

/***********************************************************************
* Name        :DevCtrl_Tree_GetPowerOffIndex
* Description :Get the current index for power down sequence for devices
* Parameter   :None
* Returns     :Index
***********************************************************************/
DEV_POWER_SQUENCE_T DevCtrl_Tree_GetPowerOffIndex(void)
{
    return gsDeviceTree.powerOffCnt;
}

/***********************************************************************
* Name        :DevCtrl_Tree_SetPowerOnIndex
* Description :Change current index for power on sequence for devices
* Parameter   :None
* Returns     :Index
***********************************************************************/
void DevCtrl_Tree_SetPowerOnIndex(DEV_POWER_SQUENCE_T index)
{
    gsDeviceTree.powerOnCnt = index;
}

/***********************************************************************
* Name        :DevCtrl_Tree_SetPowerOffIndex
* Description :Change current index for power down sequence for devices
* Parameter   :None
* Returns     :Index
***********************************************************************/
void DevCtrl_Tree_SetPowerOffIndex(DEV_POWER_SQUENCE_T index)
{
    gsDeviceTree.powerOffCnt = index;
}

/***********************************************************************
* Name        :DevCtrl_Tree_SortDeviceByID
* Description :Sort by devices by id
* Parameter   :None
* Returns     :None
***********************************************************************/
void DevCtrl_Tree_SortDeviceByID(void)
{
    S_DEV_INFO *lsPrev = NULL;
    S_DEV_INFO *lsTail = NULL;
    DEV_TREE_NUM_T ltIndex = 0,ltCnt = 0;
    if(gsDeviceTree.num <= 1)
    {
        return;
    }
	else
	{
	    for(lsPrev = gsDeviceTree.head;lsPrev!= NULL;)
	    {
	        for(lsTail = lsPrev->next;lsTail != NULL;lsTail = lsTail->next)
	        {
	            if(DevCtrl_CmpId(&(lsPrev->id),lsTail) > 0)
	            {
	                lsPrev->next = lsTail->next;
	                lsTail->next = lsPrev;
	                if(ltCnt == 0)
	                {
	                    gsDeviceTree.head = lsTail;//cycle list
	                }
					else
					{
						continue;
					}
	            }
				else
				{
					continue;
				}
	        }
	        ltCnt++;
	        for(ltIndex = 0,lsPrev = gsDeviceTree.head;ltIndex < ltCnt;ltIndex++)
	        {
	            lsPrev = lsPrev->next;  //get next device via index
	        }
	    }
	}
}


