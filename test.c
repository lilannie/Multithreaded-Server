#include "appserver.h"

#define numArgs 4
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
int requestNum = 1;
FILE * file;

// Queue a Transaction
void queue_transaction(char *args[], int size) {
	
	if ((size-1) % 2 == 0) {
		request(args, size, requestNum, 0);
	} else {
		printf("Missing arguments: TRANS <account_id> <amount>\n");
	}
}

// Queue a Balance Check
void queue_check(char *args[], int size) {
	// printf("%s\n", args[1]);
	if (size == 2) {
		request(args, size, requestNum, 1);
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
		queue_check(args, size);
	}
	else if (strcmp(command, "TRANS") == 0) {
		queue_transaction(args, size);
	}
	// Cannot read command
	else printf("Do not recognize command.\n");
} 

// Parse through arguments
void parseArguments(char input[]) {
	// printf("%s\n", input);
	// Counters
	int i = 0; 

	// Contants 
	const char space[2] = " ";

	// Variable
	char *args[21];

	// Parse through all the arguments 
	char *arg = strtok(input, space);
	while(arg != NULL && arg[0] != '\n') {
		
		int length = strlen(arg);

		// printf("Before Length: %d \n", length);
		// printf("Before String: %s \n", arg);

		// Replace \n with 0 at end of each argument
		if (length > 0)
			*(arg + length) = 0;

		// printf("After Length: %d \n", length);
		// printf("After String: %s \n", arg);

		// printf("ARG: %s\n", arg);
		args[i] = malloc(sizeof(arg));
		strcpy(args[i], arg);
		// printf("ARGS[i]: %s\n", args[i]); 

		arg = strtok(NULL, space);
		i++;
	}

	readCommand(args, i);	
}

/********************
 ** Main Function ***
 ********************
 ** argv[1] = number of workers
 ** argv[2] = number of accounts
 ** argv[3] = output file
 */
int main(int argc, char **argv) {
	// Input Variables
	int numWorkers = 0;
	int numAccounts = 0;
	char input[150];

	// Setup Application
	if (argc == numArgs) {
		numWorkers = atoi(argv[1]);
		numAccounts = atoi(argv[2]);
		file = fopen(argv[3], "w");

		// printf("Num workers: %d\n", numWorkers);
		// printf("Num accounts: %d\n", numAccounts);
		// printf("Output file: %s\n", output);
	}
	else {
		printf("Wrong arguments passed into the program.\n");
		exit(255);
	}

	// Initialize server and client
	init_server(numAccounts);
	init_client(numWorkers);
	run_client();

	// Take requests
	printf("Welcome to AppServer.\n");
	while(1) {
		fgets(input, 150, stdin);
		parseArguments(trimwhitespace(input));
		printf("ID %d\n", requestNum);
		requestNum++;
	}
	return 0;
}



