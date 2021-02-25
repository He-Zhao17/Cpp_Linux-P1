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
    return NULL;
}

void elist_destroy(struct elist *list)
{

}

int elist_set_capacity(struct elist *list, size_t capacity)
{
    return -1;
}

size_t elist_capacity(struct elist *list)
{
    return 0;
}

ssize_t elist_add(struct elist *list, void *item)
{
    return -1;
}

void *elist_add_new(struct elist *list)
{
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

