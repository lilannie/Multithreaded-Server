/********************
***	Annie Steenson	*
***	CPRE 308		*
***	Project 2		*
*************************
***	Run in Terminal:	*
***		make all		*
***		./appserver     *
*************************/

#include "appserver.h"

/**********************
*** Global Variables **
***********************/

/** Command Line Variables ****/

#define numArgs 4
#define maxArgs 21	
int numWorkers = 0;
int numAccounts = 0;
FILE * out;	

/** Server Variables **********/

size_t sizeInput = 150; 
int requestIdCounter = 1;
int workersRunning = 1;
List_t *queue;
Account_t *accounts;

/** Threads *******************/
pthread_t workers[numWorkers];

/** Mutexes *******************/

pthread_mutex_t bank_mutex;
pthread_mutex_t queue_mutex;
pthread_mutex_t input_mutex;

/**********************
**** List Functions ***
***********************/

void init_list() {
	pthread_mutex_init(&queue_mutex, NULL);
	queue = malloc(sizeof(List_t));
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;

}

void add_list(int id, char *input) {
	Request_t new = malloc(sizeof(Request_t));
	new->id = id;
	new->input = malloc(sizeof(char) * sizeInput * 2);
	gettimeofday(&(new->time), NULL);
	new->next = NULL;

	/** Insert in List *************/

	pthread_mutex_lock(&queue_mutex);
	if (queue->size <= 0) {
		queue->head = new;
		queue->tail = new;

		queue->size = 1;
	} else {
		queue->tail->next = new;
		queue->tail = new;
		queue->size++;
	}
	pthread_mutex_unlock(&queue_mutex);
}

Request_t pop_list() {
	Request_t popped;
	Request_t *head;

	pthread_mutex_lock(&queue_lock);

	// Return if List is empty
	if (queue->size <= 0){
		pthread_mutex_unlock(&queue_lock);
		popped = NULL;
		return popped;
	}

	// Get Popped Request
	head = queue->head;
	popped.id = head->id;
	popped.input = malloc(sizeof(char) * 2 * maxArgs);
	strcopy(ret.input, head->input);
	popped.time = head->time;
	popped.next = NULL;

	// List Logic and Free Memory
	queue->head = head->next;
	free(head->input);
	free(head);

	// List is empty now
	if (queue->head == NULL)
		queue->tail = NULL;

	queue->size--;

	pthread_mutex_unlock(&queue_lock);

	return popped;
}

/*************************
**** Account Functions ***
**************************/

void init_accounts() {
	// Counter 
	int i = 0;

	// Initialize Accounts in Bank.c
	initialize_accounts(numAccounts);

	// Initialize Accounts in Appserver.c
	accounts = malloc(sizeof(Account_t) * numAccounts);
	for (i = 0; i < numAccounts; i++) {
		pthread_mutex_init(&(accounts[i].lock), NULL);
		accounts[i].value = 0;
	}
}

/*************************
**** Worker Functions ****
**************************/
void init_workers() {
	// Counter
	int i = 0;

	for (i = 0; i < numWorkers; i++) {
		pthread_create(&workers[i], NULL, listen, NULL);
	}
}

void listen() {
	// Variables
	Request_t request;
	char **arguments = malloc(sizeof(char*) * maxArgs);
	int numArgs = 0;

	while (queue->size > 0 || workersRunning) {
		while (workersRunning && queue->size == 0){
			usleep(2);
		} 

		request = pop_list();

		// Empty List
		if (!request) 
			continue;

		parse_request_input(request.input, arguments, &numArgs);
		execute_request(&request, arguments, numArgs);

		for (i = 0; i < numArgs; i++) {
			free(arguments[i]);
		}
		free(request.input);
		numArgs = 0;
	}

	free(arguments);
}

void parse_request_input(char *input, char **arguments, int *numArgs) {
	// Variables
	char *arg;

	pthread_mutex_lock(&input_mutex);
	// Critical Region
	arg = strtok(input, " ");
	while (arg) {
		arguments[i] = malloc(sizeof(char) * maxArgs);
		strcopy(arguments[i], arg, maxArgs);
		
		arg = strtok(NULL, " ");
		numArgs++;
	}
	pthread_mutex_unlock(&input_mutex);
}

void execute_request(Request_t *request, char **arguments, int *numArgs) {
	if (num_tokens < 2) {
		printf("&d Error: Request Format\n", request->id);
	}
	else if (num_tokens == 2 && strcmp(arguments[0], "CHECK") == 0) {
		check(request, arguments);
	} 
	else if (num_tokens % 2 && strcmp(arguments[0], "TRANS") == 0) {
		transaction();
	}
}

