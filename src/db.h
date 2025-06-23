#ifndef DB_H
#define DB_H

#include <libpq-fe.h>
#include "bool.h"
#include "types.h"

static PGconn *gConn;

gqbool GQinitDB(const char *conninfo);
void   GQdestroyDB();

gqbool GQaddList(GQlist list);
gqbool GQaddUser(GQuser user);

#endif  // !DB_H
