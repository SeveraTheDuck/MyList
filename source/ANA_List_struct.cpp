#include "ANA_List_struct.h"

const size_t BFG9000 = 9000;

__attribute__ ((constructor))
static void CleanLogFile ();

ANA_List_error_type
ANA_List_Ctor (ANA_List* const list)
{
    if (!list)
    {
        fprintf (stderr, "ANA_List_ERROR_LIST_NULLPTR\n");
        return ANA_List_ERROR_OCCURED;
    }

    ANA_List_error_type allocation_error =
                            ANA_List_CtorAllocate (list);
    if (allocation_error)
    {
        return ANA_List_ERROR_OCCURED;
    }

    ANA_List_CtorFill      (list);

    ANA_List_VerifyAndDump (list);

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_CtorAllocate (ANA_List* const list)
{
    list->list_capacity  = ANA_List_INIT_VOLUME;
    list->list_n_elems = 1;

    list->list_data = (ANA_List_data_type*) calloc (ANA_List_INIT_VOLUME,
                                            sizeof (ANA_List_data_type));
    if (!list->list_data)
    {
        fprintf (stderr, "list_data allocation error, "
                         "out of memory.\n");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_DATA_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->next = (int*) calloc (ANA_List_INIT_VOLUME,
                                sizeof (int));
    if (!list->next)
    {
        fprintf (stderr, "next allocation error, "
                         "out of memory.\n");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_NEXT_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->prev = (int*) calloc (ANA_List_INIT_VOLUME,
                                sizeof (int));
    if (!list->prev)
    {
        fprintf (stderr, "prev allocation error, "
                         "out of memory.\n");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_PREV_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    return ANA_List_NO_ERROR;
}

// can it be anything wrong?
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
        list->prev     [list_index] = ANA_List_NO_PREV_ELEMENT;
    }

    list->next [ ANA_List_DUMMY_ELEMENT ]   = 0;
    list->next [ ANA_List_INIT_VOLUME - 1 ] = 0;
    list->prev [ ANA_List_DUMMY_ELEMENT ]   = 0;

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
    assert (list);

    ANA_List_DtorPoison (list);
    ANA_List_DtorFree   (list);

    return ANA_List_NO_ERROR;
}

ANA_List_error_type
ANA_List_DtorPoison (ANA_List* const list)
{
    ANA_List_Assert (list);

    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = 0;
        list->prev     [list_index] = ANA_List_NO_PREV_ELEMENT;
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
    assert (list);

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
        list->head >= (long long) list->list_capacity)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_HEAD_OUT_OF_RANGE = 1;
    }

    if (list->tail < 0 ||
        list->tail >= (long long) list->list_capacity)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_HEAD_OUT_OF_RANGE = 1;
    }

    if (list->free < 0 ||
        list->free >= (long long) list->list_capacity)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_FREE_OUT_OF_RANGE = 1;
    }

    if (list->list_n_elems < 0 ||
        list->list_n_elems > list->list_capacity)
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
    FILE*   log_img = fopen (ANA_List_log_img_file_name, "a");
    assert (log_img);

    FILE*   log_txt = fopen (ANA_List_log_txt_file_name, "a");
    assert (log_txt);

    ANA_List_DumpHeader        (list, log_img, log_txt);

    ANA_List_DumpElems         (list, log_img, log_txt);

    ANA_List_DumpArrows        (list, log_img);

    ANA_List_DumpListPositions (list, log_img, log_txt);

    fclose (log_img);
    fclose (log_txt);

    if (list)
    {
        system ("dot -Tpng ANA_List_logs.dot -o logs/logs_image.png");
    }
}

void
ANA_List_DumpHeader (const ANA_List* const list,
                           FILE* const log_img,
                           FILE* const log_txt)
{
    fprintf (log_img, "digraph G\n{\n");

    fprintf (log_img, "    rankdir = LR;\n");
    fprintf (log_img, "    bgcolor = \"#808080\";\n");
    fprintf (log_img, "    graph [splines = ortho];\n");
    fprintf (log_img, "    node  [width = 2, style = filled, color = white];\n");
    fprintf (log_img, "    edge  [color = \"darkgreen\", "
                                 "fontcolor = \"blue\", "
                                 "fontsize = 15];\n\n");

    fprintf (log_img, "    subgraph cluster0\n    {\n");
    fprintf (log_img, "        bgcolor = \"#606060\";");
    fprintf (log_img, "        height  = 20");
    fprintf (log_img, "        style   = filled;\n");
    fprintf (log_img, "        label   = \"My list\";\n\n");

    // console
    fprintf (log_txt, "<pre>\nList [%p]\n", list);
    if (!list)
    {
        fprintf (log_txt, "No other info avalable.\n");
    }
}

