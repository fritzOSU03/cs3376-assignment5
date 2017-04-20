// File: TwoPipesTwoChildren.cpp
// Author: James Fritz
// Date: 04/05/2017
// Purpose: CS3376
// Description:
// 	This program executes "ls -ltr | grep 3376 | wc -l" using two pipes and two child processes.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv){
	int status;
	int childpid1;
	int childpid2;
	
	char *ls_args[] = {"ls", "-ltr", NULL};
	char *grep_args[] = {"grep", "3376", NULL};
	char *wc_args[] = {"wc", "-l", NULL};
	
	//Create one pipe to send the output of "ls" process to "grep" process.
	int pipe1[2];
	pipe(pipe1);
	
	//Create another pipe to send the output of "grep" process to "wc" process.
	int pipe2[2];
	pipe(pipe2);
	
	//Fork to create first child (to execute ls).
	if((childpid1 = fork()) == -1) {
		perror("Error creating a child process.");
		exit(1);
	}
	
	if(childpid1 == 0) {	//First child process.
		//Replace ls' stdout with write part of 1st pipe.
		dup2(pipe1[1], 1);
		
		//Close all pipes.
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		
		//Execute the ls command.
		execvp(*ls_args, ls_args);
		
		exit(0);
	}
	else {
		//Fork to create second child (to execute grep).
		if((childpid2 = fork()) == -1) {
			perror("Error creating a child process.");
			exit(1);
		}
		
		if(childpid2 == 0) {	//Second child process.
			//Replace grep's stdin with read part of 1st pipe.
			dup2(pipe1[0], 0);
			
			//Replace grep's stdout with write part of 2nd pipe.
			dup2(pipe2[1], 1);
			
			//Close all pipes.
			close(pipe1[0]);
			close(pipe1[1]);
			close(pipe2[0]);
			close(pipe2[1]);
			
			//Execute the grep command.
			execvp(*grep_args, grep_args);
			
			exit(0);
		}
	}
	
	//Parent process.
	//Rreplace wc's stdin with read part of 2nd pipe.
	dup2(pipe2[0], 0);
	
	//Close all pipes.
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);
	
	//Execute the wc command.
	execvp(*wc_args, wc_args);
	
	//Wait for all child processes to complete.
	for(int i = 0; i < 2; i++) wait(&status);
	
	return(0);
}
