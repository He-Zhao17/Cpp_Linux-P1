#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "elist.h"

/**
* Default init size of the elist.
*/
#define DEFAULT_INIT_SZ 10

/**
* Times multiplied when extend the elist.
*/
#define RESIZE_MULTIPLIER 2

/**
* The declaration of elist.
*/
struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

 /**
 * @brief		To check whether the index is valid for the list.
 * @details	    To check whether the index is valid for the list.
 * @param[in]	list The list we want to use.
 * @param[in]	idx The index we want to use.
 * @return	    If the index is valid, return Ture, else return False.
 */
bool idx_is_valid(struct elist *list, size_t idx);

 /**
 * @brief		To create a elist.
 * @details	    Create a elist and return the pointer.
 * @param[in]	list_sz The capacity of the elist.
 * @param[in]	item_sz The size of the element of the elis.
 * @return	    The pointer of the elist.
 */
struct elist *elist_create(size_t list_sz, size_t item_sz)
{
    if (list_sz == 0) {
        list_sz = DEFAULT_INIT_SZ;
    }
    struct elist *res = (struct elist*) calloc (1, sizeof (struct elist));
    if (res == NULL) {
        return NULL;
    }
    res->capacity = list_sz;
    res->size = 0;
    res->item_sz = item_sz;
    res->element_storage = (void*) calloc(res->capacity, res->item_sz);
    if (res->element_storage == NULL) {
        free(res);
        return NULL;
    }
    return res;
}

 /**
 * @brief		Clear and destroy the elist.
 * @details	    Clear and destroy the elist.
 * @param[in]	list The elist that we want to destroy.
 * @return	    None.
 */
void elist_destroy(struct elist *list)
{
    free(list->element_storage);
    free(list);
}


 /**
 * @brief		To reset the capacity of elist.
 * @details	    To reset the capacity of elist. It can be extended or shortened.
 * @param[in]	list The elist we want to reset the capacity.
 * @param[in]	capacity The capacity we want to reset.
 * @return	    If success return 0, else return -1.
 */
int elist_set_capacity(struct elist *list, size_t capacity)
{
    if (capacity < 0) {
        return -1;
    }
    list->element_storage = (void*) realloc (list->element_storage, capacity * list->item_sz);
    if (capacity < list->capacity) {
        list->capacity = capacity;
        list->size = capacity;
        return 0;
    } else {
        list->capacity = capacity;
        return 0;
    }
}

 /**
 * @brief		To get the capacity of the elist.
 * @details	    To get the capacity of the elist.
 * @param[in]	list The elist whose capacity we want to get.
 * @return	    If success return the capacity, else return -1.
 */
size_t elist_capacity(struct elist *list)
{
    if (list == NULL) {
        return -1;
    } else {
        return list->capacity;
    }
}

/**
* @brief		Add the element into the elist.
* @details	    Add the element into the elist.
* @param[in]	list The elist we want to add into.
* @param[in]	item The element we want to add.
* @return	    If success return 0, else return -1.
*/
ssize_t elist_add(struct elist *list, void *item)
{
    if (list == NULL) {
        return -1;
    } else {
        if (list->capacity == list->size) {
            elist_set_capacity(list, DEFAULT_INIT_SZ * list->capacity);
        }
        memcpy(list->element_storage + list->size * list->item_sz, item, list->item_sz);
        list->size++;
        return 0;
    }
}

/**
* @brief		To add and get the pointer of the new element.
* @details	    To add and get the pointer of the new element.
* @param[in]	list The elist we want to add into.
* @return	    The pointer of the new element.
*/
void *elist_add_new(struct elist *list)
{
    if (list->size == list->capacity) {
        elist_set_capacity(list, RESIZE_MULTIPLIER * list->capacity);
    }
    list->size++;
    return list->element_storage + (list->size - 1) * list->item_sz;
}

/**
* @brief		To reset a element of the elist.
* @details	    To reset a element of the elist with index.
* @param[in]	list The elist we want to modify.
* @param[in]	idx The index of the element we want to modify.
* @param[in]    item The new content for the element.
* @return	    If success return 0, else return -1.
*/
int elist_set(struct elist *list, size_t idx, void *item)
{
    if (list == NULL) {
        return -1;
    } else {
        if (!idx_is_valid(list, idx)) {
            return -1;
        } else {
            memcpy(list->element_storage + list->item_sz * idx, item, list->item_sz);
            return 0;
        }
    }
}

