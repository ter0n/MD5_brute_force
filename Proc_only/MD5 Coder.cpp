// MD5 Coder.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"




int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	string str = "md5";
	/*
	 из-за особенностей строки в Си++ необходимо некоторые символы вводить так: 
	 "\\" = \
	 "\'" = '
	 "\"" = " 
	*/
	string setOfSymbols = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";
	int lengthSetOfSymbols = setOfSymbols.size();
	Coder *coder = new Coder(str);
	string MD5Code = "336d5ebc5436534e61d16e63ddfca327";
	double t1, t2;
	double runTime;
	t1 = omp_get_wtime();
	word A, B, C, D;
	strToA_B_C_D(MD5Code, &A, &B, &C, &D);
	AnswerString* answStr = findString_1_symbol("", coder, A, B, C, D);
	
	if (answStr->findString == false) 
	{
		delete answStr;
		answStr = findString_2_symbol("", coder, A, B, C, D);
		if (answStr->findString == false)
		{
			delete answStr;
			answStr = findString_3_symbol("", coder, A, B, C, D);
			if (answStr->findString == false)
			{
				delete answStr;
				answStr = findString_4_symbol("", coder, A, B, C, D);
			}
		}
	}
	t2 = omp_get_wtime();
	runTime = t2 - t1;
	cout << "Строка, по которой можно получить заданный MD5, найдена:" << endl;
	cout << answStr->str << endl;
	cout << "Время выполнения: " << runTime << endl;
	
	delete coder;

	
	return 0;
}

