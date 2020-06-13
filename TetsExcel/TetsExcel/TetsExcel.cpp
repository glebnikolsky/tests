// TetsExcel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TetsExcel.h"
#include "Excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
            using namespace Excel;
            CString sTmp, sComment;


            //	экспортируем
            CApplication cExcel = NULL;
            try
            {
                //	запускаем Excel
                if(!cExcel.CreateDispatch(_T("Excel.Application")))
                {
                    ::AfxMessageBox(_T("Ошибка запуска приложения Excel."), MB_ICONSTOP);
                    return;
                }


                //	отключаем сообщения
                cExcel.PutDisplayAlerts(FALSE);


                //	проверяем наличие папок и шаблонных файлов
                if(_access(sTemplateFileName, 0) != 0)
                {
                    sComment.Format("Не нашел файл  '%s' .", sTemplateFileName);
                    AfxMessageBox(sComment);
                    return;
                }


                //	добавляем книгу
                CWorkbooks cBooks = cExcel.GetWorkbooks();

                CWorkbook  cBook  = cBooks.Open(sTemplateFileName);

                //	получаем листы
                CWorksheets cSheets = cBook.GetWorksheets();
                CWorksheet  cSheet  = cSheets.GetItem(COleVariant((SHORT)1));

                //	переменные
                LONG        cIndex[] = { 0 , 0 };
                COleVariant vtValue  = COleVariant();


                //	получаем диапазон
                CRange cParticipantRange =
                    cSheet.GetRange(Excel::ToRange(2, 8 + 1), Excel::ToRange(12, nCount + 8));


		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
