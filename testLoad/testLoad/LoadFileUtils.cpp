#include "stdafx.h"
#include "loadFileUtils.h"
#include "sha1.h"
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <boost/lexical_cast.hpp>


using namespace std;

namespace {
	///  \struct BP ������ ����� ��� map, ��������� ������ ���� ������� (Box) ��� ����� (Package)
	struct BP{

		/// ����������� ����� ��� ����, ����� �������� touch_, ��������� ��� �������� ����� ����������� line_ � load_id_
		BP( long l =0, long id = 0, long t =0): 
			line_(l), 
			load_id_(id), 
			touch_(t)
		{}

		long line_;			///< ����� ������
		long load_id_;		///< id �����
		long touch_;		///< ���������� �������������
	};

	/// \typedef map BoxMap ��� �������� ����������� �������. ���� - ����� ������� (13 ����)
	typedef map< long long, BP> BoxMap;

	/// \typedef map PackMap ��� �������� ����������� �����. ���� - ����� ����� (8 ����)
	typedef map< long, BP> PackMap;


	/// \enum PackageType ��� ������ (������ �� ���) ������
	enum PackageType{ptKIM_, ptTOM_, ptZOU_, ptOVZ_  };

	/// \enum PackageType ��� �����.
	enum FileType{ftBOX_, ftPACK_, ftKIM_, ftPDF_};

	extern bool StopLoadIfExist[4];

	/// \struct KIMpackage ���������������, ��� ������������ ������ ������
	struct KIMpackage{

		KIMpackage():
			pt_(ptKIM_)
		{} 
		std::string pdfBase_;	///< ������� ��� ������ - ��, ��� � �������: (01_140101_1)_box_1.csv ����� ��� ������ ������������ pdf � ��
		PackageType pt_;		///< ��� ������
		string files_[4];		///< ���� � ������ � ������� ftBOX_, ftPACK_, ftKIM_, pdf
	};

	/// \typedef FilePackageMap -�������� �������� ������ ��� �������� ���������
	typedef map< string, KIMpackage> FilePackageMap;


	/// CalcSHA1 ��������� ����, �������� ���������� � ������, ��������� SHA1
	/**
		\param filename - ������ ����
		\param fbody    - ��������, ���������� �����
		\param hash     - �������� SHA1 
		\return         - false, ���� ���� �� ��������. ��������� ������ ���, ��������� ��� ����� ����������� ������.
	*/
	bool CalcSHA1(const char *filename, string &fbody, string &hash);


	/// GetPackageType ���������� ��� ������ ������
	/**
		\param ptype - ���������� ��� ������. ������ ����������, ��������� ���������� ���������� ����������.
		\return ��� ������ ������ ���� PackageType
	*/
	PackageType GetPackageType(const string &ptype);


	/// GetFileType ���������� ��� �����
	/**
		\param ptype - ���������� ��� �� ����� �����. ������ ����������, ��������� ���������� ���������� ����������. ������ � ������ ��������.
		\param ext - ���������� �����. ������ � ������ ��������.
		\return ��� ����� ���� FileType
	*/
	FileType GetFileType(const string &ftype, const string &ext);


	/// CheckEAN13 ��������� ����������� ����� EAN13
	/**
		\param ean - 13-�� ��������� ���
		\return bool - �������� ������, false - ���
	*/
	bool CheckEAN13(char const *ean);

	/// CreateFileList ������ ������ ������� (box, pack, kim) ������ ��� ���������� ���������
	/**
		\param srchDir - ���� ��� ������ ������
		\param dataFiles - �������� - ������ �������
		\param expr - ���������� ��������� ��� ������ ������, ��������
	*/
	void CreateFileList(TCHAR const *srchDir, FilePackageMap &dataFiles, TCHAR const *expr = NULL);


	/// PrepareFile ��������� ��� ����� �� ������������, ������ ����, ��������� ���
	/**
		\param filepath - ���� � �����
		\param filebody - ��������, ���� �����
		\param hash     - ��� �� ���� �����
		\param filebase - ������� ��� ����� 01_130527_1_box_1.csv => 01_130527_1_1 ��� 01_130527_1_kim_zou_1.csv => 01_130527_1_1
					\param filename - ��� ����� 01_130527_1_box_1.csv => 01_130527_1_box_1 ��� 01_130527_1_kim_zou_1.csv => 01_130527_1_kim_zou_1
		\param disciple - ��� �������� 01_130527_1_box_1.csv => 01
		\param date     - ���� ���������� �������� 01_130527_1_box_1.csv => 130527
		\param zone     - ��� ���� 01_130527_1_box_1.csv => 1
		\return bool - true - ���� ������� ��������, ����� - false
	*/
	bool PrepareFile(string const &filepath, string &filebody, string &hash,	string &filebase, string &disciple, string &date, string &zone);


	/// CheckLoad ��������� �� �������� �� ���� ���� � ���� ���, �� ��� ������ ������ ���������� ��� id ��������
	/**
		\param db - �������
		\param sql - sql ���������, ������������ ������ ��� ���������� ��������� ���������� - ��������� ������ ���� ����� �������
		\param pt - ��� ������
		\param ft - ��� �����
		\param hash - ���
		\param filebase - ������� ���
						`\param filename - ��� �����
		\param fileIds - ������ id
		\return -0 - ���� ����� �������, ����� - LoadFileID �������������� ��������
	*/
	int CheckLoad(CDatabase &db, CString &sql, PackageType pt, FileType ft, string const &hash, string const &filebase, string &fileIds );



	/// makeCSlist ��������� ��������� ������, ���������� ������������ (�� ��������� - ��������).
	/**
		\param str - ������ - ������, �������������� CString ��� std::string
		\param zstr - ������� ������
		\param sep - ����������� ��������� ������
		\return ���������������� ������
	*/
	template<typename S> S& makeCSlist(S &str, TCHAR const *zstr, TCHAR *sep=_T(",") )
	{
		if ( str == _T("") )
			str = zstr;
		else{
			str += sep;
			str += zstr;
		}
		return str;
	}

	template<typename S, typename N> S& makeCSlist(S &str, N no, TCHAR *sep=_T(",") )
	{
	
		if ( str == _T("") )
			str = boost::lexical_cast<S>(no);
		else{
			str += sep;
			str += boost::lexical_cast<S>(no);
		}
		return str;
	}


	/// AddFileLoad ��������� ������ � fct.LoadFile, ���������� LoadFileID
	/**
		\param db - �������
		\param sql - sql ���������, ������������ ������ ��� ���������� ��������� ���������� - ��������� ������ ���� ����� �������
		\param filepath - ������ ���� � �����
		\param filebase - ������� ���
		\param ft		- ��� ����� (enum FileType)
		\param pt - ��� ������ (enum PackageType)
		\param hash
		\param errTxt
		\return LoadFileID ����� ������
	*/
	long AddFileLoad(CDatabase &db, CString &sql, string const &filepath, string const &filebase, int ft, int pt, string const &hash, TCHAR const *errTxt = _T("")  );


