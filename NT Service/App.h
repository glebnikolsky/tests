/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __APP_H__
#define __APP_H__


////////////////////////////// Includes ///////////////////////////////////////

#include "ntserv.h"
#include "ntservServiceControlManager.h"


////////////////////////////// Classes ////////////////////////////////////////

class CMyService : public CNTService
{
public:
  CMyService();
  void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* pszArgv) override;
  void OnStop() noexcept override;
  void OnPause() noexcept override;
  void OnContinue() noexcept override;
  void ShowHelp() noexcept override;
  void OnUserDefinedRequest(DWORD dwControl) override;

protected:
  std::atomic<bool> m_bWantStop;
  std::atomic<bool> m_bPaused;
  DWORD             m_dwBeepInternal;
};

#endif //#ifndef __APP_H__