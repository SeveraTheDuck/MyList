#ifndef ANA_LIST_COMMANDS_H
#define ANA_LIST_COMMANDS_H

#include "ANA_List_struct.h"

uint32_t
ANA_List_PushBack  (const ANA_List_data_type value,
                          ANA_List*    const list);

uint32_t
ANA_List_PushFront (const ANA_List_data_type value,
                          ANA_List*    const list);

/// @brief This function inserts value after given position.
/// @param position
/// @param value
/// @param list
/// @return Index of inserted element.
uint32_t
ANA_List_Insert (const uint32_t           position,
                 const ANA_List_data_type value,
                       ANA_List*    const list);

/// @brief This function erases element on given position.
/// @note Invalids address of the last element (physical index in array) for linearization.
/// @param position
/// @param list
/// @return
uint32_t
ANA_List_Erase (const uint32_t        position,
                      ANA_List* const list);

ANA_List_error_type
ANA_List_ReallocUp (ANA_List* const list);

ANA_List_error_type
ANA_List_ReallocDown (const size_t    new_capacity,
                      ANA_List* const list);

#endif
