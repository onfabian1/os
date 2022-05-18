//		atm.cpp
//********************************************
#include "atm.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <pthread.h>
#include "account.h"

const unsigned int ATM_USLEEP = 100000;

using namespace std;

extern unsigned int i;
extern int counter;
extern vector<ATM> atms;
extern vector<account> accounts;

/*************************************************/
// Parse of txt -> line by line
// Parse of line -> word by word
// handle line command 
// Sync R/W for Accounts
// returns counter-- when EOF
/*************************************************/

ATM::ATM(int atm_id, char* input_path) :
	m_atm_id(atm_id), m_input_path(input_path) {}

void ATM::run() { //Parse the txt file in PATH and moving to func

	ifstream fd;
	fd.open(atms[m_atm_id-1].m_input_path);
	if (fd.fail()) {
		perror("");
		exit(1);
	}
	char* cmd;
	string line;
	getline(fd, line);
	char const* delimiters = " \t\n";
	char* args[MAX_ARG];  
	int i = 0, num_arg = 0;

	while(!fd.eof()) {
		cmd = strtok(&line[0], delimiters);
		if (args[0] == NULL)
			// print error
			return; 
		args[0] = cmd;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
 
		}
		/*************************************************/
		if (!strcmp(args[0], "O")) {
			if(CheckAccExist(atoi(args[1]))){
				openAccount(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				sleep(1);
				return; //write success to log
			}
			sleep(1);
			continue; //write false to log
		}
		/*************************************************/
		else if (!strcmp(args[0], "D")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].deposit(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				sleep(1);
				return; //write success to log
			}
			sleep(1);
			continue; //false
		}
		/*************************************************/
		else if (!strcmp(args[0], "W")) {
			if(!CheckAccExist(atoi(args[1]))){
				accounts[i].withdraw(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				sleep(1);
				return; //Success
			}
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else if (!strcmp(args[0], "B")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].Balance(atoi(args[1]), atoi(args[2]));
				sleep(1);
				return; //SUCCESS
			}
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else if (!strcmp(args[0], "Q")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].closeAccount(atoi(args[1]), atoi(args[2]));
				sleep(1);
				continue; //SUCCESS
			}
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else if (!strcmp(args[0], "T")) {
			if(!CheckAccExist(atoi(args[1])) && !CheckTargetAccExist(atoi(args[3]))) {
				accounts[i].transfer(atoi(args[1]), atoi(args[2]), atoi(args[3]), atoi(args[4]));
				sleep(1);
				continue; //SUCESS
			}
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else {
			//log.txt << "Illegal action" << endl;
			continue;
		}
		/*************************************************/
		getline(fd,line);
	}
	fd.close();	
	counter--;
	return;
}
