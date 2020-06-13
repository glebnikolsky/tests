/*  
Module : ntservServiceControlManager.cpp
Purpose: Implementation for the class CNTServiceControlManager
Created: PJN / 14-07-1997
History: PJN / 11-01-2006 CNTServiceControlManager::EnumServices method now uses a void* parameter for its 
                          item data instead of a DWORD.
         PJN / 18-05-2006 1. Minor update to rename the local variable of type _NTSERV_SERVICCONTROLMANAGER_DATA 
                          which is used to hold function pointers. Thanks to Frederic Metrich for reporting 
                          this issue.
         PJN / 25-06-2006 1. Combined the functionality of the _NTSERV_SERVICCONTROLMANAGER_DATA class into the 
                          main CNTServiceControlManager class.
                          2. Code now uses newer C++ style casts instead of C style casts.
         PJN / 18-12-2015 1. Added SAL annotations to all the code

Copyright (c) 1996 - 2018 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "ntservServiceControlManager.h"


///////////////////////////////// Implementation //////////////////////////////

CNTServiceControlManager::CNTServiceControlManager() noexcept : m_hSCM(nullptr),
                                                                m_hLock(nullptr),
                                                                m_pfnRegisterServiceCtrlHandlerEx(nullptr),
                                                                m_pfnEnumServicesStatusEx(nullptr)
{
  m_hAdvapi32 = GetModuleHandle(_T("ADVAPI32.DLL"));
  if (m_hAdvapi32 != nullptr)
  {
  #ifdef _UNICODE
#pragma warning(suppress: 26490)
    m_pfnRegisterServiceCtrlHandlerEx = reinterpret_cast<LPREGISTERSERVICECTRLHANDLEREX>(GetProcAddress(m_hAdvapi32, "RegisterServiceCtrlHandlerExW"));
#pragma warning(suppress: 26490)
    m_pfnEnumServicesStatusEx  = reinterpret_cast<LPENUMSERVICESSTATUSEX>(GetProcAddress(m_hAdvapi32, "EnumServicesStatusExW"));
  #else
#pragma warning(suppress: 26490)
    m_pfnRegisterServiceCtrlHandlerEx = reinterpret_cast<LPREGISTERSERVICECTRLHANDLEREX>(GetProcAddress(m_hAdvapi32, "RegisterServiceCtrlHandlerExA"));
#pragma warning(suppress: 26490)
    m_pfnEnumServicesStatusEx  = reinterpret_cast<LPENUMSERVICESSTATUSEX>(GetProcAddress(m_hAdvapi32, "EnumServicesStatusExA"));
  #endif //#ifdef _UNICODE
  }
}

CNTServiceControlManager::~CNTServiceControlManager()
{
  Unlock();
  Close();
}

CNTServiceControlManager::operator SC_HANDLE() const noexcept
{
  return m_hSCM;
}

BOOL CNTServiceControlManager::Attach(_In_opt_ SC_HANDLE hSCM) noexcept
{
  if (m_hSCM != hSCM)
    Close();

  m_hSCM = hSCM;
  return TRUE;
}

SC_HANDLE CNTServiceControlManager::Detach() noexcept
{
  SC_HANDLE hReturn = m_hSCM;
  m_hSCM = nullptr;
  return hReturn;
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTServiceControlManager::Open(_In_opt_ LPCTSTR pszMachineName, _In_ DWORD dwDesiredAccess) noexcept
{
  Close();
  m_hSCM = OpenSCManager(pszMachineName, SERVICES_ACTIVE_DATABASE, dwDesiredAccess);
  return (m_hSCM != nullptr);
}

void CNTServiceControlManager::Close() noexcept
{
  if (m_hSCM)
  {
    CloseServiceHandle(m_hSCM);
    m_hSCM = nullptr;
  }
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTServiceControlManager::QueryLockStatus(_Inout_ CNTServiceControlManagerLockStatus& lockStatus) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hSCM != nullptr);

  DWORD dwBytesNeeded = 0;
  BOOL bSuccess = QueryServiceLockStatus(m_hSCM, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    lockStatus.m_Data.Free();
    if (!lockStatus.m_Data.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    bSuccess = QueryServiceLockStatus(m_hSCM, lockStatus, dwBytesNeeded, &dwBytesNeeded);
  }
  return bSuccess;
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTServiceControlManager::EnumServices(_In_ DWORD dwServiceType, _In_ DWORD dwServiceState, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC pEnumServicesFunc) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hSCM != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pEnumServicesFunc != nullptr);

  DWORD dwBytesNeeded = 0;
  DWORD dwServices = 0;
  DWORD dwResumeHandle = 0;
  BOOL bSuccess = EnumServicesStatus(m_hSCM, dwServiceType, dwServiceState, nullptr, 0, &dwBytesNeeded, &dwServices, &dwResumeHandle);
  if (!bSuccess && (GetLastError() == ERROR_MORE_DATA))
  {
    //Allocate some memory for the API
    ATL::CHeapPtr<BYTE> pBuffer;
    if (!pBuffer.Allocate(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }

#pragma warning(suppress: 26489 26490)
    LPENUM_SERVICE_STATUS pServices = reinterpret_cast<LPENUM_SERVICE_STATUS>(pBuffer.m_pData);
#pragma warning(suppress: 26477)
    ATLASSUME(pServices != nullptr);
    DWORD dwSize = 0;
#pragma warning(suppress: 26486)
    bSuccess = EnumServicesStatus(m_hSCM, dwServiceType, dwServiceState, pServices, dwBytesNeeded, &dwSize, &dwServices, &dwResumeHandle);
    if (bSuccess)
    {
      BOOL bContinue = TRUE;
      for (DWORD i=0; i<dwServices && bContinue; i++)
#pragma warning(suppress: 26481)
        bContinue = pEnumServicesFunc(pUserData, pServices[i]);
    }
  }
  return bSuccess;
}

#pragma warning(suppress: 26429 26440)
_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTServiceControlManager::EnumServices(_In_ DWORD dwServiceType, _In_ DWORD dwServiceState, _In_opt_ LPCTSTR pszGroupName, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC2 pEnumServicesFunc) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pEnumServicesFunc != nullptr);

  //Check to see if the function pointer is available
  if (m_pfnEnumServicesStatusEx == nullptr)
  {
    ATLTRACE(_T("CNTServiceControlManager::EnumServices, EnumServicesStatusEx function is not supported on this OS. You need to be running at least Windows 2000 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  DWORD dwBytesNeeded = 0;
  DWORD dwServices = 0;
  DWORD dwResumeHandle = 0;
  BOOL bSuccess = m_pfnEnumServicesStatusEx(m_hSCM, SC_ENUM_PROCESS_INFO, dwServiceType, dwServiceState, nullptr, 0, &dwBytesNeeded, &dwServices, &dwResumeHandle, pszGroupName);
  if (!bSuccess && (GetLastError() == ERROR_MORE_DATA))
  {
    //Allocate some memory for the API
    ATL::CHeapPtr<BYTE> pBuffer;
    if (!pBuffer.Allocate(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }

#pragma warning(suppress: 26489 26490)
    LPENUM_SERVICE_STATUS_PROCESS pServices = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(pBuffer.m_pData);
#pragma warning(suppress: 26477)
    ATLASSUME(pServices != nullptr);
    DWORD dwSize = 0;
#pragma warning(suppress: 26486 26486 26490)
    bSuccess = m_pfnEnumServicesStatusEx(m_hSCM, SC_ENUM_PROCESS_INFO, dwServiceType, dwServiceState, reinterpret_cast<LPBYTE>(pServices), dwBytesNeeded, &dwSize, &dwServices, &dwResumeHandle, pszGroupName);
    if (bSuccess)
    {
      BOOL bContinue = TRUE;
      for (DWORD i=0; i<dwServices && bContinue; i++)
#pragma warning(suppress: 26481)
        bContinue = pEnumServicesFunc(pUserData, pServices[i]);
    }
  }
  return bSuccess;
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTServiceControlManager::OpenService(_In_ LPCTSTR pServiceName, _In_ DWORD dwDesiredAccess, _Out_ CNTScmService& service) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hSCM != nullptr);

  SC_HANDLE hService = ::OpenService(m_hSCM, pServiceName, dwDesiredAccess);
  const BOOL bSuccess = (hService != nullptr);
  if (bSuccess)
    service.Attach(hService);
  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTServiceControlManager::Lock() noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hSCM != nullptr);

  m_hLock = LockServiceDatabase(m_hSCM);
  return (m_hLock != nullptr);
}

_Return_type_success_(return != 0) BOOL CNTServiceControlManager::Unlock() noexcept
{
  BOOL bSuccess = TRUE;
  if (m_hLock)
  {
    bSuccess = UnlockServiceDatabase(m_hLock);
    m_hLock = nullptr;
  }

  return bSuccess;
}
