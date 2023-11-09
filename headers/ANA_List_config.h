#ifndef ANA_LIST_CONFIG_H
#define ANA_LIST_CONFIG_H

typedef int ANA_List_data_type;
#define ANA_List_OUTPUT_F "%d"

const size_t ANA_List_INIT_VOLUME = 4;

const size_t ANA_List_EXPAND_MULTIPLIER = 2;

const ANA_List_data_type ANA_List_POISON = -31415926;

const char ANA_List_log_img_file_name[] = "logs/ANA_List_img_logs.dot";

const char ANA_List_log_txt_file_name[] = "logs/logs.html";

#endif
