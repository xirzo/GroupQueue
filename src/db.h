#ifndef DB_H
#define DB_H

#include "bool.h"
#include "types.h"

gqbool GQinitDB(const char *conninfo);
void   GQdestroyDB();

gqbool    GQaddList(GQlist list);
long long GQgetListId(const char *name);
GQlist    GQgetList(long long id);
void      GQfreeAllLists(GQlist *lists, int count);
GQlist   *GQgetAllLists(int *count);

gqbool GQaddUser(GQuser user);

#endif  // !DB_H
