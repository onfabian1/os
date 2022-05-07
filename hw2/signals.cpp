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
#include <typeinfo>

using namespace std;

extern string fg_name;
extern vector<Job> jobs;
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
		if (!kill(curr_fg_pid, SIGKILL)) // process killed
		{
	
			cout << "smash: process " << curr_fg_pid << " was killed" << endl;
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
		cout << "smash: caught ctrl-Z" << endl;
	        if (curr_fg_pid == smash_pid) //smash pid
		{
	                return;
       		}	
		int id;
		if (!kill(curr_fg_pid, SIGSTOP))
		{
			cout << "smash: process " << curr_fg_pid << " was stopped" << endl;
			if (jobs.empty())
			{
				id = 1;
			}
			else
			{
				id = jobs.back()._id + 1;
			}
			Job new_job(fg_name, id, curr_fg_pid, time(NULL));
			new_job._stopped = true;
			jobs.push_back(new_job);			
			return;
		}
		else
		{
			perror("smash error: kill failed");
			exit(1);
		}	
	}
}
