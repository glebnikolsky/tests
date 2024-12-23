/*
Module : ntservCmdLineInfo.h
Purpose: Defines the interface for the class CTNServiceCommandLineInfo.
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

#ifndef __NTSERVCMDLINEINFO_H__
#define __NTSERVCMDLINEINFO_H__


////////////////////////////// Includes ///////////////////////////////////////

#include "ntservDefines.h"


////////////////////////////// Classes ////////////////////////////////////////

//The CNTServiceCommandLineInfo class aids in parsing the 
//command line at application startup of an NT Service. The 
//structure is styled upon the MFC class CCommandLineInfo
class CNTSERVICE_EXT_CLASS CNTServiceCommandLineInfo
{
public:
//Constructors / Destructors
  CNTServiceCommandLineInfo() noexcept;
  CNTServiceCommandLineInfo(_In_ const CNTServiceCommandLineInfo&) = delete;
  CNTServiceCommandLineInfo(_In_ CNTServiceCommandLineInfo&&) = delete;
  virtual ~CNTServiceCommandLineInfo() = default;

//Methods
  virtual void ParseParam(_In_z_ LPCTSTR pszParam, _In_ bool bFlag, _In_ bool bLast);
  CNTServiceCommandLineInfo& operator=(_In_ const CNTServiceCommandLineInfo&) = delete;
  CNTServiceCommandLineInfo& operator=(_In_ CNTServiceCommandLineInfo&&) = delete;
  static ATL::CAtlString UnquoteSpaces(_In_ const ATL::CAtlString& str);

//Data
  enum class ShellCommand
  {
    RunAsService,
    StartTheService,
    PauseService,
    ContinueService,
    StopService,
    InstallService,
    UninstallService,
    DebugService,
    ShowServiceHelp
  };

  ShellCommand m_nShellCommand;
  DWORD m_dwTimeout;
  bool m_bSilent;
  CNTServiceString m_sServiceCommandLine;
  CNTServiceString m_sServiceName;
  CNTServiceString m_sServiceDisplayName;
  CNTServiceString m_sServiceDescription;
  CNTServiceString m_sUserName;
  CNTServiceString m_sPassword;
  bool m_bAutoStart;
  bool m_bEnableServiceLogonRight;
};

#endif //#ifndef __NTSERVCMDLINEINFO_H__
