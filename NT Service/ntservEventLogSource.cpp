/*  
Module : ntservEventLogSource.cpp
Purpose: Implementation for the class CNTEventLogSource
Created: PJN / 14-07-1997
History: PJN / 23-07-2005 1. Provided an overriden version of Report which has 3 string parameters
                          2. GetStringArrayFromRegistry and SetStringArrayIntoRegistry methods
                          are now public as they are helpful to have available externally
         PJN / 11-01-2006 1. Addition of a default constructor for CNTEventLogSource 
                          2. Addition of CNTEventLogSource::SetServerName, CNTEventLogSource::GetServerName,
                          CNTEventLogSource::SetSourceName and CNTEventLogSource::GetSourceName methods
                          3. Addition of a DWORD* parameter to the CNTEventLogSource::GetStringArrayFromRegistry
                          and CNTEventLogSource::SetStringArrayFromRegistry functions. This allow the details
                          of the last error to be returned to client code.
         PJN / 03-06-2006 1. If CNTEventLogSource::Uninstall fails to remove the HKLM\System\CurrentControlSet\Services\EventLog\"Display Name"
                          registry key, then it aborts before doing any other work. Thanks to Matthias Miller
                          for reporting this issue.
         PJN / 13-06-2006 1. Addition of a logname parameter to the CNTEventLogSource constructor to support user defined event logs
                          instead of logging to the standard "Application" event log. Please note that the code added only enables the CNTService 
                          framework to use non "Application" event logs, but it does not contain code to create a custom event log. You will need 
                          to handle that in your application's install. For further information on doing this, please see 
                          http://msdn.microsoft.com/library/default.asp?url=/library/en-us/eventlog/base/eventlog_key.asp which contains details
                          on the various registry values which you need to create. Thanks to Jan Bartels for this addition.
                          2. Also optimized the construction of member variables in the CNTEventLogSource constructors
         PJN / 24-06-2006 1. Updated code to compile cleanly using VC 2005.
         PJN / 25-06-2006 1. Code now uses newer C++ style casts instead of C style casts.
         PJN / 01-08-2006 1. Fix for two buffer size calculations using the new _tcscpy_s functions, one in CNTEventLogSource::SetStringArrayIntoRegistry and the other in
                          CNTService::GetDependencies. Thanks to DmityShm on the codeproject forums for reporting this issue.
         PJN / 21-09-2006 1. Minor update to use %X instead of %x in various CNTEventLogSource::Report calls.
         PJN / 13-11-2007 1. Minor update to display HRESULT's correctly.
         PJN / 01-08-2010 1. Fixed a minor issue in CNTEventLogSource::Report with the use of printf format identifiers
                          2. Reworked all the internal registry management code to use ATL::CRegKey
                          3. CNTEventLogSource::Install method now supports CategoryCount, CategoryMessageFile and ParameterMessageFile settings for event sources
         PJN / 10-11-2012 1. Fixed a bug in CNTEventLogSource::GetStringArrayFromRegistry where it could not handle empty MULTI_SZ strings. Thanks to Rolf Schatten
                          for reporting this bug.
                          2. CNTEventLogSource::Install now calls CRegKey::Create instead of CRegKey::Open when installing the 
                          HKLM\SYSTEM\CurrentControlSet\Services\EventLog\"LogName" registry key. This ensures that event log sources which are created with custom
                          LogName's will be installed correctly. Thanks to Rolf Schatten for reporting this bug.
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


///////////////////////////////// Includes ////////////////////////////////////

#include "stdafx.h"
#include "ntservEventLogSource.h"
#include "ntserv_msg.h"


///////////////////////////////// Includes ////////////////////////////////////

#pragma warning(suppress: 26455)
CNTEventLogSource::CNTEventLogSource() : m_hEventSource(nullptr),
                                         m_sLogName(_T("Application"))
{
}

CNTEventLogSource::CNTEventLogSource(_In_opt_z_ LPCTSTR pUNCServerName, _In_opt_z_ LPCTSTR pSourceName, _In_opt_z_ LPCTSTR pLogName) : m_hEventSource(nullptr),
                                                                                                                                       m_sServerName((pUNCServerName == nullptr) ? _T("") : pUNCServerName),
                                                                                                                                       m_sSourceName((pSourceName == nullptr) ? _T("") : pSourceName),
                                                                                                                                       m_sLogName((pLogName == nullptr) ? _T("") : pLogName)

{
}

CNTEventLogSource::CNTEventLogSource(_In_opt_z_ LPCTSTR pUNCServerName, _In_opt_z_ LPCTSTR pSourceName) : m_hEventSource(nullptr),
                                                                                                          m_sServerName((pUNCServerName == nullptr) ? _T("") : pUNCServerName),
                                                                                                          m_sSourceName((pSourceName == nullptr) ? _T("") : pSourceName),
                                                                                                          m_sLogName(_T("Application"))
{
}

CNTEventLogSource::~CNTEventLogSource()
{
  Deregister();
}

void CNTEventLogSource::SetServerName(_In_opt_z_ LPCTSTR pszServerName)
{
  if (pszServerName != nullptr)
    m_sServerName = pszServerName;
  else
    m_sServerName = _T("");
}

void CNTEventLogSource::SetSourceName(_In_opt_z_ LPCTSTR pszSourceName)
{
  if (pszSourceName != nullptr)
    m_sSourceName = pszSourceName;
  else
    m_sSourceName = _T("");
}

void CNTEventLogSource::SetLogName(_In_opt_z_ LPCTSTR pszLogName)
{
  if (pszLogName != nullptr)
    m_sLogName = pszLogName;
  else
    m_sLogName = _T("");
}

CNTEventLogSource::operator HANDLE() const noexcept
{
  return m_hEventSource;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Attach(_In_opt_ HANDLE hEventSource) noexcept
{
  if (m_hEventSource != hEventSource)
    Deregister();

  m_hEventSource = hEventSource;
  return TRUE;
}

HANDLE CNTEventLogSource::Detach() noexcept
{
  HANDLE hReturn = m_hEventSource;
  m_hEventSource = nullptr;
  return hReturn;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Register(_In_opt_ LPCTSTR pUNCServerName, _In_ LPCTSTR pSourceName) noexcept
{
  Deregister();
  m_hEventSource = RegisterEventSource(pUNCServerName, pSourceName);
  return (m_hEventSource != nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ WORD wCategory, _In_ DWORD dwEventID, _In_opt_ PSID pUserSid,
                                                                  _In_ WORD wNumStrings, _In_ DWORD dwDataSize, _In_reads_opt_(wNumStrings) LPCTSTR* pStrings, _In_reads_bytes_opt_(dwDataSize) LPVOID pRawData)
{
  ATL::CComCritSecLock<ATL::CComAutoCriticalSection> sl(m_csReport, true);

#pragma warning(suppress: 26477)
  ATLASSERT(m_hEventSource == nullptr);
  if (!Register(m_sServerName.c_str(), m_sSourceName.c_str()))
    return FALSE;
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventSource != nullptr);

  //Call the SDK version of the function
  const BOOL bSuccess = ReportEvent(m_hEventSource, wType,	wCategory, dwEventID, pUserSid, wNumStrings, dwDataSize, pStrings, pRawData);
  Deregister();

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID)
{
  return Report(wType, 0, dwEventID, nullptr, 0, 0, nullptr, nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_opt_z_ LPCTSTR pszString)
{
  return Report(wType, CNTS_MSG_SERVICE_FULLSTRING, pszString);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszString != nullptr);

  return Report(wType, 0, dwEventID, nullptr, 1, 0, &pszString, nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszString1 != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszString2 != nullptr);

  std::array<LPCTSTR, 2> strings{ pszString1, pszString2 };
  return Report(wType, 0, dwEventID, nullptr, 2, 0, strings.data(), nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2, _In_ DWORD dwCode, _In_ BOOL bReportAsHex)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszString1 != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszString2 != nullptr);

  std::vector<LPCTSTR> strings{ pszString1, pszString2 };
  CAtlString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  strings.push_back(sError);
  return Report(wType, 0, dwEventID, nullptr, 3, 0, strings.data(), nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2, _In_opt_z_ LPCTSTR pszString3)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszString1 != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszString2 != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszString3 != nullptr);

  std::array<LPCTSTR, 3> strings{ pszString1, pszString2, pszString3 };
  return Report(wType, 0, dwEventID, nullptr, 3, 0, strings.data(), nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_ DWORD dwCode, _In_ BOOL bReportAsHex)
{
  CAtlString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  return Report(wType, dwEventID, sError);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString, _In_ DWORD dwCode, _In_ BOOL bReportAsHex)
{
  CAtlString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  return Report(wType, dwEventID, pszString, sError);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Deregister() noexcept
{
  BOOL bSuccess = TRUE;
  if (m_hEventSource != nullptr)
  {
    bSuccess = DeregisterEventSource(m_hEventSource);
    m_hEventSource = nullptr;
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Install(_In_opt_z_ LPCTSTR pszSourceName, _In_opt_z_ LPCTSTR pszEventMessageFile, _In_opt_z_ LPCTSTR pszEventCategoryMessageFile, _In_opt_z_ LPCTSTR pszEventParameterMessageFile, _In_ DWORD dwTypesSupported, _In_ DWORD dwCategoryCount)
{
  //Just delegate the work to the other version of "Install"
  return Install(_T("Application"), pszSourceName, pszEventMessageFile, pszEventCategoryMessageFile, pszEventParameterMessageFile, dwTypesSupported, dwCategoryCount);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Install(_In_opt_z_ LPCTSTR pszLogName, _In_opt_z_ LPCTSTR pszSourceName, _In_opt_z_ LPCTSTR pszEventMessageFile, _In_opt_z_ LPCTSTR pszEventCategoryMessageFile, _In_opt_z_ LPCTSTR pszEventParameterMessageFile, _In_ DWORD dwTypesSupported, _In_ DWORD dwCategoryCount)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(pszLogName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(_tcslen(pszLogName));
#pragma warning(suppress: 26477)
  ATLASSUME(pszSourceName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(_tcslen(pszSourceName));
#pragma warning(suppress: 26477)
  ATLASSUME(pszEventMessageFile != nullptr);

  //What will be the return value from this function, assume the worst
  BOOL bSuccess = FALSE;

  //Make the necessary updates to the registry
  CAtlString sKey;
  sKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s"), pszLogName);
  ATL::CRegKey appKey;
  if (appKey.Create(HKEY_LOCAL_MACHINE, sKey) == ERROR_SUCCESS)
  {
    ATL::CRegKey sourceKey;
#pragma warning(suppress: 26477)
    if (sourceKey.Create(appKey, pszSourceName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, nullptr) == ERROR_SUCCESS)
    {
      //Write the EventMessageFile string value
      bSuccess = sourceKey.SetStringValue(_T("EventMessageFile"), pszEventMessageFile) == ERROR_SUCCESS;

      //Write the TypesSupported value
      bSuccess = bSuccess && sourceKey.SetDWORDValue(_T("TypesSupported"), dwTypesSupported) == ERROR_SUCCESS;

      //Write the CategoryCount value if required
      if (dwCategoryCount && bSuccess)
        bSuccess = sourceKey.SetDWORDValue(_T("CategoryCount"), dwCategoryCount) == ERROR_SUCCESS;

      //Write the CategoryMessageFile string if required
      if (pszEventCategoryMessageFile && _tcslen(pszEventCategoryMessageFile) && bSuccess)
        bSuccess = sourceKey.SetStringValue(_T("CategoryMessageFile"), pszEventCategoryMessageFile) == ERROR_SUCCESS;

      //Write the ParameterMessageFile string if required
      if (pszEventParameterMessageFile && _tcslen(pszEventParameterMessageFile) && bSuccess)
        bSuccess = sourceKey.SetStringValue(_T("ParameterMessageFile"), pszEventParameterMessageFile) == ERROR_SUCCESS;

      //Update the sources registry key so that the event viewer can filter on the events which we write to the event log
      if (bSuccess)
      {
        CNTServiceStringArray sources;
#pragma warning(suppress: 26486)
        if (GetStringArrayFromRegistry(appKey, _T("Sources"), sources))
        {
          //If our name is not in the array then add it
          BOOL bFoundMyself = FALSE;
#pragma warning(suppress: 26489)
          bFoundMyself = (std::find(sources.begin(), sources.end(), pszSourceName) != sources.end());
          if (!bFoundMyself)
          {
            sources.push_back(pszSourceName);
            SetStringArrayIntoRegistry(appKey, _T("Sources"), sources);
          }
        }
      }
    }
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Uninstall(_In_opt_z_ LPCTSTR pSourceName)
{
  //Just delegate the work to the other version of "Install"
  return Uninstall(_T("Application"), pSourceName);
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::Uninstall(_In_opt_z_ LPCTSTR pszLogName, _In_opt_z_ LPCTSTR pszSourceName)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(pszLogName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(_tcslen(pszLogName));
#pragma warning(suppress: 26477)
  ATLASSUME(pszSourceName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(_tcslen(pszSourceName));

  //Remove the settings from the registry
  CAtlString sSubKey;
  sSubKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s\\%s"), pszLogName, pszSourceName);
  const LSTATUS nSuccess = RegDeleteKey(HKEY_LOCAL_MACHINE, sSubKey);
  if (nSuccess != ERROR_SUCCESS) //If we cannot delete this registry key, then abort this function before we go any further
  {
    SetLastError(nSuccess); //Make the last error value available to our callers 
    return FALSE;
  }

  //Remove ourself from the "Sources" registry key
  sSubKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s"), pszLogName);
  ATL::CRegKey appKey;
  if (appKey.Open(HKEY_LOCAL_MACHINE, sSubKey, KEY_WRITE | KEY_READ) == ERROR_SUCCESS)
  {
#pragma warning(suppress: 26489)
    CNTServiceStringArray sources;
#pragma warning(suppress: 26486)
    if (GetStringArrayFromRegistry(appKey, _T("Sources"), sources))
    {
      //If our name is in the array then remove it
      BOOL bFoundMyself = FALSE;
#pragma warning(suppress: 26489)
      auto iterFind = std::find(sources.begin(), sources.end(), pszSourceName);
      bFoundMyself = (iterFind != sources.end());
      if (bFoundMyself)
#pragma warning(suppress: 26444)
        sources.erase(iterFind);
      if (bFoundMyself)
        SetStringArrayIntoRegistry(appKey, _T("Sources"), sources);
    }
  }

  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::GetStringArrayFromRegistry(_In_ ATL::CRegKey& key, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszEntry != nullptr);

  //What will be the return value from this function, assume the worst
  BOOL bSuccess = FALSE;

  //Initialize the output parameters to default values
  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;
  array.clear();

  DWORD dwType = 0;
  ULONG nBytes = 0;
  LSTATUS lResult = key.QueryValue(pszEntry, &dwType, nullptr, &nBytes);
  if (lResult == ERROR_SUCCESS)
  {
    //Allocate some memory for the API
    ATL::CHeapPtr<TCHAR> pBuffer;
    ULONG nChars = nBytes / sizeof(TCHAR);
    if (nChars < 2) //Ensure we can handle an empty MULTI_SZ string
      nChars = 2;
    if (!pBuffer.Allocate(nChars))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      if (pLastError != nullptr)
        *pLastError = ERROR_OUTOFMEMORY;
      return FALSE;
    }

    lResult = key.QueryMultiStringValue(pszEntry, pBuffer, &nChars);
    if (lResult == ERROR_SUCCESS)
    {
#pragma warning(suppress: 26489)
      LPCTSTR pszStrings = pBuffer.m_pData;
#pragma warning(suppress: 26477)
      ATLASSUME(pszStrings != nullptr);
#pragma warning(suppress: 26489 26481)
      while (pszStrings[0] != _T('\0'))
      {
#pragma warning(suppress: 26489 26486)
        array.push_back(pszStrings);
#pragma warning(suppress: 26481)
        pszStrings += (_tcslen(pszStrings) + 1);
      }

      bSuccess = TRUE;
    }
    else
    {
      if (pLastError != nullptr)
        *pLastError = lResult;
    }
  }
  else
  {
    if (pLastError != nullptr)
      *pLastError = lResult;
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLogSource::SetStringArrayIntoRegistry(_In_ ATL::CRegKey& key, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our input parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszEntry != nullptr);

  //Initialize the output parameter to default value
  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  //Work out the size of the buffer we will need
  CNTServiceStringArray::size_type nSize = 0;
  for (const auto& element : array)
    nSize += element.length() + 1; //1 extra for each null terminator

  //Need one second null for the double null at the end
  nSize++;

  //Allocate some memory for the API
  ATL::CHeapPtr<TCHAR> pBuffer;
  if (!pBuffer.Allocate(nSize))
  {
    SetLastError(ERROR_OUTOFMEMORY);
    if (pLastError != nullptr)
      *pLastError = ERROR_OUTOFMEMORY;
    return FALSE;
  }

  //Now copy the strings into the buffer
#pragma warning(suppress: 26489)
  LPTSTR pszString = pBuffer.m_pData;
#pragma warning(suppress: 26477)
  ATLASSUME(pszString != nullptr);
  CNTServiceStringArray::size_type nCurOffset = 0;
  for (const auto& element : array)
  {
    const CNTServiceString::size_type nCurrentStringLength = element.length();
#pragma warning(suppress: 26489 26486 26481)
    _tcscpy_s(&pszString[nCurOffset], nCurrentStringLength+1, element.c_str());
    nCurOffset += (nCurrentStringLength + 1);
  }
  //Don't forgot to doubly null terminate
#pragma warning(suppress: 26489 26481)
  pszString[nCurOffset] = _T('\0');

  //Finally write it into the registry
  LSTATUS lResult = key.SetMultiStringValue(pszEntry, pBuffer);
  const BOOL bSuccess = (lResult == ERROR_SUCCESS);
  if (!bSuccess && (pLastError != nullptr))
    *pLastError = lResult;

  return bSuccess;
}
