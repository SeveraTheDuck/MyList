#ifndef ANA_LIST_COMMANDS_H
#define ANA_LIST_COMMANDS_H

#include "ANA_List_struct.h"

int
ANA_List_PushBack (const ANA_List_data_type value,
                         ANA_List*    const list);

/// @brief This function inserts value into given position.
/// @param position
/// @param value
/// @param list
/// @return Index of inserted element.
int
ANA_List_Insert (const int       position,
                 const ANA_List_data_type value,
                       ANA_List*    const list);

int
ANA_List_Erase (const unsigned int       position,
                      ANA_List*    const list);

#endif
