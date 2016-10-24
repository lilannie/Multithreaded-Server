#include "client.h"

void init_client(int numWorkers){
	// Counters 
	int i = 0;

	// List of Idle Workers
	linked_list_t *workers = init_list();
	for (i = 0; i < numWorkers; i++) {
		// Create a worker with empty thread 
		pthread_t *thread = malloc(sizeof(pthread_t));
		worker_t worker = (worker_t) {*thread};

		// Add worker to list of workers
		insert_node(workers, &worker);
	} 

	client = malloc(sizeof(client_t));
	*client = (client_t) {workers, numWorkers, init_list(), 0};
}

void run_client() {
	pthread_mutex_init(&mut, NULL);
  	pthread_cond_init(&client_cv, NULL);
  	pthread_cond_init(&worker_cv, NULL);
	pthread_create(&client_thread,	NULL, (void*)&listening, NULL);	
}

void listening() {
	// printf("Client Listening\n");
	// printf("Client numWorkers: %d\n", client->numWorkers);

	while (1) { 
		if (client->workers->size > 0) {
			if (client->requests->size > 0) {
				node_t *node = remove_node(client->requests);
				request_t *request = (request_t *)node->data; 

				// printf("Request id: %d\n", request->id);
				// printf("Request Args Size: %lu\n", sizeof(request->args));
				// printf("Args[0]: %s\n", request->args);

				dispatch_worker(request); 
			}
		}
	}

}

// Type = Request Type { 0: transaction, 1: check }
void request(char *args, int size, int requestId, int type) {
	// printf("Making Request\n");
	request_t *request = malloc(sizeof(request_t));

	// printf("Request id: %d\n", requestId);
	// printf("Args length: %lu\n", sizeof(args[0]));
	// printf("Request Args[0]: %s\n", args);

	*request = (request_t) {requestId, type, args, size};
	insert_node(client->requests, request);
	// node_t *node = insert_node(client->requests, request);
	// printf("Node data: %s\n", *((request_t *)(node->data))->args);
}

void dispatch_worker(request_t *request){
	printf("Dispatching worker\n");
	// printf("Request Args Size: %lu\n", sizeof(*request->args));
	// printf("Args[0]: %s\n", request->args);
	
	node_t *node = remove_node(client->workers);
	worker_t *worker = (worker_t *) node->data;
	work(request, worker);
}

void destroy_client(client_t *client){
	free(client->workers);
	free(client);
}