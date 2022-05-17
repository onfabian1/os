#ifndef _ATM_H
#define _ATM_H
#include <unistd.h> 
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <vector>
using namespace std

class account {
	private:
	int accountId;
	int password;
	double balance;
	pthread_mutex_t balanceLock;
	pthread_cond_t writePhase;
	pthread_cond_t readPhase;

	public:
	account(int _accountId, int _password, double _balance);
	bool WriteLock();
	void WriteUnlock();
	bool ReadLock();
	void ReadUnlock();
	int openAccount(int _accountId, int _password, double _balance);
	int deposit(int _accountId, int _password, double _balance);	
	int withdraw(int _accountId, int _password, double _balance);
	double balance(int _accountId, int _password);
	int closeAccount(int _accountId, int _password);
	int transfer(int _accountId, int _password, int _targetAccountId, double _balance);
	int balance_read_counter;
	double commisionToBank(int _accountId);
};

	bool CeckAccExist(int _accountId);
	bool CheckTargetAccExist(int _accountId);

#endif
