#ifndef STANDART_LIST_STRUCT_H
#define STANDART_LIST_STRUCT_H

typedef int  standart_List_data_type;
const size_t standart_List_INITIAL_CAPACITY = 10;

struct standart_List_errors
{

};

struct standart_List_node
{
    standart_List_data_type node_data;
    standart_List_node*     prev_node;
    standart_List_node*     next_node;
};

//list_add_after(Node*, data)
//for ( node = node->next )

struct standart_List_node
{
    standart_List_errors errors_field;
    int node_index;

    standart_List_data_type node_data;
    standart_List_node*     prev_node;
    standart_List_node*     next_node;
};

struct standart_List
{
    standart_List_node* array_of_nodes;
    size_t list_capacity
    int    n_of_elems;
}

#endif
