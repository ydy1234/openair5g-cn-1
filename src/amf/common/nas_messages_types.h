#define NGAP_UL_DATA_IND(mSGpTR)              (mSGpTR)->ittiMsg.ngap_data_ind
#define NAS_MM_DL_DATA_REQ(mSGpTR)              (mSGpTR)->ittiMsg.nas_mm_dl_data_req


typedef struct{
  amf_ue_ngap_id_t  ue_id;
  bstring           nas_msg;
  tai_t             tai;
  cgi_t             cgi;
}ngap_data_ind_t;
