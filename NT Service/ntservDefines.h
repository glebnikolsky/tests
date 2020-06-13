/*
Module : ntservDefines.h
Purpose: Various defines which the CNTService class framework which requires
Created: PJN / 08-09-2003

Copyright (c) 1997 - 2017 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


//////////////////////// Macros / Defines /////////////////////////////////////

#pragma once

#ifndef __NTSERVDEFINES_H__
#define __NTSERVDEFINES_H__


//////////////////////// Enums / Defines //////////////////////////////////////

#ifndef CNTSERVICE_EXT_CLASS
#define CNTSERVICE_EXT_CLASS
#endif //#ifndef CNTSERVICE_EXT_CLASS

#ifndef __ATLBASE_H__
#pragma message("To avoid this message, please put atlbase.h in your pre compiled header (normally stdafx.h)")
#include <atlbase.h>
#endif //#ifndef __ATLBASE_H__

#ifndef __ATLSTR_H__
#pragma message("To avoid this message, please put atlstr.h in your pre compiled header (normally stdafx.h)")
#include <atlstr.h>
#endif //#ifndef __ATLSTR_H__

#ifndef _STRING_
#pragma message("To avoid this message, please put string in your pre compiled header (normally stdafx.h)")
#include <string>
#endif //#ifndef _STRING_

#ifndef _VECTOR_
#pragma message("To avoid this message, please put vector in your pre compiled header (normally stdafx.h)")
#include <vector>
#endif //#ifndef _VECTOR_

#ifndef _ALGORITHM_
#pragma message("To avoid this message, please put algorithm in your pre compiled header (normally stdafx.h)")
#include <algorithm>
#endif //#ifndef _ALGORITHM_

#ifndef _ARRAY_
#pragma message("To avoid this message, please put array in your pre compiled header (normally stdafx.h)")
#include <array>
#endif //#ifndef _ARRAY_


#ifdef _UNICODE
#define CNTServiceString std::wstring
#define CNTServiceStringArray std::vector<std::wstring>
#else
#define CNTServiceString std::string
#define CNTServiceStringArray std::vector<std::string>
#endif //#ifdef _UNICODE


//Various defines from WinSvc.h to allow code to compile without the need for the latest Windows SDK
__if_not_exists (WTSSESSION_NOTIFICATION)
{
  typedef struct tagWTSSESSION_NOTIFICATION
  {
    DWORD cbSize;
    DWORD dwSessionId;
  } WTSSESSION_NOTIFICATION, *PWTSSESSION_NOTIFICATION;
}

#ifndef SERVICE_CONFIG_LAUNCH_PROTECTED
#define SERVICE_CONFIG_LAUNCH_PROTECTED 12
#endif //#ifndef SERVICE_CONFIG_LAUNCH_PROTECTED

#ifndef SERVICE_LAUNCH_PROTECTED_NONE
#define SERVICE_LAUNCH_PROTECTED_NONE 0
#endif //#ifndef SERVICE_LAUNCH_PROTECTED_NONE

#ifndef SERVICE_CONTROL_LOWRESOURCES
#define SERVICE_CONTROL_LOWRESOURCES 0x00000060
#endif //#ifndef SERVICE_CONTROL_LOWRESOURCES

#ifndef SERVICE_CONTROL_SYSTEMLOWRESOURCES
#define SERVICE_CONTROL_SYSTEMLOWRESOURCES 0x00000061
#endif //#ifndef SERVICE_CONTROL_SYSTEMLOWRESOURCES

__if_not_exists (_SERVICE_LAUNCH_PROTECTED_INFO)
{
  typedef struct _SERVICE_LAUNCH_PROTECTED_INFO 
  {
    DWORD dwLaunchProtected; //Service launch protected
  } SERVICE_LAUNCH_PROTECTED_INFO, *PSERVICE_LAUNCH_PROTECTED_INFO;
}

__if_not_exists (SC_EVENT_TYPE)
{
  typedef enum _SC_EVENT_TYPE
  {
    SC_EVENT_DATABASE_CHANGE,
    SC_EVENT_PROPERTY_CHANGE,
    SC_EVENT_STATUS_CHANGE
  } SC_EVENT_TYPE, *PSC_EVENT_TYPE;
}

__if_not_exists (SC_NOTIFICATION_CALLBACK)
{
  typedef
  VOID
  CALLBACK
  SC_NOTIFICATION_CALLBACK(
    _In_        DWORD                   dwNotify,
    _In_opt_    PVOID                   pCallbackContext
  );
}

__if_not_exists (PSC_NOTIFICATION_CALLBACK)
{
  typedef SC_NOTIFICATION_CALLBACK* PSC_NOTIFICATION_CALLBACK;
}

__if_not_exists (PSC_NOTIFICATION_REGISTRATION)
{
  typedef struct _SC_NOTIFICATION_REGISTRATION* PSC_NOTIFICATION_REGISTRATION;
}

__if_not_exists (SERVICE_REGISTRY_STATE_TYPE)
{
  typedef enum SERVICE_REGISTRY_STATE_TYPE
  {
    ServiceRegistryStateParameters = 0,
    ServiceRegistryStatePersistent = 1,
    MaxServiceRegistryStateType = 2,
  } SERVICE_REGISTRY_STATE_TYPE;
}

__if_not_exists (SERVICE_DIRECTORY_TYPE)
{
  typedef enum SERVICE_DIRECTORY_TYPE
  {
    ServiceDirectoryPersistentState = 0,
    ServiceDirectoryTypeMax = 1,
  } SERVICE_DIRECTORY_TYPE;
}

#endif //#ifndef __NTSERVDEFINES_H__
