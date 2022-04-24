//		commands.cpp
//********************************************
#include "commands.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************

using namespace std;

extern pid_t curr_fg_pid;
extern string fg_name;
bool is_prev = false;
char prevDir[MAX_LINE_SIZE];


Job::Job(string job_name, int job_id, pid_t job_pid, time_t job_time): _name(job_name), _id(job_id), _pid(job_pid), _time(job_time)
	{
		// _name += '\0';
		_stopped = false;
	}
			
void DelFinJobs(vector<Job> &jobs){
	for (unsigned int i = 0; i < jobs.size(); i++) {
        	if (waitpid(jobs[i]._pid, NULL, WNOHANG) != 0) {
            	jobs.erase(jobs.begin() + i);
       		}
	}
   
}

void Job::stop_job()
	{
		_stopped = true;
	}


void Job::print_job(time_t time)
	{
		double time_tot = difftime(time, _time);
		if (_stopped)
		{
			cout << "[" << _id << "]" << _name << ":" << _pid << " " << time_tot << " secs " << " (stopped)" << endl;
		}
		else
			cout << "[" << _id << "] " << _name << " : " << _pid << " " << time_tot <<" secs " << endl;
	}

int ExeCmd(vector<Job> &jobs, char* lineSize, char* cmdString)
{
	curr_fg_pid = getpid();
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char const* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
	DelFinJobs(jobs);
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd")) 
	{
		char tmp[MAX_LINE_SIZE];
		if (num_arg != 1) {
			cout << "smash error: " << cmd << ": too many arguments" << endl;
			return 1;
		}
		if (!strcmp(args[1],"-")) {
			if (!is_prev) {
				cout << "smash error: " << cmd << ": OLDPWD not set" << endl;
				return 1;
			}
			else {
				if (getcwd(tmp, MAX_LINE_SIZE) == NULL) {
					perror("smash error: getcwd failed");
					exit(1);
				}
				if (chdir(prevDir)) {
					perror("smash error: chdir failed)");
					exit(1);
				}
				else {
					strcpy(prevDir,tmp);
					return 0;
				}
			}
		}
		if (getcwd(prevDir, MAX_LINE_SIZE) == NULL) {
			perror("smash error: getcwd failed");
			exit(1);
		}
		else {
		strcpy(tmp, prevDir);
		}
		if (chdir(args[1])) {
			perror("smash error: chdir failed");
			exit(1);
		}
		else {
			is_prev = true;
			return 0;
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if (getcwd(pwd, MAX_LINE_SIZE) == NULL) {
			perror("smash error: getcwd failed");
			exit(1);
		}
		cout << pwd << endl;
	}

	/*************************************************/
	else if (!strcmp(cmd, "kill")) 
	{ 
		if (num_arg != 2) {
			cout << "smash error: kill: invalid arguments" << endl;
			return 1;
		}
		pid_t pid;
		int signum = atoi(args[1] + 1);
		int id = atoi(args[2]);
		bool job_exist = false;
		if ((args[1][0] != '-') || (signum > 64) || !id || id < 0 || !signum) {
			cout << "smash error: kill: invalid arguments" << endl;
			return 1;
		}
		if (!jobs.empty()) {
			vector<Job>::iterator it;
			for(it = jobs.begin(); it!=jobs.end(); ++it) {
				if (it->_id == id) { 
					job_exist = true;
					pid = it->_pid;
					break;
				}
			}
		}
		if (!job_exist) {
			cout << "smash error: kill: job-id " << id << " does not exist" << endl;
			return 1;
		}	
		if(!!kill(pid, signum)) {
			perror("smash error: kill failed");
			exit(1);
		}	
		return 0;
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{
		if (!jobs.empty())
		{
			vector<Job>::iterator it;
			for(it = jobs.begin(); it!=jobs.end(); ++it){
				time_t curr_time = time(NULL);
				it->print_job(curr_time);
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
			cout << "smash pid is " << getpid() << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		if (!jobs.empty()){
			if (num_arg > 1){
			cout << "smash error: "<< cmd <<": invalid arguments" << endl;
			return 1;
			}
			if(num_arg == 0){
				cout << "[" << jobs.back()._id << "] " << jobs.back()._name << " : " << jobs.back()._pid << " " << difftime(time(NULL), jobs.back()._time) <<" secs " << endl;
				kill(jobs.back()._pid, SIGCONT);
				waitpid(jobs.back()._pid, NULL, WUNTRACED);
				return 0;
			}
			else{
				for (unsigned int i = 0; i < jobs.size(); i++) {
        				if (jobs[i]._id == atoi(args[1])){
					cout << "[" << jobs[i]._id << "] " << jobs[i]._name << " : " << jobs[i]._pid << " " << difftime(time(NULL), jobs[i]._time) <<" secs " << endl;
            				kill(jobs[i]._pid, SIGCONT);
					waitpid(jobs[i]._pid, NULL, WUNTRACED);
					return 0;
					}
       				}
				cout << "smash error: "<< cmd << ": job-id " << args[1] <<" does not exist" << endl;
				return 1;
			}
				
		}
		else{
			if(num_arg == 0){
				cout << "smash error: "<< cmd <<": jobs list is empty" << endl;
				return 1;
			}		
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		int max_id = 0;
		int job_to_resume = 0;
  		if (!jobs.empty()){
			if (num_arg > 1){
			cout << "smash error: "<< cmd <<": invalid arguments" << endl;
			return 1;
			}
			else if(num_arg == 0){
				for (unsigned int i = 0; i < jobs.size(); i++) {
        				if (jobs[i]._id > max_id && jobs[i]._stopped){
						max_id = jobs[i]._id;
						job_to_resume = i;
					}
				}
				if(max_id != 0){
					jobs[job_to_resume]._stopped = false;
					cout << "[" << jobs[job_to_resume]._id << "] " << jobs[job_to_resume]._name << " : " << jobs[job_to_resume]._pid << " " << difftime(time(NULL), jobs[job_to_resume]._time) <<" secs " << endl;
					kill(jobs.back()._pid, SIGCONT);
					return 0;
				}
				else{
					cout << "smash error: " << cmd << ": there are no stopped jobs to resum" << endl;
					return 1;
				}
			}
			else{
				for (unsigned int i = 0; i < jobs.size(); i++) {
        				if (jobs[i]._id == atoi(args[1])){
						if(jobs[i]._stopped){	
							jobs[i]._stopped = false;
							cout << "[" << jobs[i]._id << "] " << jobs[i]._name << " : " << jobs[i]._pid << " " << difftime(time(NULL), jobs[i]._time) <<" secs " << endl;
			    				kill(jobs[i]._pid, SIGCONT);
							return 0;
						}
						else{
							cout << "smash error: " << cmd << ": job-id " << args[1] << "is already running in the background" << endl;
					return 1;
						}
					}
       				}
				cout << "smash error: "<< cmd << ": job-id " << args[1] <<" does not exist" << endl;
				return 1;
			}
				
		}
		else{
			if(num_arg == 0){
				cout << "smash error: "<< cmd <<": jobs list is empty" << endl;
				return 1;
			}		
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg == 0)
		{
			exit(0);
		}
		 else if(!strcmp(args[1], "kill"))
		{
			if (!jobs.empty()) {
				pid_t pid;
				double time_tot;
				vector<Job>::iterator it;
				for(it = jobs.begin(); it!=jobs.end(); ++it) {
					time_t stop_time = time(NULL);
					time_t start_time = time(NULL);
					cout << "[" << it->_id << "] " << it->_name << " - " << "Sending SIGTERM...";
					if(!!kill(it->_pid, SIGTERM)) {
						perror("smash error: kill failed");
						exit(1);
					}
					while(1) {
						time_tot = difftime(stop_time, start_time);
						if (time_tot >= 5) {
							if (!!kill(it->_pid, SIGKILL)) {
								perror("smash error: kill failed");
								exit(1);
							}
							cout << " (5 sec passed) Sending SIGKILL... Done" << endl;
							break;
						}
						pid = waitpid(it->_pid, NULL, WNOHANG);
						if (pid == -1) {
							perror("smash error: waitpid failed");
							exit(1);
						}
						else if (pid == 0) {
							stop_time = time(NULL);
						}
						else {
							cout << " Done" << endl;
							break;
						}
					}
				}
			}
			exit(0);
		}
		else 
		{
			cout << "smash error: " << cmd << ": too many arguments" << endl;
			return 1;
		}
		
			
	} 

	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg != 2)
		{
			cout << "smash error: " << cmd << ": invalid arguments" << endl;
			return 1;	
		}
		char c1, c2;
		ifstream fd1, fd2;
		fd1.open(args[1]);
		fd2.open(args[2]);
		while(!fd1.eof() && !fd2.eof())
		{
			if (fd1.eof() || fd2.eof())
			{ // files are not the same size
				cout << "1" << endl;
				fd1.close();
				fd2.close();
				return 1;
			}
			fd1.get(c1);
			fd2.get(c2);
			if (c1 != c2)
			{ // files are not equal
				cout << "1" << endl;
				fd1.close();
				fd2.close();
				return 1;
			}
		} // files are identical
		fd1.close();
		fd2.close();
		cout << "0" << endl;
		return 0;

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, num_arg, jobs);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}

//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************

void ExeExternal(char* args[MAX_ARG], char* cmdString, int num_arg, vector<Job> &jobs) {

	int pID;
	//bool flag = true;
	//int status;
	char *lineSize = args[num_arg];
    	switch(pID = fork()) 
	{
    		case -1 : 
			// Error 
			perror("smash error: fork failed");
			exit(1);

        	case 0 :// Child Process
               		setpgrp();
			if (lineSize[strlen(lineSize)-1] == '&'){
				lineSize[strlen(lineSize)-1] = '\0';
				args[num_arg]=NULL;
			}
			execvp(args[0], args);
			perror("smash error: execv failed");
			if(!!kill(getpid(), SIGKILL))
			{
				perror("smash error: kill failed");
				exit(1);
			}

		default:
			if (lineSize[strlen(lineSize)-1] == '&'){
				BgCmd(args, jobs, pID, num_arg);
			}
			else{
			//flag = false;
			curr_fg_pid = pID;
			waitpid(pID, NULL, WUNTRACED);
			}
                	// Parent process

			
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* args[MAX_ARG], vector<Job> &jobs, int pID, int num_arg)
{

	//char* Command;
	//char const* delimiters = " \t\n";
	//char *args[MAX_ARG];
	char *lineSize = args[num_arg];
	if (lineSize[strlen(lineSize)-1] == '&')
	{
		int id;
		if(jobs.empty()){ 
			id = 1;
		}
		else{
			id=jobs.back()._id+1;
		} 
		lineSize[strlen(lineSize)-1] = '\0';
		Job new_job(fg_name, id, pID, time(NULL));
		jobs.push_back(new_job);
		return 0;
		
	}
	return -1;
}
