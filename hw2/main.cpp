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
	atm.run();
	pthread_exit(NULL);
}

void* BankPrint(void* threadid) {
	// send to proper function
	pthread_exit(NULL)
}

void* BankCommision(void* threadid) {
	// send to proper function
	pthread_exit(NULL)
}

int main(int argc, char *argv[]) {   
    	pthread_t atm[argc-1], bank_print, bank_commisiom;
	int rc;
	int i, j;
	for (j=1; j<=argc; j++) {
		ATM new_atm(j, argv[j]);
		atms.push_back(new_atm);
	}
	for (i=0; i<argc; i++) {
		// build ATM thread and send to ATM handler
		rc = pthread_create(&(atm[i]),NULL, AtmExe, (void*)&atms[i]);  //create ATM
		if (rc) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
	}
	rc = pthread_create(&(bank_print), NULL, BankPrint, (void*)&i);
	rc = pthread_create(&(bank_commision), NULL, BankCommision, (void*)&i);
	for (i=0; i<argc; i++) {
		pthread_join(atm[i]);
	}

	pthread_cancel(bank_print);
	pthread_cancel(bank_commisiom);
	return 0;
}

