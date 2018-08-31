#define _POSIX_C_SOURCE 2
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "types.h"
#include "const.h"
#include "util.h"


// TODO: check
<<<<<<< HEAD
int serial_binsearch(int x,int v[],int n){

=======
int serial_binsearch(int x,int v[],int n)
{
>>>>>>> 5376f934c2ccb916c08b9d4f9517a1ece1b924f1
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

    /* TODO: parse arguments with getopt  */ /*CHECK*/

    int E = 0;
    int T = 0;
    int P = 0;
    int size = 1;
    int c;

    while((c = getopt (argc, argv, "E:T:P:")) != -1)
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
<<<<<<< HEAD
    }
    for(int i=1;i<=T;i++){
    	size = size*10;
    }
    size = size-1;
    if(E<1 || T<3 || 9<T || P<0 || size< P){
=======


    if(E<1 || T<3 || 9<T || P<0 || (10^T) -1< P){
>>>>>>> 5376f934c2ccb916c08b9d4f9517a1ece1b924f1
    	printf("Program terminated, value(s) out of range");
    	exit(0);
    }
    printf("T = %d \n", T);
		printf("E = %d \n", E);
		printf("P = %d \n", P);
		printf("Size = %d\n", size);
		
		char *name;

    clock_t cbegin = clock();


  /*  int pipefd[2];
    pid_t datagen_id;
    char buf;*/ //nose si esto esté bien



    /* TODO: start datagen here as a child process. */
<<<<<<< HEAD
    int dtgnid = fork();
    
    if(dtgnid == 0){
    	name = "./datagen";
    	
    }
    else if (dtgnid < 0){
    	perror("Error, process to create Datagen as child process failured");
    	exit(-1);
    }
    
		
=======
  //  pipe(pipefd);
    pid_t datagen_id = fork();
    char *datagen_file[]={"./datagen",NULL};


    if(datagen_id == 0)
    {
    	printf("%s%d\n","PID Fork Datagen : ", getpid());
    	execvp(datagen_file[0],datagen_file);
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




>>>>>>> 5376f934c2ccb916c08b9d4f9517a1ece1b924f1
    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */
     
     struct sockaddr_un addr; 
    int fd, rc; 
    char buf[size];
    
    if( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
    	perror("an error ocurred with the socket");
    	exit(-1);
    }
    
    else if(connect(fd,(struct sockaddr*)&addr, sizeof(addr))==-1){
    	perror("connect intent failed");
    	exit(-1);
    }
    
    while( (rc=read(STDIN_FILENO, buf, sizeof(buf))) >0){
    	if(write(fd,buf,rc) != rc) {
    		if(rc > 0) fprintf(stderr, "partial write");
    		else{
    			perror("write error");
    			exit(-1);
    		}
    	}
    }
    //close

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
