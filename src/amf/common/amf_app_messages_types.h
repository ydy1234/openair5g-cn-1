

#define AMF_APP_INITIAL_UE_MESSAGE(mSGpTR)               (mSGpTR)->ittiMsg.amf_app_initial_ue_message
#define AMF_APP_NGAP_AMF_UE_ID_NOTIFICATION(mSGpTR)      (mSGpTR)->ittiMsg.amf_app_ngap_amf_ue_id_notification



typedef struct itti_amf_app_initial_ue_message_s {
  sctp_assoc_id_t     sctp_assoc_id; // key stored in AMF_APP for AMF_APP forward NAS response to NGAP
  uint32_t            gnb_id; 
  amf_ue_ngap_id_t    amf_ue_ngap_id;
  ran_ue_ngap_id_t    ran_ue_ngap_id;
  bstring             nas;
  tai_t               tai;
  cgi_t               cgi;
  //as_cause_t          as_cause;          /* Establishment cause                     */

  bool                is_s_tmsi_valid;
  bool                is_csg_id_valid;
  bool                is_gummei_valid;
  //itti_ngap_initial_ue_message_t transparent;
} itti_amf_app_initial_ue_message_t;


typedef struct itti_amf_app_ngap_amf_ue_id_notification_s {
  ran_ue_ngap_id_t          ran_ue_ngap_id;
  amf_ue_ngap_id_t          amf_ue_ngap_id;
  sctp_assoc_id_t           sctp_assoc_id;
} itti_amf_app_ngap_amf_ue_id_notification_t;
