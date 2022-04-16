// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include <algorithm>
#include "signals.h"
#include "commands.h"
#include <iostream>
#include <csignal>
#include <vector>

using namespace std;

extern int curr_id;
extern string fg_name;
extern vector<Job> &jobs;
extern pid_t smash_pid; 
extern pid_t curr_fg_pid;

void sigintHandler(int sig_num)
{
	if (sig_num == SIGINT) //CTRL+C
	{
		//unsigned int i = 0;
		//int id;
		//bool exist = false;
		cout << "smash: caught ctrl-C" << endl;
		if (curr_fg_pid == smash_pid) // smash pid
		{
            		return;
		}
	/*
		else
		{ 
			for (; i < jobs.size(); i++)
			{
				cout << "wer'e looking in jobs here" << endl;
				cout << jobs.size() << endl;
				if (jobs[i]._pid == curr_fg_pid)
				{
					id = jobs[i]._id;
					exist = true;
					break;
				}
			}
		}	*/
		cout << "wtf im here" << endl;
		if (!kill(curr_fg_pid, SIGKILL)) // process killed
		{
	
			cout << "smash: process " << jobs.size() << " was killed" << endl;
			return;
		}
		else
		{
			perror("smash error: kill failed");
			exit(1);
		}
	}
}	

void sigstopHandler(int sig_num)
{
	if (sig_num == SIGTSTP) //CTRL+Z
	{
	        if (curr_fg_pid == smash_pid) //smash pid
		{
	                return;
       		}	
		unsigned int i = 0;
		int id;
		cout << "smash: caught ctrl-Z" << endl;
		bool exist = false;
		for(; i < jobs.size(); i++)
		{
			cout << "in stop finding job" << endl;
			if (jobs[i]._pid == curr_fg_pid)
			{
				exist = true;
				id = jobs[i]._id;
				break;
			}
		}
		cout << "stop - yes" << endl;
		if (!kill(curr_fg_pid, SIGSTOP))
		{
			cout << "smash: process " << id << " was stopped" << endl;
			if (exist)
			{
				jobs[i]._stopped = true;			
				return;
			}
			else
			{
				Job new_job(fg_name, jobs.size(), curr_fg_pid, time(NULL));
				jobs.push_back(new_job);
         			jobs[jobs.size()-1]._stopped = TRUE;	
			}
		}
		else
		{
			perror("smash error: kill failed");
			exit(1);
		}	
	}
}
