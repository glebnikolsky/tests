/*
Module : ntservEventLog.cpp
Purpose: Implementation for the class CNTEventLog
Created: PJN / 14-07-1997
History: PJN / 25-06-2006 1. Code now uses newer C++ style casts instead of C style casts.
         PJN / 02-02-2007 1. Optimized CNTEventLog constructor code.
         PJN / 01-08-2010 1. Added support for GetEventLogInformation
         PJN / 18-12-2015 1. Added SAL annotations to all the code
         PJN / 03-11-2019 1. Updated initialization of various structs to use C++ 11 list initialization
         PJN / 06-11-2019 1. Removed unnecessary return value from CNTEventLog::Attach method

Copyright (c) 1996 - 2020 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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
#include "ntservEventLog.h"


///////////////////////////////// Implementation //////////////////////////////

CNTEventLog::CNTEventLog() noexcept : m_hEventLog(nullptr)
{
}

CNTEventLog::~CNTEventLog()
{
  Close();
}

CNTEventLog::operator HANDLE() const noexcept
{
  return m_hEventLog;
}

void CNTEventLog::Attach(_In_opt_ HANDLE hEventLog) noexcept
{
  if (m_hEventLog != hEventLog)
    Close();
  m_hEventLog = hEventLog;
}

HANDLE CNTEventLog::Detach() noexcept
{
  HANDLE hReturn = m_hEventLog;
  m_hEventLog = nullptr;
  return hReturn;
}

_Return_type_success_(return != 0) BOOL CNTEventLog::Open(_In_opt_ LPCTSTR pUNCServerName, _In_ LPCTSTR pSourceName) noexcept
{
  Close();
  m_hEventLog = OpenEventLog(pUNCServerName, pSourceName);
  return (m_hEventLog != nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::OpenApplication(_In_opt_ LPCTSTR pUNCServerName) noexcept
{
  return Open(pUNCServerName, _T("Application"));
}

_Return_type_success_(return != 0) BOOL CNTEventLog::OpenSystem(_In_opt_ LPCTSTR pUNCServerName) noexcept
{
  return Open(pUNCServerName, _T("System"));
}

_Return_type_success_(return != 0) BOOL CNTEventLog::OpenSecurity(_In_opt_ LPCTSTR pUNCServerName) noexcept
{
  return Open(pUNCServerName, _T("Security"));
}

_Return_type_success_(return != 0) BOOL CNTEventLog::OpenBackup(_In_opt_ LPCTSTR pUNCServerName, _In_ LPCTSTR pFileName) noexcept
{
  Close();
  m_hEventLog = OpenBackupEventLog(pUNCServerName, pFileName);
  return (m_hEventLog != nullptr);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::Close() noexcept
{
  BOOL bSuccess = TRUE;
  if (m_hEventLog != nullptr)
  {
    bSuccess = CloseEventLog(m_hEventLog);
    m_hEventLog = nullptr;
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLog::Backup(_In_ LPCTSTR pBackupFileName) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);
  
  return BackupEventLog(m_hEventLog, pBackupFileName);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::Clear(_In_opt_ LPCTSTR pBackupFileName) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);
  
  return ClearEventLog(m_hEventLog, pBackupFileName);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::GetNumberOfRecords(_Out_ DWORD& dwNumberOfRecords) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);
  
  return GetNumberOfEventLogRecords(m_hEventLog, &dwNumberOfRecords);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::GetOldestRecord(_Out_ DWORD& dwOldestRecord) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);
  
  return GetOldestEventLogRecord(m_hEventLog, &dwOldestRecord);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::NotifyChange(_In_ HANDLE hEvent) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);

  return NotifyChangeEventLog(m_hEventLog, hEvent);
}

_Return_type_success_(return != 0) BOOL CNTEventLog::ReadNext(_Out_ CEventLogRecord& record) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);

  DWORD dwBytesRead = 0;
  DWORD dwBytesNeeded = 0;
  EVENTLOGRECORD el{};
  BOOL bSuccess = ReadEventLog(m_hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ, 0, &el, sizeof(EVENTLOGRECORD), &dwBytesRead, &dwBytesNeeded);
  if (bSuccess)
    record = CEventLogRecord(&el);
  else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
  {
    std::vector<BYTE> pBuffer;
    pBuffer.resize(dwBytesNeeded);
    bSuccess = ReadEventLog(m_hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ, 0, pBuffer.data(), dwBytesNeeded, &dwBytesRead, &dwBytesNeeded);
    if (bSuccess)
#pragma warning(suppress: 26490)
      record = CEventLogRecord(reinterpret_cast<EVENTLOGRECORD*>(pBuffer.data()));
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLog::ReadPrev(_Out_ CEventLogRecord& record) const
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);

  DWORD dwBytesRead;
  DWORD dwBytesNeeded;
  EVENTLOGRECORD el;
  BOOL bSuccess = ReadEventLog(m_hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ, 0, &el, sizeof(EVENTLOGRECORD), &dwBytesRead, &dwBytesNeeded);
  if (bSuccess)
    record = CEventLogRecord(&el);
  else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
  {
    std::vector<BYTE> pBuffer;
    pBuffer.resize(dwBytesNeeded);  
    bSuccess = ReadEventLog(m_hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ, 0, pBuffer.data(), dwBytesNeeded, &dwBytesRead, &dwBytesNeeded);
    if (bSuccess)
#pragma warning(suppress: 26490)
      record = CEventLogRecord(reinterpret_cast<EVENTLOGRECORD*>(pBuffer.data()));
  }

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTEventLog::GetFullInformation(_Out_ DWORD& dwFull) const noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(m_hEventLog != nullptr);

  //Call through the function pointer
  EVENTLOG_FULL_INFORMATION efi{};
  DWORD dwBytesNeeded = 0;
  const BOOL bSuccess = GetEventLogInformation(m_hEventLog, EVENTLOG_FULL_INFO, &efi, sizeof(efi), &dwBytesNeeded);
  if (bSuccess)
    dwFull = efi.dwFull;

  return bSuccess;
}
