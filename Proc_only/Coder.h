#pragma once

#include <iostream>
#include <cmath>
#include "Tchar.h"
#include "stdio.h"
#include <string>


//циклический сдвиг влево на n бит
#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))
using namespace std;

typedef unsigned int word;

static const word T[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


class Coder
{
/********************************/
//Методы
public:
	Coder(string str);
	~Coder();

	void setStringForCod(string newString);
	
	string getStringOfCode();

	string getCodeOfString();
	string getMD5CodeOfString();

	void printString(string str);

	string DecToBinary(int a);

	void initBuffer();

	word funcF(word X, word Y, word Z);

	word funcG(word X, word Y, word Z);

	word funcH(word X, word Y, word Z);

	word funcI(word X, word Y, word Z);

	long oldLength;

	void strTo16Blocks(string srcStr, word* X);

	void initT();

	void magic();

	void newTry(string str);

	string bytesToStr();

	bool equalABCD(word equalA, word equalB, word equalC, word equalD);

	 

private:
	
	string appenPaddingBits(string oldString);
	string wordToString(word a);

/*______________________________*/


/********************************/
//Переменные
private:

	string stringForCode;
	string MD5CodeOfString;

	word A;
	word B; 
	word C;
	word D;

	long strLength;

	static const char HEX_NUMBERS[16];

/*______________________________*/
};

