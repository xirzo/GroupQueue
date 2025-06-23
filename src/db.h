#ifndef DB_H
#define DB_H

#include <libpq-fe.h>
#include "bool.h"

static PGconn *gConn;

gqbool GQinitDB(const char *conninfo);
void   GQdestroyDB();

gqbool GQaddList(const char *list_name);

#endif  // !DB_H
