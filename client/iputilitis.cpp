#include "iputilitis.h"

IpUtilitis::IpUtilitis()
{

}
char* IpUtilitis::GetIpETH0()
{
    int fd;
     struct ifreq ifr;

     fd = socket(AF_INET, SOCK_DGRAM, 0);

     /* I want to get an IPv4 IP address */
     ifr.ifr_addr.sa_family = AF_INET;

     /* I want IP address attached to "eth0" */
     strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

     ioctl(fd, SIOCGIFADDR, &ifr);

     close(fd);

     /* display result */
     //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
     return Utilitis::my_strdup(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

}
