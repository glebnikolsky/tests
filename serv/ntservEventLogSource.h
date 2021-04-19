/*
Module : ntservEventLogSource.h
Purpose: Defines the interface for the class CNTEventLogSource.
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


////////////////////////////// Macros / Defines ///////////////////////////////

#pragma once

#ifndef __NTSERVEVENTLOGSOURCE_H__
#define __NTSERVEVENTLOGSOURCE_H__

#define _NODISCARD
////////////////////////////// Includes ///////////////////////////////////////

#include "ntservDefines.h"


////////////////////////////// Classes ////////////////////////////////////////

//An encapsulation of the APIs used to register, unregister, write, install and uninstall Event log entries 
//i.e. the server side to the Event log APIs
class CNTSERVICE_EXT_CLASS CNTEventLogSource
{
public:
//Constructors / Destructors
  CNTEventLogSource();
  CNTEventLogSource(_In_opt_z_ LPCTSTR pUNCServerName, _In_opt_z_ LPCTSTR pSourceName, _In_opt_z_ LPCTSTR pLogName);
  CNTEventLogSource(_In_opt_z_ LPCTSTR pUNCServerName, _In_opt_z_ LPCTSTR pSourceName);
  CNTEventLogSource(_In_ const CNTEventLogSource&) = delete;
  CNTEventLogSource(_In_ CNTEventLogSource&&) = delete;
  ~CNTEventLogSource();
  

//Accessors / Mutators
  void SetServerName(_In_opt_z_ LPCTSTR pszServerName);
  _NODISCARD CNTServiceString GetServerName() const { return m_sServerName; };
  void SetSourceName(_In_opt_z_ LPCTSTR pszSourceName);
  _NODISCARD CNTServiceString GetSourceName() const { return m_sSourceName; };
  void SetLogName(_In_opt_z_ LPCTSTR pszLogName);
  _NODISCARD CNTServiceString GetLogName() const { return m_sLogName; };

//Methods
  CNTEventLogSource& operator=(_In_ const CNTEventLogSource&) = delete;
  CNTEventLogSource& operator=(_In_ CNTEventLogSource&&) = delete;
  operator HANDLE() const noexcept;
  void Attach(_In_opt_ HANDLE hEventSource) noexcept;
  HANDLE Detach() noexcept;
  _Return_type_success_(return != 0) BOOL Register(_In_opt_ LPCTSTR pUNCServerName, _In_ LPCTSTR pSourceName) noexcept;
  _Return_type_success_(return != 0) BOOL Deregister() noexcept;
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ WORD wCategory, _In_ DWORD dwEventID, _In_opt_ PSID pUserSid,
                                                 _In_ WORD wNumStrings, _In_ DWORD dwDataSize, _In_reads_opt_(wNumStrings) LPCTSTR* pStrings, _In_reads_bytes_opt_(dwDataSize) LPVOID pRawData);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_opt_z_ LPCTSTR pszString);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2, _In_opt_z_ LPCTSTR pszString3);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString1, _In_opt_z_ LPCTSTR pszString2, _In_ DWORD dwCode, _In_ bool bReportAsHex = false);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_opt_z_ LPCTSTR pszString, _In_ DWORD dwCode, _In_ bool bReportAsHex = false);
  _Return_type_success_(return != 0) BOOL Report(_In_ WORD wType, _In_ DWORD dwEventID, _In_ DWORD dwCode, _In_ bool bReportAsHex = false);

  static _Return_type_success_(return != 0) bool Install(_In_opt_z_ LPCTSTR pszSourceName, _In_opt_z_ LPCTSTR pszEventMessageFile, _In_opt_z_ LPCTSTR pszEventCategoryMessageFile, _In_opt_z_ LPCTSTR pszEventParameterMessageFile, _In_ DWORD dwTypesSupported, _In_ DWORD dwCategoryCount);
  static _Return_type_success_(return != 0) bool Install(_In_opt_z_ LPCTSTR pszLogName, _In_opt_z_ LPCTSTR pszSourceName, _In_opt_z_ LPCTSTR pszEventMessageFile, _In_opt_z_ LPCTSTR pszEventCategoryMessageFile, _In_opt_z_ LPCTSTR pszEventParameterMessageFile, _In_ DWORD dwTypesSupported, _In_ DWORD dwCategoryCount);
  static _Return_type_success_(return != 0) bool Uninstall(_In_opt_z_ LPCTSTR pSourceName);
  static _Return_type_success_(return != 0) bool Uninstall(_In_opt_z_ LPCTSTR pszLogName, _In_opt_z_ LPCTSTR pszSourceName);
  static _Return_type_success_(return != 0) bool GetStringArrayFromRegistry(_In_ ATL::CRegKey& key, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError = nullptr);
  static _Return_type_success_(return != 0) bool SetStringArrayIntoRegistry(_In_ ATL::CRegKey& key, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError = nullptr);
  
protected:
  HANDLE m_hEventSource;
  CNTServiceString m_sServerName;
  CNTServiceString m_sSourceName;
  CNTServiceString m_sLogName;
  ATL::CComAutoCriticalSection m_csReport; //Critical section to protect multiple threads calling Report at the one time

  friend class CNTService;
};

#endif //#ifndef __NTSERVEVENTLOGSOURCE_H__
