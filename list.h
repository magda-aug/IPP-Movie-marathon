#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct list {
    uint32_t film_id;
    struct list *next;
    struct list *prev;
} list;

#include "node.h"

extern list *initialize_list(uint32_t movieRating);

extern void delete_list(list **l);

extern int remove_element(list **l, uint32_t movieRating);

#endif /* LIST_H */
