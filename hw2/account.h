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
	account(int _accountId, int _password, int _balance);
	int openAccount(int accountId, int password, int balance);
	int deposit();	
	int withdraw();
	double balance();
	int closeAccount();
	int transfer();
	int balance_read_counter;
};

#endif
