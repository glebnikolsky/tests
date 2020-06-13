/*
Module : ntserv.cpp
Purpose: Implementation for an C++ class framework which encapsulate the area of Windows services
Created: PJN / 14-07-1997
History: PJN / 17-05-1999 1. Fixed a warning when compiled with VC 6.
                          2. Fixed a bug in CNTEventLogSource::Report  
         PJN / 05-09-1999 1. Addition of more ASSERT's statements to aid in debugging.
         PJN / 02-10-1999 1. Addition of GetProfileStringArray, WriteProfileStringArray,
                             GetProfileBinary and WriteProfileBinary methods to the 
                             CNTService class.
                          2. Renamed some module names
         PJN / 10-10-1999 1. Added support for the description field which services can
                          have on Windows 2000.
                          2. Added accessor functions for the service name, friendly name
                          and the description text.
         PJN / 10-04-2000 1. Fixed a bug with the calculation of buffer sizes when compiled
                          for UNICODE in CNTEventLogSource::SetStringArrayIntoRegistry
         PJN / 21-05-2000 1. Added 2 new overridden versions of CNTEventLogSource::Report
                          2. Tweaked and set default parameters in CNTService::ReportStatusToSCM
                          following review of the book "Professional NT Services"
                          3. Disallowed accepting any controls when the service is currently 
                          processing the same control in ReportStatusToSCM
                          4. Change the ordering of parameters in ReportStatusToSCM. You should review 
                          your code to ensure it is correct if you have existing code
                          5. Class framework now by default uses a DLL of the same name for
                          the message file which the NT Event log will look to for mc string entries.
                          This means that you should review your code to ensure it is correct if you 
                          have existing code
                          6. CNTEventLogSource::Report now does not bother automatically creating
                          a SID as this should be used for services which do impersonation for clients
                          7. Changed the constructor of CNTEventLog which now takes the values it
                          needs for calling Register
                          8. CNTEventLog::Report methods are now no longer const because of change 7 above.
                          9. SCM database is now locked during installs and uninstalls of the service
                          10. Now supports all the new functionality provided for services under Windows 2000
                          while preserving binary compatability with NT 3.x and NT 4 services.
                          11. Framework now requires the presence of the January 2000 platform sdk or later. 
                          This is because of point 10 above.
                          12. Changed the prototype of the existing CNTScmService::QueryStatus function. 
                          13. Made the typedefs for the enumeration functions private to their respective
                          classes instead of polluting the global name space
         PJN / 23-05-2000 1. Added a "CNTService::WaitForStop" method
                          2. Uninstall of service now asks the service to stop and then waits for it to stop.
         PJN / 19-06-2000 1. Added an ASSERT to ReportStatusToSCM for status handle == 0
                          2. Fixed a number of other logic problems in ReportStatusToSCM
                          3. Removed an unnecessary ASSERT in CNTEventLogSource::Report
                          4. Sample service app now uses sound card default sound instead of PC speaker sound
         PJN / 20-06-2000 1. Minor change to a TRACE statement in RegisterCtrlHandler
                          2. RegisterCtrlHandler is called automatically for you now, no need to call it in
                          your derived version of ServiceMain
                          3. Added a BOOL CNTEventLogSource::Report(WORD wType, LPCTSTR lspszString) method
                          4. Fixed a bug where the sample service was not reporting its shutdown status to 
                          the SCM correctly.
         PJN / 14-12-2000 1. Fixed a bug in CNTService::Uninstall() which was causing it to fail sometimes
                          2. Made all the member variables of CNTService class public but put them down below
                          the implementation ala the way MFC classes are used declared.
                          3. Added a new overridden version of CNTEventLogSource::Report 
         PJN / 08-01-2001 1) Now includes copyright message in the source code and documentation.
                          2) CNTEventLogSource::Report now allows you to specify whether error codes should be
                          displayed in Hex or as decimal
                          3) Sample service sometimes did not display the stop event. This has been remedied by
                          reporting to the event log before we report to the SCM.
                          4) Framework has now been made independent of the need for the latest Platform SDK
                          header files. You can now compile the code in VC 5 without the Plaform SDK having to
                          be installed.
         PJN / 17-05-2001 1) Thread protected code to CNTEventLogSource::Report
         PJN / 27-05-2001 1) Added methods to framework to support the new command line options -start, -pause, 
                          -continue and -stop. Thanks to Stefan Niemeyer for this great addition.
                          2) Fixed a small issue in the testing of the CEventLogSource class in app.cpp
         PJN / 15-07-2001 1) Fixed a problem when calling CreateService with pathnames which contain embedded spaces
         PJN / 18-07-2001 1) Minor update to the sample code demonstrating the class framework.
                          2) Due to VC 6 shipping with a Winsvc.h (The main NT service header file) which includes
                          some but not all of the defines which the class requires you need to define a few preprocessor
                          define to get the code to compile cleanly for your particular setup. Use the following matrix:
                          
                                Platform SDK Installed?      NO                                                                        YES 

                          Compiler

                          Visual C++ 5.0                     "NTSERV_DO_NEW_WINSVC_DEFINES"                                            Nothing extra to define
                          Visual C++ 6.0 and later           "NTSERV_DO_NEW_WINSVC_DEFINES" & "NTSERV_EXCLUDE_VC6_WINSVC_DEFINES"      Nothing extra to define


                          The VC 5 sample app workspace included defines build configurations for the case where the Platform SDK is present and is not.
                          If you use this workspace in VC 6 or later, make sure you define the addional "NTSERV_EXCLUDE_VC6_WINSVC_DEFINES" define for the
                          Build configurations for "Missing Platform SDK". If you still cannot get the code to compile or you could not understand the 
                          explanation above, then just drop me an email.
         PJN / 09-08-2001 1) Updated SetStringArrayIntoRegistry and GetStringArrayIntoRegistry to be consistent with my other class CHKLM v1.11
         PJN / 27-11-2001 1) Fixed an issue where the class fails to read any key that is readonly. This is a problem since Win2000 defaults access to 
                          HKLM for "normal" users to readonly. Thanks to Hans-Georg Ulrich for spotting this problem
         PJN / 26-08-2002 1) Made more functions virtual to allow greater end user customisation.
                          2) All useful attributes in call to CreateService in CNTService::Install can now be modifed without having to derive a custom 
                          version of CNTService::Install in your derived class.
                          3) A boolean value is now provided to enable / disable event log integration
         PJN / 28-08-2002 1) Reviewed all TRACE statements in the code for correctness
                          2) Added a parameter to CNTService::Install to allow the time to wait for the service to stop to be specified
                          3) Added a CString parameter to the CNTService::Install and Uninstall methods to return a descriptive errors to callers.
                          4) Addition of a new boolean variable to CNTService called m_bConsole to allow service to decide whether it 
                          should use GUI or Console calls to display Install / Uninstall status messages
                          5) CNTService::ProcessShellCommand now displays error messages to the console or via a Message box when errors
                          occur in install, uninstalling, starting, stoping, resuming and pausing the service.
         PJN / 20-09-2002 1) Fixed an "issue" in calls to ReportStatusToSCM which your service should be calling in your ServiceMain. Specifically
                          calling ReportStatusToSCM(SERVICE_STOPPED) looks like it elicits the behaviour that the SCM forcefully terminates your
                          worker thread. This can cause issues when other code is executed after this call which uses something which is allocated
                          on the worker threads stack. To fix this "issue" (I could not find any documentation on this behaviour) you should remove
                          the single call to ReportStatusToSCM which uses the values "SERVICE_STOPPED" from the bottom of your service's ServiceMain
                          function. To force you to do this, I have changed the name of the function "ReportStatusToSCM" to just "ReportStatus". This
                          will mean that the compiler will force you to examine each call to this function and remove the one call as described above.
                          The class framework now looks after reporting that your service has stopped to the SCM. Thanks to "Jim" for spotting this.
         PJN / 20-10-2002 1. Fixed 3 level 4 warnings when the code is compiled using VC .NET. Thanks to Ed Eichman for spotting this problem.
         PJN / 12-03-2003 1. Fixed a problem since user code can't call ReportStatus(SERVICE_STOPPED) there is no way to report startup or shutdown 
                          errors via  dwWin32ExitCode or dwServiceSpecificExitCode. This has been resolved by adding a new protected function 
                          TerminateService to shut down the service in the event of a critical failure. This throws a private exception containing the 
                          exit codes which is caught by the framework in _ServiceMain (or the DebugService case of ProcessShellCommand). _ServiceMain 
                          then passes the codes on via _ReportStatus. Also since the dwWin32ExitCode and dwServiceSpecificExitCode arguments of 
                          ReportStatus can never be used these parameters have now been removed. Thanks to Jon Baggott for these 2 additions.
                          2. Performed since minor tidy ups in the inclusion of header files by the framework
         PJN / 08-05-2003 1) Update to the Install and Uninstall methods to return the value from GetLastError(). This is useful for client code
                          as it can be reset before the functions return.
                          2) Command line now supports a /T: param which allows you to specify a timeout to use when doing an uninstall. This timeout
                          is used to specify how long the code will wait if the service needs to be stopped.
                          3) Broke down all the classes into their own modules. This allows you to pull in only the classes you require, if you are 
                          not using the main class CNTService.
                          4) Removed all unnecessary TRACE statements throughout the framework as they sometimes reset the value as returned
                          from GetLastError.
         PJN / 17-05-2003 1) Further tweaks to _ReportStatus to avoid intermittent crashes. See item 1 on 20-09-2002 for detailed info about the issue.
                          2) Removed some unnecessary calls to locking the critical section
         PJN / 22-05-2003 1) Updated the way the sample service's message dll projects integrates the creation of the mc file. For detailed information
                          on how to integrate mc files into Visual Studio, please see the CodeProject article 
                          "http://www.codeproject.com/useritems/MCTutorial.asp". Please note that I did not follow the suggested settings as exactly 
                          outlined in this article as the message file's header file needs to be included into the nt service exe at compile time.
         PJN / 03-08-2003 1) Improved the error handling and reporting in CNTService::SetServiceToStatus. Thanks to Jon Pawley for reporting this issue.
         PJN / 15-08-2003 1) Optimized the string copying in CEventLogRecord::CEventLogRecord(const EVENTLOGRECORD* pRecord). A bug was reported in this area 
                          by Jeroen Walter but it looks like this is a documentation bug in current versions of the MSDN. This structure was defined 
                          correctly in some early versions of the MSDN.
         PJN / 08-09-2003 1) Moved all the defines which the framework requires into a separate header file. Thanks to Dima Polyakov for pointing
                          out a compilation problem which occurred when you did not have the platform SDK installed which necessitated this change.
                          2) Changed the ordering of the command line used to compile the MC file. This avoids a problem as reported by 
                          Rodrigo Oliveira Fernandez when using the framework in VC 7
                          3) Fixed a compilation error related to the use of the preprocessor define NTSERV_EXCLUDE_VC6_WINSVC_DEFINES
         PJN / 03-10-2003 1) Updated the custom build steps to compile the mc file. Now uses the VC macro "$(InputPath)" instead of 
                          "$(InputDir)\$(InputName).mc".
                          2) Removed a number of unreferenced variables level 4 warnings when compiled with VC .NET 2003.
                          3) Provision of a specific workspace (Testsrv for Visual Studio.Net.dsw) which allows the sample service to be compiled
                          cleanly out of the box with Visual Studio .NET 2000 or Visual Studio .NET 2003. The issue is that the format of various build
                          macros such as $(InputDir) has changed from VC 6. Just select File -> Open Project in VS .NET and select "Testsrv for Visual Studio.Net.dsw".
         PJN / 14-11-2003 1) The location where the message dll for the service is located can now be tweaked at runtime by filling in the
                          CNTService::m_sMessageDll variable. Thanks to Brodie Thiesfield for this update.
                          2) Reviewed all TRACE statements which use a CString parameter. Now explicitly casts the CString to a LPCTSTR. Thanks to Brodie Thiesfield
                          for this update.
                          3) Added a new CNTService::m_LoggingDetail variable. This in combination with a new method called CNTService::Win32ErrorToString
                          allows more flexible control over how errors are logging to the event log. Thanks to Brodie Thiesfield for suggesting this update.
                          4)  command line arguments other than the "-debug" argument are passed through to ServiceMain() when running in debug mode. Thanks to
                          Brodie Thiesfield for suggesting this update.
         PJN / 19-11-2003 1) Reworked the function Win32ErrorToString to include the string resource ID. Thanks to Brodie Thiesfield for suggesting this.
                          2) Fixed CNTS_MSG_SERVICE_FAIL_PAUSE, CNTS_MSG_SERVICE_FAIL_CONTINUE and CNTS_MSG_SERVICE_FAIL_STOP event log entries which were not
                          using the new Win32ErrorToString method. Thanks to Brodie Thiesfield for reporting this.
                          3) Win32ErrorToString now includes a "bEventLog" parameter. When this is TRUE the function is being called to obtain the error 
                          which gets displayed in the event log, when FALSE, it is being used to display to the end user or the TRACE window
         PJN / 23-11-2003 1) Fixed a memory leak in CNTService::Debug when the service uses TerminateService to shutdown the service prematurely.
         PJN / 26-11-2003 1) Now supports a /silent command line option. Using this setting in combination with for example /install will ensure that
                          the framework does not display any message boxes or console output when an error occurs. Thanks to Metrich Frederic for this addition.
                          2) Changed the return value from ProcessShellCommand from a BOOL to a DWORD. This value is now use to return an error code from the 
                          service exe when it exits. In combination with the /silent command line option, this allows third party programs such as installers 
                          control over the service instance via the exit code of the service. Again thanks to Metrich Frederic for this addition.
                          3) Updated the sample app provided with the framework to show how client services can return the Win32 error code as now returned
                          by ProcessShellCommand.
         PJN / 22-01-2004 1) Fixed a bug in CNTScmService::EnumDependents and CNTServiceControlManager::EnumServices where enumeration would continue even
                          when the callback functions return FALSE. Thanks to Edward Livingston for spotting this.
         PJN / 18-03-2004 1. Fixed a bug in the CNTScmService::WaitForServiceStatus function which caused it to return TRUE even if the service failed to 
                          return the desired status (if pending status isn't the expected pending staus. 
                          2. Also ensured that CNTScmService::WaitForServiceStatus does not wait past the requested timeout period. Thanks to Brodie Thiesfield 
                          for these bug reports and the fixes.
         PJN / 09-04-2004 1. Further work to CNTScmService::WaitForServiceStatus. It now ignores pending states. In addition it ignores wait hints since we are
                          calling QueryServiceStatus as opposed to calling the ControlService function with the SERVICE_CONTROL_INTERROGATE control code. It also
                          allows the polling interval to be specified by a 3rd optional parameter. Thanks to Edward Livingston and "yong" for reporting this 
                          problem. Please note that since the third parameter to this function means something different that previously, you should review all
                          of your calls to WaitForServiceStatus to ensure it continues to operate correctly.
         PJN / 12-04-2004 1. Removed an unused variable in CNTScmService::WaitForServiceStatus. Thanks to Edward Livingston for reporting this issue.
         PJN / 05-05-2004 1. Fixed some warnings in CNTService::GetDependencies and CNTEventLogSource::SetStringArrayIntoRegistry when the code is compiled using 
                          the "Force conformance in For loop scope" VC compiler option (/Zc:forScope). Thanks to Alexey Kuznetsov for reporting this problem.
         PJN / 20-06-2004 1. Fixed a bug in CEventLogRecord::CEventLogRecord(const EVENTLOGRECORD*..) to do
                          with the handling of inter string NULLs at the end of the record. Thanks to "yong" for reporting this problem.
                          2. Optimized the code in CEventLogRecord::CEventLogRecord(const EVENTLOGRECORD*..).
         PJN / 21-06-2004 1. Provided a new virtual implementation of CNTService::_ServiceMain. To achieve this the previous static _ServiceMain is now called 
                          "_SDKServiceMain". This allows your service code to completely customize the startup of your service. For example if you service does 
                          not require an active worker thread to do any work (e.g. it only ever responds to client requests thro the SCM using 
                          OnUserDefinedRequest), then you could implement your own version of _ServiceMain which does not call _ReportStatus(SERVICE_STOPPED... 
                          This would allow you to use RegisterWaitForSingleObject to implement your service clean up code. For more information on this type 
                          of service please consult the MSDN documentation on "RegisterWaitForSingleObject" and "ServiceMain". Thanks to Haug Leuschner for 
                          reporting this.
         PJN / 24-06-2004 1. Reissue of the last update as I forgot to declare _ServiceMain virtual, Doh!
                          2. Also made the class CNTServiceTerminateException visible in the global namespace. This is required if you implement your own
                          _ServiceMain.
         PJN / 15-10-2004 1. Framework now automatically installs a console ctrl handler when run in debug mode. This means that the framework will automatically
                          call your service's OnStop method when running in debug mode if your service is built as a console app and you hit Ctrl+C or Ctrl+Break or
                          hit the close button in the console window. This helps you to verify the shutdown code of your service without having to run the code as 
                          a service. Because the console ctrl handler is executed in a separate thread to ServiceMain, this simulation mode calls your OnStop method 
                          in a separate thread just like the SCM does. This proves very helpful when you need to debug shutdown problems in your service which are 
                          thread related.
                          2. Added a few checks to various header files to warn if various header files have not already been included e.g. Afxmt.h. This helps avoid
                          simple compilation problems when the framework is added to a new project initially.
                          3. Sample app now uses the InterlockedExchange functions instead of a critical section to serialize access to variables used across multiple
                          threads. In addition the "m_bPaused" variable of the sample app is now also marked as volatile
                          4. Fixed a double declaration of "dwTypesSupported" in CNTEventLogSource::Install. In the process this fixes a level 4 warning which the 
                          code generated.
         PJN / 17-10-2004 1. Fixed a number of level 4 warnings in the framework when "Detect 64 bit portability issues" is enabled in Visual Studio .NET
         PJN / 11-11-2004 1. Fixed a futher 64 bit issue in CNTEventLogSource::SetStringArrayIntoRegistry when compiled in Visual Studio .NET. 
                          2. Addition of a CNTSERVICE_EXT_CLASS preprocessor macro which allows the classes to be easily added to an extension dll
         PJN / 11-02-2005 1. Addition of a virtual GetEventLogSourceName method which allows the name of the event log source to be customized at runtime. Thanks to 
                          Bara Cek for requesting this addition.
                          2. Updated the copyright details in all modules.
                          3. Addition of true Visual Studio .NET 2003 files for the sample service (Testsrv.sln and Testsrv.vcproj)
         PJN / 26-03-2005 1. Addition of a m_bProfileWriteFlush variable to the CNTService class which dictates if the registry settings should be committed 
                          immediately as opposed to relying on the lazy writer.
         PJN / 21-04-2005 1. CNTService::GetProfileInt and GetProfileString functions now includes an optional DWORD output parameter which when provided allows the value from 
                          GetLastError to be returned to client code. This allows code to distinguish between a default value which is returned because it is actually 
                          stored as opposed to returning a default value because some of the registry API's failed. Thanks to Tony Ronan for reporting this issue.
         PJN / 31-07-2005 1. Provided an overridden version of CNTEventLogSource::Report which has 3 string parameters
                          2. CNTEventLogSource::GetStringArrayFromRegistry and SetStringArrayIntoRegistry methods are now public as they are helpful to have available 
                          externally
         PJN / 14-08-2005 1. Fixed an issue where the SID and binary values was not being set correctly in the overloaded CEventLogRecord constructor which takes 
                          a EVENTLOGRECORD pointer. Thanks to Brian Lee for reporting this issue. Also the arrays which this information is copied into is now preallocated 
                          to improve performance.
         PJN / 11-01-2006 1. The command line options which the service uses to start with can now be specified via a new /SCL command line option. This is achieved by 
                          supporting a new command line option e.g. TestSrv.exe /install "/SCL:/Param1 /SomeOtherParam:XYZ". This additional flexibility is controlled by 
                          BOOL CNTService::m_bAllowCommandLine which by default is FALSE.
                          2. Updated copyright details for all modules
                          3. The service display name string which the service uses for installing can now be specified via a new /SDN command line option. This is achieved 
                          by supporting a new command line option e.g. TestSrv.exe /install "/SDN:PJ's Second Test Service". This additional flexibility is controlled by 
                          BOOL CNTService::m_bAllowDisplayNameChange which by default is FALSE.
                          4. The description string which the service uses for installing can now be specified via a new /SD command line option. This is achieved by 
                          supporting a new command line option e.g. TestSrv.exe /install "/SD:Some description". This additional flexibility is controlled by BOOL 
                          CNTService::m_bAllowDescriptionChange which by default is FALSE.
                          5. The service name string which the service uses for installing can now be specified via a new /SN command line option. This is achieved by 
                          supporting a new command line option e.g. TestSrv.exe /install "/SN:PJSERVICE2". This additional flexibility is controlled by 
                          CNTService::m_bAllowNameChange.
                          6. Removed code which writes an event log entry when the service is being uninstalling. There is not much sense in logging an entry that you have
                          uninstalled the service, when you are also uninstalling the event log source.
                          7. Removed the ANSI mc file. Instead now the mc file included in the zip file is the Unicode mc file as provided by Brodie Thiesfield (see the 
                          history list for v1.35).
                          8. Removal of CNTService::GetEventLogSourceName method. Instead now you can call m_EventLogSource.SetSourceName() to customize this value.
                          9. Provision of CNTService::SetUseConsole and CNTService::GetUseConsole methods.
                          10. The default setting for CNTService::m_bUseConsole is now set depending on whether the standard _CONSOLE preprocessor define is declared
                          11. Removed the unnecessary critical section lock in the CNTService constructor
                          12. Provision of CNTService::SetControlsAccepted and CNTService::GetControlsAccepted methods.
                          13. Provision of a default CNTService constructor
                          14. Provision of CNTService::SetServiceName, CNTService::SetInstanceServiceName, CNTService::SetMasterServiceName, CNTService::GetInstanceServiceName, 
                          CNTService::GetMasterServiceName, CNTService::SetDisplayName, CNTService::SetMasterDisplayName, CNTService::SetInstanceDisplayName, 
                          CNTService::GetMasterDisplayName, CNTService::GetInstanceDisplayName, CNTService::SetDescription, CNTService::SetMasterDescription, 
                          CNTService::SetInstanceDescription methods, CNTService::GetMasterDescription and CNTService::GetInstanceDescription. Most of the work for this release 
                          of the framework is to support a new concept of "instances" in CNTService.  Prior to this version of the CNTService framework, you could only ever 
                          install a single copy of any one service on the one machine. Now with the introduction of the /SDN, /SD and /SN command line options which are used 
                          during install and uninstall you can register the service multiple times, using different service names, display names and descriptions. The original 
                          values you construct your CNTService instance are called the master values and are unmodified during the lifetime of all instances of your services.
                          The "instance" values are modifiable (if you allow them to be so via CNTService::m_bAllowNameChange, CNTService::m_bAllowDisplayNameChange and 
                          CNTService::m_bAllowDescriptionChange) at runtime. When these values are provided on the command line during registration, the command line which 
                          the service is setup to use upon launch by the SCM is customized so that these same command line options are passed to it. This allows the same 
                          instance values to be available to the service when it is running as a service. Without this neat trick, there would be no easy way for the service
                          to detect under which instance details it should run.
                          15. Addition of generic versions of CNTService::WriteProfileString, CNTService::WriteProfileInt, CNTService::WriteProfileStringArray, 
                          CNTService::WriteProfileBinary, CNTService::GetProfileString, CNTService::GetProfileInt, CNTService::GetProfileStringArray and 
                          CNTService::GetProfileBinary. These implementations allow the service name to be specified as well as the Flushing setting which is hidden by the other
                          implementations. The existing implementations of these functions now simply call these new implementations. Providing these implementations now allows
                          client code to read and write the settings for any service (including other instances of the same service) without the need to create a new
                          CNTService instance.
                          16. Addition of a CNTService::EnumerateInstances method which allows you to enumerate all the instances of a service which have been installed
                          using the new master / instance additions to the framework in this version. This could prove useful where you are writing configuration type applets
                          which configure the settings for all the instances of your installed service.
                          17. As a security precaution once the username and password strings have been used for installing, they are overwritten in memory.
         PJN / 27-04-2006 1. Code now uses SecureZeroMemory when wiping out the username and password strings. This should further improve the security of these values
                          when they are manipulated by the class framework.
         PJN / 31-05-2006.1. Updated the code to work correctly in VC 6 without the Platform SDK being installed. I personally have the Feb 2003 Platform SDK installed with 
                          VC 6 and this is how most of my code is primarily tested. It seems some of my recent updates to CNTService have broken compatibility with VC 6 on its 
                          own. Also renamed the define "NTSERV_DO_NEW_WINSVC_DEFINES" which is required when you are compiling the class framework on Visual C++ 6 without the Platform 
                          SDK to be "CNTSERVICE_MISSING_PLATFORMSDK". Please note that I would advise all developers to install the Platform SDK (February 2003 was the latest
                          version which was compatible with VC 6), as I will probably drop support for VC 6 without the Platform SDK in the future. It is becoming too much work
                          to support the older Header files which were shipped with VC 6 and most of my other open source code is moving towards requiring the Platform SDK.
                          Thanks to Matthias Miller for reporting this issue. The "NTSERV_EXCLUDE_VC6_WINSVC_DEFINES" define is now defunct and no longer used by the CNTService code.
         PJN / 03-06-2006 1. Fixed a minor typo in CNTService::SetServiceToStatus. Thanks to Matthias Miller for reporting this issue.
         PJN / 07-06-2006 1. Removed three unreferrenced variables from CNTService::ProcessShellCommand. Thanks to Frederic Metrich for reporting this issue.
                          2. Minor update to the declaration of some functions in CNTService to using "WINAPI" in their function implementations. Thanks to Frederic Metrich for 
                          reporting this issue.
                          3. Updated the code in CNTService::Debug to exclude the "/debug", "-app", "/app", "-application" and "/application" command line options in addition to the 
                          "-debug" value. Again thanks to Frederic Metrich for reporting this issue.
                          4. Updated the sample service provided with the CNTService download to build as a console app instead of a Windows app. This allows you to check out the
                          console handler support which CNTService includes.
         PJN / 09-06-2006 1. Made CNTService::Initialize non virtual. This change was made because it is called by the CNTService constructor and you cannot call virtual functions
                          from a C++ class constructor (or destructor). For more info on this issue please see the online article at http://www.artima.com/cppsource/nevercall.html 
                          from the book "Effective C++" by Scott Meyers. Thanks to Frederic Metrich for spotting this issue. Because of this change you should examine your own code
                          which uses CNTService::Initialize to make sure this change has no side effects on your code. 
         PJN / 13-06-2006 1. To allow customization in client projects, the resource id and message id values can now be customized through two new virtual functions, namely 
                          CNTService::MapResourceID and CNTService::MapMessageID. Thanks to Jan Bartels for this update.
                          2. CNTService::SetServiceToStatus now exposes the interval to wait as a parameter as well as the polling interval as a parameter. In addition the command line
                          timeout interval is now passed to all calls to SetServiceToStatus in CNTService::ProcessShellCommand. The additional command line operations which are passed
                          the timeout value include starting, stopping, pausing and continuing the service, in addition to uninstalling the service which when it was introduced
                          in v1.27 was the only operation which got the timeout value.
         PJN / 24-06-2006 1. Added a missing MapResourceID call in CNTService::Win32ErrorToString. 
                          2. Updated the sample dsp and vcproj files to produce Unicode mc resources instead of ASCII.
                          3. Framework now accepts the command line /console to mean the same as /debug i.e. run the program as a normal (console) application without interacting 
                          with the Service Control Manager.
                          4. Updated code to compile cleanly using VC 2005.
                          5. Included info in ntservEventLogSource.cpp on how to find further info on the various registry values which are required to create user defined event logs.
         PJN / 25-06-2006 1. Combined the functionality of the _NTSERV_DATA class into the main CNTService class.
                          2. Code now uses newer C++ style casts instead of C style casts.
                          3. The class framework now requires the Platform SDK if compiled using VC 6.
                          4. Fixed code which was not securely resetting the command line username and password strings after they were used
         PJN / 02-07-2006 1. All calls to m_EventLogSource.Report are now protected with "if (m_bRegisterEventLogSource)" check
         PJN / 27-07-2006 1. The static versions of the profile functions in CNTService have now been renamed to include the word "Service" in their method names. This fixes a
                          compiler error when the code is used in VC 2005.
         PJN / 01-08-2006 1. Fix for two buffer size calculations using the new _tcscpy_s functions, one in CNTEventLogSource::SetStringArrayIntoRegistry and the other in
                          CNTService::GetDependencies. Thanks to DmityShm on the codeproject forums for reporting this issue.
         PJN / 21-09-2006 1. Minor update to use %X instead of %x in various TRACE statements.
         PJN / 02-02-2007 1. Updated copyright details.
                          2. Reworked CNTService::SetServiceToStatus to allow the last error value to be reported back to client code. Thanks to Matthias Miller for reporting this
                          issue.
                          3. Optimized calls to Win32ErrorToString.
         PJN / 22-03-2007 1. CNTService::SetServiceToStatus and CNTService::Uninstall no longer specifies that it requires the SC_MANAGER_LOCK access right to connect to the SCM. 
                          This change helps to allow services to be started and stopped from Power User accounts. Thanks to Matthias Miller for reporting this issue.
                          2. CNTService::SetServiceToStatus no longer specifies the STANDARD_RIGHTS_REQUIRED access right when opening a service to control. This change helps to 
                          allow services to be started and stopped from Power User accounts. Thanks to Matthias Miller for reporting this issue.
         PJN / 13-11-2007 1. Minor update to display HRESULT's correctly.
         PJN / 12-01-2008 1. Updated copyright details.
                          2. Provided a link to my blog in the html documentation with details on compiling with VC 6.
                          3. CNTService::Win32ErrorToString method has been reworked to now uses the FORMAT_MESSAGE_IGNORE_INSERTS flag. For more information please see Raymond 
                          Chen's blog at http://blogs.msdn.com/oldnewthing/archive/2007/11/28/6564257.aspx. Thanks to Alexey Kuznetsov for reporting this issue.
                          4. Updated the sample app to clean compile on VC 2008.
                          5. SecureEmptyString method is now defined as "FORCEINLINE".
         PJN / 01-06-2008 1. Code now compiles cleanly using Code Analysis (/analyze)
                          2. The code has now been updated to support VC 2005 or later only.  
         PJN / 01-08-2010 1. Updated copyright details.
                          2. Updated the project settings to more modern default values.
                          3. Fixed a trailing "," in the definition of the "ELErrorLoggingDetail" enum
                          4. Updated the sample app to compile cleanly on VC 2010
                          5. Updated the test code in App.cpp to only use the necessary rights when connecting to the SCM or opening a specific service. This avoids Access denied
                          errors on Windows Vista or later when UAC is enabled. Thanks to Jan Bartels for reporting this issue.
                          6. Added support for the following SERVICE_CONTROL_* defines: SERVICE_CONTROL_SESSIONCHANGE, SERVICE_CONTROL_PRESHUTDOWN, SERVICE_CONTROL_TIMECHANGE & 
                          SERVICE_CONTROL_TRIGGEREVENT. This control values represent new functionality for services on Windows Vista and Windows 7
                          7. Added a comment to a #include to tell users to recompile TestSrvMsg if they get a compile error for "ntserv_msg.h". Thanks to Paul Pignon for reporting
                          this issue.
                          8. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          9. Fixed a minor issue in CNTService::Win32ErrorToString with the use of printf format identifiers
                          10. Reworked all the internal heap memory management code to use ATL::CHeapPtr
                          11. Reworked all the internal registry management code to use ATL::CRegKey
                          12. The CNTService::m_bRegisterEventLogSource value has been renamed to m_bEventLogSource. This is a breaking change and may require your service code
                          to be updated if you reference this value from your code.
                          13. CNTService::Install has been refactored. Internally now this function calls InstallEventLogSource and InstallServiceConfiguration
                          14. Carried out a thorough code review to ensure all functionality is correct and functional on the Windows 7 and later 
         PJN / 03-09-2010 1. Fixed a bug in CNTService::GetServiceProfileString where it would incorrectly return FALSE.
                          2. Fixed a bug in CNTService::GetServiceProfileInt where it would incorrectly return FALSE.
         PJN / 10-11-2012 1. Updated copyright details.
                          2. Updated code to clean compile on VC 2012
         PJN / 01-01-2014 1. Updated copyright details.
                          2. Updated code to clean compile on VC 2013
                          3. Reworked the classes to optionally compile without MFC. By default the classes now use STL for strings and arrays but if you define 
                          CNTSERVICE_MFC_EXTENSIONS, the classes will revert back to using the MFC collection classes.
                          4. Updated the codebase to support all Service features upto Windows 8.1. New features supported includes SERVICE_CONFIG_LAUCH_PROTECTED support available 
                          via CNTScmService::QueryLaunchProtected and CNTScmService::ChangeLaunchProtected and QueryServiceDynamicInformation support available via 
                          CNTService::QueryServiceDynamicInformation.
         PJN / 18-12-2015 1. Updated copyright details.
                          2. Verified code compiles cleanly on VC 2015.
                          3. Updated CNTService::ProcessShellCommand to use string::find instead of std::find.
                          4. Reviewed all the changes in the Windows 10 SDK to do with Services to make sure the classes support new functionality and compile correctly when used 
                          with this updated SDK.
                          5. Added SAL annotations to all the code
         PJN / 19-04-2016 1. Updated copyright details.
         PJN / 01-06-2016 1. Fixed a bug in the non MFC code path in CNTService::GetServiceProfileString where the code left a NULL byte in the std::string return value prior to 
                          returning from the method. Therefore the returned string object contained a binary zero and cannot be used to concatenate other strings. Thanks to Martin 
                          Gamperl for spotting this bug. 
                          2. Some minor tidy-ups to the code style throughout the ntserv.cpp module.
         PJN / 15-11-2016 1. Corrected initialization of a local variable in the method CNTService::WriteServiceProfileString.
         PJN / 09-04-2017 1. Updated copyright details.
                          2. Update the code to compile cleanly on VC 2017
                          3. Replaced NULL throughout the codebase with nullptr. This means that the minimum requirement for the framework is now VC 2010.
                          4. Removed all code which used function pointers for API calls prior to Windows XP / 2003
         PJN / 16-04-2017 1. Added support for SERVICE_CONTROL_LOWRESOURCES & SERVICE_CONTROL_SYSTEMLOWRESOURCES from the latest Windows 10 SDK
         PJN / 16-09-2017 1. Replaced CString::operator LPC*STR() calls throughout the codebase with CString::GetString calls
         PJN / 26-05-2018 1. Updated copyright details.
                          2. Fixed a number of C++ core guidelines compiler warnings. These changes mean that the code
                          will now only compile on VC 2017 or later.
                          3. Removed all code which supports now defunct CNTSERVICE_MFC_EXTENSIONS code paths
                          4. CNTService::Debug method is now passed unmodified command line parameters
         PJN / 02-06-2018 1. Supressed final C++ core guidelines compiler warnings from the code.
                          2. Tidied up various project settings in the sample solution and project files
         PJN / 29-08-2018 1. Fixed a number of compiler warnings when using VS 2017 15.8.1
                          2. Updated the codebase to support GetServiceRegistryStateKey and GetServiceDirectory APIs
         PJN / 01-09-2018 1. Fixed up code which supports GetServiceRegistryStateKey and GetServiceDirectory APIs to reference correct Windows DLL.

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
#include "resource.h"

#ifndef _WINSVC_
#pragma message("To avoid this message, please put winsvc.h in your pre compiled header (normally stdafx.h)")
#include <winsvc.h>
#endif //#ifndef _WINSVC_

#ifndef _NTSECAPI_
#pragma message("To avoid this message, please put ntsecapi.h in your pre compiled header (normally stdafx.h)")
#include <ntsecapi.h>
#endif //#ifndef _NTSECAPI_

#ifndef _ALGORITHM_
#pragma message("To avoid this message, please put algorithm in your pre compiled header (normally stdafx.h)")
#include <algorithm>
#endif //#ifndef _ALGORITHM_

#include "ntserv.h"
#include "ntserv_msg.h" //If you get an error about this missing header file, then please make sure you build the TestSrvMsg project first


///////////////////////////////// Macros /Defines /////////////////////////////

CNTService* CNTService::sm_pService = nullptr;


//////////////////////////////// Implementation ///////////////////////////////

CNTServiceTerminateException::CNTServiceTerminateException(_In_ DWORD dwWin32ExitCode, _In_ DWORD dwServiceSpecificExitCode)  noexcept : m_dwWin32ExitCode(dwWin32ExitCode),
                                                                                                                                         m_dwServiceSpecificExitCode(dwServiceSpecificExitCode) 
{
}

CNTService::CNTService() noexcept : m_hStatus(nullptr),
                                    m_dwCheckPoint(0),
                                    m_dwWaitHint(0),
                                    m_dwRequestedControl(0),
                                    m_dwCurrentState(SERVICE_STOPPED),
                                    m_dwControlsAccepted(SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP),
                                    m_dwServiceType(SERVICE_WIN32_OWN_PROCESS),
                                    m_dwStartType(SERVICE_DEMAND_START),
                                    m_dwErrorControl(SERVICE_ERROR_IGNORE),
                                    m_bEventLogSource(TRUE),
                                  #ifdef _CONSOLE
                                    m_bUseConsole(TRUE),
                                  #else
                                    m_bUseConsole(FALSE),
                                  #endif //#ifdef _CONSOLE
                                    m_UILoggingDetail(UIErrorLoggingDetail::StringAndErrorCodeAndErrorDescription),
                                    m_ELLoggingDetail(ELErrorLoggingDetail::ErrorCodeAndErrorDescription),
                                    m_bProfileWriteFlush(TRUE), //Default to the slower but safer form of registry writing
                                    m_bAllowCommandLine(FALSE),
                                    m_bAllowNameChange(FALSE),
                                    m_bAllowDescriptionChange(FALSE),
                                    m_bAllowDisplayNameChange(FALSE),
                                    m_bDelayedAutoStart(FALSE),
                                    m_dwServiceSidType(SERVICE_SID_TYPE_NONE), //Use the old style sid type by default
                                    m_dwPreshutdownTimeout(0),
                                    m_dwEventCategoryCount(0),
                                    m_dwEventTypesSupported(EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE),
                                    m_dwLaunchProtected(SERVICE_LAUNCH_PROTECTED_NONE),
                                    m_pfnQueryServiceDynamicInformation(nullptr),
                                    m_pfnGetServiceRegistryStateKey(nullptr),
                                    m_pfnGetServiceDirectory(nullptr)
{
  HMODULE hAdvapi32 = GetModuleHandle(_T("ADVAPI32.DLL"));
  if (hAdvapi32 != nullptr)
#pragma warning(suppress: 26490)
    m_pfnQueryServiceDynamicInformation = reinterpret_cast<LPQUERYSERVICEDYNAMICINFORMATION>(GetProcAddress(hAdvapi32, "QueryServiceDynamicInformation"));

  HMODULE hSecHost = GetModuleHandle(_T("SECHOST.DLL"));
  if (hSecHost != nullptr)
  {
#pragma warning(suppress: 26490)
    m_pfnGetServiceRegistryStateKey = reinterpret_cast<LPGETSERVICEREGISTRYSTATEKEY>(GetProcAddress(hSecHost, "GetServiceRegistryStateKey"));
#pragma warning(suppress: 26490)
    m_pfnGetServiceDirectory = reinterpret_cast<LPGETSERVICEDIRECTORY>(GetProcAddress(hSecHost, "GetServiceDirectory"));
  }

  //Copy the address of the current object so we can access it from the static member callback functions.
  //WARNING: This limits the application to only one CNTService object. i.e. the CNTService class framework only supports 
  //1 service per process i.e. you cannot use the flag SERVICE_WIN32_SHARE_PROCESS
  sm_pService = this; //hive away the "this" pointer;
}

CNTService::CNTService(_In_z_ LPCTSTR pszServiceName, _In_z_ LPCTSTR pszDisplayName, _In_ DWORD dwControlsAccepted, _In_opt_z_ LPCTSTR pszDescription) : CNTService()
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszServiceName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszDisplayName != nullptr);

  m_dwControlsAccepted = dwControlsAccepted;
  SetServiceName(pszServiceName);
  SetDisplayName(pszDisplayName);
  SetDescription(pszDescription);

  //By default use a event log source name the same as the display name. You can of course
  //customize this by calling m_EventLogSource.SetSourceName in your derived class at the 
  //appropriate time
  m_EventLogSource.SetSourceName(pszDisplayName);
}

void CNTService::SetServiceName(_In_opt_z_ LPCTSTR pszServiceName)
{
  //Change both the master service name and the active/current instance service name
  if (pszServiceName != nullptr)
    m_sServiceName = pszServiceName;
  else
    m_sServiceName = _T("");
  SetMasterServiceName(pszServiceName);
}

void CNTService::SetInstanceServiceName(_In_opt_z_ LPCTSTR pszServiceName)
{ 
  if (pszServiceName != nullptr)
    m_sServiceName = pszServiceName;
  else
    m_sServiceName = _T("");
}

void CNTService::SetMasterServiceName(_In_opt_z_ LPCTSTR pszServiceName)
{
  if (pszServiceName != nullptr)
    m_sMasterServiceName = pszServiceName;
  else
    m_sMasterServiceName = _T("");
}

void CNTService::SetDisplayName(_In_opt_z_ LPCTSTR pszDisplayName)
{
  //Change both the master service name and the active/current instance display name
  SetInstanceDisplayName(pszDisplayName);
  SetMasterDisplayName(pszDisplayName);
}

void CNTService::SetInstanceDisplayName(_In_opt_z_ LPCTSTR pszDisplayName) 
{
  if (pszDisplayName != nullptr)
    m_sDisplayName = pszDisplayName;
  else
    m_sDisplayName = _T("");
}

void CNTService::SetMasterDisplayName(_In_opt_z_ LPCTSTR pszDisplayName) 
{
  if (pszDisplayName != nullptr)
    m_sMasterDisplayName = pszDisplayName;
  else
    m_sMasterDisplayName = _T("");
}

void CNTService::SetDescription(_In_opt_z_ LPCTSTR pszDescription)
{
  //Change both the master description and the active/current instance description
  SetInstanceDescription(pszDescription);
  SetMasterDescription(pszDescription);
}

void CNTService::SetInstanceDescription(_In_opt_z_ LPCTSTR pszDescription)
{
  if (pszDescription != nullptr)
    m_sDescription = pszDescription;
  else
    m_sDescription = _T("");
}

void CNTService::SetMasterDescription(_In_opt_z_ LPCTSTR pszDescription) 
{
  if (pszDescription != nullptr)
    m_sMasterDescription = pszDescription;
  else
    m_sMasterDescription = _T("");
}

_Return_type_success_(return != 0) BOOL CNTService::_ReportStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwCheckPoint, _In_ DWORD dwWaitHint, _In_ DWORD dwControlsAccepted,
                                                                  _In_ DWORD dwWin32ExitCode, _In_ DWORD dwServiceSpecificExitCode)
{
  //synchronise access to the variables
  ATL::CComCritSecLock<ATL::CComAutoCriticalSection> sl(m_CritSect, true); 

  m_dwCurrentState = dwCurrentState;
  SERVICE_STATUS ServiceStatus;
  ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

  //The only stat that can set exit codes is STOPPED
  //Fix if necessary, just in case not properly set
  if (dwCurrentState != SERVICE_STOPPED)
  {
    dwWin32ExitCode = S_OK;
    dwServiceSpecificExitCode = 0;
  }

  //Only pending states can set checkpoints and wait hints
  //and pending states *must* set wait hints
  if (dwCurrentState == SERVICE_STOPPED || dwCurrentState == SERVICE_RUNNING || dwCurrentState == SERVICE_PAUSED)
  {
    //Requires hint and checkpoint == 0
    dwWaitHint = 0;
    dwCheckPoint = 0;
  }
  else
  {
    //You need to set wait hints and checkpoints
    if (dwWaitHint == STATE_NO_CHANGE)
#pragma warning(suppress: 26477)
      ATLASSERT(m_dwWaitHint);
    else
#pragma warning(suppress: 26477)
      ATLASSERT(dwWaitHint != 0);
    if (dwCheckPoint == STATE_NO_CHANGE)
#pragma warning(suppress: 26477)
      ATLASSERT(m_dwCheckPoint);
    else
#pragma warning(suppress: 26477)
      ATLASSERT(dwCheckPoint != 0);
  }

  if (dwCheckPoint != STATE_NO_CHANGE)
    m_dwCheckPoint = dwCheckPoint;
  if (dwWaitHint != STATE_NO_CHANGE)
    m_dwWaitHint = dwWaitHint;
  if (dwControlsAccepted != STATE_NO_CHANGE)
    m_dwControlsAccepted = dwControlsAccepted;

  //Disable control requests while the service is in a pending state
  if (dwCurrentState == SERVICE_START_PENDING || dwCurrentState == SERVICE_STOP_PENDING || 
      dwCurrentState == SERVICE_PAUSE_PENDING || dwCurrentState == SERVICE_CONTINUE_PENDING)
    ServiceStatus.dwControlsAccepted = 0;    
  else
    ServiceStatus.dwControlsAccepted = m_dwControlsAccepted;

  ServiceStatus.dwCurrentState = dwCurrentState;
  ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
  ServiceStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
  ServiceStatus.dwCheckPoint = m_dwCheckPoint;
  ServiceStatus.dwWaitHint = m_dwWaitHint;

  //Release the critical section lock now that we are finished tweaking the member variables
  sl.Unlock();

  //Only report to the SCM if we have a SCM status handle
  BOOL bSuccess = FALSE;
  if (m_hStatus != nullptr)
    bSuccess = SetServiceStatus(m_hStatus, &ServiceStatus);

  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTService::ReportStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwCheckPoint, _In_ DWORD dwWaitHint, _In_ DWORD dwControlsAccepted)
{
#pragma warning(suppress: 26477)
  ATLASSERT(dwCurrentState != SERVICE_STOPPED); //You should never call ReportStatus (previously ReportStatusToSCM) with the parameter SERVICE_STOPPED

  return _ReportStatus(dwCurrentState, dwCheckPoint, dwWaitHint, dwControlsAccepted);
}

_Return_type_success_(return != 0) BOOL CNTService::ReportStatus()
{
  return ReportStatus(m_dwCurrentState);
}

void CNTService::OnStop()
{
  //Derived classes are required to implement
  //their own code to stop a service, all we do is
  //report that we were succesfully stopped

  //Add an Event log entry to say the service was stopped
  if (m_bEventLogSource)
    m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MapMessageID(CNTS_MSG_SERVICE_STOPPED), m_sDisplayName.c_str());

#pragma warning(suppress: 26477)
  ATLASSERT(FALSE);
}

void CNTService::OnPause()
{
  //Add an Event log entry to say the service was stopped
  if (m_bEventLogSource)
    m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MapMessageID(CNTS_MSG_SERVICE_PAUSED), m_sDisplayName.c_str());

  //Derived classes are required to implement their own code to pause a service
#pragma warning(suppress: 26477)
  ATLASSERT(FALSE);
}

void CNTService::OnContinue()
{
  //Add an Event log entry to say the service was stopped
  if (m_bEventLogSource)
    m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MapMessageID(CNTS_MSG_SERVICE_CONTINUED), m_sDisplayName.c_str());

  //Derived classes are required to implement their own code to continue a service
#pragma warning(suppress: 26477)
  ATLASSERT(FALSE);
}

void CNTService::OnInterrogate()
{
  //Default implementation returns the current status
  //as stored in m_ServiceStatus
  ReportStatus();
}

void CNTService::OnShutdown()
{
  //Add an Event log entry to say the service was stopped
  if (m_bEventLogSource)
    m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MapMessageID(CNTS_MSG_SERVICE_SHUTDOWN), m_sDisplayName.c_str());

  //Derived classes are required to implement their own code to shutdown a service
#pragma warning(suppress: 26477)
  ATLASSERT(FALSE);
}

#pragma warning(suppress: 26440)
void CNTService::OnUserDefinedRequest(_In_ DWORD /*dwControl*/)
{
  ATLTRACE(_T("CNTService::OnUserDefinedRequest was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
void CNTService::OnNetBindAdd()
{
  ATLTRACE(_T("CNTService::OnNetBindAdd was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
void CNTService::OnNetBindRemove()
{
  ATLTRACE(_T("CNTService::OnNetBindRemove was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
void CNTService::OnNetBindEnable()
{
  ATLTRACE(_T("CNTService::OnNetBindEnable was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
void CNTService::OnNetBindDisable()
{
  ATLTRACE(_T("CNTService::OnNetBindDisable was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
void CNTService::OnParamChange()
{
  ATLTRACE(_T("CNTService::OnParamChange was called\n"));

  //Default implementation is do nothing
}

#pragma warning(suppress: 26440)
#ifdef _DEBUG
DWORD CNTService::OnHardwareProfileChange(_In_ DWORD dwEventType, _In_opt_ LPVOID /*lpEventData*/)
#else
DWORD CNTService::OnHardwareProfileChange(_In_ DWORD /*dwEventType*/, _In_opt_ LPVOID /*lpEventData*/)
#endif //#ifdef _DEBUG
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnHardwareProfileChange was called, EventType:%08X\n"), dwEventType);
#endif //#ifdef _DEBUG

  //Default implementation just returns NO_ERROR which grants the request
  return NO_ERROR;
}

#pragma warning(suppress: 26440)
#ifdef _DEBUG
DWORD CNTService::OnPowerEvent(_In_ DWORD dwEventType, _In_opt_ LPVOID /*lpEventData*/)
#else
DWORD CNTService::OnPowerEvent(_In_ DWORD /*dwEventType*/, _In_opt_ LPVOID /*lpEventData*/)
#endif //#ifdef _DEBUG
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnPowerEvent was called, EventType:%08X\n"), dwEventType);
#endif //#ifdef _DEBUG

  //Default implementation just returns NO_ERROR which grants the request
  return NO_ERROR;
}

#pragma warning(suppress: 26440)
#ifdef _DEBUG
DWORD CNTService::OnDeviceEvent(_In_ DWORD dwEventType, _In_opt_ LPVOID /*lpEventData*/)
#else
DWORD CNTService::OnDeviceEvent(_In_ DWORD /*dwEventType*/, _In_opt_ LPVOID /*lpEventData*/)
#endif //#ifdef _DEBUG
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnDeviceEvent was called, EventType:%08X\n"), dwEventType);
#endif //#ifdef _DEBUG

  //Default implementation just returns NO_ERROR which grants the request
  return NO_ERROR;
}

#pragma warning(suppress: 26440)
#ifdef _DEBUG
void CNTService::OnSessionChange(_In_ DWORD dwEventType, _In_opt_ WTSSESSION_NOTIFICATION* /*lpEventData*/)
#else
void CNTService::OnSessionChange(_In_ DWORD /*dwEventType*/, _In_opt_ WTSSESSION_NOTIFICATION* /*lpEventData*/)
#endif //#ifdef _DEBUG
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnSessionChange was called, EventType:%08X\n"), dwEventType);
#endif //#ifdef _DEBUG
}

#pragma warning(suppress: 26440)
void CNTService::OnPreShutdown()
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnPreShutdown was called\n"));
#endif //#ifdef _DEBUG
}

#pragma warning(suppress: 26440)
void CNTService::OnTimeChange(_In_opt_ SERVICE_TIMECHANGE_INFO* /*lpEventData*/)
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnTimeChange was called\n"));
#endif //#ifdef _DEBUG
}

#pragma warning(suppress: 26440)
void CNTService::OnTriggerEvent()
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnTriggerEvent was called\n"));
#endif //#ifdef _DEBUG
}

#pragma warning(suppress: 26440)
void CNTService::OnLowResources()
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnLowResources was called\n"));
#endif //#ifdef _DEBUG
}

#pragma warning(suppress: 26440)
void CNTService::OnSystemLowResources()
{
#ifdef _DEBUG
  ATLTRACE(_T("CNTService::OnSystemLowResources was called\n"));
#endif //#ifdef _DEBUG
}

DWORD WINAPI CNTService::ServiceCtrlHandler(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_opt_ LPVOID pEventData)
{
  //Need to keep an additional control request of the same type from coming in when your're already handling it
  if (dwControl == m_dwRequestedControl)
    return ERROR_CALL_NOT_IMPLEMENTED;

  //Just switch on the control code sent to us and call the relevant virtual function
  DWORD dwRet = NO_ERROR;
  switch (dwControl)
  {
    case SERVICE_CONTROL_STOP: 
    {
      OnStop();
      break;
    }
    case SERVICE_CONTROL_PAUSE:
    {
      OnPause();
      break;
    }
    case SERVICE_CONTROL_CONTINUE:
    {
      OnContinue();
      break;
    }
    case SERVICE_CONTROL_INTERROGATE:
    {
      OnInterrogate();
      break;
    }
    case SERVICE_CONTROL_SHUTDOWN:
    {
      OnShutdown();
      break;
    }
    case SERVICE_CONTROL_PARAMCHANGE: 
    {
      OnParamChange();
      break;
    }
    case SERVICE_CONTROL_NETBINDADD: 
    {
      OnNetBindAdd();
      break;
    }
    case SERVICE_CONTROL_NETBINDREMOVE:
    {
      OnNetBindRemove();
      break;
    }
    case SERVICE_CONTROL_NETBINDENABLE:
    {
      OnNetBindEnable();
      break;
    }
    case SERVICE_CONTROL_NETBINDDISABLE:
    {
      OnNetBindDisable();
      break;
    }
    case SERVICE_CONTROL_DEVICEEVENT:
    {
      dwRet = OnDeviceEvent(dwEventType, pEventData);
      break;
    }
    case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
    {
      dwRet = OnHardwareProfileChange(dwEventType, pEventData);
      break;
    }
    case SERVICE_CONTROL_POWEREVENT:
    {
      dwRet = OnPowerEvent(dwEventType, pEventData);
      break;
    }
    case SERVICE_CONTROL_SESSIONCHANGE:
    {
      OnSessionChange(dwEventType, static_cast<WTSSESSION_NOTIFICATION*>(pEventData));
      break;
    }
    case SERVICE_CONTROL_PRESHUTDOWN:
    {
      OnPreShutdown();
      break;
    }
    case SERVICE_CONTROL_TIMECHANGE:
    {
      OnTimeChange(static_cast<SERVICE_TIMECHANGE_INFO*>(pEventData));
      break;
    }
    case SERVICE_CONTROL_TRIGGEREVENT:
    {
      OnTriggerEvent();
      break;
    }
    case SERVICE_CONTROL_LOWRESOURCES:
    {
      OnLowResources();
      break;
    }
    case SERVICE_CONTROL_SYSTEMLOWRESOURCES:
    {
      OnSystemLowResources();
      break;
    }
    default:
    {
      OnUserDefinedRequest(dwControl);
      break;
    }
  }

  //Any request from the SCM will be acked by this service
  ReportStatus();

  return dwRet;
}

_Return_type_success_(return != 0) BOOL CNTService::RegisterCtrlHandler()
{
  ATL::CComCritSecLock<ATL::CComAutoCriticalSection> sl(m_CritSect, true); //Synchronize access to the variables

#pragma warning(suppress: 26477)
  ATLASSERT(m_hStatus == 0); //If this ASSERTs, most likely you are calling RegisterCtrlHandler
                             //yourself. Just remove this call from your code as this is now
                             //done for you automatically by the CNTService framework

  m_hStatus = RegisterServiceCtrlHandlerEx(m_sServiceName.c_str(), _ServiceCtrlHandlerEx, nullptr);
  return (m_hStatus != nullptr);
}

#pragma warning(suppress: 26440)
void WINAPI CNTService::ServiceMain(_In_ DWORD /*dwArgc*/, _In_opt_ LPTSTR* /*lpszArgv*/)
{
  //Default implementation does nothing but asserts, your version should
  //implement its own service specific code
#pragma warning(suppress: 26477)
  ATLASSERT(FALSE);
}

void WINAPI CNTService::_ServiceCtrlHandler(_In_ DWORD dwControl)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(sm_pService != nullptr);

  //Convert from the SDK world to the C++ world
  sm_pService->ServiceCtrlHandler(dwControl, 0, nullptr);
}

DWORD WINAPI CNTService::_ServiceCtrlHandlerEx(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_opt_ LPVOID pEventData, _In_opt_ LPVOID /*pContext*/)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(sm_pService != nullptr);

  //Convert from the SDK world to the C++ world
  return sm_pService->ServiceCtrlHandler(dwControl, dwEventType, pEventData);
}

void WINAPI CNTService::_SDKServiceMain(_In_ DWORD dwArgc, _In_opt_ LPTSTR* pszArgv)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(sm_pService != nullptr);

  //Convert from the SDK world to the C++ world
  sm_pService->_ServiceMain(dwArgc, pszArgv);
}

void CNTService::_ServiceMain(_In_ DWORD dwArgc, _In_opt_ LPTSTR* pszArgv)
{
  //Register the Control Handler
  RegisterCtrlHandler();
  
  DWORD dwWin32ExitCode = NO_ERROR;
  DWORD dwServiceSpecificExitCode = 0;
  
  //Call the main C++ function
  try
  {
    ServiceMain(dwArgc, pszArgv);
  }
  catch(CNTServiceTerminateException& e)
  {
    dwWin32ExitCode = e.m_dwWin32ExitCode;
    dwServiceSpecificExitCode = e.m_dwServiceSpecificExitCode;
  }
  
  //Report to the SCM that the service has stopped.
  //Note that it is important that we do not access anything on the stack 
  //after this call as the SCM could have terminated this worker thread by then
  _ReportStatus(SERVICE_STOPPED, 0, 0, 0, dwWin32ExitCode, dwServiceSpecificExitCode);
}

_Return_type_success_(return != 0) BOOL CNTService::WriteServiceProfileString(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszValue, _In_ BOOL bFlush, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  LSTATUS lResult = 0;
  if (pszEntry == nullptr) //delete whole section
  {
    ATL::CRegKey serviceKey;
#pragma warning(suppress: 26486)
    if (!GetServiceParametersKey(serviceKey, pszService, FALSE, pLastError))
      return FALSE;

    lResult = serviceKey.DeleteSubKey(pszSection);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
    if (bFlush)
      serviceKey.Flush();
  }
  else if (pszValue == nullptr)
  {
    ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
    if (!GetSectionKey(secKey, pszService, pszSection, FALSE, pLastError))
      return FALSE;

    lResult = secKey.DeleteValue(pszEntry);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
    if (bFlush)
      secKey.Flush();
  }
  else
  {
    ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
    if (!GetSectionKey(secKey, pszService, pszSection, FALSE, pLastError))
      return FALSE;

    lResult = secKey.SetStringValue(pszEntry, pszValue);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
    if (bFlush)
      secKey.Flush();
  }
  return lResult == ERROR_SUCCESS;
}

_Return_type_success_(return != 0) BOOL CNTService::WriteServiceProfileInt(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nValue, _In_ BOOL bFlush, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, FALSE, pLastError))
    return FALSE;

  LSTATUS lResult = secKey.SetDWORDValue(pszEntry, nValue);
  if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
    *pLastError = lResult;
  if (bFlush)
    secKey.Flush();
  return lResult == ERROR_SUCCESS;
}

_Return_type_success_(return != 0) BOOL CNTService::WriteServiceProfileStringArray(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array, _In_ BOOL bFlush, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, FALSE, pLastError))
    return FALSE;

  const BOOL bSuccess = CNTEventLogSource::SetStringArrayIntoRegistry(secKey, pszEntry, array, pLastError);
  if (bFlush)
    secKey.Flush();
  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTService::WriteServiceProfileBinary(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_reads_bytes_opt_(nBytes) const BYTE* pData, _In_ ULONG nBytes, _In_ BOOL bFlush, _Inout_opt_ DWORD* pLastError) noexcept
{ 
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, FALSE, pLastError))
    return FALSE;

  LSTATUS lResult = secKey.SetBinaryValue(pszEntry, pData, nBytes);
  if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
    *pLastError = lResult;
  if (bFlush)
    secKey.Flush();
  return lResult == ERROR_SUCCESS;
}

