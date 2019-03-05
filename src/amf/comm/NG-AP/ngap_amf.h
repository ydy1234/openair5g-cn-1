#include "hashtable.h"
#include "common_types.h"
struct gnb_description_s;

// The current n2 state of the AMF relating to the specific gNB
enum amf_n2_gnb_state_s{
    NGAP_INIT,
    NGAP_RESETING    
};

//enum n2_ue_state_s{
//    
//};

typedef struct gnb_description_s {

  enum amf_n2_gnb_state_s n2_state;         ///< State of the gNB specific NGAP association

  /** gNB related parameters **/
  /*@{*/
  char     gnb_name[150];      ///< Printable gNB Name
  uint32_t gnb_id;             ///< Unique gNB ID
  uint8_t  default_paging_drx; ///< Default paging DRX interval for gNB
  /*@}*/

  /** UE list for this gNB **/
  /*@{*/
  uint32_t nb_ue_associated; ///< Number of NAS associated UE on this gNB
  hash_table_ts_t  ue_coll; // contains ue_description_s, key is ue_description_s.?;
  /*@}*/

  /** SCTP stuff **/
  /*@{*/
  sctp_assoc_id_t  sctp_assoc_id;    ///< SCTP association id on this machine
  sctp_stream_id_t next_sctp_stream; ///< Next SCTP stream
  sctp_stream_id_t instreams;        ///< Number of streams avalaible on gNB -> AMF
  sctp_stream_id_t outstreams;       ///< Number of streams avalaible on AMF -> gNB
  /*@}*/
} gnb_description_t;

int ngap_amf_init(void);
