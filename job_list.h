#ifndef _JOBLIST_H
#define _JOBLIST_H
#include "job.h"
#include "string.h"
#include <time.h>
//#include <sys/typed.h>
#include <sys/wait.h>
#include <cstdbool>
#include <list>
#include <iostream>
#include <signal.h>

using namespace std;

class JobList {
	public:
		
		list<Job> job_list;

		JobList();
		~JobList();
		void print_list();
		void job_insert(Job& Job);
		void job_remove(int job_id);
		pid_t get_pid(int job_id);
		time_t get_job_time(int job_id);
};

#endif



