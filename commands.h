#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <list>
#include <vector>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;
typedef enum { FALSE , TRUE } boolean;

class Job {
	public:	

		string _name;
		int _id;
		pid_t _pid;
		time_t _time;
		bool _stopped;

		Job(string _name, int _id, pid_t _pid, time_t _time);
		void stop_job();
		void print_job(time_t _time);
		~Job(){};
};

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, vector<Job> &jobs);
int ExeCmd(vector<Job> &jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_args, vector<Job>& jobs);

#endif

