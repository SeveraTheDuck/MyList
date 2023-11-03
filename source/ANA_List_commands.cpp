#include "../headers/ANA_List_commands.h"

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

    ANA_List_VerifyAndDump (list);

    return list->tail;
}

int
ANA_List_Insert (const int                position,
                 const ANA_List_data_type value,
                       ANA_List*    const list)
{
    ANA_List_VerifyAndDump (list);

    if ((unsigned int) position > list->list_n_elems)
    {
        fprintf (stderr, "Position element out of range\n");
        return 0;
    }
    list->list_n_elems++;

    if (list->list_data [position] == ANA_List_POISON)
    {
        ANA_List_PushBack (value, list);
        return list->tail;
    }

    int new_index = list->free;
    list->free    = list->next[ list->free ];

    list->next [ list->prev [ position ] ] = new_index;
    list->prev [ new_index ]               = list->prev [ position ];

    list->prev [ position  ] = new_index;
    list->next [ new_index ] = position;

    list->list_data [ new_index ] = value;


    if (position == list->head)
    {
        list->head = new_index;
        list->prev [ new_index ] = 0;
    }

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
        return 0;
    }
    list->list_n_elems--;

    list->next [ list->prev [position] ] =
                 list->next [position];
    list->prev [ list->next [position] ] =
                 list->prev [position];

    int prev_index = list->prev [position];

    list->next [position] = list->free;
    list->prev [position] = -1;
    list->list_data [position] = ANA_List_POISON;

    list->free = (int) position;

    return prev_index;
}
