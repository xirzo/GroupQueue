#ifndef SERVER_H
#define SERVER_H

#include "bool.h"

/* may faild if callback exists, but I don`t care */

gqbool GQinitServer(const char *ip, const unsigned short port);
void   GQrunServer(void);
void   GQdestroyServer(void);

#endif  // !SERVER_H
