// ACDSelector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "selector.h"


int main()
{

	Selector sel;
	sel.Open("ACDSelector.xml");
	if (sel.IsOpen()) {
		std::cout << sel.GetComboName("/Selector/RecheckFederal") << std::endl;
		sel.ResetSelection("/Selector/RecheckFederal/Criterion");
		auto resn = sel.GetNames();
		for (auto i : resn) std::cout << i << std::endl;
		auto resr = sel.GetRequired(3);
		for (auto i : resr) std::cout << i << std::endl;
		sel.ResetSelection("/Selector/RecheckRegional/Criterion");
		sel.GetNames();
		std::cout << sel.GetAddFunc(2) << std::endl << sel.GetSql(2);
	}
	else {
		std::cout << sel.GetError() << std::endl;
		std::cout << sel.GetFileName() << std::endl;
	}


    return 0;
}

