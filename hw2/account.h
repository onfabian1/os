#ifndef _ACCOUNT_H
#define _ACCOUNT_H
#include <unistd.h> 
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <vector>

using namespace std;

class account {

	private:
	int password;
	double balance;
	pthread_mutex_t balanceLock;
	pthread_cond_t writePhase;
	pthread_cond_t readPhase;

	public:
	int accountId;
	account(int _password, double _balance,int _accountId);
	bool WriteLock(int _accountId);
	void WriteUnlock();
	bool ReadLock(int _accountId);
	void ReadUnlock();
	int deposit(int _accountId, int _password, double _balance);	
	int withdraw(int _accountId, int _password, double _balance);
	int Balance(int _accountId, int _password);
	int closeAccount(int _accountId, int _password);
	int transfer(int _accountId, int _password, int _targetAccountId, double _balance);
	int balance_read_counter;
	double commisionToBank(int _accountId);
};


#endif
