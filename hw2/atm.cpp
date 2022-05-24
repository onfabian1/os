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
#include "bank.h"

const unsigned int ATM_USLEEP = 100000;

using namespace std;

extern Bank *bank;
extern vector<ATM> atms;
extern vector<account> accounts;

/*************************************************/
// Parse of txt -> line by line
// Parse of line -> word by word
// handle line command 
// Sync R/W for Accounts
// returns counter-- when EOF
/*************************************************/

ATM::ATM(int atm_id, char* input_path, Log *log_file) {
	this->m_atm_id = atm_id;
	this->m_input_path = input_path;
	this->log_file = log_file;
}

int CheckAccExist(int accountNum) { //returns acc_num in acc list
	for(unsigned int i=0; i<accounts.size(); i++){ //Check if account exist 
		if(accountNum == accounts[i].accountId) {
			return i;
		}
	}
	return -1;
}

int ATM::openAccount(int accountNum, int pass, double balan){
	account newacc(accountNum, pass, balan);
	accounts.push_back(newacc);
	return 0; //print "<ATM ID>: New account id is <id> with password <pass> and initial balance <bal>" to log
}

int ATM::closeAccount(int accountNum, int pass, int acc_num){
	accounts[acc_num].WriteLock();
	if(pass != accounts[acc_num].password) {
		accounts[acc_num].WriteUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	accounts[acc_num].WriteUnlock(); //could be a problem but for now I dont see another solution	
	accounts.erase(accounts.begin()+acc_num); //remove from list accounts
	return 0; //print "<ATM ID>: Account <id> is now closed. Balance was <bal>" to log
}

void ATM::run() { //Parse the txt file in PATH and moving to func
	int atm_num = this->m_atm_id-1;
	int i = 0, acc_num, acc_tar_num, ret;
	ifstream fd;
	fd.open(this->m_input_path);
	if (fd.fail()) {
		perror("");
		exit(1);
	}
	char* cmd;
	string line;
	char const* delimiters = " \t\n";
	char* args[MAX_ARG];  

	while(!fd.eof()) {
		getline(fd, line);
		cmd = strtok(&line[0], delimiters);
		if (cmd == NULL) {
			return; 
		}
		args[0] = cmd;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters);
		}
		/*************************************************/
		if (!strcmp(args[0], "O")) {
			cout << "O1 " << atm_num << endl;
			bank->LockListWrite();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));

			if (acc_num == -1) {
				atms[atm_num].openAccount(atoi(args[1]), atoi(args[2]), atoi(args[3]));
				cout << "O2 " << atm_num << endl;
				bank->UnlockListWrite();  // lock accounts list
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_new_account(atms[atm_num].m_atm_id, atoi(args[1]), atoi(args[2]), atoi(args[3]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
				continue; //write success to log
			}
			bank->UnlockListWrite();  // lock accounts list
			atms[atm_num].log_file->log_lock();
			atms[atm_num].log_file->print_account_exist_error(atms[atm_num].m_atm_id);
			atms[atm_num].log_file->log_unlock();
			sleep(1);
			continue; //write false to log
		}
		/*************************************************/
		else if (!strcmp(args[0], "D")) {
			cout << "D!! " << atm_num << endl;
			bank->LockListRead();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));
			bank->UnlockListRead();  // Unlock accounts list
			if (acc_num == -1) {//acc not exist
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else if (accounts[acc_num].deposit(atoi(args[1]), atoi(args[2]), atoi(args[3]), acc_num) == -1) {//pass invalid
					atms[atm_num].log_file->log_lock();
					atms[atm_num].log_file->print_password_is_invalid(atms[atm_num].m_atm_id, atoi(args[1]));
					atms[atm_num].log_file->log_unlock();
					sleep(1);
				}
			else {//success
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_deposit(atms[atm_num].m_atm_id, atoi(args[1]),accounts[acc_num].balance, atoi(args[3]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			
		}
		/*************************************************/
		else if (!strcmp(args[0], "W")) {
			cout << "W!! " << atm_num << endl;
			bank->LockListRead();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));
			bank->UnlockListRead();  // Unlock accounts list
			if (acc_num == -1) {//acc not exist
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
				atms[atm_num].log_file->log_unlock();
				sleep(1);
				continue;
			}
			ret = accounts[acc_num].withdraw(atoi(args[1]), atoi(args[2]), atoi(args[3]), acc_num);
			if (ret == 1) {//pass invalid
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_password_is_invalid(atms[atm_num].m_atm_id, atoi(args[1]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else if (ret == 2) {//bal<0
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_not_enough_to_withdrew(atms[atm_num].m_atm_id, atoi(args[1]), atoi(args[3]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else {//success
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_withdrew(atms[atm_num].m_atm_id, atoi(args[1]),accounts[acc_num].balance, atoi(args[3]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
		}
		/*************************************************/
		else if (!strcmp(args[0], "B")) {
			cout << "B!! " << atm_num << endl;
			bank->LockListRead();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));
			bank->UnlockListRead();  // Unlock accounts list
			if (acc_num == -1) { //acc not exist
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else if (accounts[acc_num].Balance(atoi(args[1]), atoi(args[2]), acc_num) == -1) {//pass invalid
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_password_is_invalid(atms[atm_num].m_atm_id, atoi(args[1]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else { //success
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_curr_balance(atms[atm_num].m_atm_id, atoi(args[1]),accounts[acc_num].balance);
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
		}
		/*************************************************/
		else if (!strcmp(args[0], "Q")) {
			cout << "Q!! " << atm_num << endl;
			bank->LockListRead();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));
			bank->UnlockListRead();  // Unlock accounts list

			if (acc_num == -1) { //acc not exist
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1])); 
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else if (atms[atm_num].closeAccount(atoi(args[1]), atoi(args[2]), acc_num) == -1) {//pass invalid
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_password_is_invalid(atms[atm_num].m_atm_id, atoi(args[1]));
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
			else {//success
				atms[atm_num].log_file->log_lock();
				atms[atm_num].log_file->print_delete_account(atms[atm_num].m_atm_id, atoi(args[1]),accounts[acc_num].balance);
				atms[atm_num].log_file->log_unlock();
				sleep(1);
			}
		}
		/*************************************************/
		else if (!strcmp(args[0], "T")) {
			cout << "T!! " << atm_num << endl;
			bank->LockListRead();  // lock accounts list
			acc_num = CheckAccExist(atoi(args[1]));
			bank->UnlockListRead();  // Unlock accounts list

			bank->LockListRead();// lock accounts list
			acc_tar_num = CheckAccExist(atoi(args[3]));
			bank->UnlockListRead();  // Unlock accounts list

			if(acc_num == -1)//src acc not exist
				log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[1]));

			else if (acc_tar_num == -1) //dest acc not exist
				log_file->print_account_not_exist(atms[atm_num].m_atm_id, atoi(args[3]));

			ret = accounts[acc_num].transfer(atoi(args[1]), atoi(args[2]), atoi(args[3]), atoi(args[4]), acc_num, acc_tar_num);
			if (ret == 1) {//pass src invalid
				atms[atm_num].log_file->print_password_is_invalid(atms[atm_num].m_atm_id, atoi(args[1]));
				sleep(1);
			}
			else if (ret == 2) {//bal<0
				atms[atm_num].log_file->print_not_enough_to_withdrew(atms[atm_num].m_atm_id, atoi(args[1]), atoi(args[4]));
				sleep(1);
			}
			else //success
				atms[atm_num].log_file->print_transfer(atms[atm_num].m_atm_id, atoi(args[4]), atoi(args[1]), atoi(args[3]), accounts[acc_num].balance, accounts[acc_tar_num].balance);
			sleep(1);
		}

		/*************************************************/
		else {
			//log.txt << "Illegal action" << endl;
			continue;
		}
		/*************************************************/
	}
	fd.close();	
	return;
}
