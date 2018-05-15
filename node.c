#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>

#include "node.h"
#include "list.h"

node *initialize_node(uint16_t userID) {
    node *n = malloc(sizeof(node));
    if (!n)return n;
    n->id = userID;
    n->film_pref = NULL;
    n->child_start = NULL;
    n->child_end = NULL;
    n->next = NULL;
    n->prev = NULL;
    n->parent = NULL;
    return n;
}

node *initialize_root() {
    return initialize_node(0);
}

void primitive_delUser(node *user) {
    delete_list(&(user->film_pref));
    free(user);
}

int addUser(node **users, uint16_t parentUserID, uint16_t userID) {
    if (users[userID] != NULL || users[parentUserID] == NULL) {
        fprintf(stderr, "ERROR\n");
        return 0;
    }

    node *new = initialize_node(userID);
    if (new == NULL)return 1;
    node *parent = users[parentUserID];
    new->parent = parent;

    if (parent->child_start == NULL) {
        parent->child_start = new;
        parent->child_end = new;
        users[userID] = new;
        printf("OK\n");
        return 0;
    }

    new->next = parent->child_start;
    parent->child_start->prev = new;
    parent->child_start = new;
    users[userID] = new;
    printf("OK\n");
    return 0;
}


void delUser(node **users, uint16_t userID) {
    if (users[userID] == NULL || userID == 0) {
        fprintf(stderr, "ERROR\n");
        return;
    }

    node *tmp = users[userID];
    list *films = tmp->film_pref;
    delete_list(&films);

    if (tmp->prev == NULL && tmp->next != NULL) {
        if (tmp->child_start != NULL) {
            tmp->child_start->parent = tmp->parent;
            tmp->child_end->next = tmp->next;
            tmp->next->prev = tmp->child_end;
            tmp->parent->child_start = tmp->child_start;
        } else {
            tmp->next->parent = tmp->parent;
            tmp->parent->child_start = tmp->next;
            tmp->next->prev = NULL;
        }
    } else if (tmp->prev == NULL && tmp->next == NULL) {
        if (tmp->child_start != NULL) {
            tmp->child_start->parent = tmp->parent;
            tmp->child_end->parent = tmp->parent;
            tmp->parent->child_start = tmp->child_start;
            tmp->parent->child_end = tmp->child_end;
        } else {
            tmp->parent->child_start = NULL;
            tmp->parent->child_end = NULL;
        }
    } else if (tmp->prev != NULL && tmp->next == NULL) {
        if (tmp->child_start != NULL) {
            tmp->child_end->parent = tmp->parent;
            tmp->child_start->prev = tmp->prev;
            tmp->prev->next = tmp->child_start;
            tmp->parent->child_end = tmp->child_end;
        } else {
            tmp->prev->parent = tmp->parent;
            tmp->parent->child_end = tmp->prev;
            tmp->prev->next = NULL;
        }
    } else {
        if (tmp->child_start != NULL) {
            tmp->prev->next = tmp->child_start;
            tmp->child_start->prev = tmp->prev;
            tmp->child_end->next = tmp->next;
            tmp->next->prev = tmp->child_end;
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }
    }
    free(tmp);
    users[userID] = NULL;
    printf("OK\n");
}

void delete_tree(node **users) {
    for (int i = 0; i < UINT16_MAX + 1; i++) {
        if (users[i])primitive_delUser(users[i]);
    }
}


int addMovie(node **users, uint16_t userID, uint32_t movieRating) {
    if (users[userID] == NULL) {
        fprintf(stderr, "ERROR\n");
        return 0;
    }
    list *l = users[userID]->film_pref;
    if (l == NULL) {
        users[userID]->film_pref = initialize_list(movieRating);
        if (users[userID]->film_pref == NULL)return 1;
        printf("OK\n");
        return 0;
    }
    list *new = initialize_list(movieRating);
    if (new == NULL)return 1;

    if (movieRating > l->film_id) {
        new->next = l;
        l->prev = new;
        l = new;
        users[userID]->film_pref = l;
        printf("OK\n");
        return 0;
    }

    list *tmp, *prv;
    tmp = l;
    prv = l;
    while (tmp->next != NULL && tmp->film_id > movieRating) {
        prv = tmp;
        tmp = tmp->next;
    }
    if (tmp->film_id == movieRating) {
        fprintf(stderr, "ERROR\n");
        return 0;
    }
    if (tmp->next == NULL && tmp->film_id > movieRating) {
        tmp->next = new;
        new->prev = tmp;
        new->next = NULL;
        printf("OK\n");
        return 0;
    }
    new->next = tmp;
    new->prev = prv;
    prv->next = new;
    tmp->prev = new;

    printf("OK\n");
    return 0;

}


