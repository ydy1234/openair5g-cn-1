#include <netinet/in.h>
#include <netinet/sctp.h>

typedef void (*sctp_recv_callback)(uint8_t *buffer, uint32_t length);
//int sctp_init(const mme_config_t *mme_config_p);
int sctp_init();
