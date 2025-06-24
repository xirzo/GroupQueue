#ifndef TYPES_H
#define TYPES_H

#include "bool.h"

typedef struct {
    unsigned long long list_id;
    char              *list_name;
} GQlist;

typedef struct {
    unsigned long long user_id;
    long long          telegram_id;
    char              *first_name;
    char              *surname;
    char              *last_name;
    gqbool             is_admin;
} GQuser;

#endif  // !TYPES_H
