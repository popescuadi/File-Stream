#include "mastercsconstructor.h"

MasterCSConstructor::MasterCSConstructor()
{

}
CSocket * MasterCSConstructor::CreateSvSocket(char *ip, int port)
{
    CSocket *sock=nullptr;
    sock=new CSocket();
    sock->SetPort(port);
    sock->SetIpAddress(ip);
    sock->CreateSocket();
    return sock;
}
CSocket * MasterCSConstructor::CreateClSocket(char *ipSv, int portSv, char *ipCl, int portCl)
{
    CSocket *sock=nullptr;
    sock=new CSocket();
    sock->SetPort(portCl);
    sock->SetIpAddress(ipCl);
    sock->ClientData(ipSv,portSv);
    sock->CreateSocket();
    return sock;
}
