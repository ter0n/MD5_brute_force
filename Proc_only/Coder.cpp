#include "stdafx.h"
#include "Coder.h"




Coder::Coder(string str)
{
	string test = appenPaddingBits(str);
	setStringForCod(test);
	initBuffer();
	strLength = str.size();
	magic();
	MD5CodeOfString = bytesToStr();
}


Coder::~Coder()
{
}


void Coder::setStringForCod(string newString){
	stringForCode = newString;
}

string Coder::getMD5CodeOfString(){
	return MD5CodeOfString;
}

string Coder::getStringOfCode(){
	return stringForCode;
}

string Coder::appenPaddingBits(string oldString){

	string newStr = oldString;
	oldLength = oldString.size() * 8;
	newStr += "=";
	newStr[newStr.size()-1] = 0x80;
	//int lengthToMod448 = 448 - ((newStr.size()*8) % 512); 
	int lengthToMod448 = 512 - ((newStr.size() * 8) % 512);

	//заполнение нулями 
	unsigned char zer = 0x00;
	for (int i = 0; i < (lengthToMod448/8); i++){
		newStr += "=";
		newStr[newStr.size() - 1] = zer;
	}

	return newStr;
}

void Coder::printString(string str){
	int i = 0;
	while (str[i]){
		cout << str[i];
		i++;
	}
	cout << endl;
}

string Coder::DecToBinary(int a){
	string binary;
	int decim = a;
	bool flag = true;
	while (decim)
	{
		if (decim % 2 == 1){
			binary += "1";
		}
		else{
			binary += "0";
		}
		decim = decim / 2;
	}
	return binary;
}

void Coder::initBuffer(){
	
	A = 0x67452301;

	B = 0xEFCDAB89;

	C = 0x98BADCFE;

	D = 0x10325476;
}

word Coder::funcF(word X, word Y, word Z){
	//F(X,Y,Z) = XY v not(X) Z
	word newWord;
	newWord = (X & Y) | ((~X) & Z);
	return newWord;
}

word Coder::funcG(word X, word Y, word Z){
	//G(X,Y,Z) = XZ v Y not(Z)
	word newWord;
	newWord = (X & Z) | (Y & (~Z));
	return newWord;
}

word Coder::funcH(word X, word Y, word Z){
	//H(X,Y,Z) = X xor Y xor Z
	word newWord;
	newWord = X ^ Y ^ Z;
	return newWord;
}

word Coder::funcI(word X, word Y, word Z){
	//I(X,Y,Z) = Y xor (X v not(Z))
	word newWord;
	newWord = Y ^ (X | (~Z));
	return newWord;
}

void Coder::initT(){
	word* TT = new word[65];
	unsigned long long someConst = 4294967296;
	int PI = 3.14159265;
	for (int i = 1; i < 65; ++i){
		TT[i] = someConst * abs(sin(i));
	}
}