/**
* @brief		To get a element of the elist.
* @details	    To get a element of the elist. with index.
* @param[in]	list The elist we want to get from.
* @param[in]	idx The index of the element we want to get.
* @return	    If success return the pointer of the element, else return NULL.
*/
void *elist_get(struct elist *list, size_t idx)
{
    if (list == NULL) {
        return NULL;
    } else {
        if (!idx_is_valid(list, idx)) {
            return NULL;
        } else {
            void* res = (char*) list->element_storage + list->item_sz * idx;
            return res;
        }
    }
}

/**
* @brief		To get the actual size of the elist.
* @details	    To get the actual size of the elist.
* @param[in]	list The elist whose actual size we want to get.
* @return	    If success return the actual size of the elist, else return -1.
*/
size_t elist_size(struct elist *list)
{
    if (list == NULL) {
        return -1;
    } else {
        return list->size;
    }
}

/**
* @brief		To remove a element.
* @details	    To remove a element with index.
* @param[in]	list The elist whose actual size we want to remove from.
* @param[in]    idx The index of the element we want to remove from the elist.
* @return	    If success return 0, else return -1.
*/
int elist_remove(struct elist *list, size_t idx)
{
    if (list == NULL) {
        return -1;
    } else {
        if (list->size == 0) {
            return -1;
        }
        if (!idx_is_valid(list, idx)) {
            return -1;
        } else {
            memmove(list->element_storage + list->item_sz * idx,
                    list->element_storage + list->item_sz * (idx + 1),
                    (list->size - idx) * list->item_sz);
            list->size--;
            return 0;
        }
    }

    return -1;
}

/**
* @brief		To clear the elist.
* @details	    To clear the elist.
* @param[in]	list The elist whose actual size we want to clear.
* @return	    None.
*/
void elist_clear(struct elist *list)
{
    if (list == NULL) {
        return;
    } else {
        if (list->size == 0) {
            return;
        } else {
            list->size = 0;
            return;
        }
    }


}

/**
* @brief		To clear the elist and clear the memory.
* @details	    To clear the elist and clear the memory.
* @param[in]	list The elist whose actual size we want to clear.
* @return	    None.
*/
void elist_clear_mem(struct elist *list)
{
    if (list == NULL) {
        return;
    } else {
        if (list->size == 0) {
            list->element_storage = (void*) calloc(list->capacity, list->item_sz);
            return;
        } else {
            list->size = 0;
            list->element_storage = (void*) calloc(list->capacity, list->item_sz);
            return;
        }
    }

}

/**
* @brief		To get the index with the content of the element.
* @details	    To get the index with the content of the element. If there are 2 elements having this content, choose the first. etc.
* @param[in]	list The elist whose actual size we want to find from.
* @param[in]    item The content of the element.
* @return	    If success return the index of the element, else return -1.
*/
ssize_t elist_index_of(struct elist *list, void *item)
{
    if (list == NULL) {
        return -1;
    } else {
        if (list->size == 0) {
            return -1;
        } else {
            for (int i = 0; i < list->size; i++) {
                int temp = memcmp(list->element_storage + i * list->item_sz, item, list->item_sz);
                if (temp == 0) {
                    return i;
                }
            }
            return -1;

        }

    }

    return -1;
}

/**
* @brief		To sort the elist.
* @details	    To sort the elist with the given comparator.
* @param[in]	list The elist we want to sort.
* @param[in]    comparator The comparator function we use in sorting.
* @return	    None.
*/
void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{

    if (list == NULL) {
        return;
    } else {
        /*void* temp = (void*) calloc(list->size, list->item_sz);
        memcpy(temp, list->element_storage, list->size * list->item_sz);
        qsort(temp, list->size, list->item_sz, (*comparator));
        memcpy(list->element_storage, temp, list->size * list->item_sz);
        free(temp);*/

        qsort(list->element_storage, list->size, list->item_sz, comparator);
        return;
    }

}


 /**
 * @brief		To check whether the index is valid for the list.
 * @details	    To check whether the index is valid for the list.
 * @param[in]	list The list we want to use.
 * @param[out]	idx The index we want to use.
 * @return	    If the index is valid, return Ture, else return False.
 */
bool idx_is_valid(struct elist *list, size_t idx)
{
    if (list == NULL) {
        return false;
    } else {
        if (idx > list->size - 1) {
            return false;
        } else {
            return true;
        }
    }
}
