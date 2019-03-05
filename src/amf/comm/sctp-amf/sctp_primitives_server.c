#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#include "log.h"
#include "intertask_interface.h"
#include "assertions.h"

#define SCTP_RC_ERROR       -1
#define SCTP_RC_NORMAL_READ  0
#define SCTP_RC_DISCONNECT   1


typedef struct sctp_descriptor_s {
  // List of connected peers
  struct sctp_association_s              *available_connections_head;
  struct sctp_association_s              *available_connections_tail;

  uint32_t                                number_of_connections;
  uint16_t                                nb_instreams;
  uint16_t                                nb_outstreams;
} sctp_descriptor_t;
static sctp_descriptor_t                  sctp_desc;

//------------------------------------------------------------------------------
int
memory_pools_free (
  memory_pools_handle_t memory_pools_handle,
  memory_pool_item_handle_t memory_pool_item_handle,
  uint16_t info_0)
{ 
  memory_pools_t                         *memory_pools;
  memory_pool_item_t                     *memory_pool_item;
  pool_id_t                               pool;
  items_group_index_t                     item_index;
  uint32_t                                item_size;
  uint32_t                                pool_item_size;
  uint16_t                                info_1;
  int                                     result;
  
  /*
   * Recover memory_pools
   */
  memory_pools = memory_pools_from_handler (memory_pools_handle);
  AssertError (memory_pools != NULL, return (EXIT_FAILURE), "Failed to retrieve memory pools for handle %p!\n", memory_pools_handle);
  /*
   * Recover memory pool item
   */
  memory_pool_item = memory_pool_item_from_handler (memory_pool_item_handle);
  AssertError (memory_pool_item != NULL, return (EXIT_FAILURE), "Failed to retrieve memory pool item for handle %p!\n", memory_pool_item_handle);
  info_1 = memory_pool_item->start.info[1];
  VCD_SIGNAL_DUMPER_DUMP_VARIABLE_BY_NAME (VCD_SIGNAL_DUMPER_VARIABLE_MP_FREE, __sync_or_and_fetch (&vcd_mp_free, 1L << info_1));
  /*
   * Recover pool index
   */
  pool = memory_pool_item->start.pool_id;
  AssertFatal (pool < memory_pools->pools_defined, "Pool index is invalid (%u/%u)!\n", pool, memory_pools->pools_defined);
  item_size = memory_pools->pools[pool].item_data_number;
  pool_item_size = memory_pools->pools[pool].pool_item_size;
  item_index = (((void *)memory_pool_item) - ((void *)memory_pools->pools[pool].items)) / pool_item_size;
  MP_DEBUG (" Free  [%2u][%6d]{%6d}, %3u %3u,         %p, %p, %p, %u\n",
            pool, item_index,
            items_group_free_items (&memory_pools->pools[pool].items_group_free),
            memory_pool_item->start.info[0], info_1, memory_pool_item_handle, memory_pool_item, memory_pools->pools[pool].items, ((uint32_t) (item_size * sizeof (memory_pool_data_t))));
  /*        
   * Sanity check on calculated item index
   */
  AssertFatal (memory_pool_item == memory_pool_item_from_index (&memory_pools->pools[pool], item_index),
               "Incorrect memory pool item address (%p, %p) for pool %u, item %d!\n", memory_pool_item, memory_pool_item_from_index (&memory_pools->pools[pool], item_index), pool, item_index);
  /*           
   * Sanity check on end marker, must still be present (no write overflow)
   */
  AssertFatal (memory_pool_item->data[item_size] == POOL_ITEM_END_MARK, "Memory pool item is corrupted, end mark is not present for pool %u, item %d!\n", pool, item_index);
  /*
   * Sanity check on item status, must be allocated
   */
  AssertFatal (memory_pool_item->start.item_status == ITEM_STATUS_ALLOCATED, "Trying to free a non allocated (%x) memory pool item (pool %u, item %d)!\n", memory_pool_item->start.item_status, pool, item_index);
  memory_pool_item->start.item_status = ITEM_STATUS_FREE;
  result = items_group_put_free_item (&memory_pools->pools[pool].items_group_free, item_index);
  AssertError (result == EXIT_SUCCESS, {
               }
               , "Failed to free memory pool item (pool %u, item %d)!\n", pool, item_index);
  VCD_SIGNAL_DUMPER_DUMP_VARIABLE_BY_NAME (VCD_SIGNAL_DUMPER_VARIABLE_MP_FREE, __sync_and_and_fetch (&vcd_mp_free, ~(1L << info_1)));
  return (result);
}

int 
itti_free (
  task_id_t task_id,
  void *ptr)
{     
  int                                     result = EXIT_SUCCESS;
    
  AssertFatal (ptr != NULL, "Trying to free a NULL pointer (%d)!\n", task_id);
  result = memory_pools_free (itti_desc.memory_pools_handle, ptr, task_id);
  AssertError (result == EXIT_SUCCESS, {
               }, "Failed to free memory at %p (%d)!\n", ptr, task_id);
  return (result);
}

static void * sctp_intertask_interface (
    __attribute__ ((unused)) void *args_p)
{
  itti_mark_task_ready (TASK_SCTP);
  OAILOG_START_USE ();
//  MSC_START_USE ();

  while (1) {
    MessageDef                             *received_message_p = NULL;

    itti_receive_msg (TASK_SCTP, &received_message_p);

    switch (ITTI_MSG_ID (received_message_p)) {
    case SCTP_INIT_MSG:{
        }
        break;
    default:{
        }
        break;
    }
    itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
    received_message_p = NULL;
  }
  return NULL;
}

//int sctp_init (const mme_config_t * mme_config_p)
int sctp_init ()
{
  OAILOG_DEBUG (LOG_SCTP, "Initializing SCTP task interface\n");
  memset (&sctp_desc, 0, sizeof (sctp_descriptor_t));
  /*
   * Number of streams from configuration
   */     
  //sctp_desc.nb_instreams = mme_config_p->sctp_config.in_streams;
  //sctp_desc.nb_outstreams = mme_config_p->sctp_config.out_streams;

  sctp_desc.nb_instreams = 3;
  sctp_desc.nb_outstreams = 3;

  if (itti_create_task (TASK_SCTP, &sctp_intertask_interface, NULL) < 0) {
    OAILOG_ERROR (LOG_SCTP, "create task failed\n");
    OAILOG_DEBUG (LOG_SCTP, "Initializing SCTP task interface: FAILED\n");
    return -1;
  }
  
  OAILOG_DEBUG (LOG_SCTP, "Initializing SCTP task interface: DONE\n");
  return 0;
}
