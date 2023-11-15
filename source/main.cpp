#include "ANA_List_struct.h"
#include "ANA_List_commands.h"

int32_t main ()
{
    ANA_List list = {};

    ANA_List_Ctor (&list);

    ANA_List_Dump  (&list);

    ANA_List_PushBack  (10, &list);
    ANA_List_PushBack  (20, &list);
    ANA_List_Insert    (1, 15, &list);
    ANA_List_PushFront (5, &list);

    ANA_List_Erase (2, &list);

    ANA_List_Dump  (&list);

    ANA_List_ReallocDown (5, &list);

    ANA_List_Dump  (&list);

    ANA_List_Dtor (&list);

    return 0;
}
