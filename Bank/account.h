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

	public:
	int accountId;
	int password;
	double balance;
	pthread_mutex_t balanceLock;
	pthread_cond_t writePhase;
	pthread_cond_t readPhase;
	~account();

	account(int _accountId, int _password,double _balance);
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();
	int deposit(int _accountId, int _password, double _balance, int acc_num);	
	int withdraw(int _accountId, int _password, double _balance, int acc_num);
	int Balance(int _accountId, int _password, int acc_num);
	int transfer(int _accountId, int _password, int _targetAccountId, double _balance, int acc_num, int acc_tar_num);
	int balance_read_counter;
	double commisionToBank(int _accountId);
};


#endif
