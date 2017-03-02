#include "hashes.h"

Hashes::Hashes()
{

}
std::string Hashes::Get_sha512(std::string buffer)
{
    unsigned char digest[sha512::DIGEST_SIZE];
    memset(digest,0,sha512::DIGEST_SIZE);

    sha512 object_h = sha512();
    object_h.init();
    object_h.update((unsigned char*)buffer.c_str(), buffer.length());
    object_h.final(digest);

    char buf[2*sha512::DIGEST_SIZE+1];
    buf[2*sha512::DIGEST_SIZE] = 0;
    for (int i = 0; i < sha512::DIGEST_SIZE; i++)
        sprintf(buf+i*2, "%02x", digest[i]);
    return std::string(buf);
}
