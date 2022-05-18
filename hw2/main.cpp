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
#include "log.h"

using namespace std;

vector<account> accounts; //This represents the list of accounts.
vector<ATM> atms;//This represents the list of accounts.
int counter = 0;
//**************************************************************************************
// function name: main
// Description: main function. get argumants from user and calls atms, bank_print and bank_commision
//**************************************************************************************

void *AtmExe(void* m_atm) {
	ATM atm = *(ATM*)m_atm; 
	atm.run(atm_num);
	pthread_exit(NULL);
}

void* BankPrint(void* bank_print) {
	bank 
	pthread_exit(NULL)
}

void* BankCommision(void* bank_comm) {
	// send to proper function
	pthread_exit(NULL)
}

int atm_num = 0;

int main(int argc, char *argv[]) {   
    	pthread_t atm[argc-1], bank_print, bank_commisiom;
	Bank bank;
	int rc1, rc2, rc3;
	int j=0;
	Log log_file("log.txt");
	bank.bank_log = &log_file;
	for (j=1; j<=argc; j++) {
		ATM new_atm(j, argv[j], &bank);
		atms.push_back(new_atm);
	}
	for (atm_num=0; i<argc; i++) {
		// build ATM thread and send to ATM handler
		rc1 = pthread_create(&(atm[atm_num]),NULL, AtmExe, (void*)&atms[atm_num]);  //create ATM
		if (rc1) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	}
	rc2 = pthread_create(&(bank_print), NULL, BankPrint, (void*)&i);
	if (rc2) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	rc3 = pthread_create(&(bank_commision), NULL, BankCommision, (void*)&i);
	if (rc3) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	for (i=0; i<argc; i++) {
		pthread_join(atm[i]);
	}

	pthread_cancel(bank_print);
	pthread_cancel(bank_commisiom);
	return 0;
}

