#ifndef CLIENT_HEADER
#define CLIENT_HEADER

typedef int bool;
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ctype.h>
#include <limits.h>
#include <pthread.h>

#include "server.h"
#include "worker.h"
#include "linked_list.h"


#ifndef WORKER
#define WORKER
typedef struct worker {
	pthread_t thread;
}worker_t;
#endif

#ifndef REQUEST
#define REQUEST
typedef struct request {
	int id;
	int type;
	char *args;
	int size;
}request_t;
#endif

#ifndef WORK
#define WORK 
void work (request_t *request, worker_t *worker);
#endif 

typedef struct client {
	linked_list_t*workers;
	int numWorkers;
	linked_list_t *requests;
	int workersBusy;
}client_t;

// Global Variables
client_t *client;
pthread_t client_thread;
pthread_mutex_t  mut;
pthread_cond_t 	 client_cv; 	
pthread_cond_t	 worker_cv;


void init_client(int numWorkers);
void run_client();
void listening();
void dispatch_worker(request_t *request);
void request(char *args, int size, int requestId, int type);

#endif