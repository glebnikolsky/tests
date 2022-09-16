#include "stdafx.h"
#include "TestAuth.h"

#include <boost/format.hpp>





namespace TechnologiesTable{
	CString check_objects_sql = _T( "\
SELECT COALESCE( OBJECT_ID( '[fct].[ccp_GeneratePressXml]' ), 0 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[ccp_NewPressOrderServer]' ), 1 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[GetState]' ), 2 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[NoOfPdfVariants]' ), 3 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[NoOfPdfVariantsNew]' ), 4 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[PdfNameForPrint]' ), 5 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[PdfNameForPrintNew]' ), 6 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[vRegionQueries]' ), 7 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[vRegionQueriesNew]' ), 8 )\n\
	UNION\n\
SELECT COALESCE( OBJECT_ID( '[fct].[vRegionQueriesService]' ), 9 )\n\
	UNION\n\
SELECT COALESCE( (SELECT TOP 1 OBJECT_ID FROM sys.columns WHERE OBJECT_ID = OBJECT_ID( 'fct.Technologies' )\n\
		AND NAME IN( 'UsePDFSuffix_p', 'IsCDTech', 'IsPaper', 'IsBrailWrt', 'IsBrailOral' ) AND OBJECT_ID IS NULL ), 10 )\n\
	UNION\n\
SELECT CASE (SELECT name FROM sys.foreign_keys WHERE name LIKE 'FK__LoadFile__Techno%') WHEN NULL THEN -1 ELSE 11 END\n\
	UNION\n\
SELECT SELECT COUNT(*)+12 FROM dbo.tom_TomObjects" );

	CString errs[] = {
		"Отсутствует процедура [fct].[ccp_GeneratePressXml]\n",
		"Отсутствует процедура [fct].[ccp_NewPressOrderServer]\n",
		"Отсутствует функция [fct].[GetState]\n",
		"Отсутствует функция [fct].[NoOfPdfVariants]\n",
		"Отсутствует функция [fct].[NoOfPdfVariantsNew]\n",
		"Отсутствует функция [fct].[PdfNameForPrint]\n",
		"Отсутствует функция [fct].[PdfNameForPrintNew]\n",
		"Отсутствует представление [fct].[vRegionQueries]\n",
		"Отсутствует представление [fct].[vRegionQueriesNew]\n",
		"Отсутствует представление [fct].[vRegionQueriesService]\n",
		"Таблица [fct].[Technologies] не содержит требуемые поля\n",
		"Требуется удалить референс вида 'FK__LoadFile__Techno%'\n"
		"Таблица dbo.tom_TomObjects не заполнена\n"
	};

	std::pair<bool, CString> CheckDB( CDatabase &db )
	{
		std::string sql;
		CRecordset rs( &db );
		CString db_error;
		try{
			rs.Open( CRecordset::forwardOnly, check_objects_sql );
			for ( ; !rs.IsEOF(); rs.MoveNext() ){
				CDBVariant val;
				rs.GetFieldValue( (short)0, val );
				if ( val.m_iVal >= 0 && val.m_iVal <= 12 ) db_error += errs[val.m_iVal];
			}
		}
		catch ( CDBException *ex ){
			CString err = check_objects_sql + "\n";
			err += ex->m_strError.GetString();
			boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
			BOOST_LOG_SEV( lg, boost::log::trivial::error ) << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << err;
			ex->Delete();
			return std::make_pair(false,err);
		}
		return std::make_pair( db_error.IsEmpty(), db_error );
	}

	// Вставим недостающие данные из rbdc_ExamTechnologies
	void FillRows( CDatabase &db )
	{
		std::string sql = _T( "\
	INSERT INTO fct.Technologies(TechnologyCode, TechnologyName)\n\
	SELECT TechnologyCode, TechnologyName FROM dbo.rbdc_ExamTechnologies AS T WHERE TechnologyCode NOT IN( SELECT TechnologyCode FROM fct.Technologies )" );
		try{
			db.ExecuteSQL( sql.c_str() );
		}
		catch ( CDBException *ex ){
			std::string err = sql + "\n";
			err += ex->m_strError.GetString();
			boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
			BOOST_LOG_SEV( lg, boost::log::trivial::error ) << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << ex->m_strError << "\n";
			ex->Delete();
		}
		return;
	}

		// Проверим, что все строки заполнены
	std::pair<bool, CString> IsAllRowsFilled( CDatabase *db )
	{
		std::string sql = _T( "\
	SELECT COUNT(*) FROM fct.Technologies \n\
	WHERE UsePDFSuffix_p|IsCDTech|IsPaper|IsBrailWrt|IsBrailOral = 0");
		CRecordset rs( db );
		try{
			rs.Open( CRecordset::forwardOnly, sql.c_str() );
			CDBVariant val;
			rs.GetFieldValue( (short)0, val );
			return std::make_pair( val.m_lVal == 0, CString( _T("Таблица fct.Technologies заполнена не полностью!")) );
		}
		catch ( CDBException *ex ){
			CString  err = sql.c_str();
			err += "\n" + ex->m_strError;
			boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
			BOOST_LOG_SEV( lg, boost::log::trivial::error ) << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << err << "\n";
			ex->Delete();
			return std::make_pair( false, err );
		}
	}
}
