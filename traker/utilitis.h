#ifndef UTILITIS_H
#define UTILITIS_H
# include <iostream>
# include <string.h>
# include <unistd.h>
# include <fstream>

using namespace std;

class Utilitis
{
public:
    Utilitis();
    static char* my_strdup(char *buffer);
    static char* clear_and_realoc (char *buffer,int size=30);
    static char* read_file(char *sir);
};

#endif // UTILITIS_H
