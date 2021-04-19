/*
Module : ntservScmService.h
Purpose: Defines the interface for the class CNTScmService.
Created: PJN / 14-07-1998

Copyright (c) 1997 - 2020 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code.

*/


/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __NTSERVSCMSERVICE_H__
#define __NTSERVSCMSERVICE_H__
#define _NODISCARD
//#define _Return_type_success_(x)


////////////////////////////// Includes ///////////////////////////////////////

#include "ntservDefines.h"


////////////////////////////// Classes ////////////////////////////////////////

//Forward declaration
class CNTServiceControlManager;

//Class used by CNTScmService::QueryConfig
class CNTSERVICE_EXT_CLASS CNTServiceConfig
{
public:
//Methods
  operator LPQUERY_SERVICE_CONFIG() noexcept
  {
#pragma warning(suppress: 26490)
    return reinterpret_cast<LPQUERY_SERVICE_CONFIG>(m_Data.data());
  }

protected:
//Member variables
  std::vector<BYTE> m_Data;

  friend class CNTScmService;
};

//Class used by CNTScmService::QuerySecurityDescriptor
class CNTSERVICE_EXT_CLASS CNTServiceSecurityDescriptor
{
public:
//Methods
  operator PSECURITY_DESCRIPTOR() noexcept
  {
    return m_Data.data();
  }

protected:
  //Member variables
  std::vector<BYTE> m_Data;

  friend class CNTScmService;
};

//Class used by CNTScmService::QueryFailureActions
class CNTSERVICE_EXT_CLASS CNTServiceFailureActions
{
public:
//Methods
  operator LPSERVICE_FAILURE_ACTIONS() noexcept
  {
#pragma warning(suppress: 26490)
    return reinterpret_cast<LPSERVICE_FAILURE_ACTIONS>(m_Data.data());
  }

protected:
//Member variables
  std::vector<BYTE> m_Data;

  friend class CNTScmService;
};

//Class used by CNTScmService::QueryTrigger
class CNTSERVICE_EXT_CLASS CNTServiceTriggerInfo
{
public:
//Methods
  operator PSERVICE_TRIGGER_INFO() noexcept
  {
#pragma warning(suppress: 26490)
    return reinterpret_cast<PSERVICE_TRIGGER_INFO>(m_Data.data());
  }

protected:
//Member variables
  std::vector<BYTE> m_Data;

  friend class CNTScmService;
};


//An encapsulation of a service as returned from querying the SCM (i.e. an SC_HANDLE)
class CNTSERVICE_EXT_CLASS CNTScmService
{
public:
//typedefs
  using ENUM_SERVICES_PROC = bool (CALLBACK *)(_In_opt_ void* pUserData, _In_ const ENUM_SERVICE_STATUS& ss);

//Constructors / Destructors
  CNTScmService() noexcept;
  CNTScmService(_In_ const CNTScmService&) = delete;
  CNTScmService(const CNTScmService&&) = delete;
  ~CNTScmService();

//Methods
  CNTScmService& operator=(_In_ const CNTScmService&) = delete;
  CNTScmService& operator=(_In_ CNTScmService&&) = delete;
  //Releases the underlying SC_HANDLE
  void Close() noexcept;

  //Allows access to the underlying SC_HANDLE representing the service
  operator SC_HANDLE() const noexcept;

  //Attach / Detach support from an SDK SC_HANDLE
  void Attach(_In_opt_ SC_HANDLE hService) noexcept;
  SC_HANDLE Detach() noexcept;

  //Changes the configuration of this service
  _Return_type_success_(return != 0) BOOL ChangeConfig(_In_ DWORD dwServiceType, _In_ DWORD dwStartType, _In_ DWORD dwErrorControl,
                                                       _In_opt_ LPCTSTR pBinaryPathName, _In_opt_ LPCTSTR pLoadOrderGroup, _Out_opt_ LPDWORD pdwTagId,
                                                       _In_opt_ LPCTSTR pDependencies, _In_opt_ LPCTSTR pServiceStartName, _In_opt_ LPCTSTR pPassword,
                                                       _In_opt_ LPCTSTR pDisplayName) const noexcept;

  //Send a command to the service
  _Return_type_success_(return != 0) BOOL Control(_In_ DWORD dwControl) noexcept;
  _Return_type_success_(return != 0) BOOL Control(_In_ DWORD dwControl, _In_ DWORD dwInfoLevel, _Inout_ PVOID pControlParams);

  //These functions call Control() with the standard predefined control codes
  _NODISCARD _Return_type_success_(return != 0) BOOL Stop() const noexcept; //Ask the service to stop
  _NODISCARD _Return_type_success_(return != 0) BOOL Pause() const noexcept; //Ask the service to pause
  _NODISCARD _Return_type_success_(return != 0) BOOL Continue() const noexcept; //Ask the service to continue
  _NODISCARD _Return_type_success_(return != 0) BOOL Interrogate() const noexcept; //Ask the service to update its status to the SCM

