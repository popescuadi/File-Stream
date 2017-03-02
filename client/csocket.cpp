#include "csocket.h"

CSocket::CSocket()
{
    created=false;
    fromlen=sizeof (from);
}
CSocket::CSocket(CSocket &a)
{
    this->to=a.to;
    this->from=a.from;
    this->created=a.created;
    this->fromlen=a.fromlen;
    this->port=a.port;
    this->SetIpAddress(a.ipAdress);
}

void CSocket::CreateSocket(int conexNr)
{
    this->sockDescriptor=socket(AF_INET,SOCK_STREAM,0);
    if (this->sockDescriptor<0)
        throw 0;
    to.sin_family=AF_INET;
    to.sin_port=htons(this->port);
    inet_aton(this->ipAdress,&to.sin_addr);
    //const sockaddr *ss=reinterpret_cast<sockaddr_in *>(a);
    int yes=1;
    setsockopt(sockDescriptor,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
   // setsockopt(sockDescriptor,SOL_SOCKET,SO_REUSEPORT,&yes,sizeof(yes));
    //int x=bind(this->sockDescriptor,(sockaddr *)&to,sizeof(to));
    if (bind(this->sockDescriptor,(sockaddr *)&to,sizeof(to))<0)
        throw 1;
    if (this->listenS==true)
        listen(this->sockDescriptor,conexNr);

}
void CSocket::SetIpAddress(char *ip)
{
    if (this->ipAdress==nullptr)
    {

        this->ipAdress=Utilitis::my_strdup(ip);
    }
    if (this->ipAdress!=nullptr)
    {
        delete this->ipAdress;
        this->ipAdress=Utilitis::my_strdup(ip);
    }
}

void CSocket::SetPort (int port)
{
    this->port=port;
}
int CSocket::GetSockDescriptor ()
{
    return this->sockDescriptor;
}
void CSocket::ClientData(char *ip,int portT)
{
    from.sin_family=AF_INET;
    from.sin_port = htons(portT);
    inet_aton(ip, &from.sin_addr);
}
int CSocket::Accept()
{
    int newsock;
    listen(this->sockDescriptor,1000);
    newsock=accept(this->sockDescriptor,(struct sockaddr *)&from,&fromlen);
    if (newsock==-1)
        throw 2;
    return newsock;
}
int CSocket::Connect()
{
    if (connect(this->sockDescriptor,(struct sockaddr *)&from,sizeof(from))<0)
        perror("Connect:");
   return this->sockDescriptor;
}
CSocket::~CSocket()
{

}

//void Socket::Connect();
/*int Socket::Accept()
{
    int newsock;
    newscok=accept(this-
    //= accept(, (struct sockaddr *)&from, &fromlen)
}*/
