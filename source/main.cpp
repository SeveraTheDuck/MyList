#include "ANA_List_struct.h"
#include "ANA_List_commands.h"
int main ()
{
    ANA_List list = {};

    ANA_List_Ctor (&list);

    ANA_List_Insert (1, 10, &list);
    ANA_List_Insert (2, 20, &list);
    ANA_List_Insert (2, 15, &list);
    ANA_List_Insert (1,  5, &list);

    ANA_List_Erase (3, &list);

    ANA_List_Dump  (&list);

    ANA_List_Dtor (&list);

    return 0;
}