  //Waits for a service to stop with a configurable timeout
  _Return_type_success_(return != 0) bool WaitForStop(_In_ DWORD dwTimeout) noexcept;

  //Waits for a service to have a certain status (with a configurable timeout)
  _Return_type_success_(return != 0) bool WaitForServiceStatus(_In_ DWORD dwTimeout, _In_ DWORD dwWaitForStatus, _In_ DWORD dwPollingInterval = 250) noexcept;

  //Start the execution of the service
  _Return_type_success_(return != 0) BOOL Start(_In_ DWORD dwNumServiceArgs, _In_reads_opt_(dwNumServiceArgs) LPCTSTR* pServiceArgVectors) const noexcept;

  //Get the most return status of the service reported to the SCM by this service
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryStatus(_Out_ SERVICE_STATUS& ServiceStatus) const noexcept;

  //Get the configuration parameters of this service from the SCM
  _Return_type_success_(return != 0) BOOL QueryConfig(_Inout_ CNTServiceConfig& config) const;

  //Add a new service to the SCM database
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL Create(_In_ const CNTServiceControlManager& Manager, _In_ LPCTSTR pServiceName, _In_opt_ LPCTSTR pDisplayName,
                                                                       _In_ DWORD dwDesiredAccess, _In_ DWORD dwServiceType, _In_ DWORD dwStartType, _In_ DWORD dwErrorControl,
                                                                       _In_opt_ LPCTSTR pBinaryPathName, _In_opt_ LPCTSTR pLoadOrderGroup, _Out_opt_ LPDWORD pdwTagId, _In_opt_ LPCTSTR pDependencies,
                                                                       _In_opt_ LPCTSTR pServiceStartName, _In_opt_ LPCTSTR pPassword) noexcept;

  //Mark this service as to be deleted from the SCM.
  _NODISCARD _Return_type_success_(return != 0) BOOL Delete() const noexcept;

  //Enumerate the services that this service depends upon
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL EnumDependents(_In_ DWORD dwServiceState, _In_opt_ void* pUserData, _In_ ENUM_SERVICES_PROC pEnumServicesFunc) const;

  //Get the security information associated with this service
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryObjectSecurity(_In_ SECURITY_INFORMATION dwSecurityInformation, _Inout_ CNTServiceSecurityDescriptor& securityDescriptor) const;

  //Set the security descriptor associated with this service
  _Return_type_success_(return != 0) BOOL SetObjectSecurity(_In_ SECURITY_INFORMATION dwSecurityInformation, _In_ PSECURITY_DESCRIPTOR pSecurityDescriptor) const noexcept;

  //Windows 2000+ specific functions
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryDescription(_Out_ CNTServiceString& sDescription) const;
  _Return_type_success_(return != 0) BOOL ChangeDescription(_In_ const CNTServiceString& sDescription);
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryFailureActions(_Inout_ CNTServiceFailureActions& actions) const;
  _Return_type_success_(return != 0) BOOL ChangeFailureActions(_In_opt_ LPSERVICE_FAILURE_ACTIONS pActions) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryStatus(_Out_ SERVICE_STATUS_PROCESS& ssp) const noexcept;

  //Windows Vista / 2008+ specific functions
  DWORD NotifyStatusChange(_In_ DWORD dwNotifyMask, _In_ PSERVICE_NOTIFY pNotifyBuffer) const;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryDelayAutoStart(_Out_ BOOL& bDelayedAutoStart) const noexcept;
  _Return_type_success_(return != 0) BOOL ChangeDelayAutoStart(_In_ BOOL bDelayedAutoStart) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryFailureActionsFlag(_Out_ BOOL& bFailureActionsOnNonCrashFailures) const noexcept;
  _Return_type_success_(return != 0) BOOL ChangeFailureActionsFlag(_In_ BOOL bFailureActionsOnNonCrashFailures) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QuerySidInfo(_Out_ DWORD& dwServiceSidType) const noexcept;
  _Return_type_success_(return != 0) BOOL ChangeSidInfo(_In_ DWORD dwServiceSidType) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryRequiredPrivileges(_Out_ CNTServiceStringArray& privileges) const;
  _Return_type_success_(return != 0) BOOL ChangeRequiredPrivileges(_In_ const CNTServiceStringArray& priviledges);
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryPreShutdown(_Out_ DWORD& dwPreshutdownTimeout) const noexcept;
  _Return_type_success_(return != 0) BOOL ChangePreShutdown(_In_ DWORD dwPreshutdownTimeout) noexcept;

  //Windows 7 / 2008 R2+ specific functions
  _Return_type_success_(return != 0) BOOL ChangeTrigger(_In_ PSERVICE_TRIGGER_INFO pTriggerInfo) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryTrigger(_Inout_ CNTServiceTriggerInfo& triggerInfo) const;
  _Return_type_success_(return != 0) BOOL ChangePreferredNode(_In_ USHORT usPreferredNode, _In_ BOOL bDelete) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryPreferredNode(_Out_ USHORT& usPreferredNode, _Out_ BOOL& bDelete) const noexcept;

