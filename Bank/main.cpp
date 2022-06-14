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
#include "account.h"

using namespace std;

//Log log_file("log.txt");
//int listReaders = 0;
vector<ATM> atms;//This represents the list of accounts.
extern vector<account> accounts;
int counter = 0;
bool FINISH = false;

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
	Bank& bank_pri = *(Bank*)bank;
	while(!FINISH){
		sleep(0.5);
		bank_pri.StatusPrint();
	}
	pthread_exit(NULL);
}

void* BankCommision(void* bank) {
	Bank& bank_comm = *(Bank*)bank;
	while(!FINISH) {
		cout << "ERGDZRGRDG" << endl;
		cout << FINISH << endl;
		sleep(3);
		bank_comm.getCommisions();
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {  
	if (argc == 1) {
		cout << "Bank error: illegal arguments" << endl;
		exit(-1);
	}
    	pthread_t *atm = new pthread_t[argc-1];
	pthread_t bank_commision, bank_print;
	Log log_file("log.txt");
	Bank bank(&log_file);
	int rc1, rc2, rc3;
	int j=0;
	for (j=1; j<=argc; j++) {
		ATM new_atm(j, argv[j], &log_file, &bank);
		atms.push_back(new_atm);
	}
	for (counter=0; counter<argc-1; counter++) {
		// build ATM thread and send to ATM handler
		rc1 = pthread_create(&(atm[counter]),NULL, AtmExe, (void*)&atms[counter]);  //create ATM
		if (rc1) {
			perror("pthread_create: failed");
			exit(-1);
		}
	}
	rc2 = pthread_create(&(bank_print), NULL, BankPrint, (void*)&bank);
	if (rc2) {
			perror("pthread_create: failed");
			exit(-1);
		}
	rc3 = pthread_create(&(bank_commision), NULL, BankCommision, (void*)&bank);
	if (rc3) {
			perror("pthread_create: failed");
			exit(-1);
		}
	for (int i=0; i<argc-1; i++) {
		pthread_join(atm[i], NULL);
	}
	FINISH = true;
	for (unsigned int i=0; i<accounts.size(); i++) {
		accounts.erase(accounts.begin()+i); //remove from list accounts
		//accounts[i].~account();
	}
	pthread_cancel(bank_print);
	pthread_cancel(bank_commision);
	return 0;
}

