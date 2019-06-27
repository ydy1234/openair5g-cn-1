/*
 SCTP echo client 

	Build : gcc -o sctp_client sctp_client.c

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFSIZE 32000


int debug_on = 0;

#define debug_print(...) \
            do { if (debug_on) fprintf(stdout, ##__VA_ARGS__); } while (0)

int main(int argc, char **argv) 
{
	
	int i,j, pflag, aflag, bflag, errflag=0, sockfd, port;
	int len, pid, num_conn, bufsize, rflag, retval;
	struct sockaddr_in client;
      	struct hostent *hp,*gethostbyname();
	char *remote_host;

	while ((i = getopt(argc,argv,"p:n:r:b:d:?")) != -1) 
	{
        	switch(i)
            	{
                	case 'p':
                          	pflag = 1;
                          	port = atoi(optarg);
                          	break;
		  	case 'n':
                          	aflag = 1;
                          	num_conn = atoi(optarg);
                          	break;
		  	case 'b':
                          	bflag = 1;
                          	bufsize = atoi(optarg);
                          	break;
	                case 'r':
	                        rflag = 1;
                          	remote_host = optarg;
                          	break;
		  	case 'd':
                          	debug_on = atoi(optarg);
                          	break;
                 	case '?':
                          	errflag = 1;
                          	break;

		}
	}

	debug_print(" Params : port %d num_conn %d remote_host %s errflag %d\n", port, num_conn, remote_host, errflag);

        fprintf(stdout,"Initiating %d SCTP associations to %s and sending chunks of %d byte each\n", num_conn, remote_host, bufsize);

	if (errflag || (argc == 1))
      	{
          	usage(argv[0]);
          	exit(1);
      	}


	if (bflag != 1)
		bufsize = BUFFSIZE;


      	client.sin_port = htons(port);
      	client.sin_family = AF_INET;
      	hp = gethostbyname(remote_host);
      	if (hp == 0)  {
            fprintf(stderr,"unknown host %s\n",remote_host);
            exit(1);
      	}

      	memcpy((char *)&client.sin_addr,(const char *)hp->h_addr, hp->h_length);


	for (i=0; i < num_conn; i++)
	{


		pid = fork();
                if (pid < 0)
                {
                	fprintf(stderr,"fork failed, errno = %d\n",errno);
                       	continue;
                }


                if (pid > 0)				// parent 
                {	
			usleep(10000);
                	continue;
                }					// parent	


		if (pid == 0)  			// child
		{
			retval = data_tx(&client, bufsize);
		}				// child


	}  // for loop

	fprintf(stdout, "Done\n");
	return 0;
} //main

int data_tx(struct sockaddr_in *client, int bufsize)
{
	int rbytes, wbytes, sockfd, retval, i=0, j=0;
	char *buf;

      	struct linger linger;
      	int linger_time;
	
      	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
      	if (sockfd < 0)
      	{
           	fprintf(stderr,"socket failed\n");
        	exit(1);
      	}
      	debug_print("socket created - sockfd %d\n", sockfd);

        retval = connect(sockfd,(struct sockaddr *)client,sizeof(struct sockaddr_in));
        if (retval < 0)
        {
        	fprintf(stderr,"connect failed errno %d\n",errno);
        }


	buf = (char *) malloc(bufsize);

	for (i=j; i<(j+30000); i++)
		buf[i]=i;
	j = i;


	usleep(10000);
      	debug_print("connect retval %d\n", retval);
      	debug_print("Connected to server\n");
	for(;;)
	{

      		debug_print("writing - buf %s\n\n", buf);
		wbytes = write(sockfd, buf, BUFFSIZE);
		debug_print("PID %d  wrote %d bytes\n", getpid(), wbytes);			

		usleep(10000);

      		debug_print("reading - buffer\n");
		rbytes = read(sockfd, buf, BUFFSIZE);
		debug_print("PID %d read %d bytes\n", getpid(),rbytes);			
	
		sleep(2000);
	}	

	free(buf);
	return 0;
}
	

usage(char *c)
{

    fprintf(stderr,"usage: %s -p port -n num_conn -r remote_host \n",c);

    fprintf(stderr,"      -p                  : portnumber\n");
    fprintf(stderr,"      -n                  : num_conn\n");
    fprintf(stderr,"      -b                  : bufsize in bytes\n");
    fprintf(stderr,"      -r                  : remote_host\n");
    fprintf(stderr,"      -d                  : debug 1 or nodebug 0\n");

}
	
