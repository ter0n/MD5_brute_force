// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <omp.h>

using namespace std;

typedef unsigned int word;


void writeToBuffer(string str);

void writeABCD_ToBuffer(word A, word B, word C, word D);

void MD5HashTo_A_B_C_D(string md5HAsh, word* A, word* B, word* C, word* D);

word makeWordFromStr(string str, int startPosition);

void sendMD5HashToFPGA(string md5Hash);

void readFromBuffer();

char BitsToChar(string buff);

// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