	/// MakeBoxList ��������� ������ ����������� ������� ��� �������� �� ����� 
	/**
		\param db - �������
		\param sql - sql ���������, ������������ ������ ��� ���������� ��������� ���������� - ��������� ������ ���� ����� �������
		\param idloads - ������ id
		\param boxLines - id � �������������� ����������
	*/
	void MakeBoxList(CDatabase &db, CString &sql, string idloads, BoxMap &boxLines);


	/// CheckBox ��������� ������ ����������� ������� � ��������� ������� ����� �� ������������
	/**
		\param boxLines - ������ ��� ����������� �������
		\param boxNo - ����� ������
		\param loadID - ����� ��������
		\param lineNo- ����� ������
		\param errText -  ��� ��������� �� ������
		\param mustBe -  ��� true �������� ����� �������� ������� � �������� �����
		\return mustBe(false) true - ������� ��������, false - ����� ��� ���, � errText ���������
				mustBe(true) true - ����� ������, fasle - ����� ����������� , � errText ���������
	*/
	bool CheckBox(BoxMap &boxLines, __int64 boxNo, long loadID, long lineNo, string &errText, bool mustBe = false);


	/// MakePackList ��������� ������ ����������� ������� ��� �������� �� ����� 
	/**
		\param db - �������
		\param sql - sql ���������, ������������ ������ ��� ���������� ��������� ���������� - ��������� ������ ���� ����� �������
		\param idloads - ������ id
		\param packLines - id � �������������� ����������
	*/
	void MakePackList(CDatabase &db, CString &sql, string idloads, PackMap &packLines);


	/// CheckPack ��������� ������ ����������� ������� � ��������� ������� ����� �� ������������
	/**
		\param boxLines - ������ ��� ����������� �������
		\param boxNo - ����� ������
		\param loadID - ����� ��������
		\param lineNo- ����� ������
		\param errText -  ��������� �� ������
		\param mustBe -  ��� true �������� ����� �������� ������� � �������� �����
		\return mustBe(false) true - ������� ��������, false - ����� ��� ���, � errText ���������
				mustBe(true) true - ����� ������, fasle - ����� ����������� , � errText ���������
	*/
	bool CheckPack(PackMap &packLines, long boxNo, long loadID, long lineNo, string &errText, bool mustBe = false);


	/// CheckBarcode_R ��������� ������ ����������� Barcode_R
	/**
		\param checkBarcode_R - ������ ��� ����������� ��������
		\param barcode -  �������� ��� ��������
		\param loadID - ����� ��������
		\param lineNo- ����� ������
		\param errText -  ��������� �� ������
		\return true - ������� ��������, false - ����� ��� ���, � errText ���������
	*/
	bool CheckBarcode_R(set<__int64> &checkBarcode_R, __int64 barcode, long loadID, long lineNo, string &errText);


	/// CheckPdfVariant ��������� �������� pdf
	/**
		\param pdfNo - ����������� ���������
		\param variant -  ������� ��� ��������
		\param loadID - ����� ��������
		\param lineNo- ����� ������
		\param errText -  ��������� �� ������
		\return true - ������, false - �� ������, � errText ���������
	*/
	bool CheckPdfVariant(set<long> &pdfNo, string variant, long loadID, long lineNo, string &errText);


	/// LoadBarcode_R ��������� ������ Barcode_R �� ��
	/**
		\param db - �������
		\param checkBarcode_R - ������ ��� ����������� ��������
	*/
	void LoadBarcode_R(CDatabase &db, set<__int64> &checkBarcode_R );


	/// LoadPdfFromDB �������� ������ ���������
	/**
		\param db - �������
		\param pt - ��� ������ ������ - tom, zov, ovz 
		\param baseName - ������� ��� ��� ������ � ��
		\param pdfNo - ������ ������� ���������
		\return true - pdf ��� ��������, false - ���
	*/
	bool LoadPdfFromDB(CDatabase &db, PackageType pt, std::string baseName, set<long> &pdfNo);


	/// ParseFields ��������� ������, ���������� ; � ����
	/**
		\param line - ������ ��� �������
		\param fields - ������ �����
		\param sep - ����������� �����
	*/
	void ParseFields(  string const &line, vector<string> &fields, TCHAR *sep = _T(";") );



	/// RenameLoaded ��������� ������, ���������� ; � ����
	/**
		\param filename - ������ ���� � �����
		\param id - ����� �������� � ����
	*/
	void RenameLoaded(string const &filename, long id);


	/// ParseAndLoadBox ��������� ���� *box*.csv
	/**
		\param db - �������
		\param linesToLoad - ���������� sql ����������� � ������
		\param pt - ��� ������ ������ - tom, zov, ovz 
		\param filename - ���� � �����
		\param boxLines - ������ ����������� ������� �������
		\return LoadFileID ��� ����������� ������� ����, 0 � ��������� ������ 
	*/
	long ParseAndLoadBox(CDatabase &db, int linesToLoad, PackageType pt, std::string &filename, BoxMap &boxLines);

	/// ParseAndLoadPack ��������� ���� *pack*.csv
	/**
		\param db - �������
		\param linesToLoad - ���������� sql ����������� � ������
		\param pt - ��� ������ ������ - tom, zov, ovz 
		\param filename - ���� � �����
		\param packLines - ������ ����������� �������
		\param boxLines - ������ ����������� ������� �������
		\return LoadFileID ��� ����������� ������� ����, 0 � ��������� ������ 
	*/
	long ParseAndLoadPack( PackageType pt, std::string &filepath, PackMap &packLines, BoxMap &boxLines);

	/// ParseAndLoadKim ��������� ���� *kim*.csv
	/**
		\param db - �������
		\param linesToLoad - ���������� sql ����������� � ������
		\param pt - ��� ������ ������ - tom, zov, ovz 
		\param filename - ���� � �����
		\param packLines - ������ ����������� �������
		\param checkBarcode_R - ������ �������� �� res_Complects
		\param pdfNo - ������ ������� ���������
		\return LoadFileID ��� ����������� ������� ����, 0 � ��������� ������ 
	*/
	long ParseAndLoadKim( PackageType pt, std::string &filepath, PackMap &packLines, set<__int64> &checkBarcode_R, set<long> &pdfNo);


	/// ParseAndLoadKim ��������� ���� *kim*.csv
	/**
		\param db - �������
		\param linesToLoad - ���������� sql ����������� � ������
		\param pt - ��� ������ ������ - tom, zov, ovz 
		\param filename - ���� � �����
		\param pdfNo - ������ ������� ���������
		\return LoadFileID ��� ����������� ������� ����, 0 � ��������� ������ 
	*/
	long ParseAndLoadPdf(PackageType pt, std::string &filepath, set<long> &pdfNo);

