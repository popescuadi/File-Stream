#include "nod.h"

Nod::Nod()
{
    next=nullptr;
    prev=nullptr;
    ip=nullptr;
    alive=180;
}
Nod::Nod(char *ipaddr)
{
    ip=Utilitis::my_strdup(ipaddr);
}
bool Nod::Search(char *data)
{
    for (auto i=this->filesList.begin();i<filesList.end();i++)
    {
        char *buffer=*i;
        if (strcmp(data,buffer)==0)
            return true;
    }
    return false;
}
