#include "server.h"

server_t* init_server(int numAccounts){
	// Counters 
	int i = 0;

	// Initialize Bank Accounts
	initialize_accounts(numAccounts);

	// Initialize array of accounts 
	account_t **accounts = malloc(numAccounts * sizeof(account_t));
	for (i = 0; i < numAccounts; i++) {
		pthread_mutex_t lock;
		pthread_mutex_init(&lock, NULL);

		account_t account = (account_t) {i+1, lock, 0};
		accounts[i] = (account_t *) malloc(sizeof(account_t));
		accounts[i] = &account;
	}

	server_t *server = malloc(sizeof(server_t));
	*server = (server_t ) {accounts, numAccounts};
	return server;
}

void modify_account (server_t *server, int id, int modification) {

}

int get_account_amount (server_t *server, int id) {
	if (id > 0 & id <= server->numAccounts) {
		return read_account(id);
	} else {
		printf("Invalid Account Id: %d", id+1);
		return -1;
	}
}

void destroy_server(server_t *server){
	free(server->accounts);
	free(server);
}