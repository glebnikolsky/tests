// Lib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>

#include "db.h"

int main()
{
    setlocale(LC_ALL, "ru-RU");
    db::CreateDb("\\tmp\\libra.db");
    db::Fill("\\tmp\\libra.db", "\\books");
    db::CalcCrc("\\tmp\\libra.db", "\\books");
}

