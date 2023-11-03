#include "ANA_List_struct.h"

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
//union
    if (memcmp (list_errors_field,
            ref_list_errors_field,
            sizeof (ANA_List_errors)))
    {
        return ANA_List_ERROR_OCCURED;
    }

    return ANA_List_NO_ERROR;
}

void
ANA_List_Dump (ANA_List* list)
{
    fprintf (stderr, "List [%p]\n", list);
    if (!list)
    {
        fprintf (stderr, "No other info avalable.\n");
    }

    fprintf (stderr, "List info:\n");
    fprintf (stderr, "Volume %zd\nNumber of elements %zd\n",
             list->list_volume, list->list_n_elems);
    fprintf (stderr, "Head to %d\nTail to %d\n",
             list->head, list->tail);
    fprintf (stderr, "First free element %d\n",
             list->free);

    fprintf (stderr, "List filling:\nData:\n");
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
