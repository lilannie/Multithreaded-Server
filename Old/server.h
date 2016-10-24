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
}account_t;

typedef struct server{
	struct account **accounts;
	int numAccounts;
}server_t;

// Global Variables
server_t *server;

void init_server(int numAccounts);
int modify_account (int id, int modification);
int get_amount (int id);
void destroy_server();

#endif