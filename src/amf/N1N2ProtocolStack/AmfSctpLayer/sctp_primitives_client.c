/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//WARNING: sctp requires libsctp-dev and -lsctp as linker option
#include <netinet/sctp.h>
#include <arpa/inet.h>

#include "assertions.h"
#include "sctp_common.h"
#include "sctp_primitives_client.h"
#include "sctp_default_values.h"
#include "log.h"

/* Send buffer to SCTP association */
int
sctp_send_msg (
  sctp_data_t * sctp_data_p,
  const uint16_t ppid,
  const sctp_stream_id_t stream,
  const uint8_t * buffer,
  const size_t length)
{
  printf("sctp_send_msg\n");
  DevAssert (buffer != NULL);
  DevAssert (sctp_data_p != NULL);
  /*
   * Send message on specified stream of the sd association
   * * * * NOTE: PPID should be defined in network order
   */
  if (sctp_sendmsg (sctp_data_p->sd, (const void *)buffer, length, NULL, 0, htonl (ppid), 0, stream, 0, 0) < 0) {
    OAILOG_ERROR (LOG_SCTP, "Sctp_sendmsg failed: %s\n", strerror (errno));
    return -1;
  }else{
    printf("sctp_sendmsg\n");
  }

  OAILOG_DEBUG (LOG_SCTP, "Successfully sent %d bytes to port %d on stream %d\n", length, sctp_data_p->remote_port, stream);
  return 0;
}

static
  int
sctp_handle_notifications (
  union sctp_notification *snp)
{
  if (SCTP_SHUTDOWN_EVENT == snp->sn_header.sn_type) {
    /*
     * Client deconnection
     */
    OAILOG_DEBUG (LOG_SCTP, "Notification received: server deconnected\n");
  } else if (SCTP_ASSOC_CHANGE == snp->sn_header.sn_type) {
    /*
     * Association has changed
     */
    OAILOG_DEBUG (LOG_SCTP, "Notification received: server association changed\n");
  } else {
    OAILOG_DEBUG (LOG_SCTP, "Notification received: %d TODO\n", snp->sn_header.sn_type);
  }

  /*
   * TODO: handle more notif here
   */
  return 0;
}

int
sctp_connect_to_remote_host (
  char *local_ip_addr[],
  int nb_local_addr,
  char *remote_ip_addr,
  const uint16_t port,
  int socket_type,
  sctp_data_t * sctp_data_p)
{
  int                                     sd = -1;
  socklen_t                               i = 0;
  struct sctp_initmsg                     init;
  struct sctp_event_subscribe             events;
  struct sockaddr                        *bindx_add_addr;

  DevAssert (sctp_data_p != NULL);
  DevAssert (remote_ip_addr != NULL);
  DevAssert (local_ip_addr != NULL);
  DevCheck ((socket_type == SOCK_STREAM), socket_type, 0, 0);
  OAILOG_DEBUG (LOG_SCTP, "Creating socket type %d\n", socket_type);
  char *local_ip = "127.0.0.1";
  /*
   * Create new socket
   */
  if ((sd = socket (AF_INET6, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
    OAILOG_ERROR (LOG_SCTP, "Socket creation failed: %s\n", strerror (errno));
    printf ("Socket creation failed: %s\n", strerror (errno));
    return -1;
  }

  /*
   * Bind to provided IP adresses
   */
  bindx_add_addr = calloc (nb_local_addr, sizeof (struct sockaddr));

  for (i = 0; i < nb_local_addr; i++) {
    if (inet_pton (AF_INET, local_ip_addr[i], &((struct sockaddr_in *)&bindx_add_addr[i])->sin_addr.s_addr) != 1) {

    } else {
      ((struct sockaddr_in *)&bindx_add_addr[i])->sin_port = 0;
      bindx_add_addr[i].sa_family = AF_INET;
    }
  }

  if (sctp_bindx (sd, bindx_add_addr, nb_local_addr, SCTP_BINDX_ADD_ADDR) < 0) {
	  printf("Socket bind failed: %s\n", strerror (errno));
    OAILOG_ERROR (LOG_SCTP, "Socket bind failed: %s\n", strerror (errno));
    return -1;
  }

  memset ((void *)&init, 0, sizeof (struct sctp_initmsg));
  /*
   * Request a number of in/out streams
   */
  init.sinit_num_ostreams = SCTP_OUT_STREAMS;
  init.sinit_max_instreams = SCTP_IN_STREAMS;
  init.sinit_max_attempts = SCTP_MAX_ATTEMPTS;
  OAILOG_DEBUG (LOG_SCTP, "Requesting (%d %d) (in out) streams\n", init.sinit_num_ostreams, init.sinit_max_instreams);

  if (setsockopt (sd, IPPROTO_SCTP, SCTP_INITMSG, &init, (socklen_t) sizeof (struct sctp_initmsg)) < 0) {
    OAILOG_ERROR (LOG_SCTP, "Setsockopt IPPROTO_SCTP_INITMSG failed: %s\n", strerror (errno));
    printf("Setsockopt IPPROTO_SCTP_INITMSG failed: %s\n", strerror (errno));
    return -1;
  }

  /*
   * Subscribe to all events
   */
  memset ((void *)&events, 1, sizeof (struct sctp_event_subscribe));

  if (setsockopt (sd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof (struct sctp_event_subscribe)) < 0) {
	  printf("Setsockopt IPPROTO_SCTP_EVENTS failed: %s\n", strerror (errno));
    OAILOG_ERROR (LOG_SCTP, "Setsockopt IPPROTO_SCTP_EVENTS failed: %s\n", strerror (errno));
    return -1;
  }

  /*
   * SOCK_STREAM socket type requires an explicit connect to the remote host
   * * * * address and port.
   * * * * Only use IPv4 for the first connection attempt
   */
  {
    struct sockaddr_in                      addr;

    memset (&addr, 0, sizeof (struct sockaddr_in));

    if (inet_pton (AF_INET, remote_ip_addr, &addr.sin_addr.s_addr) != 1) {
    	printf("Failed to convert ip address %s to network type\n", remote_ip_addr);
      OAILOG_ERROR (LOG_SCTP, "Failed to convert ip address %s to network type\n", remote_ip_addr);
      goto err;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    OAILOG_DEBUG (LOG_SCTP, "[%d] Sending explicit connect to %s:%u\n", sd, remote_ip_addr, port);
    printf("[%d] Sending explicit connect to %s:%u\n", sd, remote_ip_addr, port);

    /*
     * Connect to remote host and port
     */
    if (sctp_connectx (sd, (struct sockaddr *)&addr, 1, NULL) < 0) {
    	printf("Connect to %s:%u failed: %s\n", remote_ip_addr, port, strerror (errno));
      OAILOG_ERROR (LOG_SCTP, "Connect to %s:%u failed: %s\n", remote_ip_addr, port, strerror (errno));
      goto err;
    }
  }
  printf ("Connected to  %s:%u\n", remote_ip_addr, port);
  printf ("Connected to  %s:%u\n", remote_ip_addr, port);
  /*
   * Get SCTP status
   */
  sctp_get_sockinfo (sd, &sctp_data_p->instreams, &sctp_data_p->outstreams, &sctp_data_p->assoc_id);
  sctp_data_p->sd = sd;
  sctp_get_peeraddresses (sd, &sctp_data_p->remote_ip_addresses, &sctp_data_p->nb_remote_addresses);
  sctp_get_localaddresses (sd, NULL, NULL);
  TAILQ_INIT (&sctp_data_p->sctp_queue);
  return sd;
err:

  if (sd != 0) {
    close (sd);
  }

  return -1;
}
