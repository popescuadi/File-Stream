#ifndef CSOCKET_H
#define CSOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
# include <iostream>
# include <string>
# include <unistd.h>
# include "utilitis.h"

using namespace std;

class CSocket
{
public:
    bool listenS=false;
    CSocket();
    CSocket(CSocket &a);
    void CreateSocket(int conexNr=100);
    void SetIpAddress(char *ip);
    void SetPort (int port);
    int GetSockDescriptor ();
    void ClientData(char *ip,int portT);
    int Connect();
    int Accept();
    int newsock;
    struct sockaddr_in from;
    struct sockaddr_in to;
    ~CSocket();
private:
    //char *msg=NULL;
    int sockDescriptor=-1;
    char *ipAdress=nullptr;
    int port=11111;
    socklen_t fromlen = 0;//sizeof (from);


    bool created=false;


};

#endif // CSOCKET_H
