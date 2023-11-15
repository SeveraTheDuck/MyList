#include "../headers/standard_List_struct.h"

int32_t main ()
{
    standard_List list;
    standard_List_Ctor (&list, 10);

    standard_List_PushBack (&list, 20);
    standard_List_PushBack (&list, 30);
    standard_List_node* erasing = standard_List_PushBack (&list, 40);

    standard_List_Erase (erasing, &list);

    standard_List_Dump (&list);

    standard_List_Dtor (&list);

    return 0;
}
