// TCL_User.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	
	double t1, t2;
	double runTime;
	t1 = omp_get_wtime();
	//string md5_hash = "0886f6cc13e9058351becc0f8d5d30fe"; //}}}}
	//string md5_hash = "cc92540afcd6110c91df90797aeaa197"; 
	string md5_hash = "90b1c250aff3151127fc14cc82893534"; 
	sendMD5HashToFPGA(md5_hash);
	int exit_status = system("F:\\intelFPGA_lite\\17.1\\quartus\\bin64\\quartus_stp -t FPGA_connect.tcl");
	readFromBuffer();
	t2 = omp_get_wtime();
	runTime = t2 - t1;
	cout << "Time: " << runTime << endl;
    return 0;
}

