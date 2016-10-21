#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ctype.h>
#include <limits.h>
#include <pthread.h>

#include "Bank.h"

typedef struct account{
	int id;
	pthread_mutex_t lock;
	int value;
}account_t;

typedef struct server{
	struct account **accounts;
	int numAccounts;
}server_t;

server_t* init_server(int numAccounts);
void modify_account (server_t *server, int id, int modification); 
int get_account_amount (server_t *server, int id);
void destroy_server(server_t *server);

#endif