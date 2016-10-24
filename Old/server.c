#include "server.h"

void init_server(int numAccounts){
	// Counters 
	int i = 0;

	// Initialize Bank Accounts
	initialize_accounts(numAccounts);

	// Initialize array of accounts 
	account_t **accounts = malloc(numAccounts * sizeof(account_t));
	for (i = 0; i < numAccounts; i++) {
		pthread_mutex_t lock;
		pthread_mutex_init(&lock, NULL);

		account_t account = (account_t) {i+1, lock};
		accounts[i] = (account_t *) malloc(sizeof(account_t));
		accounts[i] = &account;
	}

	server = malloc(sizeof(server_t));
	*server = (server_t ) {accounts, numAccounts};
}

int modify_account (int id, int modification) {
	// Get lock on account
	if (server->accounts[id-1]->id == id) {
		if (pthread_mutex_trylock(&server->accounts[id-1]->lock) == 0) {
			int amount = modification + read_account(id);
			write_account(id, amount);
			pthread_mutex_unlock(&server->accounts[id-1]->lock);
			return 0;
		} else {
			printf("Cannot unlock %d account.\n", id);
		}
	} else {
		printf("ERROR\n");
		printf("Server->accounts[id-1]->id = %d\n", server->accounts[id-1]->id);
		printf("Account Id: %d\n", id);
	}

	return -1;
}

int get_amount (int id) {
	if (id > 0 & id <= server->numAccounts) {
		// Get lock on account
		if (server->accounts[id-1]->id == id) {
			if (pthread_mutex_trylock(&server->accounts[id-1]->lock) == 0) {
				int amount = read_account(id);
				pthread_mutex_unlock(&server->accounts[id-1]->lock);
				return amount;
			} else {
				printf("Cannot unlock %d account.\n", id);
			}
		} else {
			printf("ERROR\n");
			printf("Server->accounts[id-1]->id = %d\n", server->accounts[id-1]->id);
			printf("Account Id: %d\n", id);
		}
	}
	else {
		printf("Invalid Account Id: %d", id+1);
	}
	return -1;
}

void destroy_server(){
	free(server->accounts);
	free(server);
}