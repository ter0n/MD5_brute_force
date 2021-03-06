// stdafx.cpp: исходный файл, содержащий только стандартные включаемые модули
// TCL_User.pch будет использоваться в качестве предкомпилированного заголовка
// stdafx.obj будет содержать предварительно откомпилированные сведения о типе

#include "stdafx.h"

// TODO: Установите ссылки на любые требующиеся дополнительные заголовки в файле STDAFX.H
// , а не в данном файле

void writeToBuffer(string str) {
	ofstream fout("buffer_to_FPGA");
	if (!fout.is_open()) {
		//записать в файл, что невозможно открыть файл для записи
		return;
	}
	else
	{
		fout << str << endl;
		fout.close();
	}
}

void MD5HashTo_A_B_C_D(string md5HAsh, word* A, word* B, word* C, word* D) {
	if (md5HAsh.size() == 32) {
		(*A) = makeWordFromStr(md5HAsh, 0);
		(*B) = makeWordFromStr(md5HAsh, 8);
		(*C) = makeWordFromStr(md5HAsh, 16);
		(*D) = makeWordFromStr(md5HAsh, 24);
	}
}

word makeWordFromStr(string str, int startPosition) {
	int endPosition = startPosition + 7;
	int degree = 28;
	word newWord = 0;

	for (int i = endPosition; i >= startPosition; i -= 2) {
		for (int j = 0; j < 2; j++) {
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

void writeABCD_ToBuffer(word A, word B, word C, word D) {
	ofstream fout("buffer_to_FPGA");
	if (!fout.is_open()) {
		//записать в файл, что невозможно открыть файл для записи
		return;
	}
	else
	{
		fout << A << endl;
		fout << B << endl;
		fout << C << endl;
		fout << D << endl;
		fout.close();
	}
}

void sendMD5HashToFPGA(string md5Hash) {
	word A, B, C, D;
	MD5HashTo_A_B_C_D(md5Hash, &A, &B, &C, &D);
	writeABCD_ToBuffer(A, B, C, D);
}

void readFromBuffer() {
	ifstream fin("buffer_from_FPGA");
	if (!fin.is_open()) {
		//записать в файл, что невозможно открыть файл для чтения
		return ;
	}
	else
	{
		string buff;
		string MD5String = "";
		char symb;
		for (int i = 0; i < 4; i++) {
			getline(fin, buff);
			if ((buff != "00000000") && (buff != "10000000")) {
				symb = BitsToChar(buff);
				MD5String = MD5String + symb;
			}
		}
		cout << "Result: " << MD5String << endl;
		fin.close();
	}
}

char BitsToChar(string buff) {
	char symb = 0;
	if (buff.size() == 8) {
		for (int i = 0; i < 8; i++) {
			if (buff[i] == '1')
				symb = (symb << 1) + 1;
			else
				symb = (symb << 1);
		}
	}
	return symb;
}