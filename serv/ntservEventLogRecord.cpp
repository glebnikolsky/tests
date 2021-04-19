/*  
Module : ntservEventLogRecord.cpp
Purpose: Implementation for the class CNTEventLogRecord
Created: PJN / 14-07-1997
History: PJN / 14-08-2005 1. Fixed an issue where the SID and binary values was not being set correctly in the 
                          overloaded CEventLogRecord constructor which takes a EVENTLOGRECORD pointer. Thanks 
                          to Brian Lee for reporting this issue. Also the arrays which this information is 
                          copied into is now preallocated to improve performance.
         PJN / 12-05-2006 1. Minor update to remove now unnecessary include of AfxPriv.h from this module.
         PJN / 25-06-2006 1. Optimized CEventLogRecord constructor
                          2. Code now uses newer C++ style casts instead of C style casts.
         PJN / 01-08-2010 1. CEventLogRecord::m_TimeGenerated and CEventLogRecord::m_TimeWritten values are now 
                          simple DWORD values instead of CTime instances.
         PJN / 18-12-2015 1. Added SAL annotations to all the code
         PJN / 26-07-2020 1. Fixed a compile problem in ntservEventLogRecord.cpp in VC 2019 related to missing iterator header file

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

#ifndef _ITERATOR_
#pragma message("To avoid this message, please put iterator in your pre compiled header (normally stdafx.h)")
#include <iterator>
#endif //#ifndef _ITERATOR_

#include "ntservEventLogRecord.h"


///////////////////////////////// Implementation //////////////////////////////

CEventLogRecord::CEventLogRecord() noexcept : m_dwRecordNumber(0),
                                              m_dwTimeGenerated(0),
                                              m_dwTimeWritten(0),
                                              m_dwEventID(0),
                                              m_wEventType(0),
                                              m_wEventCategory(0)
{
}

CEventLogRecord::CEventLogRecord(_In_opt_ const EVENTLOGRECORD* pRecord)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(pRecord != nullptr);

  //First the easy ones
  m_dwRecordNumber = pRecord->RecordNumber;
  m_dwTimeGenerated = pRecord->TimeGenerated;
  m_dwTimeWritten = pRecord->TimeWritten;
  m_dwEventID = pRecord->EventID;
  m_wEventType = pRecord->EventType;
  m_wEventCategory = pRecord->EventCategory;

  //Copy over the SID
#pragma warning(suppress: 26490)
  const BYTE* pBeginRecord = reinterpret_cast<const BYTE*>(pRecord);
#pragma warning(suppress: 26477)
  ATLASSUME(pBeginRecord != nullptr);
  if (pRecord->UserSidLength)
  {
    m_UserSID.reserve(pRecord->UserSidLength); //Preallocate the array to improve performance
#pragma warning(suppress: 26481)
    const BYTE* pUserSID = pBeginRecord + pRecord->UserSidOffset;
#pragma warning(suppress: 26481)
    std::copy(pUserSID, pUserSID + pRecord->UserSidLength, std::back_inserter(m_UserSID));
  }

  //Copy over the Binary data
  if (pRecord->DataLength)
  {
    m_Data.reserve(pRecord->DataLength); //Preallocate the array to improve performance
#pragma warning(suppress: 26481)
    const BYTE* pData = pBeginRecord + pRecord->DataOffset;
#pragma warning(suppress: 26481)
    std::copy(pData, pData + pRecord->DataLength, std::back_inserter(m_Data));
  }

  //Copy over the SourceName
#pragma warning(suppress: 26429 26481 26490)
  auto pszBeginRecord = reinterpret_cast<const TCHAR*>(pBeginRecord + sizeof(EVENTLOGRECORD));
  DWORD dwCurOffset = 0;
  DWORD dwStartOffset = dwCurOffset;
#pragma warning(suppress: 26481)
  while (pszBeginRecord[dwCurOffset])
    ++dwCurOffset;
#pragma warning(suppress: 26481)
  m_sSourceName = &pszBeginRecord[dwStartOffset];

  //Skip over the null
  ++dwCurOffset;

  //Copy over the ComputerName value
  dwStartOffset = dwCurOffset;
#pragma warning(suppress: 26481)
  while (pszBeginRecord[dwCurOffset])
    ++dwCurOffset;
#pragma warning(suppress: 26481)
  m_sComputerName = &pszBeginRecord[dwStartOffset];

  //Copy over the strings array
  int nStringsRead = 0;
#pragma warning(suppress: 26481 26490)
  pszBeginRecord = reinterpret_cast<const TCHAR*>(pBeginRecord + pRecord->StringOffset);
  dwCurOffset = 0;
  while (nStringsRead < pRecord->NumStrings)
  {
    //Find the next string
    dwStartOffset = dwCurOffset;
#pragma warning(suppress: 26481)
    while (pszBeginRecord[dwCurOffset])
      ++dwCurOffset;

    //Add it to the array
#pragma warning(suppress: 26481)
    CNTServiceString sText(&pszBeginRecord[dwStartOffset]);
    m_Strings.push_back(sText);

    //Increment the number of strings read
    ++nStringsRead;

    //Skip over the null
    ++dwCurOffset;
  }
}