	namespace fs = boost::filesystem;
	using fs::path;
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	PackageType GetPackageType(const string &ptype)
	{
		if ( ptype.empty() )
			return ptKIM_;
		string types("tomzouovz");
		int pos = types.find(ptype);
		switch( pos ){
			case 0:
				return ptZOU_;
				break;
			case 3:

				return ptTOM_;
				break;
			case 6:
				return ptOVZ_;
				break;
		}
		return ptKIM_; // ��� �������������� ��������������
	}


	FileType GetFileType(const string &ftype, const string &ext)
	{
		if(ext == _T("txt") )
			return ftPDF_;
		string types("kimboxpack");
		int pos = types.find(ftype);
		switch( pos ){
			case 0:
				return ftKIM_;
				break;
			case 3:
				return ftBOX_;
				break;
			case 6:
				return ftPACK_;
				break;
		}
		return ftKIM_; // ��� �������������� ��������������
	}

	#pragma warning(disable:4244) //����������� ��������������. ��� ��-�� ����, CFile::GetLength() ���������� ULONGLONG

	bool CalcSHA1(const char *filename, string &fbody, string &hash)
	{
		CFile fl;
		if ( fl.Open(filename, CFile::modeRead) ){
			char *buf = new char[fl.GetLength()+1];
			unsigned char chash[20]; 
			TCHAR shash[41];
			buf[fl.GetLength()] = 0;
			fl.Read( buf, fl.GetLength() );
			sha1::calc(buf, fl.GetLength(), chash);
			sha1::toHexString(chash, shash);
			hash = shash;
			fbody = buf;
			boost::to_lower(fbody);
			delete[] buf;
			return true;
		}
		return false;
	}
	#pragma warning(default:4244)


	bool CheckEAN13(char const * ean)
	{
		int veight[13] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1};

