// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

//#include "targetver.h";
#include <stdio.h>
//#include <cstdio>
#include "Tchar.h"
#include "stdio.h"
#include <omp.h>


#include "Coder.h"



struct AnswerString{
	string str;
	bool findString;
};
//const string SET_OF_SYMBOLS = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";

bool strEqual(string str1, string str2, int length);

void strToA_B_C_D(string str, word* A, word* B, word* C, word* D);

word makeWordFromStr(string str, int startPosition);

AnswerString* findString_1_symbol(string str, Coder* coder, word A, word B, word C, word D);

AnswerString* findString_2_symbol(string str, Coder* coder, word A, word B, word C, word D);

AnswerString* findString_3_symbol(string str, Coder* coder, word A, word B, word C, word D);

AnswerString* findString_4_symbol(string str, Coder* coder, word A, word B, word C, word D);



