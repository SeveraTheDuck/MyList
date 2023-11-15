#include "ANA_List_struct.h"

static const size_t BFG9000 = 9000;
static const size_t ANA_List_IMAGE_MAX_NUMBER_LENGTH = 2;

__attribute__ ((constructor))
static FILE* OpenLogFile ();

static ANA_List_error_type
ANA_List_CtorAllocate   (ANA_List* const list);

static ANA_List_error_type
ANA_List_CtorFill       (ANA_List* const list);

static ANA_List_error_type
ANA_List_DtorPoison     (ANA_List* const list);

static ANA_List_error_type
ANA_List_DtorFree       (ANA_List* const list);

static ANA_List_error_type
ANA_List_CheckErrorOccurence (const ANA_List_errors* const     list_errors_field,
                              const ANA_List_errors* const ref_list_errors_field);

static char*
ANA_List_DumpImage (const ANA_List* const list,
                          FILE*     const log_img,
                          FILE*     const log_txt);

static void
ANA_List_DumpImageHeader (FILE* const log_img);

static void
ANA_List_DumpImageElems (const ANA_List* const list,
                               FILE*     const log_img);

static void
ANA_List_DumpImageArrows (const ANA_List* const list,
                                FILE*     const log_img);

static void
ANA_List_DumpImagePositions (const ANA_List* const list,
                                   FILE*     const log_img);

static void
ANA_List_DumpText (const ANA_List* const list,
                         FILE*     const log_txt);

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

