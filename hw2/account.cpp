#include "account.h"
using namespace std
extern vector<accounts> accounts
account::account(int _accountId, int _password, int _balance): accountId(_accountId), password(_password), balance(_balance){

		pthread_mutex_init(&balanceLock, NULL);
		balance_read_counter = 0;
		
}

account::openAccount(int accountNum, int pass, int balan){
	for(unsigned int i=0; i<account>size(); i++){
		if(accountNum == accountId)
		sleep(1)
		return -1
	}
	 
	account newacc(accountNum, pass, balan)
	accounts.push_back(newacc)
	sleep(1)
	return 0
	
}

account::deposit(int accountNum, int pass, int amount){
	if(pass != password) return -1
	pthread_mutex_lock(balanceLock)
	while(balance_read_counter != 0) pthread_cond_wait(balanceLock, writePhase);
	balance_read_counter = -1;	
	balance = balance + amount;
	sleep(1);	
	balance_read_counter = -0;
	pthread_cond_broadcast(readPhase)
	pthread_cond_signal(writePhase)
	pthread_mutex_unlock(balanceLock);
	
}