CNTServiceString CNTService::GetServiceProfileString(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszDefault, _Inout_opt_ DWORD* pLastError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, TRUE, pLastError))
  {
    if (pszDefault == nullptr)
      return CNTServiceString(_T(""));
    else
      return CNTServiceString(pszDefault);
  }

  CNTServiceString sValue;
  DWORD dwType = 0;
  ULONG nBytes = 0;
  LSTATUS lResult = secKey.QueryValue(pszEntry, &dwType, nullptr, &nBytes);
  if (lResult == ERROR_SUCCESS)
  {
    ULONG nChars = nBytes/sizeof(TCHAR);
    sValue.resize(nChars);
#pragma warning(suppress: 26446)
    lResult = secKey.QueryStringValue(pszEntry, &(sValue[0]), &nChars);
    if (lResult == ERROR_SUCCESS)
      sValue.erase(nChars - 1, 1);
  }

  if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
    *pLastError = lResult;

  if (lResult == ERROR_SUCCESS)
    return sValue;

  if (pszDefault == nullptr)
    return CNTServiceString(_T(""));
  else
    return CNTServiceString(pszDefault);
}

UINT CNTService::GetServiceProfileInt(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nDefault, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, TRUE, pLastError))
    return nDefault;

  DWORD dwValue;
  LSTATUS lResult = secKey.QueryDWORDValue(pszEntry, dwValue);
  if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
    *pLastError = lResult;

  if (lResult == ERROR_SUCCESS)
    return static_cast<UINT>(dwValue);

  return nDefault;
}

