/*  
Module : ntservScmService.cpp
Purpose: Implementation for the class CNTScmService
Created: PJN / 14-07-1997
History: PJN / 09-01-2006 1. When registering the service, the quoting of the path name to use for the service 
                          is now done in CNTService::ProcessShellCommand instead of CNTScmService::Create.
         PJN / 18-05-2006 1. Minor update to rename the local variable of type _NTSERV_SCMSERVICE_DATA which is 
                          used to hold function pointers. Thanks to Frederic Metrich for reporting this issue.
         PJN / 25-06-2006 1. Combined the functionality of the _NTSERV_SCMSERVICE_DATA class into the 
                          main CNTScmService class.
                          2. Code now uses newer C++ style casts instead of C style casts.
                          3. Made the item data parameter to CNTScmService::EnumDependents a void* instead of 
                          the current DWORD.
         PJN / 01-08-2010 1. Added support for NotifyServiceStatusChange
                          2. Added support for ControlServiceEx
                          3. Added support for SERVICE_CONFIG_DELAYED_AUTO_START_INFO
                          4. Added support for SERVICE_CONFIG_FAILURE_ACTIONS_FLAG
                          5. Added support for SERVICE_CONFIG_SERVICE_SID_INFO
                          6. Added support for SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO
                          7. Added support for SERVICE_CONFIG_PRESHUTDOWN_INFO
                          8. Added support for SERVICE_CONFIG_TRIGGER_INFO
                          9. Added support for SERVICE_CONFIG_PREFERRED_NODE
         PJN / 10-11-2012 1. Reworked CNTScmService::WaitForServiceStatus method to avoid the need for calling
                          GetTickCount
         PJN / 18-12-2015 1. Added SAL annotations to all the code
         PJN / 16-04-2017 1. Added support for SubscribeServiceChangeNotifications, UnsubscribeServiceChangeNotifications
                          and WaitServiceState API calls from the latest Windows 10 SDK.
                          2. Removed CNTScmService::AcceptStop, AcceptPauseContinue & AcceptShutdown methods as they
                          provided not very useful encapsulated functionality.

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
#include "ntservDefines.h"
#include "ntservScmService.h"
#include "ntservServiceControlManager.h"


///////////////////////////////// Implementation //////////////////////////////

CNTScmService::CNTScmService() noexcept : m_hService(nullptr),
                                          m_pfnNotifyServiceStatusChange(nullptr),
                                          m_pfnControlServiceEx(nullptr),
                                          m_pfnSubscribeServiceChangeNotifications(nullptr),
                                          m_pfnUnsubscribeServiceChangeNotifications(nullptr),
                                          m_pfnWaitServiceState(nullptr),
                                          m_hSecHost(nullptr)
{
  m_hAdvapi32 = GetModuleHandle(_T("ADVAPI32.DLL"));
  if (m_hAdvapi32 != nullptr)
  {
  #ifdef _UNICODE
#pragma warning(suppress: 26490)
    m_pfnNotifyServiceStatusChange = reinterpret_cast<LPNOTIFYSERVICESTATUSCHANGE>(GetProcAddress(m_hAdvapi32, "NotifyServiceStatusChangeW"));
#pragma warning(suppress: 26490)
    m_pfnControlServiceEx = reinterpret_cast<LPCONTROLSERVICEEX>(GetProcAddress(m_hAdvapi32, "ControlServiceExW"));
  #else
#pragma warning(suppress: 26490)
    m_pfnNotifyServiceStatusChange = reinterpret_cast<LPNOTIFYSERVICESTATUSCHANGE>(GetProcAddress(m_hAdvapi32, "NotifyServiceStatusChangeA"));
#pragma warning(suppress: 26490)
    m_pfnControlServiceEx = reinterpret_cast<LPCONTROLSERVICEEX>(GetProcAddress(m_hAdvapi32, "ControlServiceExA"));
  #endif //#ifdef _UNICODE
#pragma warning(suppress: 26490)
    m_pfnWaitServiceState = reinterpret_cast<LPWAITSERVICESTATE>(GetProcAddress(m_hAdvapi32, "WaitServiceState"));
  }
  m_hSecHost = GetModuleHandle(_T("SECHOST.DLL"));
  if (m_hSecHost != nullptr)
  {
#pragma warning(suppress: 26490)
    m_pfnSubscribeServiceChangeNotifications = reinterpret_cast<LPSUBSCRIBESERVICECHANGENOTIFICIATIONS>(GetProcAddress(m_hSecHost, "SubscribeServiceChangeNotifications"));
#pragma warning(suppress: 26490)
    m_pfnUnsubscribeServiceChangeNotifications = reinterpret_cast<LPUNSUBSCRIBESERVICECHANGENOTIFICATIONS>(GetProcAddress(m_hSecHost, "UnsubscribeServiceChangeNotifications"));
  }
}

CNTScmService::~CNTScmService()
{
  Close();
}

void CNTScmService::Close() noexcept
{
  if (m_hService)
  {
    CloseServiceHandle(m_hService);
    m_hService = nullptr;
  }
}

CNTScmService::operator SC_HANDLE() const noexcept
{
  return m_hService;
}

_Return_type_success_(return != 0) BOOL CNTScmService::Attach(_In_opt_ SC_HANDLE hService) noexcept
{
  if (m_hService != hService)
    Close();

  m_hService = hService;
  return TRUE;
}

SC_HANDLE CNTScmService::Detach() noexcept
{
  SC_HANDLE hReturn = m_hService;
  m_hService = nullptr;
  return hReturn;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeConfig(_In_ DWORD dwServiceType, _In_ DWORD dwStartType,
                                                                    _In_ DWORD dwErrorControl, _In_opt_ LPCTSTR pBinaryPathName,
                                                                    _In_opt_ LPCTSTR pLoadOrderGroup, _Out_opt_ LPDWORD pdwTagId,
                                                                    _In_opt_ LPCTSTR pDependencies, _In_opt_ LPCTSTR pServiceStartName,
                                                                    _In_opt_ LPCTSTR pPassword, _In_opt_ LPCTSTR pDisplayName) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return ChangeServiceConfig(m_hService, dwServiceType, dwStartType, dwErrorControl, pBinaryPathName, pLoadOrderGroup, pdwTagId,
                             pDependencies, pServiceStartName, pPassword, pDisplayName);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Control(_In_ DWORD dwControl) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);
  
  SERVICE_STATUS ServiceStatus;
  return ControlService(m_hService, dwControl, &ServiceStatus);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Stop() const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);
  
  SERVICE_STATUS ServiceStatus;
  return ControlService(m_hService, SERVICE_CONTROL_STOP, &ServiceStatus);
}

_Return_type_success_(return != 0) BOOL CNTScmService::WaitForStop(_In_ DWORD dwTimeout) noexcept
{
  return WaitForServiceStatus(dwTimeout, SERVICE_STOPPED);
}

_Return_type_success_(return != 0) BOOL CNTScmService::WaitForServiceStatus(_In_ DWORD dwTimeout, _In_ DWORD dwWaitForStatus, _In_ DWORD dwPollingInterval) noexcept
{
  //wait for our desired status
  DWORD dwTimeWaited = 0;
  SERVICE_STATUS ss;
  for (;;)
  {
    if (!QueryStatus(ss))
      break;

    //if this is the status we were waiting for then we're done
    if (ss.dwCurrentState == dwWaitForStatus)
      return TRUE;

    //Do the wait
    Sleep(dwPollingInterval);

    //check for timeout
    dwTimeWaited += dwPollingInterval;
    if ((dwTimeout != INFINITE) && (dwTimeWaited > dwTimeout))
    {
      SetLastError(ERROR_TIMEOUT);
      break;
    }
  }
  return FALSE;
}

_Return_type_success_(return != 0) BOOL CNTScmService::Pause() const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_STATUS ServiceStatus;
  return ControlService(m_hService, SERVICE_CONTROL_PAUSE, &ServiceStatus);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Continue() const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_STATUS ServiceStatus;
  return ControlService(m_hService, SERVICE_CONTROL_CONTINUE, &ServiceStatus);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Interrogate() const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_STATUS ServiceStatus;
  return ControlService(m_hService, SERVICE_CONTROL_INTERROGATE, &ServiceStatus);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Start(_In_ DWORD dwNumServiceArgs, _In_reads_opt_(dwNumServiceArgs) LPCTSTR* pServiceArgVectors) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return StartService(m_hService, dwNumServiceArgs, pServiceArgVectors);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryStatus(_Out_ SERVICE_STATUS& ServiceStatus) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return QueryServiceStatus(m_hService, &ServiceStatus);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryStatus(_Out_ SERVICE_STATUS_PROCESS& ssp) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  return QueryServiceStatusEx(m_hService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
}

_Return_type_success_(return != 0) BOOL CNTScmService::QueryConfig(_Inout_ CNTServiceConfig& config) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  DWORD dwBytesNeeded = 0;
  BOOL bSuccess = QueryServiceConfig(m_hService, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    config.m_Data.Free();
    if (!config.m_Data.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    DWORD dwSize = 0;
    bSuccess = QueryServiceConfig(m_hService, config, dwBytesNeeded, &dwSize);
  }
  return bSuccess;
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::Create(_In_ const CNTServiceControlManager& Manager, _In_ LPCTSTR pServiceName, _In_opt_ LPCTSTR pDisplayName,	_In_ DWORD dwDesiredAccess,	_In_ DWORD dwServiceType, _In_ DWORD dwStartType, _In_ DWORD dwErrorControl,
                                                                                    _In_opt_ LPCTSTR pBinaryPathName, _In_opt_ LPCTSTR pLoadOrderGroup, _Out_opt_ LPDWORD pdwTagId, _In_opt_ LPCTSTR pDependencies, _In_opt_ LPCTSTR pServiceStartName, _In_opt_ LPCTSTR pPassword) noexcept
{
  Close();

  m_hService = CreateService(Manager, pServiceName, pDisplayName, dwDesiredAccess, dwServiceType, dwStartType, dwErrorControl, pBinaryPathName, pLoadOrderGroup, pdwTagId, pDependencies, pServiceStartName, pPassword);
  return (m_hService != nullptr);
}

_Return_type_success_(return != 0) BOOL CNTScmService::Delete() const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return DeleteService(m_hService);
}

_Return_type_success_(return != 0) BOOL CNTScmService::SetObjectSecurity(_In_ SECURITY_INFORMATION dwSecurityInformation, _In_ PSECURITY_DESCRIPTOR pSecurityDescriptor) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return SetServiceObjectSecurity(m_hService, dwSecurityInformation, pSecurityDescriptor);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryObjectSecurity(_In_ SECURITY_INFORMATION dwSecurityInformation, _Inout_ CNTServiceSecurityDescriptor& securityDescriptor) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  DWORD dwBytesNeeded = 0;
  BOOL bSuccess = QueryServiceObjectSecurity(m_hService, dwSecurityInformation, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    securityDescriptor.m_Data.Free();
    if (!securityDescriptor.m_Data.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    DWORD dwSize = 0;
    bSuccess = QueryServiceObjectSecurity(m_hService, dwSecurityInformation, securityDescriptor, dwBytesNeeded, &dwSize);
  }

  return bSuccess;
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::EnumDependents(_In_ DWORD dwServiceState, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC pEnumServicesFunc) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pEnumServicesFunc != nullptr);

  DWORD dwBytesNeeded = 0;
  DWORD dwServices = 0;
  BOOL bSuccess = EnumDependentServices(m_hService, dwServiceState, nullptr, 0, &dwBytesNeeded, &dwServices);
  const DWORD dwLastError = GetLastError();
  if (!bSuccess && (dwLastError == ERROR_MORE_DATA) || (dwLastError == ERROR_INSUFFICIENT_BUFFER)) //Note we use ERROR_INSUFFICIENT_BUFFER here even though it is not documented as a legal return value from EnumDependentServices here
  {
    //Allocate some memory for the API
    ATL::CHeapPtr<ENUM_SERVICE_STATUS> pServices;
    if (!pServices.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }

    DWORD dwSize = 0;
    bSuccess = EnumDependentServices(m_hService, dwServiceState, pServices.m_pData, dwBytesNeeded, &dwSize, &dwServices);
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

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeDescription(_In_ const CNTServiceString& sDescription)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_DESCRIPTION sd;
  CNTServiceString sTemp(sDescription);
#pragma warning(suppress: 26446)
  sd.lpDescription = &(sTemp[0]);
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_DESCRIPTION, &sd);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryDescription(_Out_ CNTServiceString& sDescription) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  //Initialize the output parameter to default value.
  sDescription = CNTServiceString();

  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 28020)
  BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_DESCRIPTION, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    ATL::CHeapPtr<BYTE> description;
    if (!description.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_DESCRIPTION, description, dwBytesNeeded, &dwBytesNeeded);
    if (bSuccess)
    {
#pragma warning(suppress: 26489 26490)
      LPCTSTR pszDescription = reinterpret_cast<LPSERVICE_DESCRIPTION>(description.m_pData)->lpDescription;
      if (pszDescription != nullptr)
#pragma warning(suppress: 26486)
        sDescription = pszDescription;
      else
        sDescription.clear();
    }
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeFailureActions(_In_opt_ LPSERVICE_FAILURE_ACTIONS pFailureActions) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_FAILURE_ACTIONS, pFailureActions);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryFailureActions(_Inout_ CNTServiceFailureActions& actions) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 28020)
  BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_FAILURE_ACTIONS, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    actions.m_Data.Free();
    if (!actions.m_Data.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_FAILURE_ACTIONS, actions.m_Data, dwBytesNeeded, &dwBytesNeeded);
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeDelayAutoStart(_In_ BOOL bDelayedAutoStart) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_DELAYED_AUTO_START_INFO sdasi;
  sdasi.fDelayedAutostart = bDelayedAutoStart;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &sdasi);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryDelayAutoStart(_Out_ BOOL& bDelayedAutoStart) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_DELAYED_AUTO_START_INFO sdasi = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, reinterpret_cast<LPBYTE>(&sdasi), sizeof(sdasi), &dwBytesNeeded);
  if (bSuccess)
    bDelayedAutoStart = sdasi.fDelayedAutostart;

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeFailureActionsFlag(_In_ BOOL bFailureActionsOnNonCrashFailures) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_FAILURE_ACTIONS_FLAG sfaf;
  sfaf.fFailureActionsOnNonCrashFailures = bFailureActionsOnNonCrashFailures;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, &sfaf);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryFailureActionsFlag(_Out_ BOOL& bFailureActionsOnNonCrashFailures) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_FAILURE_ACTIONS_FLAG sfaf = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, reinterpret_cast<LPBYTE>(&sfaf), sizeof(sfaf), &dwBytesNeeded);
  if (bSuccess)
    bFailureActionsOnNonCrashFailures = sfaf.fFailureActionsOnNonCrashFailures;

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeSidInfo(_In_ DWORD dwServiceSidType) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_SID_INFO ssi;
  ssi.dwServiceSidType = dwServiceSidType;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_SERVICE_SID_INFO, &ssi);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QuerySidInfo(_Out_ DWORD& dwServiceSidType) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_SID_INFO ssi = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_SERVICE_SID_INFO, reinterpret_cast<LPBYTE>(&ssi), sizeof(ssi), &dwBytesNeeded);
  if (bSuccess)
    dwServiceSidType = ssi.dwServiceSidType;

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeRequiredPrivileges(_In_ const CNTServiceStringArray& privileges) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  //Work out the size of the buffer we will need
  CNTServiceStringArray::size_type nSize = 0;
  for (const auto& privilege : privileges)
    nSize += privilege.length() + 1; //1 extra for each null terminator

  //Need one second null for the double null at the end
  nSize++;

  //Allocate some memory for the API
  ATL::CHeapPtr<TCHAR> pBuffer;
  if (!pBuffer.Allocate(nSize))
  {
    SetLastError(ERROR_OUTOFMEMORY);
    return FALSE;
  }

  //Now copy the strings into the buffer
#pragma warning(suppress: 26489)
  LPTSTR pszString = pBuffer.m_pData;
#pragma warning(suppress: 26477)
  ATLASSUME(pszString != nullptr);
  CNTServiceStringArray::size_type nCurOffset = 0;
  for (const auto& privilege : privileges)
  {
    const CNTServiceString::size_type nCurrentStringLength = privilege.length();
#pragma warning(suppress: 26489 26486 26481)
    _tcscpy_s(&pszString[nCurOffset], nCurrentStringLength+1, privilege.c_str());
    nCurOffset += (nCurrentStringLength + 1);
  }

  //Don't forgot to doubly null terminate
#pragma warning(suppress: 26489 26481)
  pszString[nCurOffset] = _T('\0');

  SERVICE_REQUIRED_PRIVILEGES_INFO srpi;
#pragma warning(suppress: 26489)
  srpi.pmszRequiredPrivileges = pBuffer.m_pData;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, &srpi);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryRequiredPrivileges(_Out_ CNTServiceStringArray& privileges) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  //Reset the output parameter
  privileges.clear();

  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 28020)
  BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    ATL::CHeapPtr<BYTE> privilegesData;
    if (!privilegesData.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, privilegesData.m_pData, dwBytesNeeded, &dwBytesNeeded);
    if (bSuccess)
    {
#pragma warning(suppress: 26490)
      LPTSTR pszStrings = reinterpret_cast<LPTSTR>(privilegesData.m_pData);
#pragma warning(suppress: 26477)
      ATLASSUME(pszStrings != nullptr);
#pragma warning(suppress: 26481)
      while (pszStrings[0] != _T('\0'))
      {
#pragma warning(suppress: 26489)
        privileges.push_back(pszStrings);
#pragma warning(suppress: 26481)
        pszStrings += (_tcslen(pszStrings ) + 1);
      }
    }
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangePreShutdown(_In_ DWORD dwPreshutdownTimeout) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_PRESHUTDOWN_INFO spsi;
  spsi.dwPreshutdownTimeout = dwPreshutdownTimeout;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_PRESHUTDOWN_INFO, &spsi);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryPreShutdown(_Out_ DWORD& dwPreshutdownTimeout) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_PRESHUTDOWN_INFO spsi = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_PRESHUTDOWN_INFO, reinterpret_cast<LPBYTE>(&spsi), sizeof(spsi), &dwBytesNeeded);
  if (bSuccess)
    dwPreshutdownTimeout = spsi.dwPreshutdownTimeout;

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeTrigger(_In_ PSERVICE_TRIGGER_INFO pTriggerInfo) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_TRIGGER_INFO, pTriggerInfo);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryTrigger(_Inout_ CNTServiceTriggerInfo& triggerInfo) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  DWORD dwBytesNeeded = 0;
  BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_TRIGGER_INFO, nullptr, 0, &dwBytesNeeded);
  if (!bSuccess && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
  {
    triggerInfo.m_Data.Free();
    if (!triggerInfo.m_Data.AllocateBytes(dwBytesNeeded))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
#pragma warning(suppress: 26490)
    bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_TRIGGER_INFO, reinterpret_cast<LPBYTE>(triggerInfo.operator PSERVICE_TRIGGER_INFO()), dwBytesNeeded, &dwBytesNeeded);
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangePreferredNode(_In_ USHORT usPreferredNode, _In_ BOOL bDelete) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_PREFERRED_NODE_INFO spni;
  spni.usPreferredNode = usPreferredNode;
  spni.fDelete = bDelete ? 1 : 0;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_PREFERRED_NODE, &spni);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryPreferredNode(_Out_ USHORT& usPreferredNode, _Out_ BOOL& bDelete) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  SERVICE_PREFERRED_NODE_INFO spni = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_PREFERRED_NODE, reinterpret_cast<LPBYTE>(&spni), sizeof(spni), &dwBytesNeeded);
  if (bSuccess)
  {
    usPreferredNode = spni.usPreferredNode;
    bDelete = spni.fDelete;
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTScmService::ChangeLaunchProtected(_In_ DWORD dwLaunchProtected) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  _SERVICE_LAUNCH_PROTECTED_INFO slpi;
  slpi.dwLaunchProtected = dwLaunchProtected;
  return ChangeServiceConfig2(m_hService, SERVICE_CONFIG_LAUNCH_PROTECTED, &slpi);
}

_Must_inspect_result_ _Return_type_success_(return != 0) BOOL CNTScmService::QueryLaunchProtected(_Out_ DWORD& dwLaunchProtected) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hService != nullptr);

  _SERVICE_LAUNCH_PROTECTED_INFO slpi = { 0 };
  DWORD dwBytesNeeded = 0;
#pragma warning(suppress: 26490)
  const BOOL bSuccess = QueryServiceConfig2(m_hService, SERVICE_CONFIG_LAUNCH_PROTECTED, reinterpret_cast<LPBYTE>(&slpi), sizeof(slpi), &dwBytesNeeded);
  if (bSuccess)
    dwLaunchProtected = slpi.dwLaunchProtected;

  return bSuccess;
}

#pragma warning(suppress: 26440)
DWORD CNTScmService::NotifyStatusChange(_In_ DWORD dwNotifyMask, _In_ PSERVICE_NOTIFY pNotifyBuffer) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_hService != nullptr);

  //Check to see if the function pointer is available
  if (m_pfnNotifyServiceStatusChange == nullptr)
  {
    ATLTRACE(_T("CNTScmService::NotifyStatusChange, NotifyServiceStatusChange function is not supported on this OS. You need to be running at least Windows Vista / Windows 2008 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  return m_pfnNotifyServiceStatusChange(m_hService, dwNotifyMask, pNotifyBuffer);
}

#pragma warning(suppress: 26440)
_Return_type_success_(return != 0) BOOL CNTScmService::Control(_In_ DWORD dwControl, _In_ DWORD dwInfoLevel, _Inout_ PVOID pControlParams)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_hService != nullptr);

  //Check to see if the function pointer is available
  if (m_pfnControlServiceEx == nullptr)
  {
    ATLTRACE(_T("CNTScmService::Control, ControlServiceEx function is not supported on this OS. You need to be running at least Windows Vista / Windows 2008 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  return m_pfnControlServiceEx(m_hService, dwControl, dwInfoLevel, pControlParams);
}

#pragma warning(suppress: 26440)
_Return_type_success_(return != 0) DWORD CNTScmService::SubscribeChangeNotifications(_In_ SC_EVENT_TYPE eEventType, _In_ PSC_NOTIFICATION_CALLBACK pCallback, _In_opt_ PVOID pCallbackContext, _Out_ PSC_NOTIFICATION_REGISTRATION* pSubscription) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_hService != nullptr);

  //Check to see if the function pointers are available
  if ((m_pfnSubscribeServiceChangeNotifications == nullptr) || (m_pfnUnsubscribeServiceChangeNotifications == nullptr))
  {
    ATLTRACE(_T("CNTScmService::SubscribeChangeNotifications, SubscribeServiceChangeNotifications/UnsubscribeServiceChangeNotifications functions are not supported on this OS. You need to be running at least Windows 8 / Windows 2012 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  const DWORD dwSuccess = m_pfnSubscribeServiceChangeNotifications(m_hService, eEventType, pCallback, pCallbackContext, pSubscription);
  if (dwSuccess != 0)
#pragma warning(suppress: 26477)
    ATLASSUME(pSubscription != nullptr);
  return dwSuccess;
}

#pragma warning(suppress: 26440)
BOOL CNTScmService::UnsubscribeChangeNotifications(_In_ PSC_NOTIFICATION_REGISTRATION pSubscription) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_hService != nullptr);

  //Check to see if the function pointer is available
  if (m_pfnUnsubscribeServiceChangeNotifications == nullptr)
  {
    ATLTRACE(_T("CNTScmService::UnsubscribeChangeNotifications, UnsubscribeServiceChangeNotifications function is not supported on this OS. You need to be running at least Windows 8 / Windows 2012 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  m_pfnUnsubscribeServiceChangeNotifications(pSubscription);
  return TRUE;
}

#pragma warning(suppress: 26440)
DWORD CNTScmService::WaitState(_In_ DWORD dwNotify, _In_ DWORD dwTimeout, _In_opt_ HANDLE hCancelEvent) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_hService != nullptr);

  //Check to see if the function pointer is available
  if (m_pfnWaitServiceState == nullptr)
  {
    ATLTRACE(_T("CNTScmService::WaitState, WaitServiceState function is not supported on this OS. You need to be running at least Windows 8 / Windows 2012 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  return m_pfnWaitServiceState(m_hService, dwNotify, dwTimeout, hCancelEvent);
}
