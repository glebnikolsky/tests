///////////////////////////////// Includes ////////////////////////////////////

#include "stdafx.h"
#include "ntserv_msg.h"
#include "ntservEventLog.h"
#include "App.h"


///////////////////////////////// Implementation //////////////////////////////

#pragma warning(suppress: 26440)
bool CALLBACK EnumServices(_In_opt_ void* pData, _In_ const ENUM_SERVICE_STATUS& Service)
{
  UNREFERENCED_PARAMETER(pData);
  UNREFERENCED_PARAMETER(Service);

#ifdef _DEBUG
  ATLTRACE(_T("Service name is %s\n"), Service.lpServiceName);
  ATLTRACE(_T("Friendly name is %s\n"), Service.lpDisplayName);
#endif //#ifdef _DEBUG

  return true; //continue enumeration
}

#pragma warning(suppress: 26440)
bool CALLBACK EnumServices2(_In_opt_ void* pData, _In_ const ENUM_SERVICE_STATUS_PROCESS& ssp)
{
  UNREFERENCED_PARAMETER(pData);
  UNREFERENCED_PARAMETER(ssp);

#ifdef _DEBUG
  ATLTRACE(_T("Service name is %s\n"), ssp.lpServiceName);
  ATLTRACE(_T("Friendly name is %s\n"), ssp.lpDisplayName);
#endif

  return true; //continue enumeration
}

VOID CALLBACK NotificationCallback(_In_ DWORD /*dwNotify*/, _In_opt_ PVOID /*pCallbackContext*/) noexcept
{
}


#pragma warning(suppress: 26485)
int _tmain(int /*argc*/, TCHAR* /*argv*/[], TCHAR* /*envp*/[]) //NOLINT(modernize-avoid-c-arrays)
{
#ifdef _DEBUG
  DWORD dwError = 0;
  CMyService testService;
  UINT nValue = testService.GetProfileInt(_T("General"), _T("IntValue"), 33, &dwError); //NOLINT(clang-analyzer-deadcode.DeadStores)
  CNTServiceString sValue = testService.GetProfileString(_T("General"), _T("StringValue"), _T("DefaultValue"), &dwError);

  BOOL bSuccess = testService.WriteProfileInt(_T("General"), _T("IntValue"), 37); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess = testService.WriteProfileString(_T("General"), _T("StringValue"), _T("Test Value")); //NOLINT(clang-analyzer-deadcode.DeadStores)

  nValue = testService.GetProfileInt(_T("General"), _T("IntValue"), 33, &dwError); //NOLINT(clang-analyzer-deadcode.DeadStores)
  sValue = testService.GetProfileString(_T("General"), _T("StringValue"), nullptr, &dwError);

  CNTServiceStringArray array;
  array.emplace_back(_T("First String"));
  array.emplace_back(_T("Second and Last String"));
  bSuccess = testService.WriteProfileStringArray(_T("General"), _T("StringArrayValue"), array); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("StringArrayValue"), array); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("Does Not Exist"), array); //NOLINT(clang-analyzer-deadcode.DeadStores)

  std::vector<BYTE> bySetData{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
#pragma warning(suppress: 26472)
  bSuccess = testService.WriteProfileBinary(_T("General"), _T("BinaryValue"), bySetData.data(), static_cast<UINT>(bySetData.size())); //NOLINT(clang-analyzer-deadcode.DeadStores)

  array.clear();
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("StringArrayValue"), array); //NOLINT(clang-analyzer-deadcode.DeadStores)

  std::vector<BYTE> getData;
  bSuccess = testService.GetProfileBinary(_T("General"), _T("BinaryValue"), getData); //NOLINT(clang-analyzer-deadcode.DeadStores)

  testService.WriteProfileString(_T("General"), _T("BinaryValue"), nullptr);
  testService.WriteProfileString(_T("General"), nullptr, nullptr);
#endif //#ifdef _DEBUG

