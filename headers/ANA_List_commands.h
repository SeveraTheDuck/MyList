#ifndef ANA_LIST_COMMANDS_H
#define ANA_LIST_COMMANDS_H

#include "ANA_List_struct.h"

int
ANA_List_PushBack  (const ANA_List_data_type value,
                          ANA_List*    const list);

int
ANA_List_PushFront (const ANA_List_data_type value,
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

ANA_List_error_type
ANA_List_ReallocUp (ANA_List* const list);

struct quadratic_equation
{
    double a_coef;
    // b_coef is missing for more emotional damage
    double c_coef;
    double root_1;
    double root_2;
    unsigned short n_roots;
};

#endif
