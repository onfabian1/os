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

ATM::ATM(int atm_id, char* input_path, Bank *bank) {
	this->m_atm_id = atm_id;
	this->m_input_path = input_path;
	this->bank = bank;
}

void ATM::run(int atm_num) { //Parse the txt file in PATH and moving to func

	ifstream fd;
	fd.open(atms[atm_num].m_input_path);
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
			if(!CheckAccExist(atoi(args[1]))){
				atms[atm_num].openAccount(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				bank->print_new_account(atms[atm_num].m_atm_id, atoi(args[1]), atoi(args[2]), atoi(args[3]));
				sleep(1);
				continue; //write success to log
			}
			bank->print_account_exist_error(atms[atm_num].m_atm_id, atoi(args[1]));
			sleep(1);
			continue; //write false to log
		}
		/*************************************************/
		else if (!strcmp(args[0], "D")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].deposit(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				bank->print_deposit(atms[atm_num].m_atm_id, atoi(args[1]),atoi(args[2]), atoi(args[3]));
				sleep(1);
				return; //write success to log
			}
			bank->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
			sleep(1);
			continue; //false
		}
		/*************************************************/
		else if (!strcmp(args[0], "W")) {
			if(!CheckAccExist(atoi(args[1]))){
				accounts[i].withdraw(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				bank->print_withdrew(atms[atm_num].m_atm_id, atoi(args[1]),atoi(args[2]), atoi(args[3]));
				sleep(1);
				return; //Success
			}
			bank->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else if (!strcmp(args[0], "B")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].Balance(atoi(args[1]), atoi(args[2]));
				bank->print_curr_balance(atms[atm_num].m_atm_id, atoi(args[1]),atoi(args[2]));
				sleep(1);
				return; //SUCCESS
			}
			bank->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
			sleep(1);
			continue; //FALSE
		}
		/*************************************************/
		else if (!strcmp(args[0], "Q")) {
			if(!CheckAccExist(atoi(args[1]))) {
				accounts[i].closeAccount(atoi(args[1]), atoi(args[2]));
				bank->print_delete_account(atms[atm_num].m_atm_id, atoi(args[1]),atoi(args[2]));
				sleep(1);
				continue; //SUCCESS
			}
			print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
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
			else if(!CheckAccExist(atoi(args[1])))
				print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1]));
			else  
				print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[3]));
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
