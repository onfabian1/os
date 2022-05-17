#include "account.h"

using namespace std

extern vector<account> accounts;
bool exist = false;
unsigned int i=0;
unsigned int j=0;

account::WriteLock() {
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
		return false; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	return true;
}

account::WriteUnlock() {
	sleep(1);	
	balance_read_counter = 0;
	pthread_cond_broadcast(readPhase);
	pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return;
}

account::ReadLock() {
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
		return false; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	return true;
}

account::ReadUnlock() {
	sleep(1);
	balance_read_counter--;
	if(!balance_read_counter)
		pthread_cond_signal(writePhase);
	pthread_mutex_unlock(balanceLock);
	return; 
}

account::CheckAccExist(int accountNum) {
	exist = false;
	for(i=0; i<accounts.size(); i++){ //Check if account exist 
		if(accountNum == accountId) {
			exist = true;
			break;
		}
	}
	if (!exist) 
		return false;
	return true;
}

account::CheckTargetAccExist(int accountNum) {
	exist = false;
	for(j=0; j<accounts.size(); j++){ //Check if account exist 
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

account::openAccount(int accountNum, int pass, double balan){
	if(account.CheckAccExist(accountNum)){ //checks account exist
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – account with the same id exists" to log
	}
	account newacc(accountNum, pass, balan);
	accounts.push_back(newacc);
	sleep(1);
	return 0; //print "<ATM ID>: New account id is <id> with password <pass> and initial balance <bal>" to log
}

account::deposit(int accountNum, int pass, double amount){
	i = 0;
	if (!account.CheckAccExist(accountNum)) { //checks account exist
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	if(pass != accounts[i].password) {
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if (!accounts[i].WriteLock())
		return -1;
	balance = balance + amount;
	accounts[i].WriteUnlock()
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited" to log
}

account::withdraw(int accountNum, int pass, double amount){
	if (!account.CheckAccExist(accountNum)) { //checks account exist
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	if (!accounts[i].WriteLock())
		return -1;
	if(pass != accounts[i].password) {
		sleep(1);
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if ((balance - amount) < 0) {
	accounts[i].WriteUnlock()
	return 0; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	balance = balance - amount;
	account.WriteUnlock();
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew" to log
}

account::balance(int accountNum, int pass){
	if (!account.CheckAccExist(accountNum)) { //checks account exist
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	if(pass != accounts[i].password) {
		sleep(1);
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if (!accounts[i].ReadLock())
		return -1;
	double bal = balance;
	accounts[i].ReadUnlock();
	return 0; //print "<ATM ID>: Account <id> balance is <bal>" to log
}

account::closeAccount(int accountNum, int pass){
	if (!account.CheckAccExist(accountNum)) { //checks account exist
		sleep(1);
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	if(pass != accounts[i].password) {
		sleep(1);
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if(!accounts[i].WriteLock())
		return -1;
	accounts[i].WriteUnlock(); //could be a problem but for now I dont see another solution	
	accounts.erase(accounts.begin()+i); //remove from list accounts
	return 0; //print "<ATM ID>: Account <id> is now closed. Balance was <bal>" to log
}

account::transfer(int accountNum, int pass, int targetAccountNum, double amount){
	if (!(account.CheckAccExist(accountNum)) { //checks account exist
		sleep(1);
		return -1; 
	}
	if (!(account.CheckAccExist(targetAccountNum)) { //checks target account exist
		sleep(1);
		return -1; 
	}
	if(pass != accounts[i].password) { //check src pass
		sleep(1);
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if(pass != accounts[j].password) { //check dest pass
		sleep(1);
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if(!accounts[i].WriteLock()) //catch source write lock
		return -1; 

	if(!accounts[j].WriteLock()) //catch destination write lock (potential DEADLOCK w/BANK)
		return -1; 
	
	if ((accounts[i].balance - amount) < 0) { //src balance
		accounts[i].WriteUnlock();
		accounts[j].WriteUnlock();
		return 0; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	accounts[i].balance -= amount; //src balance
	accounts[j].balance += amount; //dest balance
	accounts[i].WriteUnlock();
	accounts[j].WriteUnlock();
	return 0; //print "<ATM ID>: Transfer <amount> from account <account> to account <target_account> new account balance is <account_bal> new target account balance is <target_bal>" to log	
}
