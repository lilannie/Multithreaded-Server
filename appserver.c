#include "appserver.h"

/********************
***	Annie Steenson	*
***	CPRE 308		*
***	Project 2		*
*************************
***	Run in Terminal:	*
***		make all		*
***	./appserver         *
*************************/

// Global Variables
server_t *server;
client_t *client;
int requestNum = 1;

// Queue a Transaction
void transaction(char *args[], int size) {
	if ((size-1) % 2 == 0) {
		request(client, args[], size, requestId, 0);
	} else {
		printf("Missing arguments: TRANS <account_id> <amount>\n");
	}
}

// Queue a Balance Check
void check(char *args[], int size) {
	if (size == 2) {
		// int accountId = atoi(args[1]);
		// client_check(client, server, accountId, requestNum);
		request(client, args[], size, requestId, 0);
	} else {
		printf("Missing arguments: CHECK <account_id>\n");
	}
}

// End application
void end() {
	exit(0);
}

// Determine action based on Command from Input
void readCommand(char *args[], int size){
	char *command = args[0];

	/************************
	*** Built in Commands ***
	*************************/

	if (strcmp(command, "END") == 0) 
		end();
	else if (strcmp(command, "CHECK") == 0) {
		check(args, size);
	}
	else if (strcmp(command, "TRANS") == 0) {
		transaction(args, size);
	}
	// Cannot read command
	else printf("Do not recognize command.\n");

} 

// Parse through arguments
void parseArguments(char input[]) {
	// Counters
	int i = 0; 

	// Contants 
	const char space[2] = " ";

	// Variables
	char *args[21];
	args[0] = "";

	// Parse through all the arguments 
	char *arg = strtok(input, space);
	while(arg != NULL && arg[0] != '\n') {
		int length = strlen(arg);
		// Replace \n with 0 at end of each argument
		if (length > 0)
			*(arg + length) = 0;
		
		args[i] = arg;
		arg = strtok(NULL, space);
		i++;
	}

	// Arguments are NOT empty 
	if (i > 0) {
		//Remove last \n 
		if (strlen(args[0]) > 0)
			*(args[i - 1] + strlen(args[i - 1]) - 1) = 0;
		readCommand(args, i);
	}

}


int main(int argc, char **argv) {
	// Input Variables
	int numWorkers = 0;
	int numAccounts = 0;
	char input[150];
	char output[50];

	// Setup Application
	if (argc == 4) {
		numWorkers = atoi(argv[1]);
		numAccounts = atoi(argv[2]);
		strcpy(output, argv[3]);

		// printf("Num workers: %d\n", numWorkers);
		// printf("Num accounts: %d\n", numAccounts);
		// printf("Output file: %s\n", output);
	}
	else {
		printf("Wrong arguments passed into the program.\n");
		exit(255);
	}

	// Initialize server and client
	server = init_server(numAccounts);
	client = init_client(numWorkers);

	// Take requests
	printf("Welcome to AppServer.\n");
	while(1) {
		fgets(input, 150, stdin);
		parseArguments(input);
		
		printf("ID %d\n", requestNum);
		requestNum++;
	}
	return 0;
}



