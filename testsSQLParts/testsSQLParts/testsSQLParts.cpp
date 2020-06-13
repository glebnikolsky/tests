// testsSQLParts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

string teststr="   /****** Object:  Table [dbo].[useTypes]    Script Date: 08/26/2015 13:14:20 \n******/\n\
SET ANSI_NULLS ON\n\
GO\n\
GO\n\
SET QUOTED_IDENTIFIER ON\n\
GO\n\
               CREATE TABLE [dbo].[useTypes](\n\
               [ID] [int] NOT NULL,\n\
               [Name] [nvarchar](255) NULL,\n\
               [Code1] [int] NULL,\n\
               [Code2] [int] NULL,\n\
               [Code] [int] NULL,\n\
               CONSTRAINT [PK_useTypes] PRIMARY KEY CLUSTERED \n\
               (\n\
               [ID] ASC\n\
               )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]\n\
               ) ON [PRIMARY]\n\
GO\n\
               /****** Object:  Table [dbo].[useStatuses]    Script Date: 08/26/2015 13:14:20 ******/\n\
               SET ANSI_NULLS ON\n\
GO\n\
               SET QUOTED_IDENTIFIER ON\n\
GO\n\
               CREATE TABLE [dbo].[useStatuses](\n\
               [ID] [int] NOT NULL,\n\
               [Name] [nvarchar](255) NOT NULL,\n\
               [Code] [smallint] NOT NULL,\n\
               [Stage] [int] NOT NULL,\n\
               CONSTRAINT [PK_useStatuses] PRIMARY KEY CLUSTERED \n\
               (\n\
               [ID] ASC\n\
               )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]\n\
               ) ON [PRIMARY]\n\
GO\n\
               /****** Object:  Table [dbo].[useIPWSReports]    Script Date: 08/26/2015 13:14:20 ******/\n\
               SET ANSI_NULLS ON\n\
GO\n\
               SET QUOTED_IDENTIFIER ON\n\
GO\n\
               SET ANSI_PADDING OFF\n\
GO\n\
               CREATE TABLE [dbo].[useIPWSReports](\n\
               [ID] [uniqueidentifier] NOT NULL,\n\
               [DateCreate] [datetime] NOT NULL,\n\
               [WorkMode] [int] NOT NULL,\n\
               [Owner] [uniqueidentifier] NOT NULL,\n\
               [PPE] [uniqueidentifier] NULL,\n\
               [ATE] [uniqueidentifier] NULL,\n\
               [ExamID] [int] NOT NULL,\n\
               [TextValue] [varchar](2048) NULL,\n\
               CONSTRAINT [PK_useIPWSReports] PRIMARY KEY CLUSTERED \n\
               (\n\
               [ID] ASC\n\
               )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]\n\
               ) ON [PRIMARY]\n\
GO\n\
               SET ANSI_PADDING OFF\n\
GO\n\
\n\
GO\n\
/*dskjfhasjkdhfkjsadf kjasdfkjhasdjkgfhjkasdghftasd*/\nSET QUOTED_IDENTIFIER ON\n";



bool GetNextSQL(string &sql, string &partsql)
{
    size_t b;
    partsql = "";
    for(;!sql.empty();){
        b = sql.find("\nGO");
        if ( b == string::npos) {partsql = sql; sql = ""; boost::trim(partsql); break;}
        partsql = sql.substr(0,b);
        sql = sql.substr(b+3);
        boost::trim(partsql);
        if ( !partsql.empty() ) break;
    }
    return !partsql.empty();
}


void ReplaceComments(string &sql)
{
    size_t b = 0;
    while( (b = sql.find("/*")) != string::npos ){
        size_t e = sql.find("*/");
        if ( e == string::npos ) throw out_of_range("Ќесоответствие открывающих и закрывающих /* */");
        e += 2;
        sql.erase(b,e-b+1);
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    string sql;
    ReplaceComments(teststr);
    cout<<teststr<<endl<<endl;

    while ( GetNextSQL(teststr, sql) )
    {
        cout<< sql<<endl<<endl;
    }


	return 0;
}

