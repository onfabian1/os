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
#define MAX_ARG 20

using namespace std;
typedef enum { FALSE , TRUE } boolean;

class ATM {
	private:
	int m_atm_id;
	char* m_input_path;

	public:
	ATM(int atm_id, char* input_path, Bank* bank);
	void run(int atm_num);
	~ATM(){};
};

int openAccount(int _accountId, int _password, double _balance);
bool CheckAccExist(int _accountId);
bool CheckTargetAccExist(int _accountId);

#endif

