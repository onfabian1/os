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
using namespace std

class bank {
	private:
	double BankMoney;

	public:
	void getCommissions(int _accountId, int _password, double _balance);
	void statusPrint(int _accountId, int _password, double _balance);	
};

#endif
