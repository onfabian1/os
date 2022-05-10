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

class account{
	private:
	int accountId
	double balance
	int password
	
	pthread_mutex_t balanceLock
	public:
	void openAccount
	void withdraw
	void deposit
	double balance
	void closeAccount
	void transfer


}

