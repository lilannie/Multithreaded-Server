all: appserver.c bank.c server.c client.c linked_list.c
		gcc appserver.c bank.c server.c client.c linked_list.c -o appserver

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
	rm bank 	
	rm server
	rm client
	rm linked_list