_Return_type_success_(return != 0) BOOL CNTService::GetServiceProfileStringArray(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array, _Inout_opt_ DWORD* pLastError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  //reset the output parameter
  array.clear();

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, TRUE, pLastError))
    return FALSE;

  return CNTEventLogSource::GetStringArrayFromRegistry(secKey, pszEntry, array, pLastError);
}

_Return_type_success_(return != 0) BOOL CNTService::GetServiceProfileBinary(_In_opt_z_ LPCTSTR pszService, _In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ std::vector<BYTE>& data, _Inout_opt_ DWORD* pLastError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszService != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszSection != nullptr);
#pragma warning(suppress: 26477)
  ATLASSERT(pszEntry != nullptr);

  //Initialize the output parameter to default value.
  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  //Reset the output parameter
  data.clear();

  ATL::CRegKey secKey;
#pragma warning(suppress: 26486)
  if (!GetSectionKey(secKey, pszService, pszSection, TRUE, pLastError))
    return FALSE;

  DWORD dwType = 0;
  ULONG nBytes = 0;
  LSTATUS lResult = secKey.QueryValue(pszEntry, &dwType, nullptr, &nBytes);
  if (lResult == ERROR_SUCCESS)
  {
    data.resize(nBytes);
#pragma warning(suppress: 26489)
    lResult = secKey.QueryBinaryValue(pszEntry, data.data(), &nBytes);

    if (lResult == ERROR_SUCCESS)
    {
#pragma warning(suppress: 26489)
      data.resize(nBytes);
    }
    else if (pLastError != nullptr)
      *pLastError = lResult;
  }
  else
  {
    if (pLastError != nullptr)
      *pLastError = lResult;
  }

  return (lResult == ERROR_SUCCESS);
}

// returns key for:
//      HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters\pszSection
// creating it if it doesn't exist.
_Return_type_success_(return != 0) BOOL CNTService::GetSectionKey(_In_ ATL::CRegKey& sectionKey, _In_opt_z_ LPCTSTR pszServiceName, _In_opt_z_ LPCTSTR pszSection, _In_ BOOL bReadOnly, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pszServiceName != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pszSection != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey serviceParametersKey;
#pragma warning(suppress: 26486)
  if (!GetServiceParametersKey(serviceParametersKey, pszServiceName, bReadOnly, pLastError))
    return FALSE;

#pragma warning(suppress: 26477)
  LSTATUS lResult = sectionKey.Create(serviceParametersKey, pszSection, REG_NONE, REG_OPTION_NON_VOLATILE, bReadOnly ? KEY_READ : KEY_WRITE | KEY_READ, nullptr);
  if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
    *pLastError = lResult;

  return (lResult == ERROR_SUCCESS);
}

// returns key for:
//      HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters
// creating it if it doesn't exist
_Return_type_success_(return != 0) BOOL CNTService::GetServiceParametersKey(_In_ ATL::CRegKey& serviceParametersKey, _In_opt_z_ LPCTSTR pszServiceName, _In_ BOOL bReadOnly, _Inout_opt_ DWORD* pLastError) noexcept
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(pszServiceName != nullptr);

  if (pLastError != nullptr)
    *pLastError = ERROR_SUCCESS;

  ATL::CRegKey servicesKey;
  LSTATUS lResult = servicesKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"), bReadOnly ? KEY_READ : KEY_WRITE | KEY_READ);
  if (lResult == ERROR_SUCCESS)
  {
    //Create the service key
    ATL::CRegKey serviceKey;
#pragma warning(suppress: 26477)
    lResult = serviceKey.Create(servicesKey, pszServiceName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, nullptr);
    if (lResult == ERROR_SUCCESS)
    {
      //Create the parameters key
#pragma warning(suppress: 26477)
      lResult = serviceParametersKey.Create(serviceKey, _T("Parameters"), REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, nullptr);
      if ((lResult != ERROR_SUCCESS) && (pLastError != nullptr))
        *pLastError = lResult;
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

  return (lResult == ERROR_SUCCESS);
}

_Return_type_success_(return != 0) BOOL CNTService::WriteProfileString(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszValue) noexcept
{
  return WriteServiceProfileString(m_sServiceName.c_str(), pszSection, pszEntry, pszValue, m_bProfileWriteFlush);
}

_Return_type_success_(return != 0) BOOL CNTService::WriteProfileInt(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nValue) noexcept
{
  return WriteServiceProfileInt(m_sServiceName.c_str(), pszSection, pszEntry, nValue, m_bProfileWriteFlush);
}

_Return_type_success_(return != 0) BOOL CNTService::WriteProfileStringArray(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ const CNTServiceStringArray& array) noexcept
{
  return WriteServiceProfileStringArray(m_sServiceName.c_str(), pszSection, pszEntry, array, m_bProfileWriteFlush);
}

_Return_type_success_(return != 0) BOOL CNTService::WriteProfileBinary(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_reads_bytes_opt_(nBytes) const BYTE* pData, _In_ UINT nBytes) noexcept
{
  return WriteServiceProfileBinary(m_sServiceName.c_str(), pszSection, pszEntry, pData, nBytes, m_bProfileWriteFlush);
}

CNTServiceString CNTService::GetProfileString(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_opt_z_ LPCTSTR pszDefault, _Inout_opt_ DWORD* pLastError)
{
  return GetServiceProfileString(m_sServiceName.c_str(), pszSection, pszEntry, pszDefault, pLastError);
}

UINT CNTService::GetProfileInt(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _In_ int nDefault, _Inout_opt_ DWORD* pLastError) noexcept
{
  return GetServiceProfileInt(m_sServiceName.c_str(), pszSection, pszEntry, nDefault, pLastError);
}

_Return_type_success_(return != 0) BOOL CNTService::GetProfileStringArray(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ CNTServiceStringArray& array)
{
  return GetServiceProfileStringArray(m_sServiceName.c_str(), pszSection, pszEntry, array);
}

_Return_type_success_(return != 0) BOOL CNTService::GetProfileBinary(_In_opt_z_ LPCTSTR pszSection, _In_opt_z_ LPCTSTR pszEntry, _Out_ std::vector<BYTE>& data)
{
  return GetServiceProfileBinary(m_sServiceName.c_str(), pszSection, pszEntry, data);
}

_Return_type_success_(return != 0) BOOL CNTService::Run()
{
  //Set up the SERVICE table array
  CAtlString sServiceName;
  sServiceName = m_sServiceName.c_str();
  std::array<SERVICE_TABLE_ENTRY, 2> ServiceTable{ sServiceName.GetBuffer(), _SDKServiceMain, nullptr, nullptr };

  //Notify the SCM of our service
  const BOOL bSuccess = StartServiceCtrlDispatcher(ServiceTable.data());
  sServiceName.ReleaseBuffer();
  return bSuccess;
}

_Return_type_success_(return != 0) BOOL CNTService::GetDependencies(_Out_ ATL::CHeapPtr<TCHAR>& mszDependencies) noexcept
{
  //Work out the size of the string we need
  CNTServiceStringArray::size_type nSize = 0;
  const CNTServiceStringArray::size_type  nDependencies = m_sDependencies.size();
  for (const auto& dependency : m_sDependencies)
    nSize += (dependency.length() + 1);

  //Need one second null for the double null at the end
  nSize++;

  //Now form the string
  if (!mszDependencies.Allocate(nSize))
  {
    SetLastError(ERROR_OUTOFMEMORY);
    return FALSE;
  }

  //Now copy the strings into the buffer
  LPTSTR pszString = mszDependencies.m_pData;
#pragma warning(suppress: 26477)
  ATLASSUME(pszString != nullptr);
  size_t nCurOffset = 0;
  for (const auto& dependency : m_sDependencies)
  {
    const size_t nDependencyLength = dependency.length();
#pragma warning(suppress: 26481)
    _tcscpy_s(&pszString[nCurOffset], nDependencyLength+1, dependency.c_str());
    nCurOffset += (nDependencyLength + 1);
  }
  //Don't forgot to doubly null terminate
#pragma warning(suppress: 26481)
  pszString[nCurOffset] = _T('\0');

  return TRUE;
}

void CNTService::TerminateService(_In_ DWORD dwWin32ExitCode, _In_ DWORD dwServiceSpecificExitCode)
{
  throw CNTServiceTerminateException(dwWin32ExitCode, dwServiceSpecificExitCode);
}

void CNTService::SecureEmptyString(_Inout_ CNTServiceString& sVal)
{
#pragma warning(suppress: 26446)
  SecureZeroMemory(&(sVal[0]), sVal.length());
}

_Return_type_success_(return != 0) BOOL CNTService::InstallEventLogSource(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_bEventLogSource);

  //Initialize the output parameters to default values
  sErrorMsg = CNTServiceString();
  dwError = ERROR_SUCCESS;

  //Setup this service as an event log source (using the friendly name)
  //If a message dll has not been specified, then we use one with the same name 
  //as this application and in the same path
  if (m_sEventMessageFile.length() == 0)
  {
    CAtlString sAppPath;
    const DWORD dwGMFN = GetModuleFileName(nullptr, sAppPath.GetBuffer(_MAX_PATH), _MAX_PATH);
    sAppPath.ReleaseBuffer();
    if (!dwGMFN)
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_INSTALL_AS_EVENT_LOG_SOURCE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
    TCHAR szPath[_MAX_PATH];
    szPath[0] = _T('\0');
    TCHAR szDrive[_MAX_DRIVE];
    szDrive[0] = _T('\0');
    TCHAR szDir[_MAX_DIR];
    szDir[0] = _T('\0');
    TCHAR szFname[_MAX_FNAME];
    szFname[0] = _T('\0');
#pragma warning(suppress: 26485)
    _tsplitpath_s(sAppPath, szDrive, sizeof(szDrive)/sizeof(TCHAR), szDir, sizeof(szDir)/sizeof(TCHAR), szFname, sizeof(szFname)/sizeof(TCHAR), nullptr, 0);
#pragma warning(suppress: 26485)
    _tmakepath_s(szPath, sizeof(szPath)/sizeof(TCHAR), szDrive, szDir, szFname, _T("DLL"));
#pragma warning(suppress: 26485)
    m_sEventMessageFile = szPath;
  }

  if (!m_EventLogSource.Install(m_EventLogSource.GetLogName().c_str(), m_EventLogSource.GetSourceName().c_str(), m_sEventMessageFile.c_str(), m_sEventCategoryMessageFile.c_str(), m_sEventParameterMessageFile.c_str(), m_dwEventTypesSupported, m_dwEventCategoryCount))
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_INSTALL_AS_EVENT_LOG_SOURCE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    return FALSE;
  }

  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTService::InstallServiceConfiguration(_In_ CNTScmService& service, _Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError)
{
  //Initialize the output parameters to default values
  sErrorMsg = CNTServiceString();
  dwError = ERROR_SUCCESS;

  //Change the service description if necessary
  if (m_sDescription.length())
  {
    if (!service.ChangeDescription(m_sDescription))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGEDESCRIPTION), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Change the service Sid if necessary
  if (m_dwServiceSidType != SERVICE_SID_TYPE_NONE)
  {
    if (!service.ChangeSidInfo(m_dwServiceSidType))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGESIDINFO), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Change the service DelayedAutoStart setting if necessary
  if (m_bDelayedAutoStart)
  {
    if (!service.ChangeDelayAutoStart(m_bDelayedAutoStart))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGEDELAYAUTOSTART), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Change the service privileges if necessary
  if (m_sPrivileges.size())
  {
    if (!service.ChangeRequiredPrivileges(m_sPrivileges))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGEPRIVILEGES), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Change the preshutdown timeout if necessary
  if (m_dwPreshutdownTimeout)
  {
    if (!service.ChangePreShutdown(m_dwPreshutdownTimeout))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGEPRESHUTDOWNTIMEOUT), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Change the Launch protected setting if necessary
  if (m_dwLaunchProtected != SERVICE_LAUNCH_PROTECTED_NONE)
  {
    if (!service.ChangeLaunchProtected(m_dwLaunchProtected))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CHANGELAUNCHPROTECTED), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTService::EnableServiceLogonRight(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(m_sUserName.length());

  //By default empty the error message 
  sErrorMsg = CNTServiceString();
  dwError = ERROR_SUCCESS;

  //First we need to lookup the SID for the specified account
  DWORD dwSIDSize = 0;
  DWORD dwDomainNameSize = 0;
  SID_NAME_USE snu;
  LookupAccountName(nullptr, m_sUserName.c_str(), nullptr, &dwSIDSize, nullptr, &dwDomainNameSize, &snu);
  if (dwSIDSize)
  {
    ATL::CHeapPtr<BYTE> sid;
    ATL::CHeapPtr<TCHAR> domain;
    if (!sid.Allocate(dwSIDSize) || !domain.Allocate(dwDomainNameSize))
    {
      dwError = ERROR_OUTOFMEMORY;
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_ENABLESERVICELOGONRIGHT), dwError, FALSE);
      return FALSE;
    }
    if (!LookupAccountName(nullptr, m_sUserName.c_str(), sid.m_pData, &dwSIDSize, domain.m_pData, &dwDomainNameSize, &snu))
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_LOOKUP_ACCOUNT_NAME), dwError, FALSE);
      return FALSE;
    }

    //Open the Policy handle
    LSA_OBJECT_ATTRIBUTES attributes = { 0 };
    LSA_HANDLE lsaHandle = nullptr;
    NTSTATUS ntStatus = LsaOpenPolicy(nullptr, &attributes, POLICY_LOOKUP_NAMES | POLICY_CREATE_ACCOUNT, &lsaHandle);
    if (ntStatus != ERROR_SUCCESS) //ERROR_SUCCESS == STATUS_SUCCESS
    {
      dwError = LsaNtStatusToWinError(ntStatus);
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_OPEN_POLICY_HANDLE), dwError, FALSE);
      return FALSE;
    }

    //Next prep a LSA_UNICODE_STRING for the privilege name with the Service Logon Right
    LSA_UNICODE_STRING userRights;
    CAtlStringW sPrivilege(L"SeServiceLogonRight");
    userRights.Buffer = sPrivilege.GetBuffer();
    const size_t dwPrivilegeLen = sPrivilege.GetLength();
