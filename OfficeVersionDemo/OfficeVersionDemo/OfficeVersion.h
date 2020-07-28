#pragma once
// =============================================================
// #include "OfficeVersion.h"
//
// Copyright (C) 2006 Warren Stevens. All rights reserved.
// (e m a i l : w a r r e n . d . s t e v e n s AT g m a i l . c o m )
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any
// damages arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it, subject to the following restrictions:
// 
// 1. If you use this software in a product, an acknowledgment in the 
// product documentation, or about box, and an email letting me know 
// it is being used, would be appreciated, but is not required.
//
// 2. The origin of this software must not be misrepresented; you must
// not claim that you wrote the original software. Altered source versions 
// must be plainly marked as such, and must not be misrepresented as being 
// the original software.
//
// 3. Original, or altered, source versions may not be sold for 
// profit without the authors written consent.
// 
// 4. This notice may not be removed or altered from any source
// distribution.
// =============================================================
// History
// (Y-M-D), version, comment
// 2006-12-01, Version 1.00, Initial release.
// =============================================================
// For more information read the following articles:
//
// Microsoft Office System Trademark Guidelines
// http://www.microsoft.com/about/legal/intellectualproperty/trademarks/usage/office.mspx
//
// How to Determine the Path for an Office Application
// http://support.microsoft.com/kb/240794/EN-US/
//
// How To Find the Path and Version of an Office Application from Visual C++
// http://support.microsoft.com/kb/q247985/
//  *** Note *** the code in this article is out of date (the check for 
//  version 10 does not work for versions 11 and 12 (aka 2003 and 2007)
//
//////////////////////////////////////////////////////////////////////////


class OfficeVersion
{
public:
	enum eOfficeVersion
	{
		eOfficeVersion_Unknown, // error return value
		eOfficeVersion_95,
		eOfficeVersion_97,
		eOfficeVersion_2000,
		eOfficeVersion_XP,   // XP = 2002 + marketing
		eOfficeVersion_2003,
		eOfficeVersion_2007,
		eOfficeVersion_2010,
		eOfficeVersion_2013,
		eOfficeVersion_2019
	};

	enum eOfficeApp // in case you are looking for a particular app
	{
		eOfficeApp_Word,
		eOfficeApp_Excel,
		eOfficeApp_Outlook,
		eOfficeApp_Access,
		eOfficeApp_PowerPoint,
	};

	static CString GetVersionAsString(const eOfficeVersion officeVersion)
	{
		switch(officeVersion) {
			case eOfficeVersion_Unknown: { return _T("Not found");       }break;
			case eOfficeVersion_95:      { return _T("Office 95");       }break;
			case eOfficeVersion_97:      { return _T("Office 97");       }break;
			case eOfficeVersion_2000:    { return _T("Office 2000");     }break;
			case eOfficeVersion_XP:      { return _T("Office XP");       }break;
			case eOfficeVersion_2003:    { return _T("Office 2003");     }break;
			case eOfficeVersion_2007:    { return _T("Office 2007");     }break;
			case eOfficeVersion_2010:	 { return _T("Office 2010");     }break;
			case eOfficeVersion_2013:	 { return _T("Office 2013");     }break;
			case eOfficeVersion_2019:	 { return _T("Office 2019/365"); }break;
			default:                     { ASSERT(false); return _T(""); }break; // added another ???
		}
	}

	static CString GetApplicationAsString(const eOfficeApp officeApp)
	{
		switch(officeApp) {			
			case eOfficeApp_Word:       { return _T("Word");            }break;
			case eOfficeApp_Excel:      { return _T("Excel");           }break;
			case eOfficeApp_Outlook:    { return _T("Outlook");         }break;
			case eOfficeApp_Access:     { return _T("Access");          }break;
			case eOfficeApp_PowerPoint: { return _T("Powerpoint");      }break;
			default:                    { ASSERT(false); return _T(""); }break; // added another ???
		}
	}


