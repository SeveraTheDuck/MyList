#include "ANA_List_struct.h"

const size_t BFG9000 = 9000;

__attribute__ ((constructor))
static void CleanLogFile ();

__attribute__ ((format(printf, 1, 2)))
static void PrintLogs (const char* const fmt, ...);

ANA_List_error_type
ANA_List_Ctor (ANA_List* const list)
{
    if (!list)
    {
        fprintf (stderr, "ANA_List_ERROR_LIST_NULLPTR\n");
        return ANA_List_ERROR_OCCURED;
    }

    ANA_List_CtorAllocate  (list);
    ANA_List_CtorFill      (list);

    ANA_List_VerifyAndDump (list);

    return ANA_List_NO_ERROR;
}

// maybe return different errors during allocation?
ANA_List_error_type
ANA_List_CtorAllocate (ANA_List* const list)
{
    list->list_volume  = ANA_List_INIT_VOLUME;
    list->list_n_elems = 1;

    list->list_data = (ANA_List_data_type*) calloc (ANA_List_INIT_VOLUME,
                                            sizeof (ANA_List_data_type));

    list->next      = (int*)                calloc (ANA_List_INIT_VOLUME,
                                            sizeof (int));

    list->prev      = (int*)                calloc (ANA_List_INIT_VOLUME,
                                            sizeof (int));

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_CtorFill (ANA_List* const list)
{
    ANA_List_Assert (list);

    for (size_t list_index = 0;
                list_index < ANA_List_INIT_VOLUME;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = (int) list_index + 1;
        list->prev     [list_index] = -1;
    }

    list->next[0]                        = 0;
    list->next[ANA_List_INIT_VOLUME - 1] = 0;
    list->prev[0]                        = 0;
    list->prev[1]                        = 0;

    list->head = 1;
    list->tail = 0;
    list->free = 1;

    memset (&list->list_errors_field,     0, sizeof(ANA_List_errors));
    memset (&list->ref_list_errors_field, 0, sizeof(ANA_List_errors));

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_Dtor (ANA_List* const list)
{
    ANA_List_Assert (list);

    ANA_List_DtorPoison (list);
    ANA_List_DtorFree   (list);

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_DtorPoison (ANA_List* const list)
{
    ANA_List_Assert (list);

    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = 0;
        list->prev     [list_index] = -1;
    }

    list->head = 0;
    list->tail = 0;
    list->free = 0;

    memset (&list->list_errors_field, 0, sizeof(ANA_List_errors));

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_DtorFree (ANA_List* const list)
{
    ANA_List_Assert (list);

    free (list->list_data);
    free (list->next);
    free (list->prev);

    list->list_data = nullptr;
    list->next      = nullptr;
    list->prev      = nullptr;

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_Verify (ANA_List* const list)
{
    if (!list)
    {
        return ANA_List_ERROR_OCCURED;
    }

    if (!list->list_data)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_DATA_NULLPTR = 1;
    }

    if (!list->next)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_NEXT_NULLPTR = 1;
    }

    if (!list->prev)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_PREV_NULLPTR = 1;
    }

    if (list->head < 0 ||
        list->head >= (long long) list->list_volume)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_HEAD_OUT_OF_RANGE = 1;
    }

    if (list->tail < 0 ||
        list->tail >= (long long) list->list_volume)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_HEAD_OUT_OF_RANGE = 1;
    }

    if (list->free < 0 ||
        list->free >= (long long) list->list_volume)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_FREE_OUT_OF_RANGE = 1;
    }

    if (list->list_n_elems < 0 ||
        list->list_n_elems > list->list_volume)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_NUMBER_OF_ELEMENTS_OUT_OF_RANGE = 1;
    }

    return ANA_List_CheckErrorOccurence (&list->list_errors_field,
                                         &list->ref_list_errors_field);
}

ANA_List_error_type
ANA_List_CheckErrorOccurence (ANA_List_errors*     list_errors_field,
                              ANA_List_errors* ref_list_errors_field)
{
    if (memcmp (list_errors_field,
            ref_list_errors_field,
            sizeof (ANA_List_errors)))
    {
        return ANA_List_ERROR_OCCURED;
    }

    return ANA_List_NO_ERROR;
}

void
ANA_List_Dump (const ANA_List* list)
{
    ANA_List_DumpHeader        (list);

    ANA_List_DumpElems         (list);

    ANA_List_DumpArrows        (list);

    ANA_List_DumpListPositions (list);

    system ("dot ANA_List_logs.dot -T png -o logs.png");
}

