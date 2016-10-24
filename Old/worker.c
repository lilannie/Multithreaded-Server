#include "worker.h"

void work(request_t *request, worker_t *worker) {
	// printf("Working: %d\n", request->id);
	// printf("Type: %d\n", request->type);
	// printf("Size: %d\n", request->size);
	// printf("Size Args of: %lu\n", sizeof(request->args));
	// printf("Arg: %s \n", (request->args));
	// printf("%s\n", request->args[0]);
	// printf("%s\n", request->args);

	if (request->type == 0) {
		pthread_create(&worker->thread, NULL, (void*)&transaction, request) ;
	} else if (request->type == 1) {
		pthread_create(&worker->thread, NULL, (void*)&check, request) ;
	} else {
		printf("Request type not recognized. Try again.\n");
	}
}

void check(request_t *request) {
	printf("Client Check\n");
	printf("Check Args[0]: %s\n", request->args);


	// printf("Request Id: %d\n", request->id);
	printf("Size of Args: %lu\n", sizeof(request->args));
	printf("Size of Request: %d\n", request->size); 

	int offset = 0;
	char current = *request->args;
	printf("Current %c\n", current);
	while (current != '\0') {
		offset++;
		current = *(request->args + offset);
	}
	offset+=2;

	printf("Offset: %d\n", offset);
	printf("Args[1]: %c\n", *(request->args + sizeof(request->args)));

	printf("Size: %d\n", request->size);
	if (request->size == 2) {
		printf("Request string: %s\n", request->args + 5);
		int id = atoi(request->args + (sizeof(request->args)/2));
		int amount = get_amount(id);
		if (amount >= 0) {
			printf("%d BAL %d\n", request->id, amount);
		}
	}
	else {
		printf("Incorrect arguments: CHECK <account_id>\n");
	}

}

bool transaction_check(transaction_t *trans) {
	// printf("Transaction Check\n");
	// printf("Account Id: %d\n", trans->accountId);
	// printf("Amount: %d\n", trans->amount);

	// What if you can't access the amount
	int amount = get_amount(trans->accountId);
	if (trans->amount + amount < 0) {
		return false;
	} else {
		// Wait until it can modify
		modify_account(trans->accountId, trans->amount);
	}
	return true;
}

bool transaction(request_t *request) {
	char *args = request->args;
	int size = request->size; 
	int requestId = request->id;
	// Counters 
	int i = 1;

	// Keep track of transactions in case we need to revert
	int maxTrans = 10;
	transaction_t transactions[maxTrans];

	for (i = 1; i < size; i += 2) {
		transaction_t trans = (transaction_t) {atoi(&args[i]), atoi(&args[i+1]), false};
		
		if (!transaction_check(&trans)){
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

void revert(transaction_t transactions[]) {

}