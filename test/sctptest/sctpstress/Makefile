all : sctp_server

sctp_server : sctp_server.o
	gcc -o sctp_server sctp_server.o

sctp_server.o : sctp_server.c
	gcc -c sctp_server.c

clean :
	rm sctp_server *.o

test :
	echo "Running tests"	
