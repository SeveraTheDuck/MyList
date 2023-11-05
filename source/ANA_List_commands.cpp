#include "../headers/ANA_List_commands.h"

// some magic in this commands. No way to make more understandable?
int
ANA_List_PushBack (const ANA_List_data_type value,
                         ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    list->next [ list->tail ] = list->free;
    list->prev [ list->free ] = list->tail;
    list->tail = list->free;

    list->free = list->next [ list->free ];

    list->next [ list->tail ] = 0;
    list->list_data [ list->tail ] = value;

    list->prev [ ANA_List_DUMMY_ELEMENT ] = list->tail;

    ANA_List_VerifyAndDump (list);

    return list->tail;
}

int
ANA_List_PushFront (const ANA_List_data_type value,
                          ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    list->prev [ list->head ] = list->free;
    list->next [ list->free ] = list->head;
    list->head = list->free;

    list->free = list->next [ list->free ];

    list->prev [ list->head ] = 0;
    list->list_data [ list->head ] = value;

    list->next [ ANA_List_DUMMY_ELEMENT ] = list->head;

    ANA_List_VerifyAndDump (list);

    return list->tail;
}

int
ANA_List_Insert (const int                position,
                 const ANA_List_data_type value,
                       ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    if ((unsigned int) position > list->list_n_elems ||
                       position < 1)
    {
        fprintf (stderr, "Position element out of range\n");
        return ANA_List_DUMMY_ELEMENT;
    }
    list->list_n_elems++;

    if (list->prev [position] == ANA_List_NO_PREV_ELEMENT)
    {
        ANA_List_PushBack (value, list);
        return list->tail;
    }

    if (position == list->head)
    {
        ANA_List_PushFront (value, list);
        return list->head;
    }

    // insert itself
    int new_index = list->free;
    list->free    = list->next[ list->free ];

    list->next [ list->prev [ position ] ] = new_index;
    list->prev [ new_index ]               = list->prev [ position ];

    list->prev [ position  ] = new_index;
    list->next [ new_index ] = position;

    list->list_data [ new_index ] = value;

    ANA_List_VerifyAndDump (list);

    return new_index;
}

int
ANA_List_Erase (const unsigned int       position,
                      ANA_List*    const list)
{
    if (position > list->list_n_elems)
    {
        fprintf (stderr, "Position element out of range\n");
        return ANA_List_DUMMY_ELEMENT;
    }
    list->list_n_elems--;

    list->next [ list->prev [position] ] =
                 list->next [position];
    list->prev [ list->next [position] ] =
                 list->prev [position];

    int prev_index = list->prev [position];

    list->next      [position] = list->free;
    list->prev      [position] = ANA_List_NO_PREV_ELEMENT;
    list->list_data [position] = ANA_List_POISON;

    list->free = (int) position;

    return prev_index;
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
