#ifndef STANDARD_LIST_STRUCT_H
#define STANDARD_LIST_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int32_t standard_List_data_type;

struct standard_List_node
{
    standard_List_data_type node_data;
    standard_List_node*     prev_node;
    standard_List_node*     next_node;
};

struct standard_List
{
    standard_List_node* head;
    standard_List_node* tail;
    size_t n_elems;

    standard_List_node* first_node;
};

void
standard_List_Ctor (standard_List* const list,
                    const standard_List_data_type first_value);

void standard_List_Dtor (standard_List* const list);

standard_List_node*
standard_List_PushBack (standard_List* const list,
                        const standard_List_data_type value);

standard_List_node*
standard_List_Insert (standard_List_node* const prev_node,
                      const standard_List_data_type value);

void
standard_List_Erase (standard_List_node* const cur_node,
                     standard_List*      const list);

void
standard_List_Dump (const standard_List* const list);

#endif
