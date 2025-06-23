#ifndef DB_H
#define DB_H

#include <libpq-fe.h>
#include "bool.h"

static PGconn *gConn;

void   GQinitDB(PGconn *conn);
gqbool GQaddList(const char *list_name);

#endif  // !DB_H
