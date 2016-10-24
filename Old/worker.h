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
#include "client.h"

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

typedef struct transaction {
	int accountId;
	int amount;
	bool finished;
}transaction_t;

#ifndef WORK
#define WORK 
void work(request_t *request, worker_t *worker);
#endif 

void check(request_t *request);
bool transaction_check(transaction_t *trans);
bool transaction(request_t *request);
void revert(transaction_t transactions[]);

#endif