void
ANA_List_DumpElems (const ANA_List* const list,
                           FILE* const log_img,
                           FILE* const log_txt)
{
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        if (list->prev [list_index] == ANA_List_NO_PREV_ELEMENT)
        {
            fprintf (log_img, "        %zd [shape = \"Mrecord\", "
                                           "label = \"index: %zd | "
                                           "value: POISON | next: %d | prev: %d"
                                           "\"];\n",
                                           list_index, list_index,
                                           list->next [list_index],
                                           list->prev [list_index]);
            continue;
        }

        fprintf (log_img, "        %zd [shape = \"Mrecord\", "
                                      "label = \"index: %zd | "
                                      "value: %d | next: %d | prev: %d"
                                      "\"];\n",
                                      list_index, list_index,
                                      list->list_data [list_index],
                                      list->next [list_index],
                                      list->prev [list_index]);
    }

    // invisible arrows
    fprintf (log_img, "        ");
    for (size_t list_index = 0;
                list_index < list->list_capacity - 1;
              ++list_index)
    {
        fprintf (log_img, "%zd -> ", list_index);
    }
    fprintf (log_img, "%zd ", list->list_capacity - 1);
    fprintf (log_img, "[weight = %zd, color = \"#606060\"];\n\n", BFG9000);

    // console
    fprintf (log_txt, "List data:\n    ");
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        fprintf (log_txt, "%10d ", list->list_data[list_index]);
    }
    fputc ('\n', log_txt);

    fprintf (log_txt, "Next:\n    ");
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        fprintf (log_txt, "%10d ", list->next[list_index]);
    }
    fputc ('\n', log_txt);

    fprintf (log_txt, "Prev:\n    ");
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        fprintf (log_txt, "%10d ", list->prev[list_index]);
    }
    fputc ('\n', log_txt);
}

void
ANA_List_DumpArrows (const ANA_List* const list,
                           FILE* const log_img)
{
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        if (list->prev [list_index] != ANA_List_NO_PREV_ELEMENT)
        {
            fprintf (log_img, "        %zd -> %d [color = \"#FF0000\","
                                                 "weight = %zd00];\n",
                     list_index, list->next [list_index], list_index);
        }
    }

    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        if (list->prev [list_index] == ANA_List_NO_PREV_ELEMENT)
        {
            fprintf (log_img, "        %zd -> %d [color = \"#00FF00\","
                                                 "weight = %zd00];\n",
                     list_index, list->next [list_index], list_index);
        }
    }

    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        if (list->prev [list_index] != ANA_List_NO_PREV_ELEMENT)
        {
            fprintf (log_img, "        %zd -> %d [color = \"#0000FF\","
                                                 "weight = %zd00];\n",
                     list_index, list->prev [list_index], list_index);
        }
    }
    fprintf (log_img, "    }\n");
}

void
ANA_List_DumpListPositions (const ANA_List* const list,
                           FILE* const log_img,
                           FILE* const log_txt)
{
    fprintf (log_img, "    TAIL [width = 1, shape = \"circle\"];\n");
    fprintf (log_img, "    HEAD [width = 1, shape = \"circle\"];\n");
    fprintf (log_img, "    FREE [width = 1, shape = \"circle\"];\n");

    fprintf (log_img, "    HEAD -> %d;\n", list->head);
    fprintf (log_img, "    TAIL -> %d;\n", list->tail);
    fprintf (log_img, "    FREE -> %d;\n", list->free);

    fprintf (log_img, "}");

    // console
    fprintf (log_txt, "\nList info:\n");
    fprintf (log_txt, "    Volume %zd\n    Number of elements %zd\n",
             list->list_capacity, list->list_n_elems);
    fprintf (log_txt, "    Head to %d\n    Tail to %d\n",
             list->head, list->tail);
    fprintf (log_txt, "    First free element %d\n\n</pre>",
             list->free);
}

__attribute__ ((constructor))
static void CleanLogFile ()
{
    FILE*   fp_img = fopen (ANA_List_log_img_file_name, "wb");
    fclose (fp_img);

    FILE*   fp_txt = fopen (ANA_List_log_txt_file_name, "wb");
    fclose (fp_txt);
}
