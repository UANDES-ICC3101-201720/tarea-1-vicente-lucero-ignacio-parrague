#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "const.h"
#include "util.h"

// TODO: check
int serial_binsearch(int x,int v[],int n)
{
    int low,high,mid;

    low=0;
    high=n-1;

    while(low<high)
    {
        mid = ( low + high ) / 2;

        if( x < v[mid])
            high  = mid - 1;
        else if ( x > v[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

// TODO: implement
int parallel_binsearch() {
    return 0;
}

int main(int argc, char** argv) {
    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    //clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */

    int E = 0;
    int T = 0;
    int P = 0;
    int c;

    while((c = getopt(argc, argv, "E:T:P:"))!= -1){
    	switch (c)
    	{
    		case 'E':
						E = atoi(optarg);
						break;
				case 'T':
						T = atoi(optarg);
						break;
				case 'P':
						P = atoi(optarg);
    	}
    }
    if(E<1 || T<3 || 9<T || P<0 || (10^T) -1< P){
    	printf("Program terminated, value(s) out of range");
    	exit(0);
    }
    printf("T = %d \n", T);
		printf("E = %d \n", E);
		printf("P = %d \n", P);

    clock_t cbegin = clock();


  /*  int pipefd[2];
    pid_t datagen_id;
    char buf;*/ //nose si esto esté bien

    /* TODO: start datagen here as a child process. */
    pipe(pipefd);
    datagen_id = fork();

    if(datagen_id == 0)
    {
    	printf("%s%d\n","PID Fork Datagen : ", getpid());
    	char * datagen= "./datagen";
    	execlp(datagen, &datagen, NULL);

/*
      while (read(pipefd[0], &buf, 1) > 0) // read while EOF
             write(1, &buf, 1);
         write(1, "\n", 1);
         close(pipefd[0]); // close the read-end of the pipe
         exit(EXIT_SUCCESS);*/
    }
    else if(datagen_id == -1)
    {
      printf("error al crear fork \n");
    }




    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */

    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}
