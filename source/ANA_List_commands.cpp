#include "../headers/ANA_List_commands.h"

int
ANA_List_PushBack (const ANA_List_data_type value,
                         ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    ANA_List_Insert (list->prev [ ANA_List_DUMMY_ELEMENT ],
                     value, list);

    ANA_List_VerifyAndDump (list);

    return list->prev [ ANA_List_DUMMY_ELEMENT ];
}

int
ANA_List_PushFront (const ANA_List_data_type value,
                          ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    ANA_List_Insert (ANA_List_DUMMY_ELEMENT, value, list);

    ANA_List_VerifyAndDump (list);

    return list->next [ ANA_List_DUMMY_ELEMENT ];
}

int
ANA_List_Insert (const int                position,
                 const ANA_List_data_type value,
                       ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    if (position < 0 ||
        list->prev [position] == ANA_List_NO_PREV_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Insert "
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
    int new_index = list->free;
    list->free    = list->next[ list->free ];

    list->prev [ list->next [position] ] = new_index;
    list->next [ new_index ] = list->next [position];

    list->next [ position ]  = new_index;
    list->prev [ new_index ] = position;

    list->list_data [ new_index ] = value;

    ANA_List_VerifyAndDump (list);

    return new_index;
}

int
ANA_List_Erase (const unsigned int       position,
                      ANA_List*    const list)
{
    if (list->prev [position] == ANA_List_NO_PREV_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Erase "
                         "position of element out of range.\n");
        return ANA_List_DUMMY_ELEMENT;
    }
    else if (position == ANA_List_DUMMY_ELEMENT)
    {
        fprintf (stderr, "In function ANA_List_Erase "
                         "calling for dummy element, return.\n");
        return ANA_List_DUMMY_ELEMENT;
    }

    list->list_n_elems--;

    list->next [ list->prev [position] ] = list->next [position];
    list->prev [ list->next [position] ] = list->prev [position];

    int prev_index = list->prev [position];

    list->next      [position] = list->free;
    list->prev      [position] = ANA_List_NO_PREV_ELEMENT;
    list->list_data [position] = ANA_List_POISON;

    list->free = (int) position; // size_t ptrdiff_t

    return prev_index;
}

ANA_List_error_type
ANA_List_ReallocUp (ANA_List* const list)
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

    list->next = (int*) realloc (list->next,
                                 list->list_capacity * sizeof (int));
    if (!list->next)
    {
        perror ("list->next reallocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_NEXT_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->prev = (int*) realloc (list->prev,
                                 list->list_capacity * sizeof (int));
    if (!list->prev)
    {
        perror ("list->prev reallocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_PREV_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    for (size_t list_index = list->list_capacity / ANA_List_EXPAND_MULTIPLIER;
                list_index < list->list_capacity;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = (int) list_index + 1;
        list->prev     [list_index] = ANA_List_NO_PREV_ELEMENT;
    }

    list->next [ list->list_capacity - 1] = 0;
    list->free = (int) (list->list_capacity / ANA_List_EXPAND_MULTIPLIER);

    return ANA_List_NO_ERROR;
}

// int SlOW_sl0_w_S10wD0nTUseMeST0P_itsM1stAke_d0ntCoPYme_or_uGetDamagetoDiarrhea_F1nd_ElemByinDex (quadratic_equation* const square_elements, double* const b_coef)
// {
//     static bool is_equation_created = 0;
//
//     if (!is_equation_created)
//     {
//         square_elements->a_coef = (double) rand() / RAND_MAX;
//         *b_coef                 = (double) rand() / RAND_MAX;
//         square_elements->c_coef = (double) rand() / RAND_MAX;
//
//         is_equation_created = 1;
//         return *b_coef;
//     }
//
//     double disc = (*b_coef) * (*b_coef) - 4 * (square_elements->a_coef) * (square_elements->c_coef);
//     if (square_elements->n_roots == 0 && disc < 0 ||
//         square_elements->n_roots == 1 && ((square_elements->a_coef * square_elements->root_1 * square_elements->root_1) - (*b) * square_elements->root_1 == - square_elements->c_coef))
//     {
//
//     }
//
// }
