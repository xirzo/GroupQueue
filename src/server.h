#ifndef SERVER_H
#define SERVER_H

#include "bool.h"

gqbool GQinitServer(const char *ip, const unsigned short port);
void   GQrunServer();
void   GQdestroyServer();

#endif  // !SERVER_H
