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
#include "linked_list.h"

typedef struct worker {
	pthread_t thread;
}worker_t;

typedef struct client {
	struct worker **workers;
	int numWorkers;
	linked_list_t *list;
}client_t;

typedef struct request {
	int id;
	int type;
	char **args;
	int size;
}request_t;

typedef struct transaction {
	int accountId;
	int amount;
	bool finished;
}transaction_t;

client_t* init_client(int numWorkers);

void request(client_t *client, char *args[], int size, int requestId, int type);
void client_check(client_t *client, server_t *server, int accountId, int requestId);
bool client_transaction_check(client_t *client, server_t *server, transaction_t *trans);
bool client_transaction(client_t *client, server_t *server, char *args[], int size, int requestId);
void dispatch_worker();

#endif