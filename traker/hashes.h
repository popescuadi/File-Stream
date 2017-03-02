#ifndef HASHES_H
#define HASHES_H
# include "sha512.h"
# include <iostream>
using namespace std;
class Hashes
{
public:
    Hashes();
    static std::string Get_sha512 (std::string buffer);

};

#endif // HASHES_H
