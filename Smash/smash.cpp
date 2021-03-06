/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include <iostream>
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

using namespace std;

char* L_Fg_Cmd;
string fg_name;
pid_t curr_fg_pid;
pid_t smash_pid;
vector<Job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	char cmdString[MAX_LINE_SIZE]; 	   
	smash_pid = getpid();
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
        struct sigaction sigint_hand;
        sigint_hand.sa_handler = &sigintHandler;

        struct sigaction sigstop_hand;
        sigstop_hand.sa_handler = &sigstopHandler;
    	
	// Init globals 
        curr_fg_pid = smash_pid;
	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
    	        curr_fg_pid = smash_pid;
    	        sigaction(SIGINT, &sigint_hand, NULL);
    	        sigaction(SIGTSTP, &sigstop_hand, NULL);
	 	cout << "smash > ";
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		fg_name = cmdString;
					// background command	
	 	//if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

