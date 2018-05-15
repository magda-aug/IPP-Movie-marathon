#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "list.h"

list *initialize_list(uint32_t movieRating) {
    list *l = malloc(sizeof(list));
    if (!l)return l;
    l->film_id = movieRating;
    l->next = NULL;
    l->prev = NULL;
    return l;
}

void delete_list(list **l) {
    list *tmp;
    while ((*l) != NULL) {
        tmp = (*l)->next;
        free(*l);
        (*l) = tmp;
    }
    *l = NULL;
}

int remove_element(list **l, uint32_t movieRating) {
    if ((*l) == NULL)return 1;
    list *helper = *l;
    if (helper->film_id == movieRating) {
        *l = (*l)->next;
        free(helper);
        return 0;
    }
    helper = helper->next;
    while (helper != NULL) {
        if (helper->film_id == movieRating) {
            helper->prev->next = helper->next;
            if (helper->next)helper->next->prev = helper->prev;
            free(helper);
            return 0;
        }
        helper = helper->next;
    }
    return 1;
}