void
ANA_List_DumpHeader (const ANA_List* const list)
{
    PrintLogs ("digraph G\n{\n");

    PrintLogs ("    rankdir = LR;\n");
    PrintLogs ("    bgcolor = \"#80FFFF\"\n");
    PrintLogs ("    node [width = 2, style = filled, color = white];\n");
    PrintLogs ("    edge [color = \"darkgreen\", "
                         "fontcolor = \"blue\", "
                         "fontsize = 12];\n\n");

    PrintLogs ("    subgraph cluster0\n    {\n");
    PrintLogs ("        style = filled;\n");
    PrintLogs ("        label = \"My list\";\n\n");

    // console
    fprintf (stderr, "List [%p]\n", list);
    if (!list)
    {
        fprintf (stderr, "No other info avalable.\n");
    }
}

void
ANA_List_DumpElems (const ANA_List* const list)
{
    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        PrintLogs ("        %zd [shape = \"Mrecord\", "
                                "label = \"index: %zd | "
                                "value: %d | next: %d | prev: %d"
                                "\"];\n",
                                list_index, list_index,
                                list->list_data [list_index],
                                list->next [list_index],
                                list->prev [list_index]);
    }

    // invisible arrows
    PrintLogs ("        ");
    for (size_t list_index = 0;
                list_index < list->list_volume - 1;
              ++list_index)
    {
        PrintLogs ("%zd -> ", list_index);
    }
    PrintLogs ("%zd ", list->list_volume - 1);
    PrintLogs ("[weight = %zd, color = \"#80FFFF\"];\n\n", BFG9000);

    // console
    fprintf (stderr, "List data:\n");
    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        fprintf (stderr, "%10d ", list->list_data[list_index]);
    }
    fputc ('\n', stderr);

    fprintf (stderr, "Next:\n");
    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        fprintf (stderr, "%10d ", list->next[list_index]);
    }
    fputc ('\n', stderr);

    fprintf (stderr, "Prev:\n");
    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        fprintf (stderr, "%10d ", list->prev[list_index]);
    }
    fputc ('\n', stderr);
}

void
ANA_List_DumpArrows (const ANA_List* const list)
{
    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        if (list->prev [list_index] != -1)
        {
            PrintLogs ("        %zd -> %d [color = \"#FF0000\"]\n",
                       list_index, list->next [list_index]);
        }
    }

    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        if (list->prev [list_index] == -1)
        {
            PrintLogs ("        %zd -> %d [color = \"#00FF00\"]\n",
                       list_index, list->next [list_index]);
        }
    }

    for (size_t list_index = 0;
                list_index < list->list_volume;
              ++list_index)
    {
        if (list->prev [list_index] != -1)
        {
            PrintLogs ("        %zd -> %d [color = \"#0000FF\"]\n",
                       list_index, list->prev [list_index]);
        }
    }

    PrintLogs ("    }\n");
}

void
ANA_List_DumpListPositions (const ANA_List* const list)
{
    PrintLogs ("    HEAD [width = 1, shape = \"Mrecord\"]");
    PrintLogs ("    TAIL [width = 1, shape = \"Mrecord\"]");
    PrintLogs ("    FREE [width = 1, shape = \"Mrecord\"]");

    PrintLogs ("    HEAD -> %d", list->head);
    PrintLogs ("    TAIL -> %d", list->tail);
    PrintLogs ("    FREE -> %d", list->free);

    PrintLogs ("}");

    // console
    fprintf (stderr, "List info:\n");
    fprintf (stderr, "Volume %zd\nNumber of elements %zd\n",
             list->list_volume, list->list_n_elems);
    fprintf (stderr, "Head to %d\nTail to %d\n",
             list->head, list->tail);
    fprintf (stderr, "First free element %d\n",
             list->free);
}

__attribute__ ((constructor))
static void CleanLogFile ()
{
    FILE*   fp = fopen (ANA_List_log_file_name, "wb");
    fclose (fp);
}

__attribute__ ((format(printf, 1, 2)))
static void PrintLogs (const char* const fmt, ...)
{
    assert (fmt);

    FILE* fp = fopen (ANA_List_log_file_name, "a");

    va_list args = {};

    va_start (args, fmt);

    vfprintf (fp, fmt, args);

    va_end   (args);
    fclose   (fp);
    fp = nullptr;
}