	static CString GetProgID(const eOfficeApp officeApp)
	{
		// ProgIDs from http://support.microsoft.com/kb/240794/EN-US/
		switch(officeApp) {			
			case eOfficeApp_Word:       { return _T("Word.Application");       }break;
			case eOfficeApp_Excel:      { return _T("Excel.Application");      }break;
			case eOfficeApp_Outlook:    { return _T("Outlook.Application");    }break;
			case eOfficeApp_Access:     { return _T("Access.Application");     }break;
			case eOfficeApp_PowerPoint: { return _T("Powerpoint.Application"); }break;
			default:                    { ASSERT(false); return _T("");        }break; // added another ???
		}
	}

	static eOfficeVersion StringToVersion(const CString& versionString)
	{
		// mapping between the marketing version (e.g. 2003) and the behind-the-scenes version
		if(_T("7") == versionString){
			return eOfficeVersion_95;
		}else if(_T("8") == versionString){
			return eOfficeVersion_97;
		}else if(_T("9") == versionString){
			return eOfficeVersion_2000;
		}else if(_T("10") == versionString){
			return eOfficeVersion_XP;
		}else if(_T("11") == versionString){
			return eOfficeVersion_2003;
		}else if(_T("12") == versionString){
			return eOfficeVersion_2007;
		}
		else if (_T("14") == versionString) {
			return eOfficeVersion_2010;
		}
		else if (_T("15") == versionString) {
			return eOfficeVersion_2013;
		}
		else if (_T("16") == versionString) {
			return eOfficeVersion_2019;
		}else{
			return eOfficeVersion_Unknown; // added another ???
		}
	}
	
	static eOfficeVersion GetOfficeVersion()
	{
		// by default we use Word (and so on, down the list) as a proxy for "Office" 
		// (i.e. if word is there then "Office" is there)
		// if you want something more specific, then call GetApplicationVersion()

		static const eOfficeApp appsToCheck[] = {	
			eOfficeApp_Word,
			eOfficeApp_Excel,
			eOfficeApp_Outlook,
			eOfficeApp_Access,
			eOfficeApp_PowerPoint,
		};
		const int numItems( sizeof(appsToCheck) / sizeof(appsToCheck[0]) );		
		
		for(int i=0; i<numItems; ++i){
			const eOfficeVersion thisAppVersion( GetApplicationVersion(eOfficeApp_Word) );
			if(eOfficeVersion_Unknown != thisAppVersion){
				return thisAppVersion;
			}
		}
		
		return eOfficeVersion_Unknown; // probably nothing installed
	}

	static eOfficeVersion GetApplicationVersion(eOfficeApp appToCheck)
	{
		// some of this function is based on the code in the article at: http://support.microsoft.com/kb/q247985/
		const CString progID( GetProgID(appToCheck) );

		HKEY hKey( NULL);
		HKEY hKey1(NULL);

		if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_CLASSES_ROOT, progID, 0, KEY_READ, &hKey) ){
			return eOfficeVersion_Unknown;
		}

		if(ERROR_SUCCESS != ::RegOpenKeyEx(hKey, _T("CurVer"), 0, KEY_READ, &hKey1)) {
			::RegCloseKey(hKey);
			return eOfficeVersion_Unknown;
		}
 
		// Get the Version information
		const int BUFFER_SIZE(255);
		ULONG cSize(BUFFER_SIZE);
		TCHAR szVersion[BUFFER_SIZE];
		const LONG lRet( ::RegQueryValueEx(hKey1, NULL, NULL, NULL, (LPBYTE)szVersion, &cSize) );

		// Close the registry keys
		::RegCloseKey(hKey1);
		::RegCloseKey(hKey);

		// Error while querying for value
		if(ERROR_SUCCESS != lRet){
			return eOfficeVersion_Unknown;
		}

		const CString progAndVersion(szVersion);
		// At this point szVersion contains the ProgID followed by a number. 
		// For example, Word 97 will return Word.Application.8 and Word 2000 will return Word.Application.9
		
		const int lastDot( progAndVersion.ReverseFind(_T('.')) );
		const int firstCharOfVersion( lastDot + 1); // + 1 to get rid of the dot at the front
		const CString versionString( progAndVersion.Right(progAndVersion.GetLength() - firstCharOfVersion) );

		return StringToVersion(versionString);
	}
};
