#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <libconfig.h>
  
#include <arpa/inet.h>          /* To provide inet_addr */

#include "assertions.h"
#include "dynamic_memory_check.h"
#include "log.h"
#include "intertask_interface.h"
#include "spgw_config.h"

amf_config_t                            amf_config = {.rw_lock = PTHREAD_RWLOCK_INITIALIZER, 0};
