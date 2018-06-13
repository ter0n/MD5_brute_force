// stdafx.cpp: исходный файл, содержащий только стандартные включаемые модули
// MD5 Coder.pch будет предкомпилированным заголовком
// stdafx.obj будет содержать предварительно откомпилированные сведения о типе

#include "stdafx.h"
#include <string>

// TODO: Установите ссылки на любые требующиеся дополнительные заголовки в файле STDAFX.H
// , а не в данном файле


bool strEqual(string str1, string str2, int length){
	bool equal = true;
	int i = 0;
	while (equal && (i < length))
	{
		if (str1[i] != str2[i])
		{
			equal = false;
		}
		i++;
	}
	return equal;
}

void strToA_B_C_D(string str, word* A, word* B, word* C, word* D){
	if (str.size() == 32){
		(*A) = makeWordFromStr(str, 0);
		(*B) = makeWordFromStr(str, 8);
		(*C) = makeWordFromStr(str, 16);
		(*D) = makeWordFromStr(str, 24);
	}
}

word makeWordFromStr(string str, int startPosition){
	int endPosition = startPosition + 7;
	int degree = 28;
	word newWord = 0;
	
	for (int i = endPosition; i >=startPosition ; i -=2){
		for (int j = 0; j < 2; j++){
			switch (str[i - 1 + j])
			{
			case 'f':
				newWord += 15 << degree;
				degree -= 4;
				break;
			case 'F':
				newWord += 15 << degree;
				degree -= 4;
				break;
			case 'e':
				newWord += 14 << degree;
				degree -= 4;
				break;
			case 'E':
				newWord += 14 << degree;
				degree -= 4;
				break;
			case 'd':
				newWord += 13 << degree;
				degree -= 4;
				break;
			case 'D':
				newWord += 13 << degree;
				degree -= 4;
				break;
			case 'c':
				newWord += 12 << degree;
				degree -= 4;
				break;
			case 'C':
				newWord += 12 << degree;
				degree -= 4;
				break;
			case 'b':
				newWord += 11 << degree;
				degree -= 4;
				break;
			case 'B':
				newWord += 11 << degree;
				degree -= 4;
				break;
			case 'a':
				newWord += 10 << degree;
				degree -= 4;
				break;
			case 'A':
				newWord += 10 << degree;
				degree -= 4;
				break;
			case '9':
				newWord += 9 << degree;
				degree -= 4;
				break;
			case '8':
				newWord += 8 << degree;
				degree -= 4;
				break;
			case '7':
				newWord += 7 << degree;
				degree -= 4;
				break;
			case '6':
				newWord += 6 << degree;
				degree -= 4;
				break;
			case '5':
				newWord += 5 << degree;
				degree -= 4;
				break;
			case '4':
				newWord += 4 << degree;
				degree -= 4;
				break;
			case '3':
				newWord += 3 << degree;
				degree -= 4;
				break;
			case '2':
				newWord += 2 << degree;
				degree -= 4;
				break;
			case '1':
				newWord += 1 << degree;
				degree -= 4;
				break;
			case '0':
				degree -= 4;
				break;
			}
		}
	}
	
	return newWord;
}

AnswerString* findString_1_symbol(string str, Coder* coder, word A, word B, word C, word D){
	
	bool findString = false;
	AnswerString* answerStr = new AnswerString;

	string setOfSymbols = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";
	int lengthSetOfSymbols = setOfSymbols.size();

	//int MD5CodeLength = MD5Code.size();

	string resultMD5 = "";
	string strTry = "1";
	strTry.append(str);
	int symbNum = 0;
	while (!findString && (symbNum < lengthSetOfSymbols))
	{
		strTry[0] = setOfSymbols[symbNum];
		coder->newTry(strTry);
		//findString = strEqual(MD5Code, resultMD5, MD5CodeLength);
		findString = coder->equalABCD(A, B, C, D);

		symbNum++;
	}
	answerStr->findString = findString;
	answerStr->str = strTry;
	return answerStr;
}

AnswerString* findString_2_symbol(string str, Coder* coder, word A, word B, word C, word D)
{
	AnswerString* answerStr = new AnswerString;
	answerStr->findString = false;
	//bool findString = false;
	string setOfSymbols = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";
	int lengthSetOfSymbols = setOfSymbols.size();

	string strTry = "1";
	strTry.append(str);
	int symbNum = 0;
	while (!answerStr->findString && (symbNum < lengthSetOfSymbols))
	{
		strTry[0] = setOfSymbols[symbNum];
		answerStr = findString_1_symbol(strTry, coder, A, B, C, D);
		symbNum++;
	}

	return answerStr;
}

AnswerString* findString_3_symbol(string str, Coder* coder, word A, word B, word C, word D)
{
	AnswerString* answerStr = new AnswerString;
	answerStr->findString = false;
	//bool findString = false;
	string setOfSymbols = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";
	int lengthSetOfSymbols = setOfSymbols.size();

	string strTry = "1";
	strTry.append(str);
	int symbNum = 0;
	while (!answerStr->findString && (symbNum < lengthSetOfSymbols))
	{
		strTry[0] = setOfSymbols[symbNum];
		answerStr = findString_2_symbol(strTry, coder, A, B, C, D);
		symbNum++;
	}

	return answerStr;
}

AnswerString* findString_4_symbol(string str, Coder* coder, word A, word B, word C, word D)
{
	AnswerString* answerStr = new AnswerString;
	AnswerString* stepAnswer = new AnswerString;
	answerStr->findString = false;
	string setOfSymbols = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,/\\\'\"";
	int lengthSetOfSymbols = setOfSymbols.size();

	string strTry = "1";
	strTry.append(str);
	int symbNum = 0;
	
	while (!answerStr->findString && (symbNum < lengthSetOfSymbols))
	{
		strTry[0] = setOfSymbols[symbNum];
		answerStr = findString_3_symbol(strTry, coder, A, B, C, D);
		symbNum++;
	}
	return answerStr;
}
