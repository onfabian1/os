//		atms.cpp
//********************************************
#include "atm.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <pthread.h>

using namespace std;

extern int counter;

/*************************************************/
// Parse of txt -> line by line
// Parse of line -> word by word
// handle line command 
// Sync R/W for Accounts
// returns counter-- when EOF
/*************************************************/
void ExeAtm(pthread thread, int atm_id, char* path) { //Parse the txt file in PATH and moving to func

	ifstream fd;
	fd.open(path);
	if (fd.fail()) {
		perror("");
		return 1;
	}
	char* cmd;
	string line;
	getline(fd,line);
	char const* delimiters = " \t\n";
	char* args[MAX_ARG];  
	int i = 0, num_arg = 0;

	while(!fd.eof()) {
		cmd = strtok(&line[0], delimiters);
		if (args[0] == NULL)
			return 0; 
		args[0] = cmd;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
 
		}
		/*************************************************/
		if (!strcmp(args[0], 'O') {
			//calls func opening account
			continue; //need to delete "continue" after writing the func
		}
		/*************************************************/
		else if (!strcmp(args[0], 'D')) {
			//calls func Deposite
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'W')) {
			//calls func Withdraw
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'B')) {
			//calls func Ballance
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'Q')) {
			//calls func Quit account
			continue;
		}
		/*************************************************/
		else if (!strcmp(args[0], 'T')) {
			//calls func Transaction
			continue;
		}
		/*************************************************/
		else {
			log.txt << "Illegal action" << endl;
			continue;
		}
		/*************************************************/
		getline(fd,line);
	}
	fd1.close();	
	counter--;
	return;
/*
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************

void ExeExternal(char* args[MAX_ARG], char* cmdString, int num_arg, vector<Job> &jobs) {
return -1;
}
/*