#pragma warning(suppress: 26472)
    userRights.Length = static_cast<USHORT>(dwPrivilegeLen * sizeof(wchar_t));
#pragma warning(suppress: 26472)
    userRights.MaximumLength = static_cast<USHORT>((dwPrivilegeLen + 1) * sizeof(wchar_t));

    //Finally call LsaAddAccountRights to enable the privilege
    ntStatus = LsaAddAccountRights(lsaHandle, sid.m_pData, &userRights, 1);
    sPrivilege.ReleaseBuffer();
    if (ntStatus != ERROR_SUCCESS) //ERROR_SUCCESS == STATUS_SUCCESS
    {
      LsaClose(lsaHandle);
      dwError = LsaNtStatusToWinError(ntStatus);
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_ENABLESERVICELOGONRIGHT), dwError, FALSE);
      return FALSE;
    }

    //Release the policy handle now that we are finished with it
    LsaClose(lsaHandle);
  }
  else
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_LOOKUP_ACCOUNT_NAME), dwError, FALSE);
    return FALSE;
  }

  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTService::Install(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError)
{
  //By default empty the error message 
  sErrorMsg = CNTServiceString();
  dwError = ERROR_SUCCESS;

  //Open up the SCM requesting creation rights
  CNTServiceControlManager manager;
  if (!manager.Open(nullptr, SC_MANAGER_CREATE_SERVICE | SC_MANAGER_LOCK))
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_OPEN_SCM), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    return FALSE;
  }

  //Lock the SCM since we are going to install a service
  if (!manager.Lock())
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_LOCK_SCM), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    return FALSE;
  }

  //Get the dependencies for this service
  ATL::CHeapPtr<TCHAR> mszDependencies;
  if (!GetDependencies(mszDependencies))
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_GET_DEPENDENCIES), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    return FALSE;
  }

  //Create the new service entry in the SCM database
  CNTScmService service;
  if (!service.Create(manager, m_sServiceName.c_str(), m_sDisplayName.c_str(), SERVICE_CHANGE_CONFIG, m_dwServiceType, m_dwStartType, 
                      m_dwErrorControl, m_sBinaryPathName.c_str(), m_sLoadOrderGroup.length() ? m_sLoadOrderGroup.c_str() : nullptr, 
                      nullptr, mszDependencies, m_sUserName.length() ? m_sUserName.c_str() : nullptr, 
                      m_sPassword.length() ? m_sPassword.c_str() : nullptr))
  {
    dwError = GetLastError();

    //As a security precaution we nuke the username and password details once we have used them
    SecureEmptyString(m_sUserName);
    SecureEmptyString(m_sPassword);

    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_INSTALL_FAIL_CREATESERVICE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    return FALSE;
  }

  //As a security precaution we nuke the username and password details once we have used them
  SecureEmptyString(m_sUserName);
  SecureEmptyString(m_sPassword);

  //Install the event log source
  if (m_bEventLogSource)
  {
    if (!InstallEventLogSource(sErrorMsg, dwError))
      return FALSE;
  }

  //Setup the service configuration settings
