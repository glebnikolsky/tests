/*
Module : ntservServiceControlManager.h
Purpose: Defines the interface for the class CNTServiceControlManager. 
Created: PJN / 14-07-1998

Copyright (c) 1997 - 2017 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////////// Macros / Defines ////////////////////////////////

#pragma once

#ifndef __NTSERVSERVICECONTROLMANAGER_H__
#define __NTSERVSERVICECONTROLMANAGER_H__


////////////////////////////// Includes ///////////////////////////////////////

#include "ntservDefines.h"
#include "ntservScmService.h"


////////////////////////////// Classes ////////////////////////////////////////

//Class used by CNTServiceControlManager::QueryLockStatus
class CNTSERVICE_EXT_CLASS CNTServiceControlManagerLockStatus
{
public:
//Methods
  operator LPQUERY_SERVICE_LOCK_STATUS() noexcept
  {
#pragma warning(suppress: 26490)
    return reinterpret_cast<LPQUERY_SERVICE_LOCK_STATUS>(m_Data.m_pData);
  }

protected:
//Member variables
  ATL::CHeapPtr<BYTE> m_Data;

  friend class CNTServiceControlManager;
};


//An encapsulation of the NT Service Control Manager
class CNTSERVICE_EXT_CLASS CNTServiceControlManager
{
public:
//typedefs
  typedef BOOL (CALLBACK* ENUM_SERVICES_PROC)(void* pData, const ENUM_SERVICE_STATUS& ss);
  typedef BOOL (CALLBACK* ENUM_SERVICES_PROC2)(void* pData, const ENUM_SERVICE_STATUS_PROCESS& ssp);

//Constructors / Destructors
  CNTServiceControlManager() noexcept;
  CNTServiceControlManager(_In_ const CNTServiceControlManager&) = delete;
  CNTServiceControlManager(_In_ CNTServiceControlManager&&) = delete;
  ~CNTServiceControlManager();

//Methods
  CNTServiceControlManager& operator=(const CNTServiceControlManager&) = delete;
  CNTServiceControlManager& operator=(CNTServiceControlManager&&) = delete;
  //Allows access to the underlying SC_HANDLE representing the SCM
  operator SC_HANDLE() const noexcept;

  //Attach / Detach support from an SDK SC_HANDLE
  BOOL Attach(_In_opt_ SC_HANDLE hSCM) noexcept;
  SC_HANDLE Detach() noexcept;

  //Opens a connection to the SCM
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL Open(_In_opt_ LPCTSTR pszMachineName, _In_ DWORD dwDesiredAccess) noexcept;
  
  //Close the connection to the SCM
  void Close() noexcept;

  //Get the SCM Status
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryLockStatus(_Inout_ CNTServiceControlManagerLockStatus& lockStatus) const noexcept;

  //Enumerates the specified services
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL EnumServices(_In_ DWORD dwServiceType, _In_ DWORD dwServiceState, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC pEnumServicesFunc) const noexcept;

  //Opens the specified service
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL OpenService(_In_ LPCTSTR pServiceName, _In_ DWORD dwDesiredAccess, _Out_ CNTScmService& service) const noexcept;

  //Lock the SCM database
  _Return_type_success_(return != 0) BOOL Lock() noexcept;

  //Unlocks the SCM database
  _Return_type_success_(return != 0) BOOL Unlock() noexcept;

  //Windows 2000 specific functions
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL EnumServices(_In_ DWORD dwServiceType, _In_ DWORD dwServiceState, _In_opt_ LPCTSTR pszGroupName, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC2 pEnumServicesFunc) const;

protected:
//typedefs
  typedef SERVICE_STATUS_HANDLE (WINAPI REGISTERSERVICECTRLHANDLEREX)(LPCTSTR, LPHANDLER_FUNCTION_EX, LPVOID);
  typedef REGISTERSERVICECTRLHANDLEREX* LPREGISTERSERVICECTRLHANDLEREX;
  typedef BOOL (WINAPI ENUMSERVICESSTATUSEX)(SC_HANDLE, SC_ENUM_TYPE, DWORD, DWORD, LPBYTE, DWORD, LPDWORD, LPDWORD, LPDWORD, LPCTSTR);
  typedef ENUMSERVICESSTATUSEX* LPENUMSERVICESSTATUSEX;

//Member variables
  SC_HANDLE                      m_hSCM;                             //Handle to the SCM
  SC_LOCK                        m_hLock;                            //Handle of any lock on the Database
  HINSTANCE                      m_hAdvapi32;                        //Instance handle of the "ADVAPI32.DLL" which houses all of the NT Service functions
  LPREGISTERSERVICECTRLHANDLEREX m_pfnRegisterServiceCtrlHandlerEx;  //RegisterServiceCtrlHandler function pointer
  LPENUMSERVICESSTATUSEX         m_pfnEnumServicesStatusEx;          //EnumServicesStatusEx function pointer
};

#endif //#ifndef __NTSERVSERVICECONTROLMANAGER_H__
