#ifndef ANA_LIST_CONFIG_H
#define ANA_LIST_CONFIG_H

typedef int32_t ANA_List_data_type;
#define ANA_List_OUTPUT_F "%d"

const size_t ANA_List_INIT_VOLUME = 4;

const size_t ANA_List_EXPAND_MULTIPLIER = 2;

const ANA_List_data_type ANA_List_POISON = -31415926;

const char ANA_List_IMAGE_CONSTRUCT_FILE_NAME[] = "logs/ANA_List_img_logs.dot";

// without extension for setting number automatically
const char ANA_List_IMAGE_FILE_NAME[] = "logs/logs_image";

const char ANA_List_HTML_LOGS_FILE_NAME[] = "logs/logs.html";

#endif
