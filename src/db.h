#ifndef DB_H
#define DB_H

#include "bool.h"
#include "types.h"

gqbool GQinitDB(const char *conninfo);
void   GQdestroyDB();

gqbool    GQaddList(GQlist list);
long long GQgetListId(const char *name);
GQlist    GQgetList(unsigned long long id);
GQuser   *GQgetUsersInList(unsigned long long id, int *count);
void      GQfreeUsersInList(GQuser *users, int count);
GQlist   *GQgetAllLists(int *count);
void      GQfreeAllLists(GQlist *lists, int count);
gqbool    GQdeleteList(unsigned long long id);

gqbool GQaddUser(GQuser user);
gqbool GQswapListUsers(
    unsigned long long list_id,
    unsigned long long user_a_id,
    unsigned long long user_b_id
);

#endif  // !DB_H
