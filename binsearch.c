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
#include <getopt.h>
#include <unistd.h>
#include "types.h"
#include "const.h"
#include "util.h"
#include <pthread.h>


// TODO: check
typedef struct
{
	int *array;
	int num;
	int position;
	int size;
} data;


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

void *binsearch(void *info)
{
	data *args = info;
	int min, max, mid, position;
	position = args->position;
	min = args->num;
	max = args->size-1;
	
	char end = 'F';
	
	while(min<max && end != 'V')
	{
		mid = (min + max) / 2;
		if (position<mid)
		{
			max = mid-1;
		}
		else if (position>mid)
			min = mid+1;
			
		else
		{
			end = 'V';
			pthread_exit(0);
		}
	}
	return 0;
}
// TODO: implement

int parallel_binsearch(int arrey[], int size, int number, int P  ) {
		
		int max_parallels = sysconf(_SC_NPROCESSORS_ONLN);
		pthread_t m_tid[max_parallels];
		int parallels = size / max_parallels;
		int mult = 1;
		
		for(int i=0;i<max_parallels;i++)
		{
			data *infor = malloc(sizeof(data));
			infor->array = arrey;
			infor->num = number;
			infor->position = P;
			infor->size = (parallels * mult)-1;
			if (pthread_create(&m_tid[i], NULL, (void *)binsearch, infor))
			{
				free(infor);
			}
			number = parallels * mult;
			mult++; 
		}
		
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

    while((c = getopt (argc, argv, "E:T:P:")) != -1){
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

    for(int i=1;i<=T;i++){
    	size = size*10;
    }
    size = size-1;

    if(E<1 || T<3 || 9<T || P<0 || size< P){
    	printf("Program terminated, value(s) out of range");
    	exit(0);
    }
    printf("T = %d \n", T);
		printf("E = %d \n", E);
		printf("P = %d \n", P);
		printf("Size = %d\n", size);


    clock_t cbegin = clock();



    /* TODO: start datagen here as a child process. */

  //  pipe(pipefd);
    pid_t datagen_id = fork();
    char *datagen_file[]={"./datagen",NULL};


    if(datagen_id == 0)
    {
    	printf("%s%d\n","PID Fork Datagen : ", getpid());
    	execvp(datagen_file[0],datagen_file);

    }
    else if(datagen_id == -1)
    {
      printf("error al crear fork \n");
    }

    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */



     int delay1, delay2;

     for (delay1 = 1; delay1 <= 2000; delay1++)
       for (delay2 = 1; delay2 <= 1; delay2++)
       {}

    struct sockaddr_un addr;
    int fd, rc;
    char buf[100];
    char msg[80];
    sprintf(msg, "%d", T);



    strcpy(buf,"BEGIN S ");
    strcat(buf, msg);

    printf("cosa: %s\n",buf);

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
    	perror("an error ocurred with the socket");
    	exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, DSOCKET_PATH, sizeof(addr.sun_path));

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr))==-1){
    	perror("connect intent failed");
    	exit(-1);
    }
    if ((rc = write(fd, buf, sizeof(buf))) == -1){
		perror("write error\n");
		exit(-1);
	  }
    printf("Enviando info: %d bytes enviados.\n", rc);
   
    long unsigned int readvalues = 0;
    size_t numvalues = pow(10, T);
    size_t readbytes = 0;

    UINT *nums = malloc(sizeof(UINT) * numvalues);
    while(readvalues < numvalues) {
        readbytes = read(fd, nums + readvalues, sizeof(UINT)*size);
        readvalues += readbytes / 4;
    }

    printf("nums %i \n", *nums);
    
    

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */


    if ((rc = write(fd, "END", sizeof(buf))) == -1){
		perror("write error\n");
		exit(-1);
    }
    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}
