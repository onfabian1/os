#include "account.h"
using namespace std
extern vector<accounts> accounts
bool exist = false

account::CheckAccExist(int accountNum) {
	exist = false;
	for(unsigned int i=0; i<account>size(); i++){ //Check if account exist 
		if(accountNum == accountId) {
			exist = true;
			break;
		}
	}
	if (!exist) 
		return false;
	return true;
}

account::account(int _accountId, int _password, double _balance): accountId(_accountId), password(_password), balance(_balance){
		pthread_mutex balanceLock;
		pthread_mutex_init(&balanceLock, NULL);
		int balance_read_counter = 0;
		
}

account::openAccount(int accountNum, int pass, int balan){
	for(unsigned int i=0; i<account>size(); i++){ //Check if account exist
		if(accountNum == accountId){
			sleep(1);
			return -1; //print "Error <ATM ID>: Your transaction failed – account with the same id exists" to log
		}
	}
	 
	account newacc(accountNum, pass, balan);
	accounts.push_back(newacc);
	sleep(1);
	return 0; //print "<ATM ID>: New account id is <id> with password <pass> and initial balance <bal>" to log
	
}

account::deposit(int accountNum, int pass, int amount){
	if (!account.CheckAccExist(accountNum)) //checks account exist
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	if(pass != password) 
		return -1; //print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log

	pthread_mutex_lock(balanceLock); //Write Phase
	while(balance_read_counter != 0)
		pthread_cond_wait(balanceLock, writePhase);
	balance_read_counter = -1;
	if (!account.CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);	
		balance_read_counter = 0;
		pthread_cond_broadcast(readPhase);
		pthread_cond_signal(writePhase);
		pthread_mutex_unlock(balanceLock);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	balance = balance + amount;
	sleep(1);	
	balance_read_counter = 0;
	pthread_cond_broadcast(readPhase);
	pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited" to log
}

account::withdraw(int accountNum, int pass, int amount){
	if (!account.CheckAccExist(accountNum)) //checks account exist
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	if(pass != password) 
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log

	pthread_mutex_lock(balanceLock); //Write Phase
	while(balance_read_counter != 0) 
		pthread_cond_wait(balanceLock, writePhase);
	balance_read_counter = -1;
	if (!account.CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);	
		balance_read_counter = 0;
		pthread_cond_broadcast(readPhase);
		pthread_cond_signal(writePhase);
		pthread_mutex_unlock(balanceLock);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	if ((balance - amount) < 0) {
		balance_read_counter = 0;
		sleep(1);	
		pthread_cond_broadcast(readPhase);
		pthread_cond_signal(writePhase);
		pthread_mutex_unlock(balanceLock);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	balance = balance - amount;
	balance_read_counter = 0;
	sleep(1);	
	balance_read_counter = 0;
	pthread_cond_broadcast(readPhase);
	pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew" to log
}

account::balance(int accountNum, int pass){
	for(unsigned int i=0; i<account>size(); i++){ //Check if account exist
		if(accountNum == accountId) {
			exist = true;
			break;
		}
	}
	if (!exist)
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	if(pass != password) 
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log

	pthread_mutex_lock(balanceLock); //Read Phase
	while (balance_read_counter == -1)
		pthread_cond_wait(balanceLock, readPhase);
	balance_read_counter++;
	if (!account.CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);
		balance_read_counter--;
		if(!balance_read_counter)
			pthread_cond_signal(writePhase);
		pthread_mutex_unlock(balanceLock);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	double bal = balance;
	sleep(1);
	balance_read_counter--;
	if(!balance_read_counter)
		pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return 0; //print "<ATM ID>: Account <id> balance is <bal>" to log
}

account::closeAccount(int accountNum, int pass){
	int acc_position=0;
	for(unsigned int i=0; i<account>size(); i++){ //Check if account exist
		if(accountNum == accountId) {
			exist = true;
			break;
		}
		acc_position++;
	}
	if (!exist)
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	if(pass != password) 
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log

	pthread_mutex_lock(balanceLock); //Write Phase = Close Phase
	while(balance_read_counter != 0) 
		pthread_cond_wait(balanceLock, writePhase);
	balance_read_counter = -1;
	if (!account.CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);
		balance_read_counter = 0;
		pthread_cond_broadcast(readPhase);
		pthread_cond_signal(writePhase);
		pthread_mutex_unlock(balanceLock);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}	
	accounts.erase(accounts.begin()+acc_position);
	sleep(1);	
	balance_read_counter = 0;
	pthread_cond_broadcast(readPhase);
	pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return 0; //print "<ATM ID>: Account <id> is now closed. Balance was <bal>" to log
}
