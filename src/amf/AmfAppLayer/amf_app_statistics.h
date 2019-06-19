#ifndef FILE_MME_APP_STATISTICS_SEEN
#define FILE_MME_APP_STATISTICS_SEEN

int amf_app_statistics_display(void);

/*********************************** Utility Functions to update Statistics**************************************/
void update_amf_app_stats_connected_gnb_add(void);
void update_amf_app_stats_connected_gnb_sub(void);
void update_amf_app_stats_connected_ue_add(void);
void update_amf_app_stats_connected_ue_sub(void);
void update_amf_app_stats_s1u_bearer_add(void);
void update_amf_app_stats_s1u_bearer_sub(void);
void update_amf_app_stats_default_bearer_add(void);
void update_amf_app_stats_default_bearer_sub(void);
void update_amf_app_stats_attached_ue_add(void);
void update_amf_app_stats_attached_ue_sub(void);

#endif 

