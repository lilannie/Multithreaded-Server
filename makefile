all: appserver.c
		gcc appserver.c -o appserver

appserver.o: 
	gcc -o appserver appserver.c

clean:
	rm appserver