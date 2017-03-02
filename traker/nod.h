#ifndef NOD_H
#define NOD_H
# include <iostream>
# include "utilitis.h"
# include <vector>
using namespace std;

class Nod
{
public:
    Nod();
    Nod(char *ipaddr);
    Nod *next=nullptr;
    Nod *prev=nullptr;
    char *ip=nullptr;
    int alive=180;
    std::vector <char *> filesList;
    bool Search (char *data);
};

#endif // NOD_H
