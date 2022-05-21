#ifndef _BANK_H
#define _BANK_H
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

class Bank {
	private:
	//pthread_mutex_t globalBankLock;
	int bankReaders;
	int listReaders;
    	pthread_mutex_t bankReadLock;
    	pthread_mutex_t bankWriteLock;
    	pthread_mutex_t listReadLock;
    	pthread_mutex_t listWriteLock;

	public:
	Bank(/*double bankMoney, int bankReaders, int listReaders*/);
	~Bank();	
	void getCommisions();
	void StatusPrint();
	void LockListRead();
	void UnlockListRead();
	void LockListWrite();
	void UnlockListWrite();
	void LockBankRead();
	void UnlockBankRead();
	void LockBankWrite();	
	void UnlockBankWrite();
	double bankMoney;
};

#endif
