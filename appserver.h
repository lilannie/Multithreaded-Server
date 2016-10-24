#ifndef APP_HEADER
#define APP_HEADER

typedef int bool;
#define true 1
#define false 0

#ifndef STANDARD_LIBS
#define STANDARD_LIBS
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#endif

#ifndef BANK_HEADER
#define BANK_HEADER
#include "Bank.h"
#endif

#define maxTransactions 10

typedef struct Request {
	int id;
	char *input;
	struct timeval time;
	struct Request *next;
} Request_t;

typedef struct List {
	int size;
	Request *head;
	Request *tail;
} List_t;

typedef struct Account {
	pthread_mutex_t lock;
	int value;
} Account_t;

typedef struct Transaction {
	int accounts[maxTransactions];
	int amounts[maxTransactions];
	int balances[maxTransactions];
} Transaction_t;

#endif