void Coder::magic(){

	word AA;
	word BB;
	word CC;
	word DD;
	word X[16];

	word funcResult;

	int stringSize = stringForCode.size() / 64;
	int stringIndex = 0;
	string str64;
	for (int blockNum = 0; blockNum < stringSize; blockNum++){
		for (int i = 0; i < 64; i++){
			str64 += stringForCode[stringIndex];
			stringIndex++;
		}
		strTo16Blocks(str64,X);
		if (blockNum + 1 >= stringSize)
		{
			X[14] = (word)((strLength << 3) & 0xFFFFFFFF);
			X[15] = (word)((strLength >> 29) & 0x7); // ( 32 - 3 )
		}
		/*
		for (int i = 0; i < 16; i++){
			cout << X[i] << endl;
		}
		cout << "\n";
		*/
		AA = A;
		BB = B;
		CC = C;
		DD = D;


		
		int k, s, i;
		//****************Step 1*******************************
		/* [abcd k s i] a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
		k = -1;
		i = -1;
		for (int index = 0; index < 4; index++){
			k++;
			s = 7;
			i++;
			funcResult = A + funcF(B, C, D) + X[k] + T[i];
			A = B + ROTATELEFT(funcResult,s);

			k++;
			s +=5;
			i++;
			funcResult = D + funcF(A, B, C) + X[k] + T[i];
			D = A + ROTATELEFT(funcResult, s);

			k++;
			s += 5;
			i++;
			funcResult = C + funcF(D, A, B) + X[k] + T[i];
			C = D + ROTATELEFT(funcResult, s);

			k++;
			s += 5;
			i++;
			funcResult = B + funcF(C, D, A) + X[k] + T[i];
			B = C + ROTATELEFT(funcResult, s);

		}

		//--------------End Step 1-----------------------------

		//****************Step 2*******************************
		/* [abcd k s i] a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s). */
		funcResult = A + funcG(B, C, D) + X[1] + T[16];
		A = B + ROTATELEFT(funcResult, 5);
		
		funcResult = D + funcG(A, B, C) + X[6] + T[17];
		D = A + ROTATELEFT(funcResult, 9);
		
		funcResult = C + funcG(D, A, B) + X[11] + T[18];
		C = D + ROTATELEFT(funcResult, 14);

		funcResult = B + funcG(C, D, A) + X[0] + T[19];
		B = C + ROTATELEFT(funcResult, 20);

		funcResult = A + funcG(B, C, D) + X[5] + T[20];
		A = B + ROTATELEFT(funcResult, 5);

		funcResult = D + funcG(A, B, C) + X[10] + T[21];
		D = A + ROTATELEFT(funcResult, 9);

		funcResult = C + funcG(D, A, B) + X[15] + T[22];
		C = D + ROTATELEFT(funcResult, 14);

		funcResult = B + funcG(C, D, A) + X[4] + T[23];
		B = C + ROTATELEFT(funcResult, 20);

		funcResult = A + funcG(B, C, D) + X[9] + T[24];
		A = B + ROTATELEFT(funcResult, 5);

		funcResult = D + funcG(A, B, C) + X[14] + T[25];
		D = A + ROTATELEFT(funcResult, 9);

		funcResult = C + funcG(D, A, B) + X[3] + T[26];
		C = D + ROTATELEFT(funcResult, 14);

		funcResult = B + funcG(C, D, A) + X[8] + T[27];
		B = C + ROTATELEFT(funcResult, 20);

		funcResult = A + funcG(B, C, D) + X[13] + T[28];
		A = B + ROTATELEFT(funcResult, 5);

		funcResult = D + funcG(A, B, C) + X[2] + T[29];
		D = A + ROTATELEFT(funcResult, 9);

		funcResult = C + funcG(D, A, B) + X[7] + T[30];
		C = D + ROTATELEFT(funcResult, 14);

		funcResult = B + funcG(C, D, A) + X[12] + T[31];
		B = C + ROTATELEFT(funcResult, 20);

		//--------------End Step 2-----------------------------


		//****************Step 3*******************************
		/* [abcd k s i] a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s). */
		funcResult = A + funcH(B, C, D) + X[5] + T[32];
		A = B + ROTATELEFT(funcResult, 4);

		funcResult = D + funcH(A, B, C) + X[8] + T[33];
		D = A + ROTATELEFT(funcResult, 11);

		funcResult = C + funcH(D, A, B) + X[11] + T[34];
		C = D + ROTATELEFT(funcResult, 16);

		funcResult = B + funcH(C, D, A) + X[14] + T[35];
		B = C + ROTATELEFT(funcResult, 23);

		funcResult = A + funcH(B, C, D) + X[1] + T[36];
		A = B + ROTATELEFT(funcResult, 4);

		funcResult = D + funcH(A, B, C) + X[4] + T[37];
		D = A + ROTATELEFT(funcResult, 11);

		funcResult = C + funcH(D, A, B) + X[7] + T[38];
		C = D + ROTATELEFT(funcResult, 16);

		funcResult = B + funcH(C, D, A) + X[10] + T[39];
		B = C + ROTATELEFT(funcResult, 23);

		funcResult = A + funcH(B, C, D) + X[13] + T[40];
		A = B + ROTATELEFT(funcResult, 4);

		funcResult = D + funcH(A, B, C) + X[0] + T[41];
		D = A + ROTATELEFT(funcResult, 11);

		funcResult = C + funcH(D, A, B) + X[3] + T[42];
		C = D + ROTATELEFT(funcResult, 16);

		funcResult = B + funcH(C, D, A) + X[6] + T[43];
		B = C + ROTATELEFT(funcResult, 23);

		funcResult = A + funcH(B, C, D) + X[9] + T[44];
		A = B + ROTATELEFT(funcResult, 4);

		funcResult = D + funcH(A, B, C) + X[12] + T[45];
		D = A + ROTATELEFT(funcResult, 11);

		funcResult = C + funcH(D, A, B) + X[15] + T[46];
		C = D + ROTATELEFT(funcResult, 16);

		funcResult = B + funcH(C, D, A) + X[2] + T[47];
		B = C + ROTATELEFT(funcResult, 23);

		//--------------End Step 3-----------------------------

		//****************Step 4*******************************
		/* [abcd k s i] a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
		
		funcResult = A + funcI(B, C, D) + X[0] + T[48];
		A = B + ROTATELEFT(funcResult, 6);

		funcResult = D + funcI(A, B, C) + X[7] + T[49];
		D = A + ROTATELEFT(funcResult, 10);

		funcResult = C + funcI(D, A, B) + X[14] + T[50];
		C = D + ROTATELEFT(funcResult, 15);

		funcResult = B + funcI(C, D, A) + X[5] + T[51];
		B = C + ROTATELEFT(funcResult, 21);
		
		funcResult = A + funcI(B, C, D) + X[12] + T[52];
		A = B + ROTATELEFT(funcResult, 6);

		funcResult = D + funcI(A, B, C) + X[3] + T[53];
		D = A + ROTATELEFT(funcResult, 10);

		funcResult = C + funcI(D, A, B) + X[10] + T[54];
		C = D + ROTATELEFT(funcResult, 15);

		funcResult = B + funcI(C, D, A) + X[1] + T[55];
		B = C + ROTATELEFT(funcResult, 21);

		funcResult = A + funcI(B, C, D) + X[8] + T[56];
		A = B + ROTATELEFT(funcResult, 6);

		funcResult = D + funcI(A, B, C) + X[15] + T[57];
		D = A + ROTATELEFT(funcResult, 10);

		funcResult = C + funcI(D, A, B) + X[6] + T[58];
		C = D + ROTATELEFT(funcResult, 15);

		funcResult = B + funcI(C, D, A) + X[13] + T[59];
		B = C + ROTATELEFT(funcResult, 21);

		funcResult = A + funcI(B, C, D) + X[4] + T[60];
		A = B + ROTATELEFT(funcResult, 6);

		funcResult = D + funcI(A, B, C) + X[11] + T[61];
		D = A + ROTATELEFT(funcResult, 10);

		funcResult = C + funcI(D, A, B) + X[2] + T[62];
		C = D + ROTATELEFT(funcResult, 15);

		funcResult = B + funcI(C, D, A) + X[9] + T[63];
		B = C + ROTATELEFT(funcResult, 21);

		//--------------End Step 4-----------------------------

		A = AA + A;
		B = BB + B; 
		C = CC + C;
		D = DD + D;

		//------------------End--------------------------------
		str64.clear();
	}

}

void Coder::strTo16Blocks(string srcStr, word* X){
	
	for (int i = 0, j = 0; i < 16; ++i, j += 4) {
		X[i] = ((word)(unsigned char)srcStr[j]) | (((word)(unsigned char)srcStr[j + 1]) << 8) |
			(((word)(unsigned char)srcStr[j + 2]) << 16) | (((word)(unsigned char)srcStr[j + 3]) << 24);
	}
	
}

string Coder::bytesToStr(){
	string result = "";

	result.append(wordToString(A));
	result.append(wordToString(B));
	result.append(wordToString(C));
	result.append(wordToString(D));
	return result;
}

string Coder::wordToString(word a){
	string res = "";
	word new_a = a;
	int part_A;
	int byte_from_word[2] = { 0, 0 };
	for (int i = 0; i < 4; i++){
		part_A = new_a % 256;
		byte_from_word[0] = part_A / 16;
		byte_from_word[1] = part_A % 16;
		for (int bit4Pos = 0; bit4Pos < 2; bit4Pos++){
			res += "1";
			switch (byte_from_word[bit4Pos]){
			case 0: res[res.size() - 1] = '0';
				break;
			case 1: res[res.size() - 1] = '1';
				break;
			case 2: res[res.size() - 1] = '2';
				break;
			case 3: res[res.size() - 1] = '3';
				break;
			case 4: res[res.size() - 1] = '4';
				break;
			case 5: res[res.size() - 1] = '5';
				break;
			case 6: res[res.size() - 1] = '6';
				break;
			case 7: res[res.size() - 1] = '7';
				break;
			case 8: res[res.size() - 1] = '8';
				break;
			case 9: res[res.size() - 1] = '9';
				break;
			case 10: res[res.size() - 1] = 'a';
				break;
			case 11: res[res.size() - 1] = 'b';
				break;
			case 12: res[res.size() - 1] = 'c';
				break;
			case 13: res[res.size() - 1] = 'd';
				break;
			case 14: res[res.size() - 1] = 'e';
				break;
			case 15: res[res.size() - 1] = 'f';
				break;
			}
		}
		new_a = new_a / 256;
	}

	return res;
}

void Coder::newTry(string str){
	
	string test = appenPaddingBits(str);
	setStringForCod(test);
	initBuffer();
	strLength = str.size();
	magic();
	//MD5CodeOfString = bytesToStr();
	//return MD5CodeOfString;
}

bool Coder::equalABCD(word equalA, word equalB, word equalC, word equalD){
	bool equal;
	equal = (A == equalA) && (B == equalB) && (C == equalC) && (D == equalD);
	//if ((A == equalA) && (B == equalB) && (C == equalC) && (D == equalD))
	//	equal = true;
	return equal;
}
