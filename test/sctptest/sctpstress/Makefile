all : sctp_server sctp_client

sctp_server : sctp_server.o
	gcc -o sctp_server sctp_server.o

sctp_server.o : sctp_server.c
	gcc -c sctp_server.c

sctp_client : sctp_client.o
	gcc -o sctp_client sctp_client.o

sctp_client.o : sctp_client.c
	gcc -c sctp_client.c

.PHONY:clean

clean :
	rm -f *.o sctp_server sctp_client 
test :
	echo "Running tests"	