		int code(0);
		for(int i =0; i < 13; ++i ){
			if ( !*ean )
				return false; // ��� ������ 13-�� ��������
    		code += veight[i]*(ean[i]-'0');
		}
		return !(code%10);
	}


	void CreateFileList(TCHAR const *srchDir, FilePackageMap &dataFiles, TCHAR const *expr)
	{
		if ( !expr )
			expr = _T("(?i)(.*\\\\\\d{2}_\\d{6}_\\d+)_?(kim|pack|box)?_?(tom|zou|ovz)?(_\\d+)?\\.(csv|txt)");
		for ( fs::recursive_directory_iterator itr( srchDir ); itr != fs::recursive_directory_iterator(); ){
			if ( is_regular_file(itr->status()) ){
				boost::regex e(expr);
				boost::smatch what;		
				string full_name = itr->path().string();
				boost::to_lower(full_name);
				if ( boost::regex_match(full_name, what, e, boost::match_perl) ){
					string name = what[1] + what[3] + what[4];
					boost::to_lower(name);
					FilePackageMap::iterator it = dataFiles.find(name);
					//����� �� ������� pdf.txt ������ ��� ������� ������ �������
					if ( it != dataFiles.end() )
					{
						it->second.files_[GetFileType(what[2],what[5])] = full_name;
					}
					else
					{
						KIMpackage tmp;
						tmp.pt_ = GetPackageType(what[3]);
						tmp.files_[GetFileType(what[2],what[5])] = full_name;
						tmp.pdfBase_ = what[1];
						dataFiles[name] = tmp;
					}
				}
			}
			try{
				++itr;
			}catch (const exception & ex ){// ������ ��������� ��������� � �.�. �����, �� ������� � ��� ��� ����
				(void) ex;
				itr.no_push();
				++itr;
			}
		}
	}


	bool PrepareFile(string const &filepath, string &filebody, string &hash,	string &filebase, /*string &filename, */string &disciple, string &date, string &zone)
	{

		boost::regex data_ex(_T("(?i).*\\\\(\\d{2})_(\\d{6})_(\\d+)_?(kim|pack|box)?_?(tom|zou|ovz)?(_\\d+)?\\.(csv|txt)") );
		boost::smatch what;		
		boost::regex_match( filepath, what, data_ex, boost::match_perl );
		filebase = what[1] + _T("_") + what[2] +  _T("_") + what[3] + what[6];
		disciple = what[1];
		date = what[2];
		zone = what[3];
		return CalcSHA1( filepath.c_str(), filebody, hash );
	}

	int CheckLoad(CDatabase &db, CString &sql, PackageType pt, FileType ft, string const &hash, string const &filebase, string &fileIds )
	{
		CRecordset rs(&db);
		sql.Format(_T("select LoadFileID from fct.LoadFIle where LoadType = %d and sha_1 = N'%s' and FileType = %d and FileBase=N'%s'"), pt, hash.c_str(), ft, filebase.c_str() );
		rs.Open(CRecordset::forwardOnly, sql);
		if ( !rs.IsEOF() ){  // ���� ��� ��������
			CDBVariant var;
			rs.GetFieldValue(short(0), var);
			return var.m_iVal;
		}
		rs.Close();
		sql.Format(_T("select LoadFileID from fct.LoadFIle where LoadType = %d and FileType = %d and FileBase=N'%s'"), pt, ft, filebase.c_str() );
		rs.Open(CRecordset::forwardOnly, sql);
		if ( rs.IsEOF() )   // ���� ����������� �������
			return 0;	// 
		for(; !rs.IsEOF(); rs.MoveNext() ){  // ��� �� ������ ���� �� ���������
			CDBVariant id;
			rs.GetFieldValue(short(0), id);
			makeCSlist( fileIds, id.m_iVal);
		}
		return 0;
	}

	long AddFileLoad(CDatabase &db, CString &sql, string const &filepath, string const &filebase, int ft, int pt, string const &hash, TCHAR const *errText )
	{
		sql.Format(_T("insert into fct.LoadFile(FilePath, FileBase, sha_1, FileType, LoadType, ErrText)")
				   _T("values(N'%s', N'%s', N'%s', %d, %d, N'%s')"), filepath.c_str(), filebase.c_str(), hash.c_str(), ft, pt, errText );
		db.ExecuteSQL(sql);
		CRecordset rs(&db);
		sql = _T("select max(LoadFileID) from fct.LoadFile");
		rs.Open(CRecordset::forwardOnly, sql );
		CDBVariant var;
		rs.GetFieldValue((short)0, var);
		return var.m_lVal;
	}


	void MakeBoxList(CDatabase &db, CString &sql, string idloads, BoxMap &boxLines)
	{
		sql.Format(_T("select LoadFileID, BoxNumber, LineNumber from fct.PressBoxs where LoadFileID in (%s) order by BoxNumber;"), idloads.c_str());
		CRecordset rs(&db);
		rs.Open(CRecordset::forwardOnly, sql );
		for( ;!rs.IsEOF(); rs.MoveNext() ){
			CDBVariant id, line;
			CString box;
			rs.GetFieldValue((short)0, id);
			rs.GetFieldValue((short)1, box);
			rs.GetFieldValue((short)2, line);
			long long tmp = boost::lexical_cast<long long >(box.GetString());
			boxLines[tmp] = BP(line.m_iVal, id.m_iVal);
		}
	}

	bool CheckBox(BoxMap &boxLines, long long boxNo, long loadID, long lineNo, string &errText, bool mustBe)
	{
		BoxMap ::iterator it = boxLines.find(boxNo);

		if ( mustBe ){//����� �������� ������� (��� ������� �����)
			if ( it == boxLines.end()){
				ostringstream err;
				err<<boost::format(_T("���� ���� %1% ������ %2% �� ������")) % boxNo % lineNo;
				makeCSlist(errText,err.str());
				return false;
			}
			else{
				++it->second.touch_;// �������, ��� ����� �������������
				return true;
			}
		}
		else if ( it == boxLines.end()){//����� ������� ��� ��������
			boxLines[boxNo] = BP(lineNo,loadID);
			return true;
		}
		else{
			ostringstream err;
			err<<boost::format(_T("���� ���� %1% ������ %2% ���  � �������� %3% ������ %4%")) % boxNo % lineNo % it->second.load_id_  % it->second.line_;
			makeCSlist(errText,err.str());
			return false;
		}
	}

	void MakePackList(CDatabase &db, CString &sql, string idloads, PackMap &packLines)
	{
		sql.Format(_T("select LoadFileID, PackNumber, LineNumber from fct.PressPacks where LoadFileID in (%s) order by PackNumber;"), idloads.c_str());
		CRecordset rs(&db);
		rs.Open(CRecordset::forwardOnly, sql );
		for( ;!rs.IsEOF(); rs.MoveNext() ){
			CDBVariant id, line, pack;
			rs.GetFieldValue((short)0, id);
			rs.GetFieldValue((short)1, pack);
			rs.GetFieldValue((short)2, line);
			packLines[pack.m_iVal] = BP(line.m_iVal, id.m_iVal);
		}
	}


	bool CheckPack(PackMap &packLines, long packNo, long loadID, long lineNo, string &errText, bool mustBe)
	{
		PackMap::iterator it = packLines.find(packNo);
		if ( mustBe ){//����� �������� ������� (��� ������� �����)
			if ( it == packLines.end()){
				ostringstream err;
				err<<boost::format(_T("���� ����� %1% ������ %2% �� ������")) % packNo %  lineNo;
				errText = err.str();
				return false;
			}
			else{
				++it->second.touch_; // �������, ��� ����� �������������
				return true;
			}
		}else if ( it == packLines.end() ){//����� ������� ��� ��������
			packLines[packNo] = BP(lineNo,loadID);
			return true;
		}
		else{
			ostringstream err;
			err<<boost::format(_T("���� ����� %1% ������ %2% ��� � �������� %3% ������ %4%")) % packNo % lineNo % it->second.load_id_  % it->second.line_;
			makeCSlist(errText,err.str());
			return false;
		}
	}


	void ParseFields( string const &line, vector<string> &fields, TCHAR *sep)
	{
		boost::char_separator<char> field_sep(sep);
		tokenizer field_tokens(line, field_sep);
		for (tokenizer::iterator tok_iter = field_tokens.begin(); tok_iter != field_tokens.end(); ++tok_iter){
			fields.push_back(*tok_iter);
		}
	}


	void RenameLoaded(string const &filename, long id)
	{
		fs::path from(filename);
		ostringstream newName;
		newName<<boost::format(_T("%1%.%2%") ) % filename % id;
		fs::path to(newName.str());
		fs::rename(from, to);
	}

	long ParseAndLoadBox(CDatabase &db, int linesToLoad, PackageType pt, std::string &filepath, BoxMap &boxLines)
	{
		CString sql;
		long id_load;
		try{
			boost::timer::auto_cpu_timer t;
			cout<<"\nBox\n";
			std::string filebody, hash, filebase, disciple, date, zone;
			if ( !PrepareFile(filepath, filebody, hash, filebase, disciple, date, zone ) ){
				AddFileLoad(db, sql, filepath, filebase, ftBOX_, pt, hash, _T("�� ����������� ����")  );
				return 0; //������ ������ �����
			}
			//��������, �� ������� �� �� ���� ���� ��� ��� �������
			string idloads;
			if ( long old_id = CheckLoad(db, sql, pt, ftBOX_, hash, filebase, idloads ) ){
				CString tmp;
				tmp.Format(_T("��� ��������. #%d"), old_id );
				id_load = AddFileLoad(db,  sql, filepath, filebase, ftBOX_, pt, hash, tmp );
				return id_load; // ���� ��� ��������
			}

			id_load = AddFileLoad(db,  sql, filepath, filebase, ftBOX_, pt, hash );
			if( !idloads.empty() ){ //���������� ������ boxLines ��� �������� �� �����
				MakeBoxList(db, sql, idloads, boxLines); 
			}

			boost::char_separator<char> line_sep("\r\n");
			tokenizer line_tokens(filebody, line_sep);
			int lineNo = 1;
			sql = "";
			long fsize = filebody.size();
			long procSize = 0;
			// ������� ���� �� ������
			for(tokenizer::iterator line_iter = line_tokens.begin(); line_iter != line_tokens.end(); ++line_iter, ++lineNo){
				std::string line = *line_iter;
				procSize += line.size()+2;
				if ( line.empty() ) // ������ ������ ����������
					continue;
				std::vector<std::string> fields;
				//������ ������� �� ����
				ParseFields( line, fields );
				// ������ �������� ����
				string errText;
				if (  fields.size() != 6 )
					makeCSlist(errText, _T("������������ ���������� �����") );
				else{
					if( fields[0] != zone )
						makeCSlist(errText, _T("������������ ����") );
					if( fields[1] != disciple )
						makeCSlist(errText, _T("������������ ��� ��������") );
					if( fields[2] != date )
						makeCSlist(errText, _T("������������ ����") );
					if( !atol(fields[5].c_str()) )
						makeCSlist(errText, _T("��� ������� 0") ); 
					if ( !CheckEAN13(fields[3].c_str()))
						makeCSlist(errText, _T("������ ����������� ����� � ������ ������") ); 
					long long no = boost::lexical_cast<long long>(fields[3]);
					if (errText.empty() )
						CheckBox(boxLines, no, id_load, lineNo, errText);
				}			
				CString tmp;
				if ( errText.empty() ){
					tmp.Format(	_T("insert into fct.PressBoxs(LoadFileID, ZoneCode, SubjectCode, ExamDate, BoxNumber, Amount, RegionCode, LineNumber)")
						_T(" values(%d, %s, %s, N'20%s', %s, %s, %s, %d);\n"),
						id_load, fields[0].c_str(), fields[1].c_str(), fields[2].c_str(), fields[3].c_str(), fields[4].c_str(), fields[5].c_str(), lineNo);
				}else{
					tmp.Format(	_T("insert into fct.LoadErrs(LoadFileID, LineNumber, LineText, ErrText)")
						_T(" values(%d, %d, N'%s', N'%s');\n"),
						id_load, lineNo, line.c_str(), errText.c_str());
				}
				sql += tmp;
				if ( !(lineNo%linesToLoad) ){
					db.ExecuteSQL(sql);
					sql = "";
				}
				if( lineNo && !(lineNo%100))
					cout<<lineNo<<'\t'<<procSize*100/fsize<<'\r';
			}
			if ( sql != "") // ������� �����, ���� �� ����
				db.ExecuteSQL(sql);
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			ex->Delete();
			CString tmp;
			tmp.Format(_T("������:%s\n%s"),buf, sql.GetString());
			AfxMessageBox(tmp, MB_ICONSTOP );
			db.Rollback();
			return 0;
		}
		return id_load;
	}


	long ParseAndLoadPack(CDatabase &db, int linesToLoad, PackageType pt, std::string &filepath, PackMap &packLines, BoxMap &boxLines)
	{
		CString sql;
		long id_load;
		try{
			boost::timer::auto_cpu_timer t;
			cout<<"\nPack\n";
			std::string filebody, hash, filebase, disciple, date, zone;
			if ( !PrepareFile( filepath, filebody, hash, filebase, disciple, date, zone ) ){
				AddFileLoad(db, sql, filepath, filebase, ftPACK_, pt, hash, _T("�� ����������� ����")  );
				return 0; //������ ������ �����
			}

			//��������, �� ������� �� �� ���� ���� ��� ��� �������
			string idloads;
			if ( long old_id = CheckLoad(db, sql, pt, ftPACK_, hash, filebase, idloads ) ){
				CString tmp;
				tmp.Format(_T("��� ��������. #%d"), old_id );
				id_load = AddFileLoad(db, sql, filepath, filebase, ftPACK_, pt, hash, tmp );
				return id_load; // ���� ��� ��������
			}

			id_load = AddFileLoad(db, sql, filepath, filebase, ftPACK_, pt, hash );
			if( !idloads.empty() ){ //���������� ������ boxLines ��� �������� �� �����
				MakePackList(db, sql, idloads, packLines); 
			}

			boost::char_separator<char> line_sep("\r\n");
			tokenizer line_tokens(filebody, line_sep);
			int lineNo = 1;
			sql = "";
			// ������� ���� �� ������
			long fsize = filebody.size();
			long procSize = 0;
			for(tokenizer::iterator line_iter = line_tokens.begin(); line_iter != line_tokens.end(); ++line_iter, ++lineNo){
				std::string line = *line_iter;
				procSize += line.size()+2;
				if ( line.empty() ) // ������ ������ ����������
					continue;
				std::vector<std::string> fields;
				//������ ������� �� ����
				ParseFields( line, fields );
				// ������ �������� ����
				string errText;
				if (  fields.size() != 6 )
					makeCSlist(errText, _T("������������ ���������� �����") );
				else{
					if( fields[0] != zone )
						makeCSlist(errText, _T("������������ ����") );
					if( fields[1] != disciple )
						makeCSlist(errText, _T("������������ ��� ��������") );
					if( fields[2] != date )
						makeCSlist(errText, _T("������������ ����") );
					if ( !CheckEAN13(fields[5].c_str()))
						makeCSlist(errText, _T("������ ����������� ����� � ������ ������") ); 
					long long boxNo = boost::lexical_cast<long long>(fields[5]);
					long packNo = boost::lexical_cast<long>(fields[3]);
					if (errText.empty() ){
						CheckBox(boxLines, boxNo, id_load, lineNo, errText, true);
						CheckPack(packLines, packNo, id_load, lineNo, errText);
					}
				}			
				CString tmp;
				if ( errText.empty() ){
					tmp.Format(	_T("insert into fct.PressPacks(LoadFileID, ZoneCode, SubjectCode, ExamDate, PackNumber, Amount, BoxNumber, LineNumber)")
						_T(" values(%d, %s, %s, N'20%s', %s, %s, %s, %d);\n"),
						id_load, fields[0].c_str(), fields[1].c_str(), fields[2].c_str(), fields[3].c_str(), fields[4].c_str(), fields[5].c_str(), lineNo);
				}else{
					tmp.Format(	_T("insert into fct.LoadErrs(LoadFileID, LineNumber, LineText, ErrText)")
						_T(" values(%d, %d, N'%s', N'%s');\n"),
						id_load, lineNo, line.c_str(), errText.c_str());
				}
				sql += tmp;
				if ( !(lineNo%linesToLoad) ){
					db.ExecuteSQL(sql);
					sql = "";
				}
				if( lineNo && !(lineNo%100))
					cout<<lineNo<<'\t'<<procSize*100/fsize<<'\r';
			}
			if ( sql != "") // ������� �����, ���� �� ����
				db.ExecuteSQL(sql);
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			CString tmp;
			tmp.Format(_T("������:%s\n%s"),buf, sql.GetString());
			AfxMessageBox(tmp, MB_ICONSTOP );
			db.Rollback();
			ex->Delete();
			return 0;
		}
		return id_load;
	}





	long ParseAndLoadKim(CDatabase &db, int linesToLoad, PackageType pt, std::string &filepath, PackMap &packLines, set<__int64> &checkBarcode_R, set<long> &pdfNo)
	{
		CString sql;
		long id_load;
		try{
			boost::timer::auto_cpu_timer t;
			cout<<"\nKim\n";
			std::string filebody, hash, filebase, disciple, date, zone;
			if ( !PrepareFile( filepath, filebody, hash, filebase, disciple, date, zone ) ){
				AddFileLoad(db, sql, filepath, filebase, ftKIM_, pt, hash, _T("�� ����������� ����")  );
				return 0; //������ ������ �����
			}

			//��������, �� ������� �� �� ���� ���� ��� ��� �������
			string idloads;
			if ( long old_id = CheckLoad(db, sql, pt, ftKIM_, hash, filebase, idloads ) ){
				CString tmp;
				tmp.Format(_T("��� ��������. #%d"), old_id );
				id_load = AddFileLoad(db, sql, filepath, filebase, ftKIM_, pt, hash, tmp );
				return id_load; // ���� ��� ��������
			}

			id_load = AddFileLoad(db, sql, filepath, filebase, ftKIM_, pt, hash );
			boost::char_separator<char> line_sep("\r\n");

			tokenizer line_tokens(filebody, line_sep);
			int lineNo = 1;
			string noPfx = "0"+disciple; // ��� �������� ������� ������ ���
			sql = "";
			long fsize = filebody.size();
			long procSize = 0;

			// ������� ���� �� ������
			for(tokenizer::iterator line_iter = line_tokens.begin(); line_iter != line_tokens.end(); ++line_iter, ++lineNo){
				std::string line = *line_iter;
				procSize += line.size() + 2;
				if ( line.empty() ) // ������ ������ ����������
					continue;
				std::vector<std::string> fields;
				//������ ������� �� ����
				ParseFields( line, fields );
				// ������ �������� ����
				string errText;
				if (  fields.size() != 10 )
					makeCSlist(errText, _T("������������ ���������� �����") );
				else{
					if( fields[0] != zone )
						makeCSlist(errText, _T("������������ ����") );
					if( fields[1] != disciple )
						makeCSlist(errText, _T("������������ ��� ��������") );
					if( fields[2] != date )
						makeCSlist(errText, _T("������������ ����") );
					if( fields[3].substr(0,3) != noPfx )
						makeCSlist(errText, _T("������������ ����� ���") );
					if ( !CheckEAN13(fields[4].c_str()))
						makeCSlist(errText, _T("������ ����������� ����� � ������ ������ �����������") ); 
					if ( !CheckEAN13(fields[5].c_str()))
						makeCSlist(errText, _T("������ ����������� ����� � ������ ������ ������ �1") ); 
					if ( !CheckEAN13(fields[5].c_str()))
						makeCSlist(errText, _T("������ ����������� ����� � ������ ������ ������ �2") ); 
					long packNo = boost::lexical_cast<long>(fields[9]);
					if (errText.empty() ){
						CheckPack(packLines, packNo, id_load, lineNo, errText, true);
						__int64 barcode = boost::lexical_cast<__int64>(fields[4]);
						CheckBarcode_R( checkBarcode_R, barcode, id_load, lineNo, errText);
						CheckPdfVariant(pdfNo, fields[8], id_load, lineNo, errText);
					}
				}			
				CString tmp;
				if ( errText.empty() ){
					tmp.Format(	
						_T("insert into fct.PressKims(LoadFileID, ZoneCode, SubjectCode, ExamDate, StuffCode, Barcode_R, Barcode_AB, Barcode_C, PdfName, PdfNumber, PackNumber, LineNumber)")
						_T(" values(%d, %s, %s, N'20%s', %s, %s, %s, %s, N'%s',%s, %s, %d);\n"),
						id_load, fields[0].c_str(), fields[1].c_str(), fields[2].c_str(), fields[3].c_str(), fields[4].c_str(), fields[5].c_str(),
						fields[6].c_str(), fields[7].c_str(), fields[8].c_str(), fields[9].c_str(), lineNo);
				}else{
					tmp.Format(	_T("insert into fct.LoadErrs(LoadFileID, LineNumber, LineText, ErrText)")
						_T(" values(%d, %d, N'%s', N'%s');\n"),
						id_load, lineNo, line.c_str(), errText.c_str());
				}
				sql += tmp;
				if ( !(lineNo%linesToLoad) ){
					db.ExecuteSQL(sql);
					sql = "";
				}
				if( lineNo && !(lineNo%100))
					cout<<lineNo<<'\t'<<procSize*100/fsize<<'\r';
			}
			if ( sql != "") // ������� �����, ���� �� ����
				db.ExecuteSQL(sql);
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			CString tmp;
			tmp.Format(_T("������:%s\n%s"),buf, sql.GetString());
			AfxMessageBox(tmp, MB_ICONSTOP );
			db.Rollback();
			ex->Delete();
			return 0;
		}
		return id_load;
	}


	long ParseAndLoadPdf(CDatabase &db, int linesToLoad, PackageType pt, std::string &filepath, set<long> &pdfNo)
	{

		CString sql;

		long id_load;
		std::string filebase, disciple, date, zone;
		try{
			boost::timer::auto_cpu_timer t;
			cout<<"\nPdf\n";
			std::string hash, filebody ;
			if ( !PrepareFile( filepath, filebody, hash, filebase, disciple, date, zone ) ){
				AddFileLoad(db, sql, filepath, filebase, ftPDF_, pt, hash, _T("�� ����������� ����")  );
				return 0; //������ ������ �����
			}

			//��������, �� ������� �� �� ���� ���� ��� ��� �������
			string idloads;
			if ( long old_id = CheckLoad(db, sql, pt, ftPDF_, hash, filebase, idloads ) ){
				CString tmp;
				tmp.Format(_T("��� ��������. #%d"), old_id );
				id_load = AddFileLoad(db, sql, filepath, filebase, ftKIM_, pt, hash, tmp );
				LoadPdfFromDB(db, pt, filebase, pdfNo);
				return id_load; // ���� ��� ��������
			}

			id_load = AddFileLoad(db, sql, filepath, filebase, ftPDF_, pt, hash );
			boost::char_separator<char> line_sep("\r\n");

			tokenizer line_tokens(filebody, line_sep);
			int lineNo = 2;
			sql = "";

			//���������� ������ ������
			tokenizer::iterator line_iter = line_tokens.begin();
			boost::regex ex(_T("(?i).*\\\\(\\d{2}_\\d{6}_\\d+)\\.txt") ); 
			boost::smatch what;		
			if ( boost::regex_match(filepath, what, ex, boost::match_perl) )
				filebase = what[1] + _T(".pdf");

			if ( filebase != *line_iter ) 
			{
				sql.Format(_T("update fct.LoadFile set ErrText=N'������������ ��� pdf - %s ������ %s' where LoadFileID = %d"), line_iter->c_str(), filebase.c_str(), id_load);
				db.ExecuteSQL(sql);
				return 0;
			}

			boost::regex e(_T("\\d{2}_(\\d{3}).pdf"));
			long fsize = filebody.size();
			long procSize = 0;
			++line_iter;
			for(; line_iter != line_tokens.end(); ++line_iter, ++lineNo){
				std::string line = *line_iter;
				if ( line.empty() ) // ������ ������ ����������
					continue;

				CString tmp;
				boost::smatch what;		
				if ( !boost::regex_match(line, what, e, boost::match_perl) ){
					tmp.Format(	
						_T("insert into fct.LoadErrs(LoadFileID, LineNumber, LineText, ErrText)")
						_T(" values(%d, %d, N'%s', N'������������ ��� pdf �����');\n"),
						id_load, lineNo, line.c_str());
				}else{
					tmp.Format(
						_T("insert into fct.PdfFiles(LoadFileID, PdfName, SubjectCode, ExamDate, ZoneCode, PdfVariant, RowNumber, VariantCode) ")
						_T("values(%d, N'%s', %s, N'20%s', %s, '%s', %d, %s);\n"), 
						id_load, filebase.c_str(), disciple.c_str(), date.c_str(), zone.c_str(), line.c_str(), lineNo, what[1].str().c_str() );
					pdfNo.insert(atol(what[1].str().c_str()) );
				}
				sql += tmp;
				if ( !(lineNo%linesToLoad) ){
					db.ExecuteSQL(sql);
					sql = "";
				}
				if( lineNo && !(lineNo%100))
					cout<<lineNo<<'\t'<<procSize*100/fsize<<'\r';
			}
			if ( sql != "") // ������� �����, ���� �� ����
				db.ExecuteSQL(sql);
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			CString tmp;
			tmp.Format(_T("������:%s\n%s"),buf, sql.GetString());
			AfxMessageBox(tmp, MB_ICONSTOP );
			db.Rollback();
			ex->Delete();
			return 0;
		}
		return id_load;
	}


	void LoadBarcode_R( CDatabase &db, set<__int64> &checkBarcode_R )
	{
		try{
			CRecordset rs(&db);
			rs.Open(CRecordset::forwardOnly, _T("SELECT Barcode_R FROM res_Complects") );
			for( ;!rs.IsEOF(); rs.MoveNext() ){
				CString barcode;
				rs.GetFieldValue((short)0, barcode);
				__int64 tmp = boost::lexical_cast<__int64>(barcode.GetString());
				checkBarcode_R.insert(tmp);
			}
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			CString tmp;
			tmp.Format(_T("������:%s\nSELECT Barcode_R FROM res_Complects"),buf);
			AfxMessageBox(tmp, MB_ICONSTOP );
		}
	}


	bool CheckBarcode_R(set<__int64> &checkBarcode_R, __int64 barcode, long loadID, long lineNo, string &errText)
	{
		set<__int64>::iterator it = checkBarcode_R.find(barcode);
		if ( it != checkBarcode_R.end() ){
			ostringstream err;
			err<<boost::format(_T("���� Barcode_R %1% �� �������� %2% ������ %3% ��� ��������")) % barcode % loadID % lineNo;
			makeCSlist(errText, err.str());
			return false;
		}
		return true;
	}

	bool CheckPdfVariant(set<long> &pdfNo, string variant, long loadID, long lineNo, string &errText)
	{
		set<long>::iterator it = pdfNo.find(atol(variant.c_str()));
		if ( it == pdfNo.end() ){
			ostringstream err;
			err<<boost::format(_T("� �������� %1% ������ %2% ������ ������������� � pdf ������� %3%")) % loadID % lineNo % variant;
			makeCSlist(errText, err.str());
			return false;
		}
		return true;
	}


	bool LoadPdfFromDB(CDatabase &db, PackageType pt, std::string baseName, set<long> &pdfNo)
	{
		CString sql;
		sql.Format(_T("SELECT [VariantCode] FROM [fct].[PdfFiles] inner join fct.LoadFile on ")
			_T("[fct].[PdfFiles].LoadFileID = [fct].[LoadFile].[LoadFileID] where ")
			_T("[fct].[LoadFile].[LoadType] = %d and [fct].[LoadFile].[FileBase] = '%s'"), pt, baseName.c_str());
		try{
			CRecordset rs(&db);
			rs.Open(CRecordset::forwardOnly, sql );
			for( ;!rs.IsEOF(); rs.MoveNext() ){
				CDBVariant val;
				rs.GetFieldValue((short)0, val);
				pdfNo.insert(val.m_iVal);
			}
			return !pdfNo.empty();
		}catch( CException *ex){
			TCHAR buf[1000];
			ex->GetErrorMessage(buf, 1000);
			CString tmp;
			tmp.Format(_T("������:%s\n%s"),buf,sql.GetString());
			AfxMessageBox(tmp, MB_ICONSTOP );
		}
		return false;
	}


}// ����� ���������� namespace

