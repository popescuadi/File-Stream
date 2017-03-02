#include "utilitis.h"

Utilitis::Utilitis()
{

}
char* Utilitis::my_strdup(char*buffer)
{
    int len=strlen(buffer);
    char *newBuffer=new char[len+1];
    if (!newBuffer)
        return nullptr;
    memcpy(newBuffer,buffer,len+1);
}
char* Utilitis::clear_and_realoc(char *buffer, int size)
{
    if (buffer!=nullptr && buffer!=NULL)
    {
        delete buffer;
        buffer=new char[size];
    }
    else
        buffer=new char[size];
    return buffer;
}

char *Utilitis::read_file(char *sir)
{

        ifstream file;
        file.open(sir, std::istream::in | std::ios::binary);
        std::streampos fsize = 0;
        fsize = file.tellg();
        file.seekg(0, std::ios::end);
        fsize = file.tellg() - fsize;
        file.close();
        int n = fsize;
        file.open(sir, istream::in | ios::binary);
        std::cout << n << " " << endl;
        int z = 0;
        char *p = new  char[n + 1];
        for (int i = 0;i < n;i++)
        {
        char ch;
        file.get(ch);
        p[i] = ch;
        }
        p[n]=NULL;
        file.close();
        return p;
}
long int Utilitis::FileSize(char *filen)
{
    char *sir=filen;
    ifstream file;
    file.open(sir, std::istream::in | std::ios::binary);
    std::streampos fsize = 0;
    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    file.close();
    long int n = fsize;
    return n;
}
