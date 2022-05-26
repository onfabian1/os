#include "account.h"
#include <iostream>
#include <pthread.h>

using namespace std;

vector<account> accounts; //This represents the list of accounts.
unsigned int i=0;
unsigned int j=0;

void account::WriteLock() {
	sleep(1);
	pthread_mutex_lock(&balanceLock); //Write Phase
	while(balance_read_counter != 0){
		pthread_cond_wait(&writePhase, &balanceLock);
	}
	balance_read_counter = -1;
	return;
}

void account::WriteUnlock() {	
	balance_read_counter = 0;
	pthread_cond_broadcast(&readPhase);
	pthread_cond_signal(&writePhase);
	pthread_mutex_unlock(&balanceLock);
	return;
}

void account::ReadLock() {
	sleep(1);
	pthread_mutex_lock(&balanceLock); //Read Phase
	while (balance_read_counter == -1)
		pthread_cond_wait(&readPhase, &balanceLock);
	balance_read_counter++;
	return;
}

void account::ReadUnlock() {
	balance_read_counter--;
	if(!balance_read_counter)
		pthread_cond_signal(&writePhase);
	pthread_mutex_unlock(&balanceLock);
	return; 
}

account::account(int _accountId, int _password ,double _balance): accountId(_accountId), password(_password), balance(_balance) {
		pthread_mutex_init(&balanceLock, nullptr);
		pthread_cond_init(&readPhase,NULL);
		pthread_cond_init(&writePhase,NULL);
		balance_read_counter = 0;
}

account::~account(){
	pthread_mutex_destroy(&balanceLock);
	pthread_cond_destroy(&readPhase);
	pthread_cond_destroy(&writePhase);
}

int account::deposit(int accountNum, int pass, double amount, int acc_num){
	accounts[acc_num].WriteLock();
	if(pass != accounts[acc_num].password) {
		accounts[acc_num].WriteUnlock();
		return -1; //print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	accounts[acc_num].balance += amount;
	accounts[acc_num].WriteUnlock();
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited" to log
}

int account::withdraw(int accountNum, int pass, double amount, int acc_num){
	accounts[acc_num].WriteLock();
	if(pass != accounts[acc_num].password) {
		accounts[acc_num].WriteUnlock();
		return 1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if ((accounts[acc_num].balance - amount) < 0) {
		accounts[acc_num].WriteUnlock();
		return 2; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	accounts[acc_num].balance -= amount;

	accounts[acc_num].WriteUnlock();
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew" to log
}

int account::Balance(int accountNum, int pass, int acc_num){
	accounts[acc_num].ReadLock();
	if(pass != accounts[acc_num].password) {
		accounts[acc_num].ReadUnlock();
		return 1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	//double bal = accounts[i].balance;
	accounts[acc_num].ReadUnlock();
	return 0; //print "<ATM ID>: Account <id> balance is <bal>" to log
}

int account::transfer(int accountNum, int pass, int targetAccountNum, double amount, int acc_num, int acc_tar_num){
	accounts[acc_num].WriteLock(); //catch source write lock 
	accounts[acc_tar_num].WriteLock(); //catch destination write lock (potential DEADLOCK w/BANK)

	if(pass != accounts[acc_num].password) { //check src pass
		accounts[acc_num].WriteUnlock();
		accounts[acc_tar_num].WriteUnlock();
		return 1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	
	if ((accounts[acc_num].balance - amount) < 0) { //src balance
		accounts[acc_num].WriteUnlock();
		accounts[acc_tar_num].WriteUnlock();
		return 2; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	accounts[acc_num].balance -= amount; //src balance
	accounts[acc_tar_num].balance += amount; //dest balance
	accounts[acc_num].WriteUnlock();
	accounts[acc_tar_num].WriteUnlock();
	return 0; //print "<ATM ID>: Transfer <amount> from account <account> to account <target_account> new account balance is <account_bal> new target account balance is <target_bal>" to log	
}
