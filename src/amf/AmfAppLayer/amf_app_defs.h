#ifndef _FILE_AMF_APP_DEFS_H_
#define _FILE_AMF_APP_DEFS_H_
#include "intertask_interface.h"

typedef struct {
  /* UE contexts + some statistics variables */
  //mme_ue_context_t mme_ue_contexts;

  long statistic_timer_id;
  uint32_t statistic_timer_period;

  /* Reader/writer lock */
  pthread_rwlock_t rw_lock;

  /* ***************Statistics*************
   * number of attached UE,number of connected UE,
   * number of idle UE,number of default bearers, 
   * number of S1_U bearers,number of PDN sessions
   */

  uint32_t               nb_enb_connected;
  uint32_t               nb_ue_attached;
  uint32_t               nb_ue_connected;
  //uint32_t               nb_default_eps_bearers;
  //uint32_t               nb_s1u_bearers;

  /* ***************Changes in Statistics**************/

  uint32_t               nb_ue_attached_since_last_stat;
  uint32_t               nb_ue_detached_since_last_stat;
  uint32_t               nb_ue_connected_since_last_stat;
  uint32_t               nb_ue_disconnected_since_last_stat;
  uint32_t               nb_eps_bearers_established_since_last_stat;
  uint32_t               nb_eps_bearers_released_since_last_stat;
  uint32_t               nb_enb_connected_since_last_stat;
  uint32_t               nb_enb_released_since_last_stat;
  uint32_t               nb_s1u_bearers_released_since_last_stat;
  uint32_t               nb_s1u_bearers_established_since_last_stat;
} amf_app_desc_t;


#endif
