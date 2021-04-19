/*
Module : ntserv.h
Purpose: Defines the interface for a C++ class framework which encapsulate the area of Windows services
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

#ifndef __NTSERV_H__
#define __NTSERV_H__


////////////////////////////// Includes ///////////////////////////////////////

#include "ntservDefines.h"
#include "ntservCmdLineInfo.h"
#include "ntservEventLogSource.h"
#include "ntservServiceControlManager.h"


////////////////////////////// Classes ////////////////////////////////////////

//Class which carries the exit code information when you call CNTService::TerminateService
class CNTSERVICE_EXT_CLASS CNTServiceTerminateException
{
public:
//Constructors / Destructors
  CNTServiceTerminateException(_In_ DWORD dwWin32ExitCode, _In_ DWORD dwServiceSpecificExitCode) noexcept;

//Member variables
  DWORD m_dwWin32ExitCode;
  DWORD m_dwServiceSpecificExitCode;
};

//The class framework encapsulation of an NT service. You are meant to derive your own class
//from this and override its functions to implement your own service specific functionality.
class CNTSERVICE_EXT_CLASS CNTService
{
public:
//Defines
  #define STATE_NO_CHANGE 0xFFFFFFFF

//Enums
  enum class ServiceAction
  {
    StartService,
    PauseService,
    ContinueService,
    StopService
  };

  enum class UIErrorLoggingDetail
  {
    ErrorCode,
    ErrorCodeAndErrorDescription,
    ErrorDescription,
    StringAndErrorCode,
    StringAndErrorCodeAndErrorDescription,
    StringAndErrorDescription
  };

  enum class ELErrorLoggingDetail
  {
    ErrorCode,
    ErrorCodeAndErrorDescription,
    ErrorDescription
  };

//Constructors / Destructors
  CNTService() noexcept;
  CNTService(_In_z_ LPCTSTR pszServiceName, _In_z_ LPCTSTR pszDisplayName, _In_ DWORD dwControlsAccepted, _In_opt_z_ LPCTSTR pszDescription = nullptr);
  CNTService(_In_ const CNTService&) = delete;
  CNTService(_In_ CNTService&&) = delete;
  virtual ~CNTService() = default;

//Accessors / Mutators
  _NODISCARD CNTServiceString GetServiceName() const { return m_sServiceName; };
  _NODISCARD CNTServiceString GetInstanceServiceName() const { return m_sServiceName; };
  _NODISCARD CNTServiceString GetMasterServiceName() const { return m_sMasterServiceName; }
  void SetServiceName(_In_opt_z_ LPCTSTR sServiceName);
  void SetInstanceServiceName(_In_opt_z_ LPCTSTR sServiceName);
  void SetMasterServiceName(_In_opt_z_ LPCTSTR sServiceName);
  _NODISCARD CNTServiceString GetDisplayName() const { return m_sDisplayName; };
  _NODISCARD CNTServiceString GetInstanceDisplayName() const { return m_sDisplayName; };
  _NODISCARD CNTServiceString GetMasterDisplayName() const { return m_sMasterDisplayName; };
  void SetDisplayName(_In_opt_z_ LPCTSTR pszDisplayName);
  void SetInstanceDisplayName(_In_opt_z_ LPCTSTR pszDisplayName);
  void SetMasterDisplayName(_In_opt_z_ LPCTSTR pszDisplayName);
  _NODISCARD CNTServiceString GetDescription() const { return m_sDescription; };
  _NODISCARD CNTServiceString GetInstanceDescription() const { return m_sDescription; };
  _NODISCARD CNTServiceString GetMasterDescription() const { return m_sMasterDescription; };
  void SetDescription(_In_opt_z_ LPCTSTR pszDescription);
  void SetInstanceDescription(_In_opt_z_ LPCTSTR pszDescription);
  void SetMasterDescription(_In_opt_z_ LPCTSTR pszDescription);
  void SetAllowCommandLine(_In_ bool bCommandLine) noexcept { m_bAllowCommandLine = bCommandLine; };
  _NODISCARD bool GetAllowCommandLine() const noexcept { return m_bAllowCommandLine; };
  void SetAllowNameChange(_In_ bool bNameChange) noexcept { m_bAllowNameChange = bNameChange; };
  _NODISCARD bool GetAllowNameChange() const noexcept { return m_bAllowNameChange; };
  void SetAllowDescriptionChange(_In_ bool bDescriptionChange) noexcept { m_bAllowDescriptionChange = bDescriptionChange; };
  _NODISCARD bool GetAllowDescriptionChange() const noexcept { return m_bAllowDescriptionChange; };
  void SetAllowDisplayNameChange(_In_ bool bDisplayNameChange) noexcept { m_bAllowDisplayNameChange = bDisplayNameChange; };
  _NODISCARD bool GetAllowDisplayNameChange() const noexcept { return m_bAllowDisplayNameChange; };
  void SetUseConsole(_In_ bool bUseConsole) noexcept { m_bUseConsole = bUseConsole; };
  _NODISCARD bool GetUseConsole() const noexcept { return m_bUseConsole; };
  void SetControlsAccepted(_In_ DWORD dwControlsAccepted) noexcept { m_dwControlsAccepted = dwControlsAccepted; };
  _NODISCARD DWORD GetControlsAccepted() const noexcept { return m_dwControlsAccepted; };

//Persistance support
  //Allows saving and restoring of a services settings to the 
  //"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters" location in the registry
  virtual _Return_type_success_(return!=0) bool WriteProfileString(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszValue) noexcept;
  virtual _Return_type_success_(return!=0) bool WriteProfileInt(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nValue) noexcept;
  virtual _Return_type_success_(return!=0) bool WriteProfileStringArray(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array);
  virtual _Return_type_success_(return!=0) bool WriteProfileBinary(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_reads_bytes_opt_(nBytes) const BYTE* pData, _In_ UINT nBytes) noexcept;
  virtual CNTServiceString GetProfileString(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszDefault = nullptr, _Inout_opt_ DWORD* pLastError = nullptr);
  virtual UINT GetProfileInt(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nDefault, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  virtual _Return_type_success_(return!=0) bool GetProfileStringArray(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array);
  virtual _Return_type_success_(return!=0) bool GetProfileBinary(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ std::vector<BYTE>& data);

  //These versions of the persistence functions allow control over the "ServiceName" to write to as described above as well as control over the bFlush value 
  static _Return_type_success_(return!=0) bool WriteServiceProfileString(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszValue, _In_ bool bFlush, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  static _Return_type_success_(return!=0) bool WriteServiceProfileInt(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nValue, _In_ bool bFlush, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  static _Return_type_success_(return!=0) bool WriteServiceProfileStringArray(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array, _In_ bool bFlush, _Inout_opt_ DWORD* pLastError = nullptr);
  static _Return_type_success_(return!=0) bool WriteServiceProfileBinary(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_reads_bytes_opt_(nBytes) const BYTE* pData, _In_ ULONG nBytes, _In_ bool bFlush, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  static CNTServiceString GetServiceProfileString(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszDefault = nullptr, _Inout_opt_ DWORD* pLastError = nullptr);
  static UINT GetServiceProfileInt(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nDefault, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  static _Return_type_success_(return!=0) bool GetServiceProfileStringArray(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError = nullptr);
  static _Return_type_success_(return!=0) bool GetServiceProfileBinary(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ std::vector<BYTE>& data, _Inout_opt_ DWORD* pLastError = nullptr);

  //Allows client apps to decide if the above settings should be flushed to the registry or allow them to be written to the registry in a lazy fashion
  void SetProfileWriteFlush(_In_ bool bFlush) noexcept { m_bProfileWriteFlush = bFlush; };
  _NODISCARD bool _Return_type_success_(return != 0) GetProfileWriteFlush() const noexcept { return m_bProfileWriteFlush; };

//Other Methods
  CNTService& operator=(_In_ const CNTService&) = delete;
  CNTService& operator=(_In_ CNTService&&) = delete;

  //Helpful functions to parse the command line and execute the results
  virtual void ParseCommandLine(_In_ CNTServiceCommandLineInfo& rCmdInfo);
  virtual DWORD ProcessShellCommand(_In_ CNTServiceCommandLineInfo& rCmdInfo);

  //Reports the status of this service back to the SCM
  virtual _Return_type_success_(return != 0) BOOL ReportStatus();
  virtual _Return_type_success_(return != 0) BOOL ReportStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwCheckPoint = STATE_NO_CHANGE,
                                                               _In_ DWORD dwWaitHint = STATE_NO_CHANGE, _In_ DWORD dwControlsAccepted = STATE_NO_CHANGE);

  //Installs the callback funtion by calling RegisterServiceCtrlHandler
  virtual _Return_type_success_(return != 0) BOOL RegisterCtrlHandler();

  //Member function which does the job of responding to SCM requests
  virtual DWORD WINAPI ServiceCtrlHandler(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_opt_ LPVOID pEventData);

  //The ServiceMain function for this service
  virtual void WINAPI ServiceMain(_In_ DWORD dwArgc, _In_opt_ LPTSTR* pszArgv);

  //Called in reponse to a shutdown request
  virtual void OnStop();

  //Called in reponse to a pause request
  virtual void OnPause();

  //Called in reponse to a continue request
  virtual void OnContinue();

  //Called in reponse to a Interrogate request
  virtual void OnInterrogate();

  //Called in reponse to a Shutdown request
  virtual void OnShutdown();

  //Called in response to a "param change"
  virtual void OnParamChange();

  //Called in response to a "hardware profile change"
  virtual DWORD OnHardwareProfileChange(_In_ DWORD dwEventType, _In_opt_ LPVOID pEventData);

  //Called in response to a "power event"
  virtual DWORD OnPowerEvent(_In_ DWORD dwEventType, _In_opt_ LPVOID pEventData);

  //Called in response to a something being added to the services network binding
  virtual void OnNetBindAdd();

  //Called in response to a something being removed from the services network binding
  virtual void OnNetBindRemove();

  //Called in response to a something being enabled in a services network binding
  virtual void OnNetBindEnable();

  //Called in response to a something being disabled in a services network binding
  virtual void OnNetBindDisable();

  //Called in response to a SERVICE_CONTROL_DEVICEEVENT control request
  virtual DWORD OnDeviceEvent(_In_ DWORD dwEventType, _In_opt_ LPVOID pEventData);

  //Called in reponse to a user defined request
  virtual void OnUserDefinedRequest(_In_ DWORD dwControl);

  //Called when a session change event occurs
  virtual void OnSessionChange(_In_ DWORD dwEventType, _In_opt_ WTSSESSION_NOTIFICATION* pEventData);

  //Called in response to a PreShutdown request
  virtual void OnPreShutdown();

  //Called when a time change event occurs
  virtual void OnTimeChange(_In_opt_ SERVICE_TIMECHANGE_INFO* pEventData);

  //Called in response to a trigger event
  virtual void OnTriggerEvent();

  //Called in response to "low resources" event
  virtual void OnLowResources();

  //Caled in response to "system low resources" event
  virtual void OnSystemLowResources();

  //Kicks off the Service. You would normally call this some where in your main/wmain or InitInstance. If you are
  //using the CNTServiceCommandLineInfo class, then internally it will call this function for you.
  virtual _Return_type_success_(return != 0) bool Run();

  //Puts a service to a certain status
  virtual _Return_type_success_(return != 0) bool SetServiceToStatus(_In_ CNTService::ServiceAction esaStatusWanted, _Inout_ DWORD& dwError, _In_ DWORD dwTimeout = 5000, _In_ DWORD dwPollingInterval = 250);

  //Installs the service
  virtual _Return_type_success_(return != 0) bool Install(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError);

  //Uninstalls the service
  virtual _Return_type_success_(return != 0) bool Uninstall(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError, _In_ DWORD dwTimeToWaitForStop = 5000);

  _Must_inspect_result_ _Return_type_success_(return == 0) DWORD GetRegistryStateKey(_In_ SERVICE_REGISTRY_STATE_TYPE StateType, _In_ DWORD AccessMask, _Out_ HKEY* ServiceStateKey);
  _Must_inspect_result_ _Return_type_success_(return == 0) DWORD GetServiceDirectory(_In_ SERVICE_DIRECTORY_TYPE eDirectoryType, _Out_writes_opt_(cchPathBufferLength) PWCHAR lpPathBuffer, _In_ DWORD cchPathBufferLength, _Out_ DWORD *lpcchRequiredBufferLength);

  //Runs the service as a normal function as opposed to a service
  virtual void Debug();

  //Displays help for this service
  virtual void ShowHelp();

  //Called to display a message for the service to the end-user
  virtual void DisplayMessage(_In_ const CNTServiceString& sMessage);

  //Called to display a message to the debugger output window
  virtual void TraceMessage(_In_opt_z_ LPCTSTR pszMessage);
  virtual void TraceMessage(_In_ const CNTServiceString& sMessage);

  //Equivalent to the AfxFormatStringX functions
  virtual void FormatString(_In_ CNTServiceString& sMsg, _In_ UINT nIDS, _In_z_ LPCTSTR psz1);
  virtual void FormatString(_In_ CNTServiceString& sMsg, _In_ UINT nIDS, _In_z_ LPCTSTR psz1, _In_z_ LPCTSTR psz2);

  //Enumerate all instances of this service installed on the local machine
  virtual _Return_type_success_(return != 0) bool EnumerateInstances(_Out_ CNTServiceStringArray& ServiceNames, _Out_ DWORD& dwError);

  //QueryServiceDynamicInformation API wrapper
  virtual _Return_type_success_(return != 0) BOOL QueryServiceDynamicInformation(_In_ DWORD dwInfoLevel, _Outptr_ PVOID* ppDynamicInfo);

protected:
//Methods
  //These three static functions are used internally to go from the SDK functions to the C++ member functions
  static void WINAPI _ServiceCtrlHandler(_In_ DWORD dwControl);	
  static DWORD WINAPI _ServiceCtrlHandlerEx(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_opt_ LPVOID pEventData, _In_opt_ LPVOID pContext);
  static void WINAPI _SDKServiceMain(_In_ DWORD dwArgc, _In_opt_ LPTSTR* pszArgv);

  //Actual C++ implementation of ServiceMain
  virtual void _ServiceMain(_In_ DWORD dwArgv, _In_opt_ LPTSTR* pszArgv);

  //Helper functions to convert a Win32 Error code to a string and handle resource id and message id mapping
  virtual CNTServiceString Win32ErrorToString(_In_ UINT nStringID, _In_ DWORD dwError, _In_ bool bEventLog);
  virtual UINT MapResourceID(_In_ UINT nID);
  virtual UINT MapMessageID(_In_ UINT nID);
  virtual CNTServiceString GetErrorMessage(_In_ DWORD dwError);

  //Used internally by the persistance functions
  static _Return_type_success_(return != 0) bool GetSectionKey(_In_ ATL::CRegKey& sectionKey, _In_opt_z_ LPCTSTR pszServiceName, _In_opt_z_ LPCTSTR pszSection, _In_ bool bReadOnly = false, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;
  static _Return_type_success_(return != 0) bool GetServiceParametersKey(_In_ ATL::CRegKey& serviceKey, _In_opt_z_ LPCTSTR pszServiceName, _In_ bool bReadOnly = false, _Inout_opt_ DWORD* pLastError = nullptr) noexcept;

  //Actual implementation of the function to report status to the SCM
  virtual _Return_type_success_(return != 0) bool _ReportStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwCheckPoint = STATE_NO_CHANGE,
                                                                _In_ DWORD dwWaitHint = STATE_NO_CHANGE, _In_ DWORD dwControlsAccepted = STATE_NO_CHANGE,
                                                                _In_ DWORD dwWin32ExitCode = NO_ERROR, _In_ DWORD dwServiceSpecificExitCode = 0);

  //Used during the Install procedure
  virtual _Return_type_success_(return != 0) bool GetDependencies(_Out_ std::vector<TCHAR>& mszDependencies);

  //Used by EnumerateInstances
  static _Return_type_success_(return != 0) bool CALLBACK _EnumerateInstances(_In_opt_ void* pData, _In_ const ENUM_SERVICE_STATUS& ss);

  //Terminate the service
  void TerminateService(_In_ DWORD dwWin32ExitCode = NO_ERROR, _In_ DWORD dwServiceSpecificExitCode = 0);

  //Console Control handler
  static _Return_type_success_(return != 0) BOOL WINAPI _ConsoleCtrlHandler(_In_ DWORD dwCtrlType);
  virtual _Return_type_success_(return != 0) BOOL ConsoleCtrlHandler(_In_ DWORD dwCtrlType);

  //Simple helper function which securely nukes the contents of a CNTServiceString
  FORCEINLINE static void SecureEmptyString(_Inout_ CNTServiceString& sVal);

  //Used during Install method
  virtual _Return_type_success_(return != 0) bool InstallEventLogSource(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError);
  virtual _Return_type_success_(return != 0) bool InstallServiceConfiguration(_In_ CNTScmService& service, _Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError);
  virtual _Return_type_success_(return != 0) bool EnableServiceLogonRight(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError);

//typedefs of the function pointers
  using LPQUERYSERVICEDYNAMICINFORMATION = BOOL (WINAPI *)(SERVICE_STATUS_HANDLE, DWORD, PVOID);
  using LPGETSERVICEREGISTRYSTATEKEY = DWORD (WINAPI *)(SERVICE_STATUS_HANDLE, SERVICE_REGISTRY_STATE_TYPE, DWORD, HKEY*);
  using LPGETSERVICEDIRECTORY = DWORD (WINAPI *)(SERVICE_STATUS_HANDLE, SERVICE_DIRECTORY_TYPE, PWCHAR, DWORD, DWORD*);

//Member variables
  DWORD m_dwServiceType;
  DWORD m_dwStartType;
  DWORD m_dwErrorControl;
  CNTServiceString m_sLoadOrderGroup;
  CNTServiceString m_sUserName;
  CNTServiceString m_sPassword;
  DWORD m_dwEventCategoryCount;
  CNTServiceString m_sEventCategoryMessageFile;
  CNTServiceString m_sEventMessageFile;
  CNTServiceString m_sEventParameterMessageFile;
  DWORD m_dwEventTypesSupported;
  UIErrorLoggingDetail m_UILoggingDetail;
  ELErrorLoggingDetail m_ELLoggingDetail;
  CNTServiceStringArray m_sDependencies;
  bool m_bEventLogSource; //Is this service also an event log source
  SERVICE_STATUS_HANDLE m_hStatus;
  DWORD m_dwControlsAccepted; //What Control requests will this service repond to
  DWORD m_dwCheckPoint;
  DWORD m_dwWaitHint;
  DWORD m_dwCurrentState; //Current Status of the service
  DWORD m_dwRequestedControl; //The most recent value sent into ServiceCtrlHandler
  CNTServiceString m_sServiceName; //Name of the service
  CNTServiceString m_sMasterServiceName; //The name of the service (unaffected by calls to change it via /SN)
  CNTServiceString m_sDisplayName; //Display name for the service
  CNTServiceString m_sMasterDisplayName; //The display name of the service (unaffected by calls to change it via /SDN)
  CNTServiceString m_sDescription; //The description text for the service
  CNTServiceString m_sMasterDescription; //The description text for the service (unaffected by calls to change it via /SD)
  bool m_bDelayedAutoStart; //true enables a delayed Auto start service
  DWORD m_dwServiceSidType; //The service sid type
  CNTServiceStringArray m_sPrivileges; //The required privileges for this service
  DWORD m_dwPreshutdownTimeout; //The pre shutdown timeout for this service
  DWORD m_dwLaunchProtected; //The launch protected setting for this service
  CNTEventLogSource m_EventLogSource; //For reporting to the event log
  static CNTService* sm_pService; //Static which contains the this pointer
  ATL::CComAutoCriticalSection m_CritSect; //Protects changes to any member variables from multiple threads
  bool m_bProfileWriteFlush; //Should calls to WriteProfile... be synchronous or left to be written by the lazy writer
  bool m_bAllowCommandLine; //Should /SCL command line settings be observed
  bool m_bAllowNameChange; //Should /SN command line settings be observed 
  bool m_bAllowDescriptionChange; //Should /SD command line settings be observed
  bool m_bAllowDisplayNameChange; //Should /SDN command line settings be observed
  bool m_bUseConsole; //true if console output should be used, false for GUI
  CNTServiceString m_sBinaryPathName; //The binary path we use for installing
  LPQUERYSERVICEDYNAMICINFORMATION m_pfnQueryServiceDynamicInformation; //QueryServiceDynamicInformation function pointer
  LPGETSERVICEREGISTRYSTATEKEY m_pfnGetServiceRegistryStateKey; //GetServiceRegistryStateKey function pointer
  LPGETSERVICEDIRECTORY m_pfnGetServiceDirectory; //GetServiceDirectory function pointer
};

#endif //#ifndef __NTSERV_H__
