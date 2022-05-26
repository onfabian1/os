/*	main.cpp
main file.
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include "atm.h"
#include "bank.h"

using namespace std;

Log log_file("log.txt");
Bank *bank = new Bank(&log_file);
vector<ATM> atms;//This represents the list of accounts.
int counter = 0;

//**************************************************************************************
// function name: main
// Description: main function. get argumants from user and calls atms, bank_print and bank_commision
//**************************************************************************************

void *AtmExe(void* m_atm) {
	ATM atm = *(ATM*)m_atm; 
	atm.run();
	pthread_exit(NULL);
}

void* BankPrint(void* bank) {
	Bank bank_pri = *(Bank*)bank;
	while(!atms.empty()){
		sleep(0.5);
		//bank_pri.StatusPrint();
	}
	pthread_exit(NULL);
}

void* BankCommision(void* bank) {
	Bank bank_comm = *(Bank*)bank;
	while(!atms.empty()){
		sleep(3);
		//bank_comm.getCommisions();
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {  
    	pthread_t *atm = new pthread_t[argc-1];
	pthread_t bank_commision, bank_print;
	int rc1, rc2, rc3;
	int j=0;
	for (j=1; j<=argc; j++) {
		ATM new_atm(j, argv[j], &log_file);
		atms.push_back(new_atm);
	}
	for (counter=0; counter<argc-1; counter++) {
		// build ATM thread and send to ATM handler
		rc1 = pthread_create(&(atm[counter]),NULL, AtmExe, (void*)&atms[counter]);  //create ATM
		if (rc1) {
			cout << "Error: unable to create thread, " << rc1 << endl;
			exit(-1);
		}
	}
	rc2 = pthread_create(&(bank_print), NULL, BankPrint, (void*)&bank);
	if (rc2) {
			//log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	rc3 = pthread_create(&(bank_commision), NULL, BankCommision, (void*)&bank);
	if (rc3) {
			//log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	for (int i=0; i<argc-1; i++) {
		pthread_join(atm[i], nullptr);
		cout << "FINISH!!!!" << endl;
	}

	pthread_cancel(bank_print);
	pthread_cancel(bank_commision);
	return 0;
}

