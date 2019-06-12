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


#include <stdint.h>

#include "sctp_gNB_defs.h"
#include "common_types.h"

#ifndef FILE_SCTP_PRIMITIVES_CLIENT_SEEN
#define FILE_SCTP_PRIMITIVES_CLIENT_SEEN

/** @defgroup _sctp_impl_ SCTP Layer Reference Implementation
 * @ingroup _ref_implementation_
 * @{
 */

/** \brief Perform association to a remote peer
 * \param ip_addr Peer IPv4 address
 * \param port Remote port to connect to
 * \param args Upper layer args that will be provided to connected callback
 * \param connected_callback Connected callback
 * \param recv_callback Data received callback
 * @return < 0 in case of failure
 */
int sctp_connect_to_remote_host(char *local_ip_addr[],
                                int nb_local_addr,
                                char *remote_ip_addr,
                                const uint16_t port,
                                int socket_type,
                                sctp_data_t *sctp_data_p);

/** \brief Send message over SCTP
 * \param sctp_data_p Pointer to the SCTP desc
 * \param ppid Payload Protocol Identifier
 * \param stream SCTP stream on which data will be sent
 * \param buffer Pointer to buffer
 * \param length Buffer length
 * @return < 0 in case of failure
 */
int sctp_send_msg(sctp_data_t *sctp_data_p, const uint16_t ppid, const sctp_stream_id_t stream,
                  const uint8_t * const buffer, const size_t length);


int ngap_sctp_send_msg(int fd, const uint16_t ppid, const sctp_stream_id_t stream,
                  const uint8_t * const buffer, const size_t length);


/** \brief Properly disconnect the peer
 * \param assoc_id The SCTP association ID
 * @return < 0 in case of failure
 */
void sctp_disconnect(const sctp_assoc_id_t assoc_id);

void sctp_terminate(void);

/* @} */
#endif /* FILE_SCTP_PRIMITIVES_CLIENT_SEEN */
