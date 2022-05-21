#ifndef _ATM_H
#define _ATM_H
#include <unistd.h> 
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include "log.h"

#define MAX_ARG 5

using namespace std;
typedef enum { FALSE , TRUE } boolean;

class ATM {
	private:
	Log *log_file;
	int m_atm_id;
	char* m_input_path;

	public:
	ATM(int atm_id, char* input_path, Log* log_file);
	void run();
	int openAccount(int _accountId, int _password, double _balance);
	int closeAccount(int _accountId, int _password, int acc_num);
	~ATM(){};
};

int CheckAccExist(int _accountId);
bool CheckTargetAccExist(int _accountId);

#endif

