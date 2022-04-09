//		commands.cpp
//********************************************
#include "commands.h"
#include <iostream>
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
using namespace std;
int counter = 0;
char prevDir[MAX_LINE_SIZE];
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
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
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd")) 
	{
		if (num_arg > 1)
		{
			cout << "smash error: " << cmd << ": too many arguments" << endl;
			return 1;
		}
		if (!strcmp(args[1],"-"))
		{
			if (!counter)
			{
				cout << "smash error: " << cmd << ": OLDPWD not set" << endl;
				return 1;
			}
			else
			{
				if (!chdir(prevDir))
				{
					cout << prevDir << endl;
					return 0;
				}
			}
		}
		else if (num_arg == 1)
		{
			getcwd(prevDir, MAX_LINE_SIZE);
			if (!chdir(args[1]))
			{
				counter++;
				return 0;
			}
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		getcwd(pwd, MAX_LINE_SIZE);
		cout << pwd << endl;
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
			cout << "smash pid is " << getpid() << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
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

void ExeExternal(char* args[MAX_ARG], char* cmdString)
{
	int pID;
	int status;
    	switch(pID = fork()) 
	{
    		case -1 : 
			// Error 
			perror("smash error: fork failed");
			exit(1);

        	case 0 :
                	// Child Process
               		setpgrp();
			execvp(cmdString, args);
			perror("smash error: execv failed");
			kill(getpid(), SIGKILL);

		default:
                	// Parent process
			while (wait(&status) != pID);
			
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
/*
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		
		
		
	} 
	return -1;
}
*/
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
/*
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char const* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)

		
	}
	return -1;
}
*/