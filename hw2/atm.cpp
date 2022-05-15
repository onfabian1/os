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

const unsigned int ATM_USLEEP = 100000;

using namespace std;

extern int counter;

/*************************************************/
// Parse of txt -> line by line
// Parse of line -> word by word
// handle line command 
// Sync R/W for Accounts
// returns counter-- when EOF
/*************************************************/

ATM::ATM(int atm_id, char* input_path) :
	m_atm_id(atm_id), m_input_path(input_path) {}

ATM::~ATM() {}

void ATM::run() { //Parse the txt file in PATH and moving to func

	ifstream fd;
	fd.open(ATM.m_input_path);
	if (fd.fail()) {
		perror("");
		return 1;
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
			return 0; 
		args[0] = cmd;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
 
		}
		/*************************************************/
		if (!strcmp(args[0], 'O') {
			account.openAccount(args[1], args[2], args[3]);
			continue; //need to delete "continue" after writing the func
		}
		/*************************************************/
		else if (!strcmp(args[0], 'D')) {
			account.deposit(args[1], args[2], args[3]);
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'W')) {
			account.withdraw(args[1], args[2], args[3]);
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'B')) {
			account.balance(args[1], args[2]);
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'Q')) {
			account.closeAccount(args[1], args[2]);
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'T')) {
			account.transfer(args[1], args[2], args[3], args[4]);
			continue;
		}
		/*************************************************/
		else {
			log.txt << "Illegal action" << endl;
			continue;
		}
		/*************************************************/
		getline(fd,line);
	}
	fd1.close();	
	counter--;
	return;
