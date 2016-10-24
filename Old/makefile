all: clean worker.c appserver.c  bank.c server.c client.c linked_list.c
		gcc worker.c appserver.c bank.c server.c client.c linked_list.c -o appserver

app: worker.c appserver.c  bank.c server.c client.c linked_list.c
		gcc worker.c appserver.c bank.c server.c client.c linked_list.c -o appserver

worker.o:
	gcc -o worker worker.c

linked_list.o: 
	gcc -o linked_list linked_list.c

client.o: 
	gcc -o client client.c

server.o: 
	gcc -c server.c

bank.o: 
	gcc -o bank bank.c

appserver.o: 
	gcc -o appserver appserver.c

clean:
	rm appserver