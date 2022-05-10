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
	int id;
	char* input_path;

	public:
	ATM(long id, char* input_path);
	void run();
	~ATM(){};
};

#endif

