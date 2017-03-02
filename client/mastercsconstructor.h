#ifndef MASTERCSCONSTRUCTOR_H
#define MASTERCSCONSTRUCTOR_H
#include "csocket.h"

class MasterCSConstructor
{
public:
    MasterCSConstructor();
    static CSocket * CreateSvSocket (char *ip,int port);
    static CSocket * CreateClSocket (char *ipSv,int portSv,char *ipCl,int portCl);
};

#endif // MASTERCSCONSTRUCTOR_H
