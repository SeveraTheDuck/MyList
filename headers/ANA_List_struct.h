#ifndef ANA_LIST_STRUCT_H
#define ANA_LIST_STRUCT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ANA_List_config.h"

#define ANA_List_Assert(list)   \
    assert (list);              \
    assert (list->list_data);   \
    assert (list->next);        \
    assert (list->prev);

#define ANA_List_VerifyAndDump(list)    \
    if (ANA_List_Verify (list))         \
    {                                   \
        ANA_List_Dump (list);           \
        ANA_List_Dtor (list);           \
        exit (ANA_List_ERROR_OCCURED);  \
    }

typedef unsigned short ANA_List_error_type;

const ANA_List_error_type ANA_List_ERROR_OCCURED = 1;
const ANA_List_error_type ANA_List_NO_ERROR      = 0;

const int8_t ANA_List_NO_PREV_ELEMENT = -1;

const int8_t ANA_List_DUMMY_ELEMENT  =  0;

struct ANA_List_errors_struct
{
    ANA_List_error_type ANA_List_ERROR_LIST_NULLPTR                     : 1;
    ANA_List_error_type ANA_List_ERROR_LIST_DATA_NULLPTR                : 1;
    ANA_List_error_type ANA_List_ERROR_LIST_NEXT_NULLPTR                : 1;
    ANA_List_error_type ANA_List_ERROR_LIST_PREV_NULLPTR                : 1;

    ANA_List_error_type ANA_List_ERROR_HEAD_OUT_OF_RANGE                : 1;
    ANA_List_error_type ANA_List_ERROR_TAIL_OUT_OF_RANGE                : 1;
    ANA_List_error_type ANA_List_ERROR_FREE_OUT_OF_RANGE                : 1;
    ANA_List_error_type ANA_List_ERROR_NUMBER_OF_ELEMENTS_OUT_OF_RANGE  : 1;

    ANA_List_error_type ANA_List_ERROR_CYCLE_FOUND                      : 1;
};

/// @brief This union is used to avoid UB when compare two bit fields.
union ANA_List_errors
{
    ANA_List_errors_struct errors_struct;
    char filler_array[sizeof (ANA_List_errors_struct)];
};

struct ANA_List
{
    ANA_List_data_type* list_data;
    uint32_t* next;
    int32_t * prev;

    uint32_t free;

    size_t list_capacity;
    size_t list_n_elems;

    // Two bit fields, the first one is for errors.
    ANA_List_errors     list_errors_field;
    // The second one is for comparison, it is full of zeros.
    ANA_List_errors ref_list_errors_field;
};

ANA_List_error_type
ANA_List_Ctor           (ANA_List* const list);

ANA_List_error_type
ANA_List_Dtor           (ANA_List* const list);

ANA_List_error_type
ANA_List_Verify         (ANA_List* const list);

void
ANA_List_Dump              (const ANA_List* const list);

#endif
