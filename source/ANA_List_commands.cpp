#include "../headers/ANA_List_commands.h"

static ANA_List_error_type
ANA_List_Realloc (ANA_List* const list);

uint32_t
ANA_List_PushBack (const ANA_List_data_type value,
                         ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    ANA_List_Insert ((uint32_t) list->prev [ ANA_List_DUMMY_ELEMENT ],
                     value, list);

    ANA_List_VerifyAndDump (list);

    return (uint32_t) list->prev [ ANA_List_DUMMY_ELEMENT ];
}

uint32_t
ANA_List_PushFront (const ANA_List_data_type value,
                          ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    ANA_List_Insert (ANA_List_DUMMY_ELEMENT, value, list);

    ANA_List_VerifyAndDump (list);

    return list->next [ ANA_List_DUMMY_ELEMENT ];
}

uint32_t
ANA_List_Insert (const uint32_t           position,
                 const ANA_List_data_type value,
                       ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    if (position >= list->list_capacity ||
        list->prev [position] == ANA_List_NO_PREV_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Insert() "
                         "position of element out of range.\n");
        return ANA_List_DUMMY_ELEMENT;
    }

    if (list->list_n_elems == list->list_capacity)
    {
        list->list_capacity *= ANA_List_EXPAND_MULTIPLIER;
        ANA_List_ReallocUp (list);
    }
    list->list_n_elems++;

    // insert itself
    uint32_t new_index = list->free;
    list->free = (uint32_t) list->next[ list->free ];

    list->prev [ list->next [position] ] = (int32_t) new_index;
    list->next [ new_index ] = list->next [position];

    list->next [ position ]  = new_index;
    list->prev [ new_index ] = (int32_t) position;

    list->list_data [ new_index ] = value;

    ANA_List_VerifyAndDump (list);

    return new_index;
}

uint32_t
ANA_List_Erase (const uint32_t        position,
                      ANA_List* const list)
{
    if (list->prev [position] == ANA_List_NO_PREV_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Erase() "
                         "position of element out of range.\n");
        return ANA_List_DUMMY_ELEMENT;
    }

    else if (position == ANA_List_DUMMY_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Erase() "
                         "calling for dummy element, return.\n");
        return ANA_List_DUMMY_ELEMENT;
    }

    list->list_n_elems--;

    // connect next and prev of position
    list->next [ list->prev [position] ] = list->next [position];
    list->prev [ list->next [position] ] = list->prev [position];

    uint32_t prev_index = (uint32_t) list->prev [position];

    // move the last element for linearization
    list->next      [position] = list->next [ list->list_n_elems ];
    list->prev      [position] = list->prev [ list->list_n_elems ];
    list->list_data [position] = list->list_data [ list->list_n_elems ];

    list->next      [ list->list_n_elems ] = list->free;
    list->prev      [ list->list_n_elems ] = ANA_List_NO_PREV_ELEMENT;
    list->list_data [ list->list_n_elems ] = ANA_List_POISON;

    list->free = (uint32_t) list->list_n_elems;

    return prev_index;
}

ANA_List_error_type
ANA_List_ReallocUp (ANA_List* const list)
{
    if (ANA_List_Realloc (list))
    {
        return ANA_List_ERROR_OCCURED;
    }

    for (size_t list_index = list->list_capacity / ANA_List_EXPAND_MULTIPLIER;
                list_index < list->list_capacity;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = (uint32_t) list_index + 1;
        list->prev     [list_index] = ANA_List_NO_PREV_ELEMENT;
    }

    list->next [ list->list_capacity - 1] = 0;
    list->free = (uint32_t) (list->list_capacity / ANA_List_EXPAND_MULTIPLIER);

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_ReallocDown (const size_t    new_capacity,
                      ANA_List* const list)
{
    size_t old_capacity = list->list_capacity;
    list->list_capacity = new_capacity;

    if (ANA_List_Realloc (list))
    {
        fprintf (stderr, "ANA_List_ReallocDown() error,"
                         "old capacity saved\n");
        list->list_capacity = old_capacity;

        return ANA_List_ERROR_OCCURED;
    }

    if (list->prev [ new_capacity - 1] == ANA_List_NO_PREV_ELEMENT)
    {
        list->next [ new_capacity - 1] = ANA_List_DUMMY_ELEMENT;
    }

    return ANA_List_NO_ERROR;
}

static ANA_List_error_type
ANA_List_Realloc (ANA_List* const list)
{
    list->list_data = (ANA_List_data_type*) realloc (list->list_data,
                                                     list->list_capacity *
                                                     sizeof (ANA_List_data_type));
    if (!list->list_data)
    {
        perror ("list->list_data reallocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_DATA_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->next = (uint32_t*) realloc (list->next,
                             list->list_capacity * sizeof (uint32_t));
    if (!list->next)
    {
        perror ("list->next reallocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_NEXT_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->prev = (int32_t*)  realloc (list->prev,
                             list->list_capacity * sizeof (uint32_t));
    if (!list->prev)
    {
        perror ("list->prev reallocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_PREV_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    return ANA_List_NO_ERROR;
}