#pragma warning(suppress: 26486 26489)
  if (!InstallServiceConfiguration(service, sErrorMsg, dwError))
    return FALSE;

  //Add an Event log entry to say the service was successfully installed
  if (m_bEventLogSource)
    m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MapMessageID(CNTS_MSG_SERVICE_INSTALLED), m_sDisplayName.c_str());

  ATLTRACE(_T("CNTService::Install, Service: %s was succesfully installed\n"), m_sServiceName.c_str());

  return TRUE;
}

//Class used by EnumerateInstances
struct _NTSERV_EnumerateInstancesInfo
{
//Constructors / Destructors
  _NTSERV_EnumerateInstancesInfo() noexcept : pServiceNames(nullptr), 
                                              psAppPath(nullptr),
                                              pManager(nullptr) 
  {
  }

//Member variables
  CNTServiceStringArray*    pServiceNames; //The array to populate
  CAtlString*               psAppPath;     //The full path to this exe
  CNTServiceControlManager* pManager;      //Pointer to the controlling manager
};

_Return_type_success_(return != 0) BOOL CNTService::EnumerateInstances(_Out_ CNTServiceStringArray& ServiceNames, _Out_ DWORD& dwError)
{
  //By default empty the error message 
  dwError = ERROR_SUCCESS;

  //Empty the array before we go any further
  ServiceNames.clear();

  //Open up the SCM requesting enumeration rights
  CNTServiceControlManager manager;
  if (!manager.Open(nullptr, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE))
  {
    dwError = GetLastError();
    return FALSE;
  }

  //Get the full path to this exe
  CAtlString sAppPath;
  const DWORD dwGMFN = GetModuleFileName(nullptr, sAppPath.GetBuffer(_MAX_PATH), _MAX_PATH);
  sAppPath.ReleaseBuffer();
  if (!dwGMFN)
  {
    dwError = GetLastError();
    return FALSE;
  }
  sAppPath.MakeUpper(); //Comparisons will be case insensitive

  //Create the info struct we will pass to the callback function
  _NTSERV_EnumerateInstancesInfo info;
  info.pServiceNames = &ServiceNames;
  info.psAppPath = &sAppPath;
  info.pManager = &manager;

  //Finally call the enumeration function
  return manager.EnumServices(SERVICE_WIN32, SERVICE_STATE_ALL, &info, _EnumerateInstances);
}

