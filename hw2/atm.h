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

void DelFinJobs(vector<Job> &jobs);
int ExeComp(char* lineSize);
int BgCmd(char* args[MAX_ARG], vector<Job> &jobs, pid_t pid, int num_arg);
int ExeCmd(vector<Job> &jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_args, vector<Job>& jobs);

#endif

