// Regexp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <boost/algorithm/string/trim_all.hpp>
//#include <boost/algorithm/string/classification.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>



void FindGO(std::string &file, std::string &batch)
{
	using namespace std;
	using namespace boost;
	auto begin = file.begin();
	auto cbegin = begin;
	auto cend = begin;
	regex go( _T( "(?i)^\\s*go(\\s+|$)\\d*.*$" ) );
	bool go_found{false};
	auto NextLine = [&](){
		if ( cend == file.end() ) return false;
		while ( cend != file.end() && *cend != '\n' ) ++cend;
		if ( cend != file.end() ) ++cend;
		return true;
	};
	while ( NextLine() ){
		if ( !regex_match( cbegin, cend, go, match_perl ) ) cbegin = cend;
		else break;
	}
	batch = string( file.begin(), cbegin );
	trim_all( batch );
	file = string( cend, file.end() );
}

bool ParseScript( std::string &file, std::string &batch )
{
	do{
		FindGO( file, batch );
	} while ( batch.empty() && !file.empty() );
	return !batch.empty();
}


bool CheckSQL( const std::string &file, std::string &err )
{
	using namespace std;
	using namespace boost;
	bool checked{true};
	char_separator<TCHAR> field_sep( _T( "\n" ), _T( "" ), boost::keep_empty_tokens );
	tokenizer<char_separator<TCHAR> > line( file, field_sep );
	int line_no{1};
	regex dbtype( _T( "(?i).*values.*'DbType'.*" ) );
	regex dbver( _T( "(?i).*values.*'DbVersion'.*" ) );
	for ( auto i = line.begin(); i != line.end(); ++i, ++line_no ){
		if ( regex_match( *i, dbtype, match_perl ) ){
			checked = false;
			err += _T( "Строка " ) + to_string( line_no ) + _T( " содержит 'DbType'\n" );
		}
		if ( regex_match( *i, dbver, match_perl ) ){
			checked = false;
			err += _T( "Строка " ) + to_string( line_no ) + _T( " содержит 'DbVersion'\n" );
		}
	}
	return checked;
}



int main()
{
	std::string s = _T("a\nvalues'DbType'\nvalues(    'DbType' )\n\nnasdasdasdasdas values ( 'DbType')\nA\nVALUES'DBTYPE'\nVALUES(    'DBTYPE' )\n\nNASDASDASDASDAS VALUES ( 'DbVersion')");
	std::string s1 = _T( "SET QUOTED_IDENTIFIER ON \n\
GO \n\
-- ====================================================== \n\
-- Автор:          Ryan Malayter \n\
-- Дата создания:  ??.??.12 \n\
-- Описание:       Функция кодировния строки в Base32 \n\
-- ====================================================== \n\
 \n\
--Copyright © 2012 Ryan Malayter. All Rights Reserved.  \n\
--Redistribution and use in source and binary forms, with or without  \n\
--modification, are permitted provided that the following conditions are  \n\
--met:  \n\
--1. Redistributions of source code must retain the above copyright  \n\
--notice, this list of conditions and the following disclaimer.  \n\
--2. Redistributions in binary form must reproduce the above copyright  \n\
--notice, this list of conditions and the following disclaimer in the  \n\
--documentation and/or other materials provided with the distribution.  \n\
--3. The name of the author may not be used to endorse or promote products  \n\
--derived from this software without specific prior written permission.  \n\
--THIS SOFTWARE IS PROVIDED BY Ryan Malayter \"AS IS\" AND ANY EXPRESS OR  \n\
--IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED  \n\
--WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  \n\
--DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,  \n\
--INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES  \n\
--(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  \n\
--SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)  \n\
--HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,  \n\
--STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  \n\
--ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  \n\
--POSSIBILITY OF SUCH DAMAGE.  \n\
 \n\
CREATE FUNCTION [dbo].[Base32enc] ( \n\
	@x VARBINARY(max) \n\
	,@pad INT = 1 \n\
	) \n\
RETURNS VARCHAR(max) \n\
AS \n\
BEGIN \n\
	/* RFC 4648 compliant BASE32 encoding function, takes varbinary data to  \n\
	encode as first parameter, and then a 0 or 1 to indicate whether or not \n\
	padding characters (equals signs) should be included in the output. */ \n\
	DECLARE @p INT \n\
	DECLARE @c BIGINT \n\
	DECLARE @s BIGINT \n\
	DECLARE @q BIGINT \n\
	DECLARE @t BIGINT \n\
	DECLARE @o VARCHAR(max) \n\
	DECLARE @op VARCHAR(8) \n\
 \n\
	SET @o = '' \n\
	SET @p = DATALENGTH(@x) % 5 --encode with 40-bit blocks \n\
 \n\
	IF @p <> 0 \n\
		SET @x = @x + SUBSTRING(0x0000000000, 1, 5 - @p) \n\
	SET @c = 0 \n\
 \n\
	WHILE @c < DATALENGTH(@x) \n\
	BEGIN \n\
		SET @s = 0 \n\
		SET @t = CAST(SUBSTRING(@x, @c + 1, 5) AS BIGINT) \n\
		SET @op = '' \n\
 \n\
		WHILE @s < 8 \n\
		BEGIN \n\
			SET @q = @t % 32 \n\
			SET @op = CASE  \n\
					WHEN @q BETWEEN 0 \n\
							AND 25 \n\
						THEN CHAR(@q + 65) \n\
					ELSE CHAR(@q + 24) \n\
					END + @op \n\
			SET @t = @t / 32 \n\
			SET @s = @s + 1 \n\
		END \n\
 \n\
		SET @o = @o + @op \n\
		SET @c = @c + 5 \n\
	END \n\
 \n\
	DECLARE @padc CHAR(1) \n\
 \n\
	--padding section \n\
	SET @padc = CASE  \n\
			WHEN @pad IS NULL \n\
				OR @pad = 1 \n\
				THEN '=' \n\
			ELSE '' \n\
			END \n\
	SET @o = CASE  \n\
			WHEN @p = 1 \n\
				THEN SUBSTRING(@o, 1, DATALENGTH(@o) - 6) + REPLICATE(@padc, 6) \n\
			WHEN @p = 2 \n\
				THEN SUBSTRING(@o, 1, DATALENGTH(@o) - 4) + REPLICATE(@padc, 4) \n\
			WHEN @p = 3 \n\
				THEN SUBSTRING(@o, 1, DATALENGTH(@o) - 3) + REPLICATE(@padc, 3) \n\
			WHEN @p = 4 \n\
				THEN SUBSTRING(@o, 1, DATALENGTH(@o) - 1) + REPLICATE(@padc, 1) \n\
			ELSE @o \n\
			END \n\
 \n\
	RETURN LTRIM(RTRIM(@o)) \n\
END \n\
GO");
	std::string err;
	if ( !CheckSQL( s1, err) ){
		std::cout << err;
	}
	std::string p;
	while ( ParseScript( s1, p )) std::cout << p<<'\n';
	return 0;
}