_Return_type_success_(return != 0) BOOL CALLBACK CNTService::_EnumerateInstances(_In_opt_ void* pData, _In_ const ENUM_SERVICE_STATUS& ss)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSERT(pData != nullptr);

  _NTSERV_EnumerateInstancesInfo* pInfo = static_cast<_NTSERV_EnumerateInstancesInfo*>(pData);
#pragma warning(suppress: 26477)
  ATLASSUME(pInfo != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pInfo->pManager != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pInfo->psAppPath != nullptr);
#pragma warning(suppress: 26477)
  ATLASSUME(pInfo->pServiceNames != nullptr);

  //Open up the service to get its configuration
  CNTScmService service;
  if (pInfo->pManager->OpenService(ss.lpServiceName, SERVICE_QUERY_CONFIG, service))
  {
    //Now that we have the service opened, query it configuration to get its binary path name
    CNTServiceConfig config;
    if (service.QueryConfig(config))
    {
      //If the binary path in the SCM contains our path, then add this found service to the instance array
      CAtlString sBinaryPath(config.operator LPQUERY_SERVICE_CONFIG()->lpBinaryPathName);
      sBinaryPath.MakeUpper();
      if (sBinaryPath.Find(*pInfo->psAppPath) != -1)
      {
#pragma warning(suppress: 26489)
        pInfo->pServiceNames->push_back(ss.lpServiceName);
      }
    }
    else
      ATLTRACE(_T("CNTService::_EnumerateInstances, Failed to query service configuration, Service:%s\n"), ss.lpServiceName);
  }
  else
    ATLTRACE(_T("CNTService::_EnumerateInstances, Failed to open service, Service:%s\n"), ss.lpServiceName);

  //Always continue enumeration
  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTService::Uninstall(_Inout_ CNTServiceString& sErrorMsg, _Inout_ DWORD& dwError, _In_ DWORD dwTimeToWaitForStop)
{
  //By default empty the error message 
  sErrorMsg = CNTServiceString();
  dwError = ERROR_SUCCESS;

  //Open up the SCM requesting connect rights
  CNTServiceControlManager manager;
  if (!manager.Open(nullptr, SC_MANAGER_CONNECT))
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_OPEN_SCM), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      CNTServiceString sError(Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_OPEN_SCM), dwError, TRUE));
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_CONNECT_SCM), sError.c_str());
    }
    return FALSE;
  }

  //Open up the existing service requesting deletion rights
  CNTScmService service;
  if (!manager.OpenService(m_sServiceName.c_str(), DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS, service))
  {
    dwError = GetLastError();
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_OPEN_SERVICE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      CNTServiceString sError(Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_OPEN_SERVICE), dwError, TRUE));
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_OPEN_SERVICE), sError.c_str());
    }
    return FALSE;
  }

  //Ask the service to stop if it is running
  SERVICE_STATUS status;
  if (service.QueryStatus(status) && status.dwCurrentState != SERVICE_STOPPED)
  {
    if (!service.Stop())
    {
      dwError = GetLastError();
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_STOP), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
    }

    //Wait for the service to stop
    if (!service.WaitForStop(dwTimeToWaitForStop))
    {
      dwError = GetLastError();
#pragma warning(suppress: 26489)
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_WAIT_FOR_STOP), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  //Delete the service from the SCM database
  if (!service.Delete())
  {
    dwError = GetLastError();
#pragma warning(suppress: 26489)
    sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_DELETE_SERVICE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      CNTServiceString sError(Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_DELETE_SERVICE), dwError, TRUE));
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_DELETE_SERVICE), sError.c_str());
    }
    return FALSE;
  }

  if (m_bEventLogSource)
  {
    //Remove this service as an event log source
    if (!m_EventLogSource.Uninstall(m_EventLogSource.GetLogName().c_str(), m_EventLogSource.GetSourceName().c_str()))
    {
      dwError = GetLastError();
#pragma warning(suppress: 26489)
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_UNINSTALL_FAIL_UNINSTALL_EVENTLOG_SOURCE), dwError, FALSE);
#pragma warning(suppress: 26486 26489)
      TraceMessage(sErrorMsg);
      return FALSE;
    }
  }

  ATLTRACE(_T("CNTService::Uninstall, Service: %s was succesfully uninstalled\n"), m_sServiceName.c_str());

  return TRUE;
}

_Return_type_success_(return != 0) BOOL CNTService::SetServiceToStatus(_In_ ServiceAction esaStatusWanted, _Inout_ DWORD& dwError, _In_ DWORD dwTimeout, _In_ DWORD dwPollingInterval)
{
  //Initialize the output parameter to default value.
  dwError = ERROR_SUCCESS;

  DWORD dwStatusWanted = 0;
  DWORD dwRightNeeded = 0;

  switch (esaStatusWanted)
  {
    case ServiceAction::StartService:
    {
      dwStatusWanted = SERVICE_RUNNING;
      dwRightNeeded = SERVICE_START;
      break;
    }
    case ServiceAction::PauseService:
    {
      dwStatusWanted = SERVICE_PAUSED;
      dwRightNeeded = SERVICE_PAUSE_CONTINUE;
      break;
    }
    case ServiceAction::ContinueService:
    {
      dwStatusWanted = SERVICE_RUNNING;
      dwRightNeeded = SERVICE_PAUSE_CONTINUE;
      break;
    }
    case ServiceAction::StopService:
    {
      dwStatusWanted = SERVICE_STOPPED;
      dwRightNeeded = SERVICE_STOP;
      break;
    }
    default:
    {
#pragma warning(suppress: 26477)
      ATLASSERT(FALSE); //unknown status wanted
      break;
    }
  }

  //Open up the SCM requesting connect rights
  CNTServiceControlManager manager;
  if (!manager.Open(nullptr, SC_MANAGER_CONNECT))
  {
    dwError = GetLastError();
    CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_OPEN_SCM), dwError, FALSE));
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_OPEN_SCM), dwError, TRUE);
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_CONNECT_SCM), sErrorMsg.c_str());
    }
    return FALSE;
  }

  //Open up the existing service requesting the necessary rights
  CNTScmService service;
  if (!manager.OpenService(m_sServiceName.c_str(), dwRightNeeded | SERVICE_QUERY_STATUS, service))
  {
    dwError = GetLastError();
    CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_OPEN_SERVICE), dwError, FALSE));
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_OPEN_SERVICE), dwError, TRUE);
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_OPEN_SERVICE), sErrorMsg.c_str());
    }
    return FALSE;
  }

  //Ask the service to change to the requested status if it is not already in that status
  SERVICE_STATUS status;
  if (service.QueryStatus(status))
  {
    if (status.dwCurrentState != dwStatusWanted)
    {
      BOOL bSuccess = FALSE;
      switch (esaStatusWanted)
      {
        case ServiceAction::StartService:
        {
          bSuccess = service.Start(0, nullptr);
          if (!bSuccess)
          {
            dwError = GetLastError();
            CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_START), dwError, FALSE));
            TraceMessage(sErrorMsg);
            if (m_bEventLogSource)
            {
              sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_START), dwError, TRUE);
              m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_START), sErrorMsg.c_str());
            }
          }
          break;
        }
        case ServiceAction::PauseService:
        {
          bSuccess = service.Pause();
          if (!bSuccess)
          {
            dwError = GetLastError();
            CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_PAUSE), dwError, FALSE));
            TraceMessage(sErrorMsg);
            if (m_bEventLogSource)
            {
              sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_PAUSE), dwError, TRUE);
              m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_PAUSE), sErrorMsg.c_str());
            }
          }
          break;
        }
        case ServiceAction::ContinueService:
        {
          bSuccess = service.Continue();
          if (!bSuccess)
          {
            dwError = GetLastError();
            CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_CONTINUE), dwError, FALSE));
            TraceMessage(sErrorMsg);
            if (m_bEventLogSource)
            {
              sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_CONTINUE), dwError, TRUE);
              m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_CONTINUE), sErrorMsg.c_str());
            }
          }
          break;
        }
        case ServiceAction::StopService:
        {
          bSuccess = service.Stop();
          if (!bSuccess)
          {
            dwError = GetLastError();
            CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_STOP), dwError, FALSE));
            TraceMessage(sErrorMsg);
            if (m_bEventLogSource)
            {
              sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_STOP), dwError, TRUE);
              m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_STOP), sErrorMsg.c_str());
            }
          }
          break;
        }
        default:
        {
#pragma warning(suppress: 26477)
          ATLASSERT(FALSE);
          break;
        }
      }

      if (!bSuccess)
        return FALSE;
    }

    //Wait for the service to have the status wanted
    if (!service.WaitForServiceStatus(dwTimeout, dwStatusWanted, dwPollingInterval))
    {
      dwError = GetLastError();
      CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_WAITFORCHANGE), dwError, FALSE));
      TraceMessage(sErrorMsg);
      if (m_bEventLogSource)
      {
        sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_WAITFORCHANGE), dwError, TRUE);
        m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_CHANGE_STATE), sErrorMsg.c_str(), dwStatusWanted, FALSE);
      }

      return FALSE;
    }
  }
  else
  {
    dwError = GetLastError();
    CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_QUERYSTATUS), dwError, FALSE));
    TraceMessage(sErrorMsg);
    if (m_bEventLogSource)
    {
      sErrorMsg = Win32ErrorToString(MapResourceID(IDS_NTSERV_SETSERVICETOSTATUS_FAIL_QUERYSTATUS), dwError, TRUE);
      m_EventLogSource.Report(EVENTLOG_ERROR_TYPE, MapMessageID(CNTS_MSG_SERVICE_FAIL_QUERY_STATUS), sErrorMsg.c_str());
    }
    return FALSE;
  }

  ATLTRACE(_T("CNTService::SetServiceToStatus, Service: %s status was successfully set\n"), m_sServiceName.c_str());

  return TRUE;
}

void CNTService::Debug()
{
  //Runing as an EXE not as a service by just executing the SeviceMain method

  //Register the console handler to allow the console to generate requests to simulate stopping the service
  SetConsoleCtrlHandler(_ConsoleCtrlHandler, TRUE);

  //Now simply call ServiceMain with out modified set of parameters
  ServiceMain(__argc, __targv);
}

void CNTService::ShowHelp() noexcept
{
  //Default behaviour is to do nothing. In your service application, you should override this function 
  //to either display something	helpful to the console or if the service is running in the GUI subsystem, 
  //to display a messagebox or dialog to provide info about your service.
}

//Based upon the function of the same name in CWinApp
void CNTService::ParseCommandLine(_In_ CNTServiceCommandLineInfo& rCmdInfo)
{
  for (int i=1; i<__argc; i++)
  {
#pragma warning(suppress: 26481)
    CAtlString sParam(__targv[i]);
    BOOL bFlag = FALSE;
    const BOOL bLast = ((i + 1) == __argc);
    if (sParam.GetLength() && ((sParam[0] == _T('-')) || (sParam[0] == _T('/'))))
    {
      //Remove the flag specifier
      bFlag = TRUE;
      sParam = sParam.Mid(1);
    }
    rCmdInfo.ParseParam(sParam, bFlag, bLast);
  }
}

//Callback function to handle console control signals
_Return_type_success_(return != 0) BOOL WINAPI CNTService::_ConsoleCtrlHandler(_In_ DWORD dwCtrlType)
{
  //Validate our parameters
#pragma warning(suppress: 26477)
  ATLASSUME(sm_pService != nullptr);

  //Convert from the SDK world to the C++ world
  return sm_pService->ConsoleCtrlHandler(dwCtrlType);
}

_Return_type_success_(return != 0) BOOL CNTService::ConsoleCtrlHandler(_In_ DWORD dwCtrlType)
{
  //If the event is a shutdown event then call the OnStop virtual method
  if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_BREAK_EVENT || dwCtrlType == CTRL_CLOSE_EVENT)
  {
    sm_pService->OnStop();
    return TRUE;
  }
  else
    return FALSE;
}

void CNTService::DisplayMessage(_In_ const CNTServiceString& sMessage) noexcept
{
  if (m_bUseConsole)
    _tprintf(_T("%s\n"), sMessage.c_str());
  else
    MessageBox(nullptr, sMessage.c_str(), m_sDisplayName.c_str(), MB_OK | MB_ICONSTOP);
}

#pragma warning(suppress: 26440)
void CNTService::TraceMessage(_In_opt_z_ LPCTSTR pszMessage)
{
#ifdef _DEBUG
  ATLTRACE(_T("%s\n"), pszMessage);
#else
  UNREFERENCED_PARAMETER(pszMessage);
#endif //#ifdef _DEBUG
}

void CNTService::TraceMessage(_In_ const CNTServiceString& sMessage)
{
  TraceMessage(sMessage.c_str());
}

void CNTService::FormatString(_In_ CNTServiceString& sMsg, _In_ UINT nIDS, _In_z_ LPCTSTR psz1)
{
  CAtlString sResource;
  if (!sResource.LoadString(GetModuleHandle(nullptr), nIDS))
    return;
  sMsg = sResource;
  const CNTServiceString::size_type nFind = sMsg.find(_T("%1"));
  if (nFind != CNTServiceString::npos)
    sMsg.replace(nFind, 2, psz1);
}

