#ifndef _JOB_H
#define _JOB_H
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <cstdbool>
#include <iostream>
using namespace std;

class Job {
	private:	

		string job_name;
		int job_id;
		pid_t job_pid;
		time_t job_time;
	public:
		Job(string job_name, int job_id, pid_t job_pid, time_t job_time);
		bool stopped;
		void stop_job();
		void print_job();
		~Job();
};

#endif
