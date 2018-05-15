#ifndef NODE_H
#define NODE_H

#include "list.h"

#include <stdint.h>

typedef struct node {
    uint16_t id;
    list *film_pref;
    struct node *child_start;
    struct node *child_end;
    struct node *next;
    struct node *prev;
    struct node *parent;
} node;

extern node *initialize_node(uint16_t userID);

extern node *initialize_root();

extern int addUser(node **users, uint16_t parentUserID, uint16_t userID);

extern void delUser(node **users, uint16_t userID);

extern void delete_tree(node **users);

extern int addMovie(node **users, uint16_t userID, uint32_t movieRating);

extern void delMovie(node **users, uint16_t userID, uint32_t movieRating);

extern list *marathon(node **users, uint16_t userID, uint32_t k, uint32_t max, bool *close);

extern void print_marathon(list *l);

#endif /* NODE_H */
