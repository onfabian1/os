/*	smash.c
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
#define MAX_LINE_SIZE 80

using namespace std;

vector<Account> accounts; //This represents the list of accounts. Please change to a preferred type (e.g array of char*)
vector<ATM> atms;
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of bank. get command from user and calls command functions
//**************************************************************************************

void *Init(void *threadid) {
   long tid;
   tid = (long)threadid;
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	time_t start_time = time(NULL);	   
    	pthread_t atm[argc-1];
	int rc;
	int i;
	for (i=0; i<argc; i++) {
		// build ATM thread and insert to atms vector
		rc = pthread_create(&atm[i],NULL, Init, (void *)i);  
		if (rc) {
			log.txt << "Error: unable to create thread, " << rc << endl;
			exit(-1);
		}
		else {
			ExeAtm(atm[i], i, argv[i+1]);
    	while (1) {
		time_tot = difftime(time(NULL), start_time);
		if (time_tot >= 0.5) { 
			// print all accounts (w/ Sync R/W)
			start_time = time(NULL);
		}
		// if EOF in all ATM's -> break
	}
    return 0;
}

