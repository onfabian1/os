/*	bank.cpp
main file. This file contains the main function of smash
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

vector<Account> accounts; //This represents the list of accounts.
int counter = 0;
//**************************************************************************************
// function name: main
// Description: main function of bank. get argumants from user and calls atms
//**************************************************************************************

void *Init(void *threadid) {
   long tid;
   tid = (long)threadid;
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	time_t start_time_commision = time(NULL);
	time_t start_time_print = time(NULL);	   
    	pthread_t atm[argc-1];
	int rc;
	int i;
	for (i=0; i<argc; i++) {
		// build ATM thread and send to ATM handler
		rc = pthread_create(&(atm[i]),NULL, ExeAtm, (void*)&i);  //create ATM
		if (rc) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
		else {
			ExeAtm(atm[i], i+1, argv[i+1]); //send ATM to handler
			counter++;
			// semaphore down
		}
		time_tot = difftime(time(NULL), start_time);
		if (time_tot >= 0.5) { 
			// print all accounts (w/ Sync R/W)
			start_time = time(NULL);
		}
	}
    	while (1) { //print all accounts every 0.5 sec
		time_tot = difftime(time(NULL), start_time);
		if (time_tot >= 0.5) { 
			// print all accounts (w/ Sync R/W)
			start_time = time(NULL);
		}
		if (counter == 0) break; // if EOF in all ATM's -> break
	}
    return 0;
}