#ifdef _DEBUG
  {
    CMyService testService2;
    CNTServiceStringArray instances;
    DWORD dwError2 = 0;
    if (testService2.EnumerateInstances(instances, dwError2))
    {
      for (const auto& instance : instances)
        ATLTRACE(_T("Found instance, %s\n"), instance.c_str());
    }
  }
#endif //#ifdef _DEBUG

#ifdef _DEBUG
  CNTServiceControlManager manager;
  SC_HANDLE hScm = manager; //NOLINT(clang-analyzer-deadcode.DeadStores)
  BOOL bSuccess2 = manager.Open(nullptr, SC_MANAGER_CONNECT | SC_MANAGER_QUERY_LOCK_STATUS | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_LOCK); //We only need SC_MANAGER_CONNECT, SC_MANAGER_LOCK_STATUS and SC_MANAGER_LOCK access here
  if (bSuccess2)
  {
    hScm = manager; //NOLINT(clang-analyzer-deadcode.DeadStores)
    CNTServiceControlManagerLockStatus lockStatus;
    bSuccess2 = manager.QueryLockStatus(lockStatus); //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.Lock(); //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.Unlock(); //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.QueryLockStatus(lockStatus); //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.EnumServices(SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, EnumServices); //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.EnumServices(SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, nullptr, EnumServices2); //NOLINT(clang-analyzer-deadcode.DeadStores)

    CNTScmService service;
    SC_HANDLE hService = service; //NOLINT(clang-analyzer-deadcode.DeadStores)
    bSuccess2 = manager.OpenService(_T("PJSERVICE"), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG | SERVICE_ENUMERATE_DEPENDENTS, service); //Use only the required access rights we need
    if (bSuccess2)
    {
      hService = service; //NOLINT(clang-analyzer-deadcode.DeadStores)
      /*
      bSuccess2 = service.Start(0, nullptr);
      Sleep(4000);
      bSuccess2 = service.Control(128);
      bSuccess2 = service.Pause();
      Sleep(4000);
      bSuccess2 = service.Continue();
      */
      bSuccess2 = service.Stop(); //NOLINT(clang-analyzer-deadcode.DeadStores)
      CNTServiceConfig config;
      bSuccess2 = service.QueryConfig(config);
      if (bSuccess2)
      {
        ATLTRACE(_T("dwServiceType is %d\n"),      config.operator LPQUERY_SERVICE_CONFIG()->dwServiceType);
        ATLTRACE(_T("dwStartType is %d\n"),        config.operator LPQUERY_SERVICE_CONFIG()->dwStartType);
        ATLTRACE(_T("dwErrorControl is %d\n"),     config.operator LPQUERY_SERVICE_CONFIG()->dwErrorControl);
        ATLTRACE(_T("lpBinaryPathName is %s\n"),   config.operator LPQUERY_SERVICE_CONFIG()->lpBinaryPathName);
        ATLTRACE(_T("lpLoadOrderGroup is %s\n"),   config.operator LPQUERY_SERVICE_CONFIG()->lpLoadOrderGroup);
        ATLTRACE(_T("dwTagId is %d\n"),            config.operator LPQUERY_SERVICE_CONFIG()->dwTagId);
        ATLTRACE(_T("lpDependencies are %s\n"),    config.operator LPQUERY_SERVICE_CONFIG()->lpDependencies);
        ATLTRACE(_T("lpServiceStartName is %s\n"), config.operator LPQUERY_SERVICE_CONFIG()->lpServiceStartName);
        ATLTRACE(_T("lpDisplayName is %s\n"),      config.operator LPQUERY_SERVICE_CONFIG()->lpDisplayName);
      }
      bSuccess2 = service.EnumDependents(SERVICE_STATE_ALL, nullptr, EnumServices);
      UNREFERENCED_PARAMETER(bSuccess2);

      CNTServiceSecurityDescriptor securityDescriptor;
      bSuccess2 = service.QueryObjectSecurity(DACL_SECURITY_INFORMATION, securityDescriptor);
      UNREFERENCED_PARAMETER(bSuccess2);

      CNTServiceString sDescription;
      if (service.QueryDescription(sDescription))
        bSuccess2 = service.ChangeDescription(sDescription); //NOLINT(clang-analyzer-deadcode.DeadStores)

      DWORD dwPreShutdownTimeout = 0;
      if (service.QueryPreShutdown(dwPreShutdownTimeout))
        bSuccess2 = service.ChangePreShutdown(dwPreShutdownTimeout); //NOLINT(clang-analyzer-deadcode.DeadStores)

      CNTServiceFailureActions failureActions;
      bSuccess2 = service.QueryFailureActions(failureActions);
      UNREFERENCED_PARAMETER(bSuccess2);
      bSuccess2 = service.ChangeFailureActions(failureActions); //NOLINT(clang-analyzer-deadcode.DeadStores)

      bSuccess2 = service.ChangeDelayAutoStart(TRUE); //NOLINT(clang-analyzer-deadcode.DeadStores)
      BOOL bDelayedAutoStart = FALSE;
      bSuccess2 = service.QueryDelayAutoStart(bDelayedAutoStart);
      UNREFERENCED_PARAMETER(bSuccess2);

      bSuccess2 = service.ChangeFailureActionsFlag(TRUE); //NOLINT(clang-analyzer-deadcode.DeadStores)
      BOOL bFailureActionsOnNonCrashFailures = FALSE;
      bSuccess2 = service.QueryFailureActionsFlag(bFailureActionsOnNonCrashFailures);
      UNREFERENCED_PARAMETER(bSuccess2);

      bSuccess2 = service.ChangeSidInfo(3); //3 is SERVICE_SID_TYPE_RESTRICTED //NOLINT(clang-analyzer-deadcode.DeadStores)
      DWORD dwServiceSidType = 0;
      bSuccess2 = service.QuerySidInfo(dwServiceSidType);
      UNREFERENCED_PARAMETER(bSuccess2);

      CNTServiceStringArray sPrivileges;
      sPrivileges.emplace_back(_T("SeIncreaseQuotaPrivilege"));
      bSuccess2 = service.ChangeRequiredPrivileges(sPrivileges); //NOLINT(clang-analyzer-deadcode.DeadStores)
      bSuccess2 = service.QueryRequiredPrivileges(sPrivileges);
      UNREFERENCED_PARAMETER(bSuccess2);

      bSuccess2 = service.ChangePreferredNode(2, FALSE); //NOLINT(clang-analyzer-deadcode.DeadStores)
      USHORT usPreferredNode = 0;
      BOOL bDelete = FALSE;
      bSuccess2 = service.QueryPreferredNode(usPreferredNode, bDelete);
      UNREFERENCED_PARAMETER(bSuccess2);

      bSuccess2 = service.ChangeLaunchProtected(SERVICE_LAUNCH_PROTECTED_NONE); //NOLINT(clang-analyzer-deadcode.DeadStores)
      DWORD dwLaunchProtected = 0;
      bSuccess2 = service.QueryLaunchProtected(dwLaunchProtected);
      UNREFERENCED_PARAMETER(bSuccess2);

      CNTServiceTriggerInfo triggerInfo;
      bSuccess2 = service.QueryTrigger(triggerInfo);
      UNREFERENCED_PARAMETER(bSuccess2);
      bSuccess2 = service.ChangeTrigger(triggerInfo); //NOLINT(clang-analyzer-deadcode.DeadStores)

      SERVICE_CONTROL_STATUS_REASON_PARAMS scsrp{};
      bSuccess2 = service.Control(SERVICE_STOP, SERVICE_CONTROL_STATUS_REASON_INFO, &scsrp);
      UNREFERENCED_PARAMETER(bSuccess2);

      SERVICE_STATUS_PROCESS ssp;
#pragma warning(suppress: 28193)
      bSuccess2 = service.QueryStatus(ssp);
      UNREFERENCED_PARAMETER(bSuccess2);

      //PSC_NOTIFICATION_REGISTRATION pRegistration = nullptr;
      //DWORD dwValue = service.SubscribeChangeNotifications(SC_EVENT_STATUS_CHANGE, NotificationCallback, nullptr, &pRegistration);
      //bSuccess2 = service.UnsubscribeChangeNotifications(pRegistration);
      //dwValue = service.WaitState(SERVICE_STOPPED, 1000, nullptr);
    }
  }