void CNTService::FormatString(_In_ CNTServiceString& sMsg, _In_ UINT nIDS, _In_z_ LPCTSTR psz1, _In_z_ LPCTSTR psz2)
{
  CAtlString sResource;
  if (!sResource.LoadString(GetModuleHandle(nullptr), nIDS))
    return;
  sMsg = sResource;
  CNTServiceString::size_type nFind = sMsg.find(_T("%1"));
  if (nFind != CNTServiceString::npos)
    sMsg.replace(nFind, 2, psz1);
#pragma warning(suppress: 26489)
  nFind = sMsg.find(_T("%2"));
  if (nFind != CNTServiceString::npos)
#pragma warning(suppress: 26489)
    sMsg.replace(nFind, 2, psz2);
}

//Based upon the function of the same name in CWinApp
DWORD CNTService::ProcessShellCommand(_In_ CNTServiceCommandLineInfo& rCmdInfo)
{
  //Update the service name if allowed and provided on the command line
  if (m_bAllowNameChange && rCmdInfo.m_sServiceName.length())
    m_sServiceName = rCmdInfo.m_sServiceName;

  //Update the display name if allowed and provided on the command line
  if (m_bAllowDisplayNameChange && rCmdInfo.m_sServiceDisplayName.length())
  {
    m_sDisplayName = rCmdInfo.m_sServiceDisplayName;

    //Also by default update the event log source name 
    m_EventLogSource.SetSourceName(m_sDisplayName.c_str());
  }

  //Update the description string if allowed and provided on the command line
  if (m_bAllowDescriptionChange && rCmdInfo.m_sServiceDescription.length())
    m_sDescription = rCmdInfo.m_sServiceDescription;

  //What will be the return value from this function
  DWORD dwError = ERROR_SUCCESS;

  switch (rCmdInfo.m_nShellCommand)
  {
    case CNTServiceCommandLineInfo::ShellCommand::RunAsService:
    {
      if (!Run())
        dwError = GetLastError();
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::StartTheService:
    {
      //Display the error message if the install failed
      if (!SetServiceToStatus(ServiceAction::StartService, dwError, rCmdInfo.m_dwTimeout))
      {
        CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_FAIL_START_SERVICE), dwError, FALSE));
        if (!rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::PauseService:
    {
      //Display the error message if the install failed
      if (!SetServiceToStatus(ServiceAction::PauseService, dwError, rCmdInfo.m_dwTimeout))
      {
        CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_FAIL_PAUSE_SERVICE), dwError, FALSE));
        if (!rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::ContinueService:
    {
      //Display the error message if the install failed
      if (!SetServiceToStatus(ServiceAction::ContinueService, dwError, rCmdInfo.m_dwTimeout))
      {
        CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_FAIL_CONTINUE_SERVICE), dwError, FALSE));
        if (!rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::StopService:
    {
      //Display the error message if the install failed
      if (!SetServiceToStatus(ServiceAction::StopService, dwError, rCmdInfo.m_dwTimeout))
      {
        CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_FAIL_STOP_SERVICE), dwError, FALSE));
        if (!rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::InstallService:
    {
      //Form the command line options which we use for the command line when 
      //registering the service
      CNTServiceString sCommandLine;

      //First get the full path of this exe
      CAtlString sAppPath;
      const DWORD dwGMFN = GetModuleFileName(nullptr, sAppPath.GetBuffer(_MAX_PATH), _MAX_PATH);
      sAppPath.ReleaseBuffer();
      if (dwGMFN)
      {
        m_sBinaryPathName = sAppPath;

        //Always ensure the sevice path is quoted if there are spaces in it
        if (m_sBinaryPathName.find(_T(' ')) != CNTServiceString::npos)
          m_sBinaryPathName = _T('\"') + m_sBinaryPathName + _T('\"');

        //Add in the pertinent options we have already parsed from the command line
        if (m_bAllowNameChange && rCmdInfo.m_sServiceName.length())
        {
          CAtlString sName;
          sName.Format(_T(" \"/SN:%s\""), rCmdInfo.m_sServiceName.c_str());
          m_sBinaryPathName += sName;
        }
        if (m_bAllowDisplayNameChange && rCmdInfo.m_sServiceDisplayName.length())
        {
          CAtlString sDisplayName;
          sDisplayName.Format(_T(" \"/SDN:%s\""), rCmdInfo.m_sServiceDisplayName.c_str());
          m_sBinaryPathName += sDisplayName;
        }
        if (m_bAllowDescriptionChange && rCmdInfo.m_sServiceDescription.length())
        {
          CAtlString sDescription;
          sDescription.Format(_T(" \"/SD:%s\""), rCmdInfo.m_sServiceDescription.c_str());
          m_sBinaryPathName += sDescription;
        }

        //If a service command line is used, then use it for registration
        if (m_bAllowCommandLine && rCmdInfo.m_sServiceCommandLine.length())
        {
          m_sBinaryPathName += _T(" ");
          m_sBinaryPathName += rCmdInfo.m_sServiceCommandLine;
        }

        //If a username was specified, then use it for registration
        BOOL bHaveUserName = FALSE;
        if (rCmdInfo.m_sUserName.length())
        {
          bHaveUserName = TRUE;
          m_sUserName = rCmdInfo.m_sUserName;

          //As a security precaution nuke the command line copy of the username now that we have used it
          SecureEmptyString(rCmdInfo.m_sUserName);
        }

        //If a password was specified, then use it for registration
        if (rCmdInfo.m_sPassword.length())
        {
          m_sPassword = rCmdInfo.m_sPassword;

          //As a security precaution nuke the command line copy of the username now that we have used it
          SecureEmptyString(rCmdInfo.m_sPassword);
        }

        //Should the service be started automatically
        if (rCmdInfo.m_bAutoStart)
          m_dwStartType = SERVICE_AUTO_START;

        //Display the error message if the enable of the service logon right failed
        CNTServiceString sErrorMsg;
        if (bHaveUserName && rCmdInfo.m_bEnableServiceLogonRight && !EnableServiceLogonRight(sErrorMsg, dwError) && !rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);

        //Display the error message if the install failed
        if ((dwError == ERROR_SUCCESS) && !Install(sErrorMsg, dwError) && !rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      else
      {
        CNTServiceString sErrorMsg(Win32ErrorToString(MapResourceID(IDS_NTSERV_FAIL_GET_MODULE_FILENAME), dwError, FALSE));
        if (!rCmdInfo.m_bSilent)
          DisplayMessage(sErrorMsg);
      }
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::UninstallService:
    {
      //Display the error message if the uninstall failed
      CNTServiceString sErrorMsg;
      if (!Uninstall(sErrorMsg, dwError, rCmdInfo.m_dwTimeout) && !rCmdInfo.m_bSilent)
        DisplayMessage(sErrorMsg);
      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::DebugService:
    {
      try
      {
        Debug();
      }
      catch(CNTServiceTerminateException& e)
      {
        dwError = e.m_dwWin32ExitCode;
      }

      break;
    }
    case CNTServiceCommandLineInfo::ShellCommand::ShowServiceHelp:
    {
      ShowHelp();
      break;
    }
    default:
    {
#pragma warning(suppress: 26477)
      ATLASSERT(FALSE);
      break;
    }
  }

  return dwError;
}

CNTServiceString CNTService::GetErrorMessage(_In_ DWORD dwError)
{
  //What will be the return value from this function
  CNTServiceString sError;

  //Look up the error description using FormatMessage
  LPTSTR pBuffer = nullptr;
#pragma warning(suppress: 26490)
  if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), reinterpret_cast<LPTSTR>(&pBuffer), 0, nullptr) != 0)
  {
    sError = pBuffer;
    LocalFree(pBuffer);
  }

  return sError;
}

CNTServiceString CNTService::Win32ErrorToString(_In_ UINT nStringID, _In_ DWORD dwError, _In_ BOOL bEventLog)
{
  //What will be the return value from this function
  CNTServiceString sMsg;

  if (bEventLog)
  {
    switch (m_ELLoggingDetail)
    {
      case ELErrorLoggingDetail::ErrorCode:
      {
        CAtlString sTemp;
        sTemp.Format(_T("%u"), dwError);
        sMsg = sTemp;
        break;
      }
      case ELErrorLoggingDetail::ErrorDescription:
      {
        sMsg = GetErrorMessage(dwError);
        break;
      }
      case ELErrorLoggingDetail::ErrorCodeAndErrorDescription:
      {
        CNTServiceString sDescription(GetErrorMessage(dwError));
        CAtlString sErrorCode;
        sErrorCode.Format(_T("%u"), dwError);
        FormatString(sMsg, MapResourceID(IDS_NTSERV_VERBOSE_ERROR_STRING), sErrorCode, sDescription.c_str());
        break;
      }
      default:
      {
#pragma warning(suppress: 26477)
        ATLASSERT(FALSE);
        break;
      }
    }
  }
  else
  {
    switch (m_UILoggingDetail)
    {
      case UIErrorLoggingDetail::ErrorCode:
      {
        CAtlString sTemp;
        sTemp.Format(_T("%u"), dwError);
        sMsg = sTemp;
        break;
      }
      case UIErrorLoggingDetail::ErrorDescription:
      {
        sMsg = GetErrorMessage(dwError);
        break;
      }
      case UIErrorLoggingDetail::ErrorCodeAndErrorDescription:
      {
        CNTServiceString sDescription(GetErrorMessage(dwError));
        CAtlString sErrorCode;
        sErrorCode.Format(_T("%u"), dwError);
        FormatString(sMsg, MapResourceID(IDS_NTSERV_VERBOSE_ERROR_STRING), sErrorCode, sDescription.c_str());
        break;
      }
      case UIErrorLoggingDetail::StringAndErrorCode:
      {
        CAtlString sError;
        sError.Format(_T("%u"), dwError);
        FormatString(sMsg, nStringID, sError);
        break;
      }
      case UIErrorLoggingDetail::StringAndErrorDescription:
      {
        CNTServiceString sDescription(GetErrorMessage(dwError));
        FormatString(sMsg, nStringID, sDescription.c_str());
        break;
      }
      case UIErrorLoggingDetail::StringAndErrorCodeAndErrorDescription:
      {
        CNTServiceString sDescription(GetErrorMessage(dwError));
        CAtlString sError;
        sError.Format(_T("%u"), dwError);
        CNTServiceString sErrorAndDescription;
        FormatString(sErrorAndDescription, MapResourceID(IDS_NTSERV_VERBOSE_ERROR_STRING), sError.GetString(), sDescription.c_str());
        FormatString(sMsg, nStringID, sErrorAndDescription.c_str());
        break;
      }
      default:
      {
#pragma warning(suppress: 26477)
        ATLASSERT(FALSE);
        break;
      }
    }
  }

  return sMsg;
}

UINT CNTService::MapResourceID(_In_ UINT nID) noexcept
{
  //Just return the same value as what was asked for. Derived classes can customize the values 
  //for their own use
  return nID;
}

UINT CNTService::MapMessageID(_In_ UINT nID) noexcept
{
  //Just return the same value as what was asked for. Derived classes can customize the values 
  //for their own use
  return nID;
}

#pragma warning(suppress: 26440)
_Return_type_success_(return != 0) BOOL CNTService::QueryServiceDynamicInformation(_In_ DWORD dwInfoLevel, _Outptr_ PVOID* ppDynamicInfo)
{
  //Check to see if the function pointer is available
  if (m_pfnQueryServiceDynamicInformation == nullptr)
  {
    ATLTRACE(_T("CNTService::EnumServices, QueryServiceDynamicInformation function is not supported on this OS. You need to be running at least Windows 8 / Windows Server 2012 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  //Call through the function pointer
  return m_pfnQueryServiceDynamicInformation(m_hStatus, dwInfoLevel, ppDynamicInfo);
}

#pragma warning(suppress: 26440)
_Must_inspect_result_ _Return_type_success_(return != 0) DWORD CNTService::GetRegistryStateKey(_In_ SERVICE_REGISTRY_STATE_TYPE StateType, _In_ DWORD AccessMask, _Out_ HKEY* ServiceStateKey)
{
  //Check to see if the function pointer is available
  if (m_pfnGetServiceRegistryStateKey == nullptr)
  {
    ATLTRACE(_T("CNTService::GetRegistryStateKey, GetServiceRegistryStateKey function is not supported on this OS. You need to be running at least Windows 10 RS4 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  return m_pfnGetServiceRegistryStateKey(m_hStatus, StateType, AccessMask, ServiceStateKey);
}

#pragma warning(suppress: 26440)
_Must_inspect_result_ _Return_type_success_(return != 0) DWORD CNTService::GetServiceDirectory(_In_ SERVICE_DIRECTORY_TYPE eDirectoryType, _Out_writes_opt_(cchPathBufferLength) PWCHAR lpPathBuffer, _In_ DWORD cchPathBufferLength, _Out_ DWORD* lpcchRequiredBufferLength)
{
  //Check to see if the function pointer is available
  if (m_pfnGetServiceDirectory == nullptr)
  {
    ATLTRACE(_T("CNTService::GetServiceDirectory, GetServiceDirectory function is not supported on this OS. You need to be running at least Windows 10 RS5 to use this function\n"));
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
  }

  return m_pfnGetServiceDirectory(m_hStatus, eDirectoryType, lpPathBuffer, cchPathBufferLength, lpcchRequiredBufferLength);
}
