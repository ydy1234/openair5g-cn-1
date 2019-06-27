/*
 SCTP echo server 

	Build : gcc -o sctp_server sctp_server.c

 Copyright (C) 2014 Red Hat Inc.
 Author Vivek Dasgupta <vivek.dasgupta@gmail.com> | <vdasgupt@redhat.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFSIZE 32000

int debug_on = 0;

#define debug_print(...) \
            do { if (debug_on) fprintf(stdout, ##__VA_ARGS__); } while (0)

int data_tx(int newsockfd, int bufsize, char *buf);
int usage(char *c);
int main(int argc, char **argv) 
{
	
	int i,j, pflag, aflag, bflag, cflag, nflag, errflag=0, sockfd, newsockfd, port;
	int len, retval, pid, num_conn, bufsize, num_pkts, status;
	struct sockaddr_in server;
	char *buf;


	while ((i = getopt(argc,argv,"b:p:n:d:?")) != -1) 
	{
        	switch(i)
            	{
                	case 'p':
                          	pflag = 1;
                          	port = atoi(optarg);
                          	break;
		  	case 'b':
                          	bflag = 1;
                          	bufsize = atoi(optarg);
                          	break;
		  	case 'n':
                          	nflag = 1;
                          	num_pkts = atoi(optarg);
                          	break;
		  	case 'c':
                          	cflag = 1;
                          	num_conn = atoi(optarg);
                          	break;
		  	case 'd':
                          	debug_on = atoi(optarg);
                          	break;
                        case '?':
                                errflag = 1;
                                break;

		}
	}

        debug_print(" Params : port %d errflag %d\n", port, errflag);


        if (errflag || (argc == 1))
        {
                usage(argv[0]);
                exit(1);
        }

	if (bflag != 1)
		bufsize = BUFFSIZE;

      	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
      	if (sockfd < 0)
      	{
           	fprintf(stderr,"socket failed\n");
        	exit(1);
      	}
	
   	debug_print("socket created - sockfd %d\n", sockfd);

	i = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));

	server.sin_family = AF_INET;
      	server.sin_addr.s_addr = INADDR_ANY;
      	server.sin_port = htons(port);
      	len = sizeof server;


	if (bind(sockfd,(struct sockaddr *) &server,len) < 0)  {
        	fprintf(stderr,"proc %d: bind failed port %d errno %d\n",getpid(),port,errno);
		exit(1);
        }
   	debug_print("bound to port %d\n", port);

	if (listen(sockfd,200) < 0)
	{
                fprintf(stderr,"proc %d: listen failed port %d errno %d\n",getpid(),port,errno);
                exit(1);
        }
   	debug_print("listening at port %d\n", port);
        fprintf(stdout,"SCTP server listening at port %d handling %d chunks per connection each %d bytes...\n", port, num_pkts, bufsize);

	for (;;)
	{

   		debug_print("waiting at accept \n");
		newsockfd = accept(sockfd,(struct sockaddr *) 0,(int *) 0);
          	if (newsockfd < 0)
          	{
                 	fprintf(stderr,"accept failed errno %d\n",errno);
			exit(1);
          	}

   		debug_print("accepting a connection - forking a child for newsockfd %d\n", newsockfd);

		pid = fork();
                if (pid < 0)
                {
                	fprintf(stderr,"fork failed, errno = %d\n",errno);
                       	close(newsockfd);
                       	continue;
                }


                if (pid > 0)				// parent 
                {
			waitpid(-1, &status, WNOHANG);
   			debug_print("parent - continue listening at port %d\n", port);
                	continue;
                }					// parent	


		if (pid == 0)  			// child
		{
			buf = (char *) malloc(bufsize);
   			debug_print("child to handle newsockfd = %d\n", newsockfd);
			for(i=0; i<num_pkts; i++) 
			{
				retval = data_tx(newsockfd, bufsize, buf);
			}
			free(buf);
			exit(1);
		}				// child


	}  // for 1


} // main

int data_tx(int newsockfd, int bufsize, char *buf)
{
	int rbytes, wbytes;


        usleep(10000);
	
	rbytes = read(newsockfd, buf, bufsize);
	debug_print(" read %d bytes\n", rbytes);			

	wbytes = write(newsockfd, buf, rbytes);
	debug_print("  wrote %d bytes\n", wbytes);			

	
	return 0;
}
	

int usage(char *c)
{

    fprintf(stderr,"usage: %s -p port \n",c);

    fprintf(stderr,"      -p                  : portnumber\n");
    fprintf(stderr,"      -b                  : bufsize in bytes\n");
    fprintf(stderr,"      -n                  : number of packets\n");
    fprintf(stderr,"      -d                  : debug 1 or nodebug 0\n");
    return 0;
}
	
