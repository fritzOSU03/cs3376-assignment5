// File: DynPipe.cpp
// Author: James Fritz
// Date: 04/07/2017
// Purpose: CS3376
// Description:
// 	This program executes a dynamic number instructions using multiple processes and pipes.

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv) {
	//Return an error if less than two arguments (optional).
	if(argc < 3) {
		errno = EINVAL;
		perror("\033[1;31mERROR:\033[0m At least two arguments are required ");
		exit(1);
	}
	//Return an error if more than five arguments (optional).
	else if(argc > 6) {
		errno = E2BIG;
		perror("\033[1;31mERROR:\033[0m No more than five arguments are allowed ");
		exit(1);
	}
	
	
	int status;
	int numArgs = argc - 1;
	int childpids[numArgs];
	int numPipes = numArgs - 1;
	std::string arg;
	
	//Create the required number of pipes.
	int pipes[numPipes][2];
	for(int i = 0; i < numPipes; i++) pipe(&pipes[i][0]);
	
	for(int i = 0; i < numArgs; i++) {
		//Fork to create a child.
		if((childpids[i] = fork()) == -1) {
			errno = ECHILD;
			perror("\033[1;31mERROR:\033[0m Could not create child process ");
			exit(1);
		}
		
		if(childpids[i] == 0) {
			//Store the instruction to a string (req. for execlp).
			arg = argv[i+1];
			
			//Replace this stdout with write end of previous pipe (omit final).
			if(i < numArgs - 1) dup2(pipes[i][1], 1);
			
			//Replace this stdin with read end of next pipe (omit initial).
			if(i > 0) dup2(pipes[i - 1][0], 0);
			
			//Close all pipes.
			for(int j = 0; j < numPipes; j++) {close(pipes[j][0]); close(pipes[j][1]);}
			
			//Execute the command.
			execlp("/bin/sh", "/bin/sh", "-c", arg.c_str(), NULL);
			
			//Exit if not final command.
			if(i < numArgs) exit(0);
		}
	}
	
	//Close all pipes.
	for(int j = 0; j < numPipes; j++) {close(pipes[j][0]); close(pipes[j][1]);}
	
	//Wait for all child processes to complete.
	for(int i = 0; i < numArgs; i++) wait(&status);
	
	return(0);
}
