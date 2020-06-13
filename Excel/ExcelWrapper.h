//	Файл:		ExcelWrapper.h
//	Описание:	Компоненты диалогов и окон для апелляций и коррекций  
//	(с) 2013 Федеральный центр тестирования

#pragma once
#ifndef __FTCCONTROLAC_EXCELWARAPPER_H_INCLUDED
#define __FTCCONTROLAC_EXCELWARAPPER_H_INCLUDED

//	EXCEL Wrapper
namespace ExcelWrapper
{
	//	Класс:	СApplication
	class CApplication : public COleDispatchDriver
	{
	public:
		CApplication() { }
		CApplication(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CApplication(const CApplication& cApplication) : COleDispatchDriver(cApplication) { }

	public:
		IDispatch* GetWorkbooks()
		{
			IDispatch* result = NULL;
			InvokeHelper(0x23c, DISPATCH_PROPERTYGET, VT_DISPATCH, (VOID*)&result, NULL);
			return result;
		}
		VOID PutDisplayAlerts(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL;
			InvokeHelper(0x157, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		VOID Quit()
		{
			InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		}
	};

	//	Класс:	CWorkbooks
	class CWorkbooks : public COleDispatchDriver
	{
	public:
		CWorkbooks() { }
		CWorkbooks(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CWorkbooks(const CWorkbooks& cWorkBooks) : COleDispatchDriver(cWorkBooks) { }
	
	public:
		IDispatch* Add(VARIANT Template)
		{
			IDispatch* result = NULL;
			static BYTE parms[] = VTS_VARIANT;
			InvokeHelper(0xb5, DISPATCH_METHOD, VT_DISPATCH, (VOID*)&result, parms, &Template);
			return result;
		}
		IDispatch* Open(LPCTSTR Filename, VARIANT UpdateLinks = vtMissing, VARIANT ReadOnly = vtMissing, VARIANT Format = vtMissing, VARIANT Password = vtMissing, VARIANT WriteResPassword = vtMissing, VARIANT IgnoreReadOnlyRecommended = vtMissing, VARIANT Origin = vtMissing, VARIANT Delimiter = vtMissing, VARIANT Editable = vtMissing, VARIANT Notify = vtMissing, VARIANT Converter = vtMissing, VARIANT AddToMru = vtMissing)
		{
			IDispatch* result = NULL;
			static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
			InvokeHelper(0x2aa, DISPATCH_METHOD, VT_DISPATCH, (VOID*)&result, parms, Filename, &UpdateLinks, &ReadOnly, &Format, &Password, &WriteResPassword, &IgnoreReadOnlyRecommended, &Origin, &Delimiter, &Editable, &Notify, &Converter, &AddToMru);
			return result;
		}

        void Close()
        {
            InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
        }
	};

	//	Класс:	CWorkbook
	class CWorkbook : public COleDispatchDriver
	{
	public:
		CWorkbook() { }
		CWorkbook(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CWorkbook(const CWorkbook& cWorkbook) : COleDispatchDriver(cWorkbook) { }

	public:
		VOID PrintOut(VARIANT From = vtMissing, VARIANT To = vtMissing, VARIANT Copies = vtMissing, VARIANT Preview = vtMissing, VARIANT ActivePrinter = vtMissing, VARIANT PrintToFile = vtMissing, VARIANT Collate = vtMissing)
		{
			static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
			InvokeHelper(0x389, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate);
		}
		VOID SaveAs(VARIANT Filename, VARIANT FileFormat = vtMissing, VARIANT Password = vtMissing, VARIANT WriteResPassword = vtMissing, VARIANT ReadOnlyRecommended = vtMissing, VARIANT CreateBackup = vtMissing, LONG AccessMode = 1, VARIANT ConflictResolution = vtMissing, VARIANT AddToMru = vtMissing, VARIANT TextCodepage = vtMissing, VARIANT TextVisualLayout = vtMissing)
		{
			static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
			InvokeHelper(0x11c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename, &FileFormat, &Password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, AccessMode, &ConflictResolution, &AddToMru, &TextCodepage, &TextVisualLayout);
		}
		IDispatch* GetWorksheets()
		{
			IDispatch* result = NULL;
			InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (VOID*)&result, NULL);
			return result;
		}
        void Close(VARIANT SaveChanges, VARIANT Filename, VARIANT RouteWorkbook)
        {
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SaveChanges, &Filename, &RouteWorkbook);
        }
	};

	//	Класс:	CWorksheets
	class CWorksheets : public COleDispatchDriver
	{
	public:
		CWorksheets() { }
		CWorksheets(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CWorksheets(const CWorksheets& cWorksheets) : COleDispatchDriver(cWorksheets) { }
	public:
		IDispatch* GetItem(VARIANT Index)
		{
			IDispatch* result = NULL;
			static BYTE parms[] = VTS_VARIANT ;
			InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (VOID*)&result, parms, &Index);
			return result;
		}

        LPDISPATCH get_Application()
        {
            LPDISPATCH result;
            InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        long get_Creator()
        {
            long result;
            InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_Parent()
        {
            LPDISPATCH result;
            InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH Add(VARIANT Before, VARIANT After, VARIANT Count, VARIANT Type)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0xb5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Before, &After, &Count, &Type);
            return result;
        }
        void Copy(VARIANT Before, VARIANT After)
        {
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0x227, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Before, &After);
        }
        long get_Count()
        {
            long result;
            InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        void Delete()
        {
            InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
        }
        void FillAcrossSheets(LPDISPATCH Range, long Type)
        {
            static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
            InvokeHelper(0x1d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Range, Type);
        }
        LPDISPATCH get_Item(VARIANT Index)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_VARIANT ;
            InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
            return result;
        }
        void Move(VARIANT Before, VARIANT After)
        {
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0x27d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Before, &After);
        }
        LPUNKNOWN get__NewEnum()
        {
            LPUNKNOWN result;
            InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
            return result;
        }
        void _PrintOut(VARIANT From, VARIANT To, VARIANT Copies, VARIANT Preview, VARIANT ActivePrinter, VARIANT PrintToFile, VARIANT Collate)
        {
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0x389, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate);
        }
        void PrintPreview(VARIANT EnableChanges)
        {
            static BYTE parms[] = VTS_VARIANT ;
            InvokeHelper(0x119, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &EnableChanges);
        }
        void Select(VARIANT Replace)
        {
            static BYTE parms[] = VTS_VARIANT ;
            InvokeHelper(0xeb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Replace);
        }
        LPDISPATCH get_HPageBreaks()
        {
            LPDISPATCH result;
            InvokeHelper(0x58a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_VPageBreaks()
        {
            LPDISPATCH result;
            InvokeHelper(0x58b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        VARIANT get_Visible()
        {
            VARIANT result;
            InvokeHelper(0x22e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
            return result;
        }
        void put_Visible(VARIANT newValue)
        {
            static BYTE parms[] = VTS_VARIANT ;
            InvokeHelper(0x22e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
        }
        LPDISPATCH get__Default(VARIANT Index)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_VARIANT ;
            InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
            return result;
        }
        void PrintOut(VARIANT From, VARIANT To, VARIANT Copies, VARIANT Preview, VARIANT ActivePrinter, VARIANT PrintToFile, VARIANT Collate, VARIANT PrToFileName)
        {
            static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
            InvokeHelper(0x6ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
        }

	};

	//	Класс:	CWorksheet
	class CWorksheet : public COleDispatchDriver
	{
	public:
		CWorksheet() { }
		CWorksheet(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CWorksheet(const CWorksheet& cWorksheet) : COleDispatchDriver(cWorksheet) { }
	public:
		CString GetName()
		{
			CString result = _T("");
			InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (VOID*)&result, NULL);
			return result;
		}
		VOID PutName(const CString& Value)
		{
			static BYTE parms[] = VTS_BSTR;
			InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Value);
		}
		IDispatch* GetRange(VARIANT Cell1, VARIANT Cell2)
		{
			IDispatch* result = NULL;
			static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
			InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (VOID*)&result, parms, &Cell1, &Cell2);
			return result;
		}

		LPDISPATCH get_Columns()
		{
			LPDISPATCH result;
			InvokeHelper(0xf1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_PageSetup()
		{
			LPDISPATCH result;
			InvokeHelper(0x3e6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
        VOID UnprotectSheet( const VARIANT &sPwd ){
            DISPID dispID = 0x011d;
            static BYTE parms[] = VTS_VARIANT;
            InvokeHelper(dispID, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &sPwd );
            return;
        }

	};

	//	Класс:	CRange
	class CRange : public COleDispatchDriver
	{
	public:
		CRange() { }
		CRange(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CRange(const CRange& cRange) : COleDispatchDriver(cRange) { }
	public:
	
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Activate()
	{
		VARIANT result;
		InvokeHelper(0x130, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_AddIndent()
	{
		VARIANT result;
		InvokeHelper(0x427, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AddIndent(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x427, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	CString get_Address(VARIANT RowAbsolute, VARIANT ColumnAbsolute, long ReferenceStyle, VARIANT External, VARIANT RelativeTo)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xec, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
		return result;
	}
	CString get_AddressLocal(VARIANT RowAbsolute, VARIANT ColumnAbsolute, long ReferenceStyle, VARIANT External, VARIANT RelativeTo)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1b5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
		return result;
	}
	VARIANT AdvancedFilter(long Action, VARIANT CriteriaRange, VARIANT CopyToRange, VARIANT Unique)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x36c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Action, &CriteriaRange, &CopyToRange, &Unique);
		return result;
	}
	VARIANT ApplyNames(VARIANT Names, VARIANT IgnoreRelativeAbsolute, VARIANT UseRowColumnNames, VARIANT OmitColumn, VARIANT OmitRow, long Order, VARIANT AppendLast)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x1b9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Names, &IgnoreRelativeAbsolute, &UseRowColumnNames, &OmitColumn, &OmitRow, Order, &AppendLast);
		return result;
	}
	VARIANT ApplyOutlineStyles()
	{
		VARIANT result;
		InvokeHelper(0x1c0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Areas()
	{
		LPDISPATCH result;
		InvokeHelper(0x238, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString AutoComplete(LPCTSTR String)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4a1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, String);
		return result;
	}
	VARIANT AutoFill(LPDISPATCH Destination, long Type)
	{
		VARIANT result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x1c1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Destination, Type);
		return result;
	}
	VARIANT AutoFilter(VARIANT Field, VARIANT Criteria1, long Operator, VARIANT Criteria2, VARIANT VisibleDropDown)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x319, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Field, &Criteria1, Operator, &Criteria2, &VisibleDropDown);
		return result;
	}
	VARIANT AutoFit()
	{
		VARIANT result;
		InvokeHelper(0xed, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT AutoFormat(long Format, VARIANT Number, VARIANT Font, VARIANT Alignment, VARIANT Border, VARIANT Pattern, VARIANT Width)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x72, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Format, &Number, &Font, &Alignment, &Border, &Pattern, &Width);
		return result;
	}
	VARIANT AutoOutline()
	{
		VARIANT result;
		InvokeHelper(0x40c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT BorderAround(VARIANT LineStyle, long Weight, long ColorIndex, VARIANT Color)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x42b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &LineStyle, Weight, ColorIndex, &Color);
		return result;
	}
	LPDISPATCH get_Borders()
	{
		LPDISPATCH result;
		InvokeHelper(0x1b3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Calculate()
	{
		VARIANT result;
		InvokeHelper(0x117, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Cells()
	{
		LPDISPATCH result;
		InvokeHelper(0xee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Characters(VARIANT Start, VARIANT Length)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x25b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Start, &Length);
		return result;
	}
	VARIANT CheckSpelling(VARIANT CustomDictionary, VARIANT IgnoreUppercase, VARIANT AlwaysSuggest, VARIANT SpellLang)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1f9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &CustomDictionary, &IgnoreUppercase, &AlwaysSuggest, &SpellLang);
		return result;
	}
	VARIANT Clear()
	{
		VARIANT result;
		InvokeHelper(0x6f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearContents()
	{
		VARIANT result;
		InvokeHelper(0x71, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearFormats()
	{
		VARIANT result;
		InvokeHelper(0x70, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearNotes()
	{
		VARIANT result;
		InvokeHelper(0xef, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearOutline()
	{
		VARIANT result;
		InvokeHelper(0x40d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_Column()
	{
		long result;
		InvokeHelper(0xf0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH ColumnDifferences(VARIANT Comparison)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1fe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Comparison);
		return result;
	}
	LPDISPATCH get_Columns()
	{
		LPDISPATCH result;
		InvokeHelper(0xf1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_ColumnWidth()
	{
		VARIANT result;
		InvokeHelper(0xf2, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ColumnWidth(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xf2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT Consolidate(VARIANT Sources, VARIANT Function, VARIANT TopRow, VARIANT LeftColumn, VARIANT CreateLinks)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1e2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Sources, &Function, &TopRow, &LeftColumn, &CreateLinks);
		return result;
	}
	VARIANT Copy(VARIANT Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x227, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination);
		return result;
	}
	long CopyFromRecordset(LPUNKNOWN Data, VARIANT MaxRows, VARIANT MaxColumns)
	{
		long result;
		static BYTE parms[] = VTS_UNKNOWN VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x480, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Data, &MaxRows, &MaxColumns);
		return result;
	}
	VARIANT CopyPicture(long Appearance, long Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd5, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Appearance, Format);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT CreateNames(VARIANT Top, VARIANT Left, VARIANT Bottom, VARIANT Right)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1c9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Top, &Left, &Bottom, &Right);
		return result;
	}
	VARIANT CreatePublisher(VARIANT Edition, long Appearance, VARIANT ContainsPICT, VARIANT ContainsBIFF, VARIANT ContainsRTF, VARIANT ContainsVALU)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1ca, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Edition, Appearance, &ContainsPICT, &ContainsBIFF, &ContainsRTF, &ContainsVALU);
		return result;
	}
	LPDISPATCH get_CurrentArray()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CurrentRegion()
	{
		LPDISPATCH result;
		InvokeHelper(0xf3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Cut(VARIANT Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x235, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination);
		return result;
	}
	VARIANT DataSeries(VARIANT Rowcol, long Type, long Date, VARIANT Step, VARIANT Stop, VARIANT Trend)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1d0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Rowcol, Type, Date, &Step, &Stop, &Trend);
		return result;
	}
	VARIANT get__Default(VARIANT RowIndex, VARIANT ColumnIndex)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RowIndex, &ColumnIndex);
		return result;
	}
	void put__Default(VARIANT RowIndex, VARIANT ColumnIndex, VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RowIndex, &ColumnIndex, &newValue);
	}
	VARIANT Delete(VARIANT Shift)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x75, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Shift);
		return result;
	}
	LPDISPATCH get_Dependents()
	{
		LPDISPATCH result;
		InvokeHelper(0x21f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT _DialogBox()
	{
		VARIANT result;
		InvokeHelper(0xf5, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DirectDependents()
	{
		LPDISPATCH result;
		InvokeHelper(0x221, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DirectPrecedents()
	{
		LPDISPATCH result;
		InvokeHelper(0x222, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT EditionOptions(long Type, long Option, VARIANT Name, VARIANT Reference, long Appearance, long ChartSize, VARIANT Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x46b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Type, Option, &Name, &Reference, Appearance, ChartSize, &Format);
		return result;
	}
	LPDISPATCH get_End(long Direction)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Direction);
		return result;
	}
	LPDISPATCH get_EntireColumn()
	{
		LPDISPATCH result;
		InvokeHelper(0xf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EntireRow()
	{
		LPDISPATCH result;
		InvokeHelper(0xf7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT FillDown()
	{
		VARIANT result;
		InvokeHelper(0xf8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillLeft()
	{
		VARIANT result;
		InvokeHelper(0xf9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillRight()
	{
		VARIANT result;
		InvokeHelper(0xfa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillUp()
	{
		VARIANT result;
		InvokeHelper(0xfb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Find(VARIANT What, VARIANT After, VARIANT LookIn, VARIANT LookAt, VARIANT SearchOrder, long SearchDirection, VARIANT MatchCase, VARIANT MatchByte, VARIANT SearchFormat)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x18e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &What, &After, &LookIn, &LookAt, &SearchOrder, SearchDirection, &MatchCase, &MatchByte, &SearchFormat);
		return result;
	}
	LPDISPATCH FindNext(VARIANT After)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x18f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &After);
		return result;
	}
	LPDISPATCH FindPrevious(VARIANT After)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x190, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &After);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x92, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Formula()
	{
		VARIANT result;
		InvokeHelper(0x105, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Formula(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaArray()
	{
		VARIANT result;
		InvokeHelper(0x24a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaArray(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x24a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_FormulaLabel()
	{
		long result;
		InvokeHelper(0x564, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLabel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x564, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_FormulaHidden()
	{
		VARIANT result;
		InvokeHelper(0x106, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaHidden(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x106, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaLocal()
	{
		VARIANT result;
		InvokeHelper(0x107, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLocal(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x107, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaR1C1()
	{
		VARIANT result;
		InvokeHelper(0x108, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x108, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaR1C1Local()
	{
		VARIANT result;
		InvokeHelper(0x109, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1Local(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x109, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT FunctionWizard()
	{
		VARIANT result;
		InvokeHelper(0x23b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL GoalSeek(VARIANT Goal, LPDISPATCH ChangingCell)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT VTS_DISPATCH ;
		InvokeHelper(0x1d8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &Goal, ChangingCell);
		return result;
	}
	VARIANT Group(VARIANT Start, VARIANT End, VARIANT By, VARIANT Periods)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Start, &End, &By, &Periods);
		return result;
	}
	VARIANT get_HasArray()
	{
		VARIANT result;
		InvokeHelper(0x10a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_HasFormula()
	{
		VARIANT result;
		InvokeHelper(0x10b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Height()
	{
		VARIANT result;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Hidden()
	{
		VARIANT result;
		InvokeHelper(0x10c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Hidden(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_HorizontalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x88, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_HorizontalAlignment(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x88, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_IndentLevel()
	{
		VARIANT result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_IndentLevel(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void InsertIndent(long InsertAmount)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x565, DISPATCH_METHOD, VT_EMPTY, NULL, parms, InsertAmount);
	}
	VARIANT Insert(VARIANT Shift, VARIANT CopyOrigin)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xfc, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Shift, &CopyOrigin);
		return result;
	}
	LPDISPATCH get_Interior()
	{
		LPDISPATCH result;
		InvokeHelper(0x81, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Item(VARIANT RowIndex, VARIANT ColumnIndex)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RowIndex, &ColumnIndex);
		return result;
	}
	void put_Item(VARIANT RowIndex, VARIANT ColumnIndex, VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RowIndex, &ColumnIndex, &newValue);
	}
	VARIANT Justify()
	{
		VARIANT result;
		InvokeHelper(0x1ef, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Left()
	{
		VARIANT result;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_ListHeaderRows()
	{
		long result;
		InvokeHelper(0x4a3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT ListNames()
	{
		VARIANT result;
		InvokeHelper(0xfd, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_LocationInTable()
	{
		long result;
		InvokeHelper(0x2b3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Locked()
	{
		VARIANT result;
		InvokeHelper(0x10d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Locked(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void Merge(VARIANT Across)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x234, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Across);
	}
	void UnMerge()
	{
		InvokeHelper(0x568, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_MergeArea()
	{
		LPDISPATCH result;
		InvokeHelper(0x569, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_MergeCells()
	{
		VARIANT result;
		InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_MergeCells(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Name()
	{
		VARIANT result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Name(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT NavigateArrow(VARIANT TowardPrecedent, VARIANT ArrowNumber, VARIANT LinkNumber)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x408, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &TowardPrecedent, &ArrowNumber, &LinkNumber);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Next()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString NoteText(VARIANT Text, VARIANT Start, VARIANT Length)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x467, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &Text, &Start, &Length);
		return result;
	}
	VARIANT get_NumberFormat()
	{
		VARIANT result;
		InvokeHelper(0xc1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormat(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_NumberFormatLocal()
	{
		VARIANT result;
		InvokeHelper(0x449, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLocal(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x449, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Offset(VARIANT RowOffset, VARIANT ColumnOffset)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xfe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &RowOffset, &ColumnOffset);
		return result;
	}
	VARIANT get_Orientation()
	{
		VARIANT result;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_OutlineLevel()
	{
		VARIANT result;
		InvokeHelper(0x10f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_OutlineLevel(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_PageBreak()
	{
		long result;
		InvokeHelper(0xff, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PageBreak(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xff, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Parse(VARIANT ParseLine, VARIANT Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1dd, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &ParseLine, &Destination);
		return result;
	}
	VARIANT _PasteSpecial(long Paste, long Operation, VARIANT SkipBlanks, VARIANT Transpose)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x403, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Paste, Operation, &SkipBlanks, &Transpose);
		return result;
	}
	LPDISPATCH get_PivotField()
	{
		LPDISPATCH result;
		InvokeHelper(0x2db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotItem()
	{
		LPDISPATCH result;
		InvokeHelper(0x2e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotTable()
	{
		LPDISPATCH result;
		InvokeHelper(0x2cc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Precedents()
	{
		LPDISPATCH result;
		InvokeHelper(0x220, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_PrefixCharacter()
	{
		VARIANT result;
		InvokeHelper(0x1f8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Previous()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT _PrintOut(VARIANT From, VARIANT To, VARIANT Copies, VARIANT Preview, VARIANT ActivePrinter, VARIANT PrintToFile, VARIANT Collate)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x389, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate);
		return result;
	}
	VARIANT PrintPreview(VARIANT EnableChanges)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x119, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &EnableChanges);
		return result;
	}
	LPDISPATCH get_QueryTable()
	{
		LPDISPATCH result;
		InvokeHelper(0x56a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Range(VARIANT Cell1, VARIANT Cell2)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Cell1, &Cell2);
		return result;
	}
	VARIANT RemoveSubtotal()
	{
		VARIANT result;
		InvokeHelper(0x373, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL Replace(VARIANT What, VARIANT Replacement, VARIANT LookAt, VARIANT SearchOrder, VARIANT MatchCase, VARIANT MatchByte, VARIANT SearchFormat, VARIANT ReplaceFormat)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xe2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &What, &Replacement, &LookAt, &SearchOrder, &MatchCase, &MatchByte, &SearchFormat, &ReplaceFormat);
		return result;
	}
	LPDISPATCH get_Resize(VARIANT RowSize, VARIANT ColumnSize)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &RowSize, &ColumnSize);
		return result;
	}
	long get_Row()
	{
		long result;
		InvokeHelper(0x101, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH RowDifferences(VARIANT Comparison)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1ff, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Comparison);
		return result;
	}
	VARIANT get_RowHeight()
	{
		VARIANT result;
		InvokeHelper(0x110, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_RowHeight(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x110, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Rows()
	{
		LPDISPATCH result;
		InvokeHelper(0x102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Run(VARIANT Arg1, VARIANT Arg2, VARIANT Arg3, VARIANT Arg4, VARIANT Arg5, VARIANT Arg6, VARIANT Arg7, VARIANT Arg8, VARIANT Arg9, VARIANT Arg10, VARIANT Arg11, VARIANT Arg12, VARIANT Arg13, VARIANT Arg14, VARIANT Arg15, VARIANT Arg16, VARIANT Arg17, VARIANT Arg18, VARIANT Arg19, VARIANT Arg20, VARIANT Arg21, VARIANT Arg22, VARIANT Arg23, VARIANT Arg24, VARIANT Arg25, VARIANT Arg26, VARIANT Arg27, VARIANT Arg28, VARIANT Arg29, VARIANT Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0xeb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Show()
	{
		VARIANT result;
		InvokeHelper(0x1f0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ShowDependents(VARIANT Remove)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x36d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Remove);
		return result;
	}
	VARIANT get_ShowDetail()
	{
		VARIANT result;
		InvokeHelper(0x249, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ShowDetail(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x249, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT ShowErrors()
	{
		VARIANT result;
		InvokeHelper(0x36e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ShowPrecedents(VARIANT Remove)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x36f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Remove);
		return result;
	}
	VARIANT get_ShrinkToFit()
	{
		VARIANT result;
		InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ShrinkToFit(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT Sort(VARIANT Key1, long Order1, VARIANT Key2, VARIANT Type, long Order2, VARIANT Key3, long Order3, long Header, VARIANT OrderCustom, VARIANT MatchCase, long Orientation, long SortMethod, long DataOption1, long DataOption2, long DataOption3)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x370, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Key1, Order1, &Key2, &Type, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, SortMethod, DataOption1, DataOption2, DataOption3);
		return result;
	}
	VARIANT SortSpecial(long SortMethod, VARIANT Key1, long Order1, VARIANT Type, VARIANT Key2, long Order2, VARIANT Key3, long Order3, long Header, VARIANT OrderCustom, VARIANT MatchCase, long Orientation, long DataOption1, long DataOption2, long DataOption3)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x371, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SortMethod, &Key1, Order1, &Type, &Key2, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, DataOption1, DataOption2, DataOption3);
		return result;
	}
	LPDISPATCH get_SoundNote()
	{
		LPDISPATCH result;
		InvokeHelper(0x394, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH SpecialCells(long Type, VARIANT Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x19a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Type, &Value);
		return result;
	}
	VARIANT get_Style()
	{
		VARIANT result;
		InvokeHelper(0x104, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Style(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT SubscribeTo(LPCTSTR Edition, long Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1e1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Edition, Format);
		return result;
	}
	VARIANT Subtotal(long GroupBy, long Function, VARIANT TotalList, VARIANT Replace, VARIANT PageBreaks, long SummaryBelowData)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x372, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, GroupBy, Function, &TotalList, &Replace, &PageBreaks, SummaryBelowData);
		return result;
	}
	VARIANT get_Summary()
	{
		VARIANT result;
		InvokeHelper(0x111, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Table(VARIANT RowInput, VARIANT ColumnInput)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1f1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &RowInput, &ColumnInput);
		return result;
	}
	VARIANT get_Text()
	{
		VARIANT result;
		InvokeHelper(0x8a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT TextToColumns(VARIANT Destination, long DataType, long TextQualifier, VARIANT ConsecutiveDelimiter, VARIANT Tab, VARIANT Semicolon, VARIANT Comma, VARIANT Space, VARIANT Other, VARIANT OtherChar, VARIANT FieldInfo, VARIANT DecimalSeparator, VARIANT ThousandsSeparator, VARIANT TrailingMinusNumbers)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x410, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination, DataType, TextQualifier, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers);
		return result;
	}
	VARIANT get_Top()
	{
		VARIANT result;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Ungroup()
	{
		VARIANT result;
		InvokeHelper(0xf4, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_UseStandardHeight()
	{
		VARIANT result;
		InvokeHelper(0x112, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_UseStandardHeight(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_UseStandardWidth()
	{
		VARIANT result;
		InvokeHelper(0x113, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_UseStandardWidth(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x113, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Validation()
	{
		LPDISPATCH result;
		InvokeHelper(0x56b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Value(VARIANT RangeValueDataType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RangeValueDataType);
		return result;
	}
	void put_Value(VARIANT RangeValueDataType, VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RangeValueDataType, &newValue);
	}
	VARIANT get_Value2()
	{
		VARIANT result;
		InvokeHelper(0x56c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Value2(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x56c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_VerticalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_VerticalAlignment(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x89, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Width()
	{
		VARIANT result;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Worksheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x15c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_WrapText()
	{
		VARIANT result;
		InvokeHelper(0x114, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_WrapText(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH AddComment(VARIANT Text)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x56d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Text);
		return result;
	}
	LPDISPATCH get_Comment()
	{
		LPDISPATCH result;
		InvokeHelper(0x38e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ClearComments()
	{
		InvokeHelper(0x56e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Phonetic()
	{
		LPDISPATCH result;
		InvokeHelper(0x56f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_FormatConditions()
	{
		LPDISPATCH result;
		InvokeHelper(0x570, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x3cf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3cf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Hyperlinks()
	{
		LPDISPATCH result;
		InvokeHelper(0x571, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Phonetics()
	{
		LPDISPATCH result;
		InvokeHelper(0x713, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetPhonetic()
	{
		InvokeHelper(0x714, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_ID()
	{
		CString result;
		InvokeHelper(0x715, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x715, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT PrintOut(VARIANT From, VARIANT To, VARIANT Copies, VARIANT Preview, VARIANT ActivePrinter, VARIANT PrintToFile, VARIANT Collate, VARIANT PrToFileName)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6ec, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
		return result;
	}
	LPDISPATCH get_PivotCell()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Dirty()
	{
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Errors()
	{
		LPDISPATCH result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SmartTags()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Speak(VARIANT SpeakDirection, VARIANT SpeakFormulas)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SpeakDirection, &SpeakFormulas);
	}
	VARIANT PasteSpecial(long Paste, long Operation, VARIANT SkipBlanks, VARIANT Transpose)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x788, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Paste, Operation, &SkipBlanks, &Transpose);
		return result;
	}
	BOOL get_AllowEdit()
	{
		BOOL result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ListObject()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_XPath()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
    // BAV: новое 07.06.12
    //void SetStyle(const VARIANT& newValue)
    //{
    //    static BYTE parms[] = VTS_VARIANT;
    //    InvokeHelper(0x104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
    //}
	};
	
	//	Класс:	CBorders
	class CBorders : public COleDispatchDriver
	{
	public:
		CBorders() { }
		CBorders(IDispatch* pDispatch) : COleDispatchDriver(pDispatch) { }
		CBorders(const CBorders& cBorders) : COleDispatchDriver(cBorders) { }
	public:
		VARIANT GetValue()
		{
			VARIANT result;
			InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (VOID*)&result, NULL);
			return result;
		}
		VOID PutValue(VARIANT Value)
		{
			static BYTE parms[] = VTS_VARIANT;
			InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Value);
		}
		VARIANT GetWeight()
		{
			VARIANT result;
			InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_VARIANT, (VOID*)&result, NULL);
			return result;
		}
		VOID PutWeight(VARIANT Value)
		{
			static BYTE parms[] = VTS_VARIANT;
			InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Value);
		}
	};

	//	получение диапазона
	inline COleVariant ToRange(LONG I, LONG J)
	{
		CString Result = _T(""), First  = _T(""), Second = _T("");
		if(I % 26 == 0)
		{
			First  = _T("Z");
			Second = CString((TCHAR)('A' + I / 26 - 2)); 
		}
		else
		{
			First  = CString((TCHAR)('A' + I % 26 - 1));
			Second = CString((TCHAR)('A' + I / 26 - 1)); 
		}

		if(I > 26)
			Result.Format("%s%s%d", Second.GetString(), First.GetString(), J);
		else
			Result.Format("%s%d", First.GetString(), J);

		return COleVariant(Result);
	}

	class CPageSetup : public COleDispatchDriver
	{
	public:
		CPageSetup() {}
		CPageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
		CPageSetup(const CPageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	public:
		LPDISPATCH get_Application()
		{
			LPDISPATCH result;
			InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		long get_Creator()
		{
			long result;
			InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_Parent()
		{
			LPDISPATCH result;
			InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		BOOL get_BlackAndWhite()
		{
			BOOL result;
			InvokeHelper(0x3f1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_BlackAndWhite(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3f1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		double get_BottomMargin()
		{
			double result;
			InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_BottomMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3ea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_CenterFooter()
		{
			CString result;
			InvokeHelper(0x3f2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_CenterFooter(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3f2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_CenterHeader()
		{
			CString result;
			InvokeHelper(0x3f3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_CenterHeader(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3f3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_CenterHorizontally()
		{
			BOOL result;
			InvokeHelper(0x3ed, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_CenterHorizontally(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3ed, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_CenterVertically()
		{
			BOOL result;
			InvokeHelper(0x3ee, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_CenterVertically(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3ee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_ChartSize()
		{
			long result;
			InvokeHelper(0x3f4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_ChartSize(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x3f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_Draft()
		{
			BOOL result;
			InvokeHelper(0x3fc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_Draft(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3fc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_FirstPageNumber()
		{
			long result;
			InvokeHelper(0x3f0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_FirstPageNumber(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x3f0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		VARIANT get_FitToPagesTall()
		{
			VARIANT result;
			InvokeHelper(0x3f5, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
			return result;
		}
		void put_FitToPagesTall(VARIANT newValue)
		{
			static BYTE parms[] = VTS_VARIANT ;
			InvokeHelper(0x3f5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
		}
		VARIANT get_FitToPagesWide()
		{
			VARIANT result;
			InvokeHelper(0x3f6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
			return result;
		}
		void put_FitToPagesWide(VARIANT newValue)
		{
			static BYTE parms[] = VTS_VARIANT ;
			InvokeHelper(0x3f6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
		}
		double get_FooterMargin()
		{
			double result;
			InvokeHelper(0x3f7, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_FooterMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3f7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		double get_HeaderMargin()
		{
			double result;
			InvokeHelper(0x3f8, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_HeaderMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3f8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_LeftFooter()
		{
			CString result;
			InvokeHelper(0x3f9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_LeftFooter(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3f9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_LeftHeader()
		{
			CString result;
			InvokeHelper(0x3fa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_LeftHeader(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3fa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		double get_LeftMargin()
		{
			double result;
			InvokeHelper(0x3e7, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_LeftMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3e7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_Order()
		{
			long result;
			InvokeHelper(0xc0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_Order(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0xc0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_Orientation()
		{
			long result;
			InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_Orientation(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_PaperSize()
		{
			long result;
			InvokeHelper(0x3ef, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_PaperSize(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x3ef, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_PrintArea()
		{
			CString result;
			InvokeHelper(0x3fb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_PrintArea(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3fb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_PrintGridlines()
		{
			BOOL result;
			InvokeHelper(0x3ec, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_PrintGridlines(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3ec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_PrintHeadings()
		{
			BOOL result;
			InvokeHelper(0x3eb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_PrintHeadings(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3eb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		BOOL get_PrintNotes()
		{
			BOOL result;
			InvokeHelper(0x3fd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
			return result;
		}
		void put_PrintNotes(BOOL newValue)
		{
			static BYTE parms[] = VTS_BOOL ;
			InvokeHelper(0x3fd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		VARIANT get_PrintQuality(VARIANT Index)
		{
			VARIANT result;
			static BYTE parms[] = VTS_VARIANT ;
			InvokeHelper(0x3fe, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
			return result;
		}
		void put_PrintQuality(VARIANT Index, VARIANT newValue)
		{
			static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
			InvokeHelper(0x3fe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Index, &newValue);
		}
		CString get_PrintTitleColumns()
		{
			CString result;
			InvokeHelper(0x3ff, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_PrintTitleColumns(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x3ff, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_PrintTitleRows()
		{
			CString result;
			InvokeHelper(0x400, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_PrintTitleRows(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x400, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_RightFooter()
		{
			CString result;
			InvokeHelper(0x401, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_RightFooter(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x401, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		CString get_RightHeader()
		{
			CString result;
			InvokeHelper(0x402, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
			return result;
		}
		void put_RightHeader(LPCTSTR newValue)
		{
			static BYTE parms[] = VTS_BSTR ;
			InvokeHelper(0x402, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		double get_RightMargin()
		{
			double result;
			InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_RightMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3e8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		double get_TopMargin()
		{
			double result;
			InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
			return result;
		}
		void put_TopMargin(double newValue)
		{
			static BYTE parms[] = VTS_R8 ;
			InvokeHelper(0x3e9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		VARIANT get_Zoom()
		{
			VARIANT result;
			InvokeHelper(0x297, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
			return result;
		}
		void put_Zoom(VARIANT newValue)
		{
			static BYTE parms[] = VTS_VARIANT ;
			InvokeHelper(0x297, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
		}
		long get_PrintComments()
		{
			long result;
			InvokeHelper(0x5f4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_PrintComments(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x5f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		long get_PrintErrors()
		{
			long result;
			InvokeHelper(0x865, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
			return result;
		}
		void put_PrintErrors(long newValue)
		{
			static BYTE parms[] = VTS_I4 ;
			InvokeHelper(0x865, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
		}
		LPDISPATCH get_CenterHeaderPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x866, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_CenterFooterPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x867, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_LeftHeaderPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x868, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_LeftFooterPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x869, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_RightHeaderPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x86a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
		LPDISPATCH get_RightFooterPicture()
		{
			LPDISPATCH result;
			InvokeHelper(0x86b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
			return result;
		}
	};

    class CVPageBreak : public COleDispatchDriver
    {
    public:
        CVPageBreak(){} // Calls COleDispatchDriver default constructor
        CVPageBreak(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
        CVPageBreak(const CVPageBreak& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

        // Attributes
    public:

        // Operations
    public:


        // VPageBreak methods
    public:
        LPDISPATCH get_Application()
        {
            LPDISPATCH result;
            InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        long get_Creator()
        {
            long result;
            InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_Parent()
        {
            LPDISPATCH result;
            InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        void Delete()
        {
            InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
        }
        void DragOff(long Direction, long RegionIndex)
        {
            static BYTE parms[] = VTS_I4 VTS_I4 ;
            InvokeHelper(0x58c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Direction, RegionIndex);
        }
        long get_Type()
        {
            long result;
            InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        void put_Type(long newValue)
        {
            static BYTE parms[] = VTS_I4 ;
            InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
        }
        long get_Extent()
        {
            long result;
            InvokeHelper(0x58e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_Location()
        {
            LPDISPATCH result;
            InvokeHelper(0x575, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        void putref_Location(LPDISPATCH newValue)
        {
            static BYTE parms[] = VTS_DISPATCH ;
            InvokeHelper(0x575, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
        }

        // VPageBreak properties
    public:

    };

    class CVPageBreaks : public COleDispatchDriver
    {
    public:
        CVPageBreaks(){} // Calls COleDispatchDriver default constructor
        CVPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
        CVPageBreaks(const CVPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

        // Attributes
    public:

        // Operations
    public:


        // VPageBreaks methods
    public:
        LPDISPATCH get_Application()
        {
            LPDISPATCH result;
            InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        long get_Creator()
        {
            long result;
            InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_Parent()
        {
            LPDISPATCH result;
            InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
            return result;
        }
        long get_Count()
        {
            long result;
            InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH get_Item(long Index)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_I4 ;
            InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
            return result;
        }
        LPDISPATCH get__Default(long Index)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_I4 ;
            InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
            return result;
        }
        LPUNKNOWN get__NewEnum()
        {
            LPUNKNOWN result;
            InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
            return result;
        }
        LPDISPATCH Add(LPDISPATCH Before)
        {
            LPDISPATCH result;
            static BYTE parms[] = VTS_DISPATCH ;
            InvokeHelper(0xb5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Before);
            return result;
        }

        // VPageBreaks properties
    public:

    };

};

#endif