void ProcFiles(CDatabase &db, TCHAR *baseDir, int linesToLoad)
{
	FilePackageMap dataFiles;			//������ ������� ������ *_box.csv, *_pack.csv, *_kim.csv, *.txt
	set<__int64> checkBarcode_R;		//������ �������� �� ��, ����������� �� ����� ��������

	CreateFileList(baseDir, dataFiles);
	LoadBarcode_R( db, checkBarcode_R );
	for( std::map< std::string, KIMpackage>::iterator it = dataFiles.begin(); it != dataFiles.end(); ++it){
		long idBox, idPack, idKim, idPdf;
		PackMap packLines; // 
		BoxMap boxLines;
		set<long> pdfNo;
		db.SetQueryTimeout(1000);
		db.BeginTrans();
		bool isPdf = false;
// �������� ������ ��������� �� ����� ��� �� ��
		if ( !it->second.files_[3].empty()){
			if( !(idPdf = ParseAndLoadPdf(db, linesToLoad, it->second.pt_, it->second.files_[ftPDF_], pdfNo)) )
				db.Rollback();
			else{
				isPdf = true;
				RenameLoaded(it->second.files_[ftPDF_], idPdf);
			}
		}
		else{
			isPdf = LoadPdfFromDB(db, it->second.pt_, it->second.pdfBase_, pdfNo);
		}
		if( !(it->second.files_[ftBOX_].empty() || it->second.files_[ftPACK_].empty() || it->second.files_[ftKIM_].empty() || !isPdf) ){ //���� ��� ������ ����� 
			if ( !(idBox = ParseAndLoadBox(db, linesToLoad, it->second.pt_, it->second.files_[ftBOX_], boxLines )) ){
				db.Rollback();
				continue;
			}
			RenameLoaded(it->second.files_[ftBOX_], idBox );
			if ( !(idPack = ParseAndLoadPack(db, linesToLoad, it->second.pt_, it->second.files_[ftPACK_], packLines, boxLines)) ){
				db.Rollback();
				continue;
			}
			RenameLoaded(it->second.files_[ftPACK_], idPack );
			if ( !(idKim = ParseAndLoadKim(db, linesToLoad, it->second.pt_, it->second.files_[ftKIM_], packLines, checkBarcode_R, pdfNo)) ){
				db.Rollback();
			continue;
			}
			RenameLoaded(it->second.files_[ftKIM_], idKim);
			CString sql;
			try{
				for(PackMap::iterator it = packLines.begin(); it != packLines.end(); ++it){
					if ( !it->second.touch_){
						sql.Format(_T("insert into fct.LoadErrs(LoadFileID, LineNumber, ErrText)")
							_T(" values(%d, %d, N'����� %d �� ������������');\n"), it->second.load_id_, it->second.line_, it->first);
						db.ExecuteSQL(sql);
					}
				}
				for(BoxMap ::iterator it = boxLines.begin(); it != boxLines.end(); ++it){
					if ( !it->second.touch_){
						sql.Format(_T("insert into fct.LoadErrs(LoadFileID, LineNumber, ErrText)")
							_T(" values(%d, %d, N'����� %lli �� ������������');\n"), it->second.load_id_, it->second.line_, it->first);
						db.ExecuteSQL(sql);
					}
				}
				// ������� ���������� ������ � LoadFile
				db.ExecuteSQL(_T("update [fct].[LoadFile] set [LinesErr] = (select count(*) ")
					_T("from [fct].[LoadErrs] where [fct].[LoadErrs].[LoadFileID] = [fct].[LoadFile].[LoadFileID])"));
				boost::timer::auto_cpu_timer t;
				sql.Format(_T("INSERT INTO [res_Complects] \n"
					"           ([REGION],[ComplectID],[Barcode_AB],[Barcode_C],[Barcode_R],[SheetFK_AB],[SheetFK_C], \n"
					"            [SheetFK_R],[RegionCode],[DepartmentCode],[TestTypeCode],[SubjectCode],[ExamDate], \n"
					"            [ZoneCode],[StuffCode],[VariantCode],[IsTom]) \n"
					"SELECT \n"
					"  CASE \n"
					"       WHEN fct.PressBoxs.RegionCode IS NULL \n"
					"       THEN 0 \n"
					"       ELSE fct.PressBoxs.RegionCode \n"
					"   END                       AS REGION, \n"
					"       PressKims.PressKimGUID AS ComplectID, \n"
					"       PressKims.Barcode_AB AS Barcode_AB, \n"
					"       PressKims.Barcode_C AS Barcode_C, \n"
					"       PressKims.Barcode_R AS Barcode_R, \n"
					"       NULL AS SheetFK_AB, \n"
					"       NULL AS SheetFK_C, \n"
					"       NULL AS SheetFK_R, \n"
					"  CASE \n"
					"       WHEN fct.PressBoxs.RegionCode IS NULL \n"
					"       THEN 0 \n"
					"       ELSE fct.PressBoxs.RegionCode \n"
					"   END AS RegionCode, \n"
					"  CASE \n"
					"       WHEN fct.PressBoxs.RegionCode IS NULL \n"
					"       THEN NULL \n"
					"       WHEN fct.PressBoxs.RegionCode < 10 \n"
					"       THEN '0' + CAST(fct.PressBoxs.RegionCode AS VARCHAR) + '00' \n"
					"       ELSE CAST(fct.PressBoxs.RegionCode AS VARCHAR) + '00' \n"
					"   END AS DepartmentCode, \n"
					"       4 AS TestTypeCode, \n"
					"       PressKims.SubjectCode AS SubjectCode, \n"
					"       PressKims.ExamDate    AS ExamDate, \n"
					"       PressKims.ZoneCode    AS ZoneCode, \n"
					"       PressKims.StuffCode   AS StuffCode, \n"
					"       0                         AS VariantCode, \n"
					"       0                         AS IsTom \n"
					"  FROM \n"
					"  (SELECT * FROM fct.PressKims \n"
					"   WHERE (Barcode_R NOT IN (SELECT Barcode_R FROM res_Complects)  \n"
					"		  AND Barcode_AB IS NOT NULL) AND SubjectCode > 0) as PressKims \n"
					" INNER JOIN fct.PressPacks \n"
					"         ON PressKims.PackNumber = fct.PressPacks.PackNumber \n"
					" INNER JOIN fct.PressBoxs \n"
					"         ON fct.PressPacks.BoxNumber = fct.PressBoxs.BoxNumber \n"));
				db.ExecuteSQL(sql);
				//	��������� ��������
				sql.Format(_T("INSERT INTO scp_ComplectVariants (ComplectFK, ExamVariantFK)\n"
					"SELECT ComplectID AS ComplectFK, ExamVariantID AS ExamVariantFK\n"
					"  FROM (SELECT * FROM sc_ExamVariants\n"
					"         INNER JOIN sc_Exams ON sc_ExamVariants.ExamFK = sc_Exams.ExamID\n"
					"         INNER JOIN sc_Subjects ON sc_Exams.SubjectFK = sc_Subjects.SubjectID\n"
					"         WHERE (sc_ExamVariants.TechnologyCode = 1)) ExamVariants\n"
					" INNER JOIN fct.PdfFiles\n"
					"    ON (fct.PdfFiles.SubjectCode = ExamVariants.SubjectCode AND\n"
					"        fct.PdfFiles.ExamDate = ExamVariants.ExamDate AND\n"
					"        fct.PdfFiles.ZoneCode = ExamVariants.ZoneCode AND\n"
					"        fct.PdfFiles.VariantCode = ExamVariants.VariantCode)\n"
					"  INNER JOIN fct.PressKims\n"
					"     ON (fct.PressKims.PdfName = fct.PdfFiles.PdfName AND\n"
					"         fct.PressKims.PdfNumber = fct.PdfFiles.RowNumber)\n"
					"  INNER JOIN res_Complects\n"
					"     ON (res_Complects.Barcode_R = fct.PressKims.Barcode_R)\n"
					"  WHERE (NOT EXISTS (SELECT ComplectVariantID\n"
					"                       FROM scp_ComplectVariants\n"
					"                      WHERE (scp_ComplectVariants.ComplectFK = res_Complects.ComplectID AND\n"
					"                             scp_ComplectVariants.ExamVariantFK = ExamVariants.ExamVariantID)))"));
				db.ExecuteSQL(sql);
				db.CommitTrans();
			}catch(CException *ex){
				TCHAR buf[1000];
				ex->GetErrorMessage(buf, 1000);
				CString tmp;
				tmp.Format(_T("������:%s\n%s"),buf, sql.GetString());
				AfxMessageBox(tmp, MB_ICONSTOP );
				ex->Delete();
				db.Rollback();
			}
		}else{
			std::string filebody, hash, filebase, disciple, date, zone, filepath;
			CString sql;
			if ( ! it->second.files_[ftBOX_].empty() ){
				PrepareFile( it->second.files_[ftBOX_], filebody, hash, filebase, disciple, date, zone );
				AddFileLoad(db, sql, it->second.files_[ftBOX_], filebase, ftBOX_, it->second.pt_, hash, _T("�������� �����") );
			}
			if ( ! it->second.files_[ftPACK_].empty() ){
				PrepareFile( it->second.files_[ftPACK_], filebody, hash, filebase, disciple, date, zone );
				AddFileLoad(db, sql, it->second.files_[ftPACK_], filebase, ftPACK_, it->second.pt_, hash, _T("�������� �����") );
			}
			if ( ! it->second.files_[ftKIM_].empty() ){
				PrepareFile( it->second.files_[ftKIM_], filebody, hash, filebase, disciple, date, zone );
				AddFileLoad(db, sql, it->second.files_[ftKIM_], filebase, ftKIM_, it->second.pt_, hash, _T("�������� �����") );
			}
			if ( ! it->second.files_[ftPDF_].empty() ){
				PrepareFile( it->second.files_[ftPDF_], filebody, hash, filebase, disciple, date, zone );
				AddFileLoad(db, sql, it->second.files_[ftPDF_], filebase, ftKIM_, it->second.pt_, hash, _T("�������� �����") );
			}
			db.CommitTrans();
		}
	}
}