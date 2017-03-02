#ifndef IPUTILITIS_H
#define IPUTILITIS_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
# include "utilitis.h"
class IpUtilitis
{
public:
    IpUtilitis();
    static char* GetIpETH0();
};

#endif // IPUTILITIS_H
