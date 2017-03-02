#ifndef CLIST_H
#define CLIST_H
# include "nod.h"

class CList
{
public:
    CList();
    void push_back(char *ipaddr,char *files=NULL);
    void pop_up(char *ipaddr);
    void renew(char *ip);
    int size();
    char * operator [] (int poz);
    void addFile(char *ip,char *file);
    bool Search (char *buffer,int poz);
private:
    Nod *head=nullptr;
};

#endif // CLIST_H
