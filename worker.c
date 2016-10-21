#include "worker.h"

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