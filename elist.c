#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "elist.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

bool idx_is_valid(struct elist *list, size_t idx);

struct elist *elist_create(size_t list_sz, size_t item_sz)
{
    struct elist *res = NULL;
    res->capacity = list_sz;
    res->size = 0;
    res->item_sz = item_sz;
    res->element_storage = (void*) calloc(res->capacity, res->item_sz);
    if (res->element_storage ==NULL) {
        return NULL;
    }
    return res;
}

void elist_destroy(struct elist *list)
{
    free(list->element_storage);
    free(list);
}

int elist_set_capacity(struct elist *list, size_t capacity)
{
    if (capacity < list->size) {
        return -1;
    } else {
        list->capacity = capacity;
        list->element_storage = (void*) realloc (list->element_storage, list->capacity * list->item_sz);
        if (list->element_storage == NULL) {
            return -1;
        }
        return 0;
    }
}

size_t elist_capacity(struct elist *list)
{
    if (list == NULL) {
        return -1;
    } else {
        return list->capacity;
    }
}

ssize_t elist_add(struct elist *list, void *item)
{
    if (list == NULL) {
        return -1;
    } else {
        if (list->capacity == list->size) {
            elist_set_capacity(list, 2 * list->capacity);
        }
        memcpy(list->element_storage + list->size * list->item_sz, item, list->item_sz);
        list->size++;
        return 0;
    }
}

void *elist_add_new(struct elist *list)
{
    if (list == NULL) {
        return -1;
    } else {
        if (list->size == list->capacity) {
            elist_set_capacity(list, 2 * list->capacity);
        }
        void* res = (char*) list->element_storage + list->size * list->item_sz;
        return res;
    }
    return NULL;
}

int elist_set(struct elist *list, size_t idx, void *item)
{
    return -1;
}

void *elist_get(struct elist *list, size_t idx)
{
    return NULL;
}

size_t elist_size(struct elist *list)
{
    return 0;
}

int elist_remove(struct elist *list, size_t idx)
{
    return -1;
}

void elist_clear(struct elist *list)
{

}

void elist_clear_mem(struct elist *list)
{

}

ssize_t elist_index_of(struct elist *list, void *item)
{
    return -1;
}

void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{

}

bool idx_is_valid(struct elist *list, size_t idx)
{
    return false;
}