void check(Request_t *request, char **arguments) {
	pthread_mutex_lock(&input_mutex);
	int account = atoi(arguments[1]);
	pthread_mutex_unlock(&input_mutex);

	pthread_mutex_lock(&(accounts[account-1].lock));
	int amount = read_account(account);
	pthread_mutex_unlock(&(accounts[account-1].lock));

	struct timeval timestamp;
	gettimeofday(&timestamp, NULL);

	flockfile(out);
	fprintf(out, "%d BAL %d TIME %d.%06d %d.%06d\n", 
		request->id, amount, request->time.tv_sec, request->time.tv_usec, timestamp.tv_sec, timestamp.tv_usec);
	funlockfile(out);
}

void transaction(Request_t *request, char **arguments, int num) {
	// Counter
	int i = 0;

	// Variables
	struct timeval timestamp;

	Transaction_t trans = malloc(sizeof(Transaction_t));
	for (i = 0; i < num; i++) {
		int position = i*2;

		pthread_mutex_lock(&input_mutex);
		trans.accounts[i] = atoi(args[position+1]);
		trans.amounts[i] = atoi(args[position+2]);
		pthread_mutex_unlock(&input_mutex);
	}

	// Lock All
	for(i = 0; i < num_trans; i++) {
		pthread_mutex_lock(&(accounts[trans.accounts[i]-1].lock));
	}

	// No Insufficient Funds
	if (!isf(request, &trans, num)) {
		for(i = 0; i < num; i++){
			write_account(trans.accounts[i], (trans.amounts[i] + trans.balances[i]));
		}
		
		gettimeofday(&timestamp, NULL);
		flockfile(out);
		fprintf(out, "%d OK TIME %d.%06d %d.%06d\n", 
			request->id, request->time.tv_sec, request->time.tv_usec, 
			timestamp.tv_sec, timestamp.tv_usec);
		funlockfile(out);
	}

	// Unlock All
	for(i = num - 1; i >=0; i--){
		pthread_mutex_unlock(&(accounts[trans.accounts[i]-1].lock));
	}

	free(trans);
}

int isf(Request_t *request, Transaction_t *trans, int num) {
	// Variables
	struct timeval timestamp;

	for(i = 0; i < num; i++) {
		trans.balances[i] = read_account(trans->accounts[i]);
		if (trans->accounts[i] + trans->balances[i] < 0) {
			gettimeofday(&timestamp, NULL);

			flockfile(out);
			fprintf(out, "%d ISF %d TIME %d.06%d %d.06%d\n", 
				request->id, trans->accounts[i], 
				request->time.tv_sec, request->time.tv_usec, 
				timestamp.tv_sec, timestamp.tv_usec);
			funlockfile(out);
			return 1;
		}
	}
	return 0;
}

/*************************
**** Request Functions ***
**************************/

void run() {
	// Counter
	int i;

	char *input = malloc(sizeof(char) * sizeInput * 2);

	while(1) {
		// Get Input
		int sizeInputRead = getline(&input, &sizeInput, stdin);
		input[sizeInputRead -1] = '\0';
	
		if(strcmp(input, "END") == 0) {
			break;
		}

		printf("ID %d\n", requestIdCounter);
		add_list(requestIdCounter, input);
		requestIdCounter++;
	}

	finish();
	free(input);
	cleanup();
}

void finish() {
	while(queue->size > 0) 
		usleep(2);

	for (i = 0; i < numWorkers; i++) {
		pthread_join(workers[i], NULL);
	}
}

void cleanup() {
	free(accounts);
	free(queue);
	fclose(out);
	// Free accounts from Bank.c
	free_accounts();
}

/********************
 ** Main Function ***
 ********************
 **
 ** argv[1] = number of workers
 ** argv[2] = number of accounts
 ** argv[3] = output file
 */
int main(int argc, char **argv) {
	// Input Variables
	char input[150];

	// Setup Application
	if (argc != numArgs) {
		printf("Wrong arguments passed into the program.\n");
		printf("Exiting program.\n");		
		exit(255);
	}

	numWorkers = atoi(argv[1]);
	numAccounts = atoi(argv[2]);
	out = fopen(argv[3], "w");

	if (!out) {
		printf("Error opening output file. \n");
		printf("Exiting program.\n");
		exit(255);
	}

	// printf("Num workers: %d\n", numWorkers);
	// printf("Num accounts: %d\n", numAccounts);
	// printf("Output file: %s\n", output);


	init_accounts();
	init_list();
	init_workers();

	pthread_mutex_init(&input_mutex, NULL);
	pthread_mutex_init(&bank_mutex, NULL);

	run();
	return 0;
}



