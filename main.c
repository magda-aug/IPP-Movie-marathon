#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "list.h"
#include "node.h"

node *users[UINT16_MAX + 1];

void close_program() {
    delete_tree(users);
    exit(1);
}

void dump() {
    char c;
    do {
        c = (char) getchar();
    } while (c != '\n' && c != EOF);
}

void error() {
    dump();
    fprintf(stderr, "ERROR\n");
}

//READING NUMBERS
uint32_t get_uint32(bool *is_error) {
    char digits[11];
    int i = 0;
    char c = (char) getchar();
    while (i < 10) {
        if (c >= '0' && c <= '9') {
            digits[i] = c;
            i++;
            c = (char) getchar();
        } else break;
    }
    if (c == '\n' || c == ' ') {
        digits[i] = 0;
        ungetc(c, stdin);
        uint64_t result = (uint64_t) atoll(digits);
        if (result <= INT32_MAX) {
            return result;
        }
        *is_error = true;
        return (uint32_t) -1;
    } else {
        *is_error = true;
        return (uint32_t) -1;
    }
}

uint16_t get_uint16(bool *is_error) {
    char digits[6];
    int i = 0;
    char c = (char) getchar();
    while (i < 5) {
        if (c >= '0' && c <= '9') {
            digits[i] = c;
            i++;
            c = (char) getchar();
        } else break;
    }
    if (c == '\n' || c == ' ') {
        digits[i] = 0;
        ungetc(c, stdin);
        uint64_t result = (uint64_t) atoll(digits);
        if (result <= UINT16_MAX) {
            return result;
        }
        *is_error = true;
        return (uint16_t) -1;
    } else {
        *is_error = true;
        return (uint16_t) -1;
    }
}


void parse() {
    char c = (char) getchar();
    if (c == '#') {
        dump();
        return;
    }
    if (c == '\n') {
        return;
    }
    char command[9];
    int i = 0;
    while (i < 7 && c != '\n') {
        command[i] = c;
        i++;
        c = (char) getchar();
    }
    ungetc(c, stdin);
    if (i != 7) {
        error();
        return;
    }

    command[7] = 0;


    //DELUSER
    if (strcmp(command, "delUser") == 0) {
        if ((c = (char) getchar()) == ' ') {
            bool is_error = false;
            uint16_t userID = get_uint16(&is_error);
            if (is_error == false) {
                if ((c = (char) getchar()) == '\n') {
                    delUser(users, userID);
                } else error();
            } else error();
        } else {
            ungetc(c, stdin);
            error();
        }
        return;
    }

    //ADDUSER
    if (strcmp(command, "addUser") == 0) {
        if ((c = (char) getchar()) == ' ') {
            bool is_error = false;
            uint16_t parentUserID = get_uint16(&is_error);
            if (!is_error) {
                if ((c = (char) getchar()) == ' ') {
                    uint16_t userID = get_uint16(&is_error);
                    if (!is_error) {
                        if ((c = (char) getchar()) == '\n') {
                            if (addUser(users, parentUserID, userID) == 1)close_program();
                        } else error();
                    } else error();
                } else {
                    ungetc(c, stdin);
                    error();
                }
            } else error();
        } else {
            ungetc(c, stdin);
            error();
        }
        return;
    }

    //CHECK FOR NEWLINE
    if ((c = (char) getchar()) == '\n') {
        fprintf(stderr, "ERROR\n");
        return;
    }

    //CHECK FOR LONGER COMMANDS
    command[7] = c;
    command[8] = 0;

    //DELMOVIE
    if (strcmp(command, "delMovie") == 0) {
        if ((c = (char) getchar()) == ' ') {
            bool is_error = false;
            uint16_t userID = get_uint16(&is_error);
            if (!is_error) {
                if ((c = (char) getchar()) == ' ') {
                    uint32_t movieRating = get_uint32(&is_error);
                    if (!is_error) {
                        if ((c = (char) getchar()) == '\n') {
                            delMovie(users, userID, movieRating);
                        } else error();
                    } else error();
                } else {
                    ungetc(c, stdin);
                    error();
                }
            } else error();
        } else {
            ungetc(c, stdin);
            error();
        }
        return;
    }

    //ADDMOVIE
    if (strcmp(command, "addMovie") == 0) {
        if ((c = (char) getchar()) == ' ') {
            bool is_error = false;
            uint16_t userID = get_uint16(&is_error);
            if (!is_error) {
                if ((c = (char) getchar()) == ' ') {
                    uint32_t movieRating = get_uint32(&is_error);
                    if (!is_error) {
                        if ((c = (char) getchar()) == '\n') {
                            if (addMovie(users, userID, movieRating) == 1)close_program();
                        } else error();
                    } else error();
                } else {
                    ungetc(c, stdin);
                    error();
                }
            } else error();
        } else {
            ungetc(c, stdin);
            error();
        }
        return;
    }

    //  MARATHON
    if (strcmp(command, "marathon") == 0) {
        if ((c = (char) getchar()) == ' ') {
            bool is_error = false;
            uint16_t userID = get_uint16(&is_error);
            if (!is_error) {
                if ((c = (char) getchar()) == ' ') {
                    uint32_t k = get_uint32(&is_error);
                    if (!is_error) {
                        if ((c = (char) getchar()) == '\n') {
                            if (users[userID] == NULL) {
                                fprintf(stderr, "ERROR\n");
                            } else {
                                bool close = false;
                                list *l = marathon(users, userID, k, 0, &close);
                                if (close)close_program();
                                print_marathon(l);
                            }
                        } else error();
                    }
                } else {
                    error();
                }
            } else error();
        } else {
            ungetc(c, stdin);
            error();
        }
    } else error();
}

int main() {
    for (int i = 0; i <= UINT16_MAX; i++) {
        users[i] = NULL;
    }
    node *root = initialize_root();
    if (root == NULL)close_program();
    users[0] = root;
    char c = (char) getchar();
    while (c != EOF) {
        ungetc(c, stdin);
        parse();
        c = (char) getchar();
    }
    delete_tree(users);
    return 0;
}
