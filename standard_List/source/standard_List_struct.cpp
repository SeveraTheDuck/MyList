#include "../headers/standard_List_struct.h"

static void
standard_List_AllocateNode (standard_List_node** node);

void
standard_List_Ctor (standard_List* const list,
                    const standard_List_data_type first_value)
{
    if (!list)
    {
        fprintf (stderr, "standard_List_ERROR_LIST_NULLPTR\n");
        return;
    }

    standard_List_AllocateNode (&list->first_node);

    list->head    = list->first_node;
    list->tail    = list->first_node;
    list->n_elems = 1;

    list->first_node->node_data = first_value;
    list->first_node->prev_node = nullptr;
    list->first_node->next_node = nullptr;
}

static void
standard_List_AllocateNode (standard_List_node** node)
{
    *node = (standard_List_node*) calloc (1, sizeof (standard_List_node));
}

void
standard_List_Dtor (standard_List* list)
{
    if (!list)
    {
        fprintf (stderr, "standard_List_ERROR_LIST_NULLPTR\n");
        return;
    }

    standard_List_node* prev_node = nullptr;

    for (standard_List_node* cur_node = list->tail;
                             cur_node != nullptr;
                             cur_node = prev_node)
    {
        prev_node = cur_node->prev_node;

        cur_node->prev_node = nullptr;
        cur_node->next_node = nullptr;
        cur_node->node_data = 0;

        free (cur_node);
    }
}

standard_List_node*
standard_List_PushBack (standard_List* const list,
                        const standard_List_data_type value)
{
    standard_List_node* new_node = nullptr;
    standard_List_AllocateNode (&new_node);

    new_node->node_data = value;
    new_node->prev_node = list->tail;
    new_node->next_node = nullptr;

    list->tail->next_node = new_node;
    list->tail = new_node;

    return new_node;
}

standard_List_node*
standard_List_Insert (standard_List_node* const prev_node,
                      const standard_List_data_type value)
{
    standard_List_node* new_node = nullptr;
    standard_List_AllocateNode (&new_node);

    new_node->prev_node = prev_node;
    new_node->next_node = prev_node->next_node;
    new_node->node_data = value;

    prev_node->next_node->prev_node = new_node;
    prev_node->next_node = new_node;

    return new_node;
}

void
standard_List_Erase (standard_List_node* const cur_node)
{
    cur_node->next_node->prev_node = cur_node->prev_node;
    cur_node->prev_node->next_node = cur_node->next_node;
    cur_node->node_data = 0;

    free (cur_node);
}

void
standard_List_Dump (const standard_List* const list)
{
    for (standard_List_node* cur_node = list->head;
                             cur_node != nullptr;
                             cur_node = cur_node->next_node)
    {
        fprintf (stderr, "%p %d %p %p\n", cur_node, cur_node->node_data,
                                          cur_node->next_node,
                                          cur_node->prev_node);
    }
}
