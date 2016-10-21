#include "client.h"

void revert(transaction_t transactions[]) {

}

client_t* init_client(int numWorkers){
	// Counters 
	int i = 0;

	// Create array of workers
	worker_t **workers = malloc(numWorkers * sizeof(worker_t));	
	for (i = 0; i < numWorkers; i++) {
		// Create a worker with empty thread 
		pthread_t thread;
		worker_t worker = (worker_t) {thread};

		// Add worker to array of workers
		workers[i] = malloc(sizeof(worker_t));
		workers[i] = &worker;
	} 

	client_t *client = malloc(sizeof(client_t));
	*client = (client_t) {workers, numWorkers, init_list(), 0};
	return client;
}

// Type = Request Type { 0: transaction, 1: check }
void request(client_t *client, char *args[], int size, int requestId, int type) {
	request_t request = (request_t) {requestId, type, args, size};
	insert_node(client->list, &request);
}

void dispatch_worker(){

}

void run_client(client_t *client) {
	while (1) {
		request_t request;
		if (client->list->size > 0) {
			request = remove_node(client->list); 
		}
	}
}

void client_check(client_t *client, server_t *server, int accountId, int requestId) {
	// printf("Client Check\n");
	// printf("Account Id: %d\n", accountId);
	int amount = get_account_amount(server, accountId);
	if (amount >= 0) {
		printf("%d BAL %d\n", requestId, amount);
	}
}

bool client_transaction_check(client_t *client, server_t *server, transaction_t *trans) {
	// printf("Transaction Check\n");
	// printf("Account Id: %d\n", trans->accountId);
	// printf("Amount: %d\n", trans->amount);

	int amount = get_account_amount(server, trans->accountId);
	if (trans->amount + amount < 0) {
		return false;
	} else {
		modify_account(server, trans->accountId, trans->amount);
	}
	return true;
}

bool client_transaction(client_t *client, server_t *server, char *args[], int size, int requestId) {
	// Counters 
	int i = 1;

	// Keep track of transactions in case we need to revert
	int maxTrans = 10;
	transaction_t transactions[maxTrans];

	for (i = 1; i < size; i += 2) {
		transaction_t trans = (transaction_t) {atoi(args[i]), atoi(args[i+1]), false};
		
		if (!client_transaction_check(client, server, &trans)){
			printf("%d ISF\n", requestId);
			revert(transactions);
			return false;
		}
		else  {
			printf("%d OK\n", requestId);
			transactions[i] = trans;
		}
	}

	return true;
}

void destroy_client(client_t *client){
	free(client->workers);
	free(client);
}