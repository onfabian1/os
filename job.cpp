#include "job.h"
using namespace std;

Job::Job(string job_name, int job_id, pid_t job_pid, time_t job_time): job_name(job_name), job_id(job_id), job_pid(job_pid), job_time(job_time)
	{
		//job_name += '\0';
		stopped = false;
	}

Job::~Job(){}

void Job::stop_job()
	{
		stopped = true;
	}


void Job::print_job()
	{
		cout << job_id << job_name << ":" << job_pid << job_time << endl;
	}

