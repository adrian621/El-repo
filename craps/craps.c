/**
 * Game of luck: Implementation of the Gamemaster
 *
 * Course: Operating Systems and Multicore Programming - OSM lab
 * assignment 1: game of luck.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

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
	/*
	char arg0[] = "./shooter";
	char arg1[10];
	char *args[] = {arg0, arg1, NULL};
	*/
	/* TODO: initialize the communication with the players */
	int fd[2];
	int *seedPipes[NUM_PLAYERS];
	int *scorePipes[NUM_PLAYERS];
	for (i = 0; i < NUM_PLAYERS; i++) {
	  seedPipes[i] = fd;
	  if(pipe(seedPipes[i]) == -1){
	    perror("perror");
	    exit(-1);
	  }
	  scorePipes[i] = fd;
	  if(pipe(scorePipes[i]) == -1){
	    perror("perror");
	    exit(-1);
	  }
	}
	int status;
	for (i = 0; i < NUM_PLAYERS; i++) {
		/* TODO: spawsn the processes that simulate the players */
	  pid_t pid = fork();
	  //fprintf("%s", "fork created");
	  //printf("\nfork created\n");
	  if(pid == 0){
	    //printf("in fork child\n");
	    shooter(i, seedPipes[i][0], scorePipes[i][1]);	    
	    //int status = system(*args);
	    //exit(0);	    	    
	  }
	  else if(pid > 0){
	    
	    // printf("\nIn fork parent\n");
	  }
	  else {
	    perror("perror");
	    exit(-1);
	  }
	  
	}
	
	seed = time(NULL);
	//close(seedPipes[0][1]);
	for (i = 1; i < NUM_PLAYERS; i++) {
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
	printf("master: player %d WINS\n", winner);

	/* TODO: signal the winner */

	/* TODO: signal all players the end of game */
	for (i = 0; i < NUM_PLAYERS; i++) {

	}

	printf("master: the game ends\n");
	
	/* TODO: cleanup resources and exit with success */
	for (i = 0; i < NUM_PLAYERS; i++) {
	  
	  wait(&status); 
	}
	return 0;
}