void delMovie(node **users, uint16_t userID, uint32_t movieRating) {
    if (users[userID] == NULL) {
        fprintf(stderr, "ERROR\n");
        return;
    }
    if (remove_element(&(users[userID]->film_pref), movieRating) == 1) {

        fprintf(stderr, "ERROR\n");
        return;
    }
    printf("OK\n");
}


list *merge_and_trim(list **l1, list **l2, int k, bool *close) {
    list *result_start = malloc(sizeof(list));
    if (!result_start) {
        *close = true;
        return NULL;
    }
    list *result_end = result_start;
    result_start->next = NULL;
    result_start->prev = NULL;
    while (*l1 && *l2) {
        if ((*l1)->film_id == (*l2)->film_id) {
            result_end->next = *l1;
            (*l1)->prev = result_end;
            *l1 = (*l1)->next;
            result_end = result_end->next;
            result_end->next = NULL;
            if (*l1)(*l1)->prev = NULL;
            list *tmp = (*l2)->next;
            free(*l2);
            (*l2) = tmp;
            if (*l2)(*l2)->prev = NULL;
        } else if ((*l1)->film_id > (*l2)->film_id) {
            result_end->next = *l1;
            (*l1)->prev = result_end;
            *l1 = (*l1)->next;
            result_end = result_end->next;
            result_end->next = NULL;
            if (*l1)(*l1)->prev = NULL;
        } else {
            result_end->next = *l2;
            (*l2)->prev = result_end;
            *l2 = (*l2)->next;
            result_end = result_end->next;
            result_end->next = NULL;
            if (*l2)(*l2)->prev = NULL;
        }
    }
    if (*l1) {
        result_end->next = *l1;
        (*l1)->prev = result_end;
        while (result_end->next != NULL)result_end = result_end->next;
    }
    if (*l2) {
        result_end->next = *l2;
        (*l2)->prev = result_end;
        while (result_end->next != NULL)result_end = result_end->next;

    }
    list *iter = result_start->next;
    int counter = 0;
    while (iter != NULL) {
        if (counter == k) {
            iter->prev->next = NULL;
            list *temp = iter->next;
            while (iter) {
                free(iter);
                iter = temp;
                if (iter)temp = iter->next;
            }
        } else {
            iter = iter->next;
            counter++;
        }

    }

    list *pom = result_start->next;
    free(result_start);
    if (pom) pom->prev = NULL;
    *l1 = NULL;
    *l2 = NULL;
    return pom;
}

uint32_t _max(uint32_t a, uint32_t b) {
    if (a >= b)return a;
    else return b;
}

list *marathon(node **users, uint16_t userID, uint32_t k, uint32_t max, bool *close) {
    if (k == 0)return NULL;
    list *start = NULL, *end = NULL;
    uint32_t new_max = 0;
    if (users[userID]->film_pref != NULL) {
        list *films = users[userID]->film_pref;
        new_max = films->film_id;
        uint32_t count = 0;
        while (films != NULL && count < k && films->film_id >= max) {
            if (start == NULL) {
                start = initialize_list(films->film_id);
                if (start == NULL) {
                    *close = true;
                    return NULL;
                }
                end = start;
            } else {
                list *new = initialize_list(films->film_id);
                if (new == NULL) {
                    *close = true;
                    return NULL;
                }
                new->prev = end;
                end->next = new;
                end = new;
            }
            count++;
            films = films->next;
        }
    }
    node *iter = users[userID]->child_start;
    while (iter != NULL) {
        list *result = marathon(users, iter->id, k, _max(new_max, max), close);
        start = merge_and_trim(&start, &result, k, close);
        if (!close) {
            return NULL;
        }
        iter = iter->next;
    }

    return start;

}

void print_marathon(list *l) {
    list *ll = l;
    if (l == NULL) {
        printf("NONE\n");
    } else {
        while (l->next != NULL) {
            printf("%"PRIu32" ", l->film_id);
            l = l->next;
        }
        printf("%"PRIu32"\n", l->film_id);
        delete_list(&ll);
    }
}