#endif //#ifdef _DEBUG

#ifdef _DEBUG
  CNTEventLog el;
  HANDLE hEventLog = el; //NOLINT(clang-analyzer-deadcode.DeadStores)
  BOOL bSuccess3 = el.OpenSecurity(nullptr); //NOLINT(clang-analyzer-deadcode.DeadStores)
  hEventLog = el; //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.OpenSystem(nullptr); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.OpenApplication(nullptr); //NOLINT(clang-analyzer-deadcode.DeadStores)
  DWORD dwOldest;
  bSuccess3 = el.GetOldestRecord(dwOldest); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.Backup(_T("C:\\BACKUPLOG.TXT")); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.Close(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.OpenBackup(nullptr, _T("C:\\BACKUPLOG.TXT")); //NOLINT(clang-analyzer-deadcode.DeadStores)
  DWORD dwRecords = 0;
  bSuccess3 = el.OpenSystem(nullptr); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess3 = el.GetNumberOfRecords(dwRecords);
#pragma warning(suppress: 26477)
  ATLASSUME(bSuccess3);
  //bSuccess = el.Clear(nullptr);
  CEventLogRecord record;
  DWORD ii=0;
  while (ii<dwRecords)
  {
#pragma warning(suppress: 26486)
    bSuccess3 = el.ReadNext(record); //NOLINT(clang-analyzer-deadcode.DeadStores)
    ii++;
  }
#pragma warning(suppress: 26486)
  bSuccess3 = el.ReadPrev(record); //NOLINT(clang-analyzer-deadcode.DeadStores)
  DWORD dwFull = 0;
  bSuccess3 = el.GetFullInformation(dwFull); //NOLINT(clang-analyzer-deadcode.DeadStores)
#endif //#ifdef _DEBUG

#ifdef _DEBUG
  CNTEventLogSource els(nullptr, _T("PJ's Demo NT Service"));
  LPCTSTR pString = _T("PJ's Demo NT Service");
  LPCTSTR* ppString = &pString;
  BOOL bSuccess4 = els.Register(nullptr, pString); //NOLINT(clang-analyzer-deadcode.DeadStores)
  bSuccess4 = els.Deregister(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  const DWORD dwError2 = GetLastError();
  UNREFERENCED_PARAMETER(dwError2);
  bSuccess4 = els.Report(EVENTLOG_INFORMATION_TYPE, 0, CNTS_MSG_SERVICE_STARTED, nullptr, 1, 0, ppString, nullptr); //NOLINT(clang-analyzer-deadcode.DeadStores)
#endif //#ifdef _DEBUG

  //All that is required to get the service up and running
  CNTServiceCommandLineInfo cmdInfo;
  CMyService Service;
  Service.SetAllowCommandLine(true);
  Service.SetAllowNameChange(true);
  Service.SetAllowDescriptionChange(true);
  Service.SetAllowDisplayNameChange(true);
#pragma warning(suppress: 26486)
  Service.ParseCommandLine(cmdInfo);
  return Service.ProcessShellCommand(cmdInfo);
}


#pragma warning(suppress: 26455)
CMyService::CMyService() : CNTService(_T("PJSERVICE"), _T("PJ's Demo NT Service"), SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE, _T("Description text for PJ's Demo Service")),
                           m_bWantStop(false), //Simple boolean which is set to request the service to stop
                           m_bPaused(false), //The boolean used to represent that the service is paused
                           m_dwBeepInternal(1000) //The default beep interval of 1 second
{
  //For demonstration purposes, make this service dependent on Windows Audio. Please note that because of Session Isolation on Windows Vista and
  //later, when you start the "PJSERVICE" service, you will not hear any message beeps because the service is running in a different session
  m_sDependencies.emplace_back(_T("AudioSrv"));

  //Uncomment the following code (or put it in the member variable initialization above) to setup the service with different requirements
  /*
  m_dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  m_dwStartType = SERVICE_AUTO_START;
  m_dwErrorControl = SERVICE_ERROR_NORMAL;
  m_sUserName = _T(".\\auser");
  m_sPassword = _T("apassword");
  */
}

void CMyService::ServiceMain(DWORD /*dwArgc*/, LPTSTR* /*pszArgv*/)
{
  //Pretend that starting up takes some time
  ReportStatus(SERVICE_START_PENDING, 1, 1100);
  Sleep(1000);
  ReportStatus(SERVICE_RUNNING);

  //Report to the event log that the service has started successfully
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_STARTED, m_sDisplayName.c_str());

#ifdef _DEBUG
  ATL::CAtlStringW sDirectory;
  DWORD dwSize = _MAX_PATH;
  DWORD dwRet = GetServiceDirectory(ServiceDirectoryPersistentState, sDirectory.GetBuffer(_MAX_PATH), _MAX_PATH, &dwSize);
  UNREFERENCED_PARAMETER(dwRet);
  sDirectory.ReleaseBuffer();

  HKEY hKey = nullptr;
#pragma warning(suppress: 28193)
  dwRet = GetRegistryStateKey(ServiceRegistryStateParameters, 0, &hKey);
  UNREFERENCED_PARAMETER(dwRet);
  if (hKey)
    RegCloseKey(hKey);
#endif //#ifdef _DEBUG

  //The tight loop which constitutes the service
  bool bOldPause = m_bPaused;
  while (!m_bWantStop)
  {
    //As a demo, we just do a message beep
    if (!m_bPaused)
      MessageBeep(MB_OK);

    //Wait for the specified time
    Sleep(m_dwBeepInternal);

    //SCM has requested a Pause / Continue
    if (m_bPaused != bOldPause)
    {
      if (m_bPaused)
      {
        ReportStatus(SERVICE_PAUSED);

        //Report to the event log that the service has paused successfully
        m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_PAUSED, m_sDisplayName.c_str());
      }
      else
      {
        ReportStatus(SERVICE_RUNNING);

        //Report to the event log that the service has stopped continued
        m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_CONTINUED, m_sDisplayName.c_str());        
      }
    }

    bOldPause = m_bPaused;
  }

  //Pretend that closing down takes some time
  ReportStatus(SERVICE_STOP_PENDING, 1, 1100);
  Sleep(1000);

  //Report to the event log that the service has stopped successfully
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_STOPPED, m_sDisplayName.c_str());
}

void CMyService::OnStop() noexcept
{
  //Signal the other thread to end via the boolean variable
  m_bWantStop = true;
}

void CMyService::OnPause() noexcept
{
  //Signal the other thread via the boolean variable
  m_bPaused = true;
}

void CMyService::OnContinue() noexcept
{
  //Signal the other thread to continue via the boolean variable
  m_bPaused = false;
}

void CMyService::OnUserDefinedRequest(DWORD dwControl)
{
  //Any value greater than 200 increments the doubles the beep frequency
  //otherwise the frequency is halved
  if (dwControl > 200)
    m_dwBeepInternal /= 2;
  else
    m_dwBeepInternal *= 2;

  //Report to the event log that the beep interval has been changed
  ATL::CAtlString sInterval;
  sInterval.Format(_T("%u"), m_dwBeepInternal);
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MSG_SERVICE_SET_FREQUENCY, sInterval);
}

void CMyService::ShowHelp() noexcept
{
  _tprintf(_T("A demo service which just beeps the speaker\nUsage: testsrv [ -install | -uninstall | -remove | -start | -pause | -continue | -stop | -help | -? ]\n"));
}
