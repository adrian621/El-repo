/**
 * Game of luck: Implementation of the Gamemaster
 *
 * Course: Operating Systems and Multicore Programming - OSM lab
 * assignment 1: game of luck.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */
#include <string.h>
#include <stdio.h> /* I/O functions: printf() ... */
#include <stdlib.h> /* rand(), srand() */
#include <unistd.h> /* read(), write() calls */
#include <assert.h> /* assert() */
#include <time.h>   /* time() */
#include <signal.h> /* kill(), raise() and SIG???? */

#include <sys/types.h> /* pid */
#include <sys/wait.h> /* waitpid() */

#include "common.h"

int main(int argc, char *argv[])
{
	int i, seed;

        /* TODO: Use the following variables in the exec system call. Using the
	 * function sprintf and the arg1 variable you can pass the id parameter
	 * to the children
	 */
	
	char arg0[] = "./shooter";
	char arg1[10];
	char *args[] = {arg0, arg1, NULL};
	
	/* TODO: initialize the communication with the players */
	int seedPipes[NUM_PLAYERS][2];
	int scorePipes[NUM_PLAYERS][2];
	for (i = 0; i < NUM_PLAYERS; i++) {
	  if(pipe(seedPipes[i]) == -1){
	    perror("perror");
	    exit(-1);
	  }
	  if(pipe(scorePipes[i]) == -1){
	    perror("perror");
	    exit(-1);
	  }
	}
	int status;
	pid_t pids[NUM_PLAYERS];
	for (i = 0; i < NUM_PLAYERS; i++) {
		/* TODO: spawsn the processes that simulate the players */	  
	  pid_t pid = fork();
	  //fprintf("%s", "fork created");
	  //printf("\nfork created\n");
	  if(pid == 0){
	    //printf("in fork child\n");
	    close(seedPipes[i][1]);
	    close(scorePipes[i][0]);
	    // char sprintf_buffer[10];
	    //sprintf(arg1, "%d", i);
	    dup2(seedPipes[i][0], STDIN_FILENO);
	    dup2(scorePipes[i][1], STDOUT_FILENO);
	    sprintf(arg1, "%d", i);
	    //arg1 = strdup(sprintf_buffer);
	    if(execv(args[0],args) == -1){
	    perror("execv");
	    }
	  //shooter(i, seedPipes[i][0], scorePipes[i][1]);	      	    	   
	  }
	  else if(pid > 0){
	    pids[i] = pid;
	    // printf("\nIn fork parent\n");
	  }
	  else {
	    perror("perror");
	    exit(-1);
	  }
	  
	}
	
	seed = time(NULL);
	for (i = 0; i < NUM_PLAYERS; i++) {
	  seed++;	    
	    close (seedPipes[i][0]);	    
	      write((seedPipes[i][1]), &seed, sizeof(seed));
		/* TODO: send the seed to the players */
	  
	}

	/* TODO: get the dice results from the players, find the winner */
	int win;
	int tempscore = 0;
	int score = 0;
	for (i = 0; i < NUM_PLAYERS; i++) {
	  close(scorePipes[i][1]);
	  read(scorePipes[i][0], &tempscore, sizeof(tempscore));
	  if(score < tempscore){
	     score = tempscore;
	     win = i;
	  }
	}
	
	printf("master: player %d WINS\n", win);

	/* TODO: signal the winner */
	//kill(pids[winner] , SIGUSR1);
	kill(pids[win], SIGUSR1);
	/* TODO: signal all players the end of game */
	for (i = 0; i < NUM_PLAYERS; i++) {
	  kill(pids[i], SIGUSR2);  
	}

	printf("master: the game ends\n");
	
	/* TODO: cleanup resources and exit with success */
	for (i = 0; i < NUM_PLAYERS; i++) {
	  pid_t child_pid = wait(&status);
	  //waitstat(pids[i], child_pid);

	  //waitpid(-1, pids[i], 0);
	  //	  wait(pids[i], 0);
	}
	return 0;
}
