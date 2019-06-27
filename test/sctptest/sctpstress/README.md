sctpstress
==========

SCTP stress tools

Tool to create a large number of persistent SCTP associations

==========

Build :
        Build All: 
		make all

	Clean All:
		make clean

	gcc -o sctp_server sctp_server.c
	
	gcc -o sctp_client sctp_client.c

==========

Usage :

$sctp_server 

usage: ./sctp_server -p port 

      -p                  : portnumber
      
      -b                  : bufsize in bytes
      
      -n                  : number of packets
      
      -d                  : debug 1 or nodebug 0
      

$sctp_client 

usage: ./sctp_client -p port -n num_conn -r remote_host 

      -p                  : portnumber
      
      -n                  : num_conn
      
      -b                  : bufsize in bytes
      
      -r                  : remote_host
      
      -d                  : debug 1 or nodebug 0
      

==========

Example :

$sctp_server  -p 8000 -b 2048 -n 1000 

SCTP server listening at port 8000 handling 1000 chunks per connection each 2048 bytes...


$sctp_client -p 8000 -r 192.168.0.1 -n 1000 -b 2048 

Initiating 1000 SCTP associations to 192.168.0.1 and sending chunks of 2048 byte each

Done

==========