static ANA_List_error_type
ANA_List_CtorAllocate (ANA_List* const list)
{
    list->list_capacity = ANA_List_INIT_VOLUME;
    list->list_n_elems  = 1;

    list->list_data = (ANA_List_data_type*) calloc (ANA_List_INIT_VOLUME,
                                            sizeof (ANA_List_data_type));
    if (!list->list_data)
    {
        perror ("list->list_data allocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_DATA_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->next = (uint32_t*) calloc (ANA_List_INIT_VOLUME,
                                     sizeof (uint32_t));
    if (!list->next)
    {
        perror ("list->next allocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_NEXT_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    list->prev = (int32_t*) calloc (ANA_List_INIT_VOLUME,
                                    sizeof (int32_t));
    if (!list->prev)
    {
        perror ("list->prev allocation error");
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_LIST_PREV_NULLPTR = 1;

        return ANA_List_ERROR_OCCURED;
    }

    return ANA_List_NO_ERROR;
}

static ANA_List_error_type
ANA_List_CtorFill (ANA_List* const list)
{
    ANA_List_Assert (list);

    for (size_t list_index = 0;
                list_index < ANA_List_INIT_VOLUME;
              ++list_index)
    {
        list->list_data[list_index] = ANA_List_POISON;
        list->next     [list_index] = (uint32_t) list_index + 1;
        list->prev     [list_index] = ANA_List_NO_PREV_ELEMENT;
    }

    list->next [ ANA_List_INIT_VOLUME - 1 ] = 0;
    list->next [ANA_List_DUMMY_ELEMENT] = 0;
    list->prev [ANA_List_DUMMY_ELEMENT] = 0;

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

static ANA_List_error_type
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

    list->next [ANA_List_DUMMY_ELEMENT] = 0;
    list->prev [ANA_List_DUMMY_ELEMENT] = 0;
    list->free = 0;

    memset (&list->list_errors_field, 0, sizeof(ANA_List_errors));

    return ANA_List_NO_ERROR;
}

static ANA_List_error_type
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

    if (list->next [ANA_List_DUMMY_ELEMENT] < 0 ||
        list->next [ANA_List_DUMMY_ELEMENT] >= (long long) list->list_capacity)
    {
        list->list_errors_field.errors_struct
            .ANA_List_ERROR_HEAD_OUT_OF_RANGE = 1;
    }

    if (list->prev [ANA_List_DUMMY_ELEMENT] < 0 ||
        list->prev [ANA_List_DUMMY_ELEMENT] >= (long long) list->list_capacity)
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

static ANA_List_error_type
ANA_List_CheckErrorOccurence (const ANA_List_errors* const     list_errors_field,
                              const ANA_List_errors* const ref_list_errors_field)
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
    FILE* const log_img = fopen (ANA_List_IMAGE_CONSTRUCT_FILE_NAME, "wb");
    assert (log_img);

    FILE* const log_txt = OpenLogFile ();

    char* const system_call_str = ANA_List_DumpImage (list, log_img, log_txt);

    fclose (log_img);

    system (system_call_str);
    free   (system_call_str);

    ANA_List_DumpText  (list, log_txt);

    fclose (log_txt);
}

static char*
ANA_List_DumpImage (const ANA_List* const list,
                          FILE*     const log_img,
                          FILE*     const log_txt)
{
    static uint8_t number_of_images = 0;

    if (!list) return nullptr;

    ++number_of_images;

    ANA_List_DumpImageHeader    (log_img);

    ANA_List_DumpImageElems     (list, log_img);

    ANA_List_DumpImageArrows    (list, log_img);

    ANA_List_DumpImagePositions (list, log_img);

    size_t system_call_length =
        strlen ("dot ") +
        strlen (ANA_List_IMAGE_CONSTRUCT_FILE_NAME) +
        strlen (" -Tpng -o ") +
        strlen (ANA_List_IMAGE_FILE_NAME) +
        ANA_List_IMAGE_MAX_NUMBER_LENGTH +
        strlen (".png");

    char* system_call = (char*)
        calloc (sizeof (char), system_call_length);

    snprintf (system_call, system_call_length,
             "dot %s -Tpng -o %s%d.png",
              ANA_List_IMAGE_CONSTRUCT_FILE_NAME,
              ANA_List_IMAGE_FILE_NAME,
              number_of_images);

    fprintf (log_txt, "<img src = \"../%s%d.png\" width = 80%%>",
             ANA_List_IMAGE_FILE_NAME, number_of_images);

    return system_call;
}

static void
ANA_List_DumpImageHeader (FILE* const log_img)
{
    fprintf (log_img, "digraph G\n{\n"
                      "    rankdir = LR;\n"
                      "    bgcolor = \"#808080\";\n"
                      "    graph [splines = ortho];\n"
                      "    node  [width = 2, style = filled, color = white];\n"
                      "    edge  [color = \"darkgreen\", "
                                 "fontcolor = \"blue\", "
                                 "fontsize = 15];\n\n"

                      "    subgraph cluster0\n    {\n"
                      "        bgcolor = \"#606060\";"
                      "        height  = 20"
                      "        style   = filled;\n"
                      "        label   = \"My list\";\n\n");
}

static void
ANA_List_DumpImageElems (const ANA_List* const list,
                               FILE*     const log_img)
{
    for (size_t list_index = 0;
                list_index < list->list_capacity;
              ++list_index)
    {
        if (list->list_data [list_index] == ANA_List_POISON)
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
}

static void
ANA_List_DumpImageArrows (const ANA_List* const list,
                                FILE*     const log_img)
{
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

static void
ANA_List_DumpImagePositions (const ANA_List* const list,
                                   FILE*     const log_img)
{
    fprintf (log_img, "    TAIL [width = 1, shape = \"circle\"];\n");
    fprintf (log_img, "    HEAD [width = 1, shape = \"circle\"];\n");
    fprintf (log_img, "    FREE [width = 1, shape = \"circle\"];\n");

    fprintf (log_img, "    HEAD -> %d;\n",
             list->next [ANA_List_DUMMY_ELEMENT]);
    fprintf (log_img, "    TAIL -> %d;\n",
             list->prev [ANA_List_DUMMY_ELEMENT]);
    fprintf (log_img, "    FREE -> %d;\n", list->free);

    fprintf (log_img, "}");
}

static void
ANA_List_DumpText (const ANA_List* const list,
                         FILE*     const log_txt)
{
    fprintf (log_txt, "<pre>\nList [%p]\n", list);
    if (!list)
    {
        fprintf (log_txt, "List is nullptr. No other info avalable.\n");
    }

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

    fprintf (log_txt, "\nList info:\n");
    fprintf (log_txt, "    Volume %zd\n    Number of elements %zd\n",
             list->list_capacity, list->list_n_elems);
    fprintf (log_txt, "    Head to %d\n    Tail to %d\n",
             list->next [ANA_List_DUMMY_ELEMENT],
             list->prev [ANA_List_DUMMY_ELEMENT]);
    fprintf (log_txt, "    First free element %d\n\n</pre>\n",
             list->free);
}

static FILE* OpenLogFile ()
{
    static bool is_opened = 0;
    FILE* fp_txt = nullptr;

    if (!is_opened)
    {
        fp_txt = fopen (ANA_List_HTML_LOGS_FILE_NAME, "wb");
        fprintf (fp_txt,
                 "<!DOCTYPE html>\n"
                 "<html lang=\"en\">\n"
                 "<head>\n"
                 "<meta charset=\"UTF-8\">\n");
        fprintf (fp_txt,
                 "<meta name=\"viewport\" "
                 "content=\"width=device-width, initial-scale=1.0\">\n"
                 "<title>Logs for my list</title>\n"
                 "</head>\n"
                 "<body>\n"
                 "<pre>\n");

        is_opened = 1;

        fclose (fp_txt);
    }

    fp_txt = fopen (ANA_List_HTML_LOGS_FILE_NAME, "a");

    return fp_txt;
}