  //Windows 8 / 2012+ specific functions
  _Return_type_success_(return != 0) DWORD SubscribeChangeNotifications(_In_ SC_EVENT_TYPE eEventType, _In_ PSC_NOTIFICATION_CALLBACK pCallback, _In_opt_ PVOID pCallbackContext, _Out_ PSC_NOTIFICATION_REGISTRATION* pSubscription) const;
  BOOL UnsubscribeChangeNotifications(_In_ PSC_NOTIFICATION_REGISTRATION pSubscription) const;
  DWORD WaitState(_In_ DWORD dwNotify, _In_ DWORD dwTimeout, _In_opt_ HANDLE hCancelEvent) const;

  //Windows 8.1+ specific functions
  _Return_type_success_(return != 0) BOOL ChangeLaunchProtected(_In_ DWORD dwLaunchProtected) noexcept;
  _Must_inspect_result_ _Return_type_success_(return != 0) BOOL QueryLaunchProtected(_Out_ DWORD& dwLaunchProtected) const noexcept;

  //Windows 10 codename '20H2' functions
  _Must_inspect_result_ _Return_type_success_(return == 0) DWORD GetSharedRegistryStateKey(_In_ SERVICE_SHARED_REGISTRY_STATE_TYPE StateType, _In_ DWORD AccessMask, _Out_ HKEY* ServiceStateKey) const;
  _Must_inspect_result_ _Return_type_success_(return == 0) DWORD GetSharedDirectory(_In_ SERVICE_SHARED_DIRECTORY_TYPE DirectoryType, _Out_writes_to_opt_(PathBufferLength, *RequiredBufferLength) PWCHAR PathBuffer, _In_ DWORD PathBufferLength, _Out_ DWORD * RequiredBufferLength) const;

protected:
//typedefs
  using LPNOTIFYSERVICESTATUSCHANGE = DWORD (WINAPI *)(_In_ SC_HANDLE, _In_ DWORD, _In_ PSERVICE_NOTIFY);
  using LPCONTROLSERVICEEX = BOOL (WINAPI *)(_In_ SC_HANDLE, _In_ DWORD, _In_ DWORD, _Inout_ PVOID);
  using LPSUBSCRIBESERVICECHANGENOTIFICIATIONS = DWORD (WINAPI *)(_In_ SC_HANDLE, _In_ SC_EVENT_TYPE, _In_ PSC_NOTIFICATION_CALLBACK, _In_opt_ PVOID, _Out_ PSC_NOTIFICATION_REGISTRATION*);
  using LPUNSUBSCRIBESERVICECHANGENOTIFICATIONS = VOID (WINAPI *)(_In_ PSC_NOTIFICATION_REGISTRATION);
  using LPWAITSERVICESTATE = DWORD (WINAPI *)(_In_  SC_HANDLE, _In_ DWORD, _In_opt_ DWORD, _In_opt_ HANDLE);
  using LPGETSHAREDSERVICEREGISTRYSTATEKEY = DWORD (WINAPI *)(_In_ SC_HANDLE, _In_ SERVICE_SHARED_REGISTRY_STATE_TYPE, _In_ DWORD, _Out_ HKEY*);
  using LPGETSHAREDSERVICEDIRECTORY = DWORD (WINAPI *)(_In_ SC_HANDLE, _In_ SERVICE_SHARED_DIRECTORY_TYPE, _Out_writes_to_opt_(PathBufferLength, *RequiredBufferLength) PWCHAR PathBuffer, _In_ DWORD PathBufferLength, _Out_ DWORD* RequiredBufferLength);

//Member variables
  SC_HANDLE m_hService;
  HINSTANCE m_hAdvapi32; //Instance handle of the "ADVAPI32.DLL" which houses most of the NT Service functions
  HINSTANCE m_hSecHost; //Instance handle of the "SECHOST.DLL"
  LPNOTIFYSERVICESTATUSCHANGE m_pfnNotifyServiceStatusChange; //NotifyServiceStatusChange function pointer
  LPCONTROLSERVICEEX m_pfnControlServiceEx; //ControlServiceEx function pointer
  LPSUBSCRIBESERVICECHANGENOTIFICIATIONS m_pfnSubscribeServiceChangeNotifications; //SubscribeServiceChangeNotifications function pointer
  LPUNSUBSCRIBESERVICECHANGENOTIFICATIONS m_pfnUnsubscribeServiceChangeNotifications; //UnsubscribeServiceChangeNotifications function pointer
  LPWAITSERVICESTATE m_pfnWaitServiceState; //WaitServiceState function pointer
  LPGETSHAREDSERVICEREGISTRYSTATEKEY m_pfnGetSharedServiceRegistryStateKey; //GetSharedServiceRegistryStateKey function pointer
  LPGETSHAREDSERVICEDIRECTORY m_pfnGetSharedServiceDirectory; //GetSharedServiceDirectory function pointer
};

#endif //#ifndef __NTSERVSCMSERVICE_H__
