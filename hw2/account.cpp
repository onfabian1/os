#include "account.h"

using namespace std;

extern vector<account> accounts;
bool exist = false;
unsigned int i=0;
unsigned int j=0;

bool CheckAccExist(int accountNum) {
	exist = false;
	for(i=0; i<accounts.size(); i++){ //Check if account exist 
		if(accountNum == accounts[i].accountId) {
			exist = true;
			break;
		}
	}
	if (!exist) 
		return false;
	return true;
}

bool account::WriteLock(int accountNum) {
	pthread_mutex_lock(&balanceLock); //Write Phase
	while(balance_read_counter != 0)
		pthread_cond_wait(&writePhase, &balanceLock);
	balance_read_counter = -1;
	if (!CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);	
		balance_read_counter = 0;
		pthread_cond_broadcast(&readPhase);
		pthread_cond_signal(&writePhase);
		pthread_mutex_unlock(&balanceLock);
		return false; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	return true;
}

void account::WriteUnlock() {
	sleep(1);	
	balance_read_counter = 0;
	pthread_cond_broadcast(&readPhase);
	pthread_cond_signal(&writePhase);
	pthread_mutex_unlock(&balanceLock);
	return;
}

bool account::ReadLock(int accountNum) {
	pthread_mutex_lock(&balanceLock); //Read Phase
	while (balance_read_counter == -1)
		pthread_cond_wait(&readPhase, &balanceLock);
	balance_read_counter++;
	if (!CheckAccExist(accountNum)) { //checks account exist after waiting
		sleep(1);
		balance_read_counter--;
		if(!balance_read_counter)
			pthread_cond_signal(&writePhase);
		pthread_mutex_unlock(&balanceLock);
		return false; //print "Error <ATM ID>: Your transaction failed – account id <id> does not exist" to log
	}
	return true;
}

void account::ReadUnlock() {
	sleep(1);
	balance_read_counter--;
	if(!balance_read_counter)
		pthread_cond_signal(&writePhase);
	pthread_mutex_unlock(&balanceLock);
	return; 
}

bool CheckTargetAccExist(int accountNum) {
	exist = false;
	for(j=0; j<accounts.size(); j++){ //Check if account exist 
		if(accountNum == accounts[j].accountId) {
			exist = true;
			break;
		}
	}
	if (!exist) 
		return false;
	return true;
}

account::account(int _password, double _balance ,int _accountId): password(_password), balance(_balance), accountId(_accountId) {
		pthread_mutex_init(&balanceLock, nullptr);
		balance_read_counter = 0;
}

int openAccount(int accountNum, int pass, double balan){
	account newacc(accountNum, pass, balan);
	accounts.push_back(newacc);
	return 0; //print "<ATM ID>: New account id is <id> with password <pass> and initial balance <bal>" to log
}

int account::deposit(int accountNum, int pass, double amount){
	if (!accounts[i].WriteLock(accountNum))
		return -1;
	if(pass != accounts[i].password) {
		accounts[i].WriteUnlock();
		return -1; //print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	accounts[i].balance += amount;
	accounts[i].WriteUnlock();
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited" to log
}

int account::withdraw(int accountNum, int pass, double amount){
	if (!accounts[i].WriteLock(accountNum))
		return -1;
	if(pass != accounts[i].password) {
		accounts[i].WriteUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if ((accounts[i].balance - amount) < 0) {
		accounts[i].WriteUnlock();
		return 0; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	accounts[i].balance -= amount;
	accounts[i].WriteUnlock();
	return 0; //print "<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew" to log
}

int account::Balance(int accountNum, int pass){
	if (!accounts[i].ReadLock(accountNum))
		return -1;
	if(pass != accounts[i].password) {
		accounts[i].ReadUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	//double bal = accounts[i].balance;
	accounts[i].ReadUnlock();
	return 0; //print "<ATM ID>: Account <id> balance is <bal>" to log
}

int account::closeAccount(int accountNum, int pass){
	if(!accounts[i].WriteLock(accountNum))
		return -1;
	if(pass != accounts[i].password) {
		accounts[i].WriteUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	accounts[i].WriteUnlock(); //could be a problem but for now I dont see another solution	
	accounts.erase(accounts.begin()+i); //remove from list accounts
	return 0; //print "<ATM ID>: Account <id> is now closed. Balance was <bal>" to log
}

int account::transfer(int accountNum, int pass, int targetAccountNum, double amount){
	if(!accounts[i].WriteLock(accountNum)) //catch source write lock
		return -1; 
	if(!accounts[j].WriteLock(accountNum)) //catch destination write lock (potential DEADLOCK w/BANK)
		return -1; 

	if(pass != accounts[i].password) { //check src pass
		accounts[i].WriteUnlock();
		accounts[j].WriteUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	if(pass != accounts[j].password) { //check dest pass
		accounts[i].WriteUnlock();
		accounts[j].WriteUnlock();
		return -1;//print "Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect" to log
	}
	
	if ((accounts[i].balance - amount) < 0) { //src balance
		accounts[i].WriteUnlock();
		accounts[j].WriteUnlock();
		return -1; //print "Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>" to log
	}
	
	accounts[i].balance -= amount; //src balance
	accounts[j].balance += amount; //dest balance
	accounts[i].WriteUnlock();
	accounts[j].WriteUnlock();
	return 0; //print "<ATM ID>: Transfer <amount> from account <account> to account <target_account> new account balance is <account_bal> new target account balance is <target_bal>" to log	
}
