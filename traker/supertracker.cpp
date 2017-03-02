#include "supertracker.h"
pthread_mutex_t SuperTracker::mutexList;
pthread_mutex_t SuperTracker::torMutex;
std::vector<char*> SuperTracker::Torrents;
CList * SuperTracker::clientList=nullptr;
SuperTracker::SuperTracker()
{

}
void SuperTracker::AutoIp()
{

}
void SuperTracker::SetIpAddr(char *ip)
{
    if (this->ipAddr!=nullptr)
    {
        delete this->ipAddr;
        this->ipAddr=nullptr;
    }
    this->ipAddr=Utilitis::my_strdup(ip);
}
void SuperTracker::PrimeReceiver()
{
    CSocket *receiver=nullptr;
    ThreadParameters *param;
    int socket;
    while (1)
    {
        try
        {
        if (receiver==nullptr)
        {
           receiver= MasterCSConstructor::CreateSvSocket(this->ipAddr,this->portComunicatii);
        }
        }
        catch (exception &ex)
        {

        }

        socket=receiver->Accept();
        param=new ThreadParameters();
        param->socket=socket;
        param->ipaddr=Utilitis::my_strdup(inet_ntoa(receiver->from.sin_addr));
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_create(&thread,&attr,ComunicationProtocol,param);
        param=nullptr;
    }
}
void * SuperTracker::ComunicationProtocol(void *parameter)
{
    char *msg=nullptr;
    msg=Utilitis::clear_and_realoc(msg);
    char filesNames[100];
    ThreadParameters *param=(ThreadParameters*)parameter;
    int socket=param->socket;
    if (clientList==nullptr)
    {
        pthread_mutex_lock(&mutexList);
        clientList=new CList();
         pthread_mutex_unlock(&mutexList);

    }

    int breakSet=0;
    //usleep(100);
    read(socket,msg,30);
    int choise=str2int(msg);
    switch (choise) {
    case 0:
    {
        ofstream f;
        pthread_mutex_lock(&mutexList);
        int n=clientList->size();
        for (int i=0;i<n;i++)
        {
            char *buffer=(*clientList)[i];
            if (strcmp(buffer,param->ipaddr)==0)
                  break;
        }
        clientList->push_back(param->ipaddr);
        msg=Utilitis::clear_and_realoc(msg);
        write (socket,"registred",10);
        while (read(socket,msg,30)>0)
        {
            if (strcmp("no file",msg)==0 ||str2int(msg)!=10)
            {
                write (socket,"no file confirmed",20);
                break;
            }
            clientList->addFile(param->ipaddr,msg);
            write(socket,"file received",14);
            msg=Utilitis::clear_and_realoc(msg);
        }
        int total,aux;
        msg=Utilitis::clear_and_realoc(msg);
        pthread_mutex_lock(&torMutex);
        while (read(socket,msg,30)>1)
        {

             //read(socket,msg,30);
            if (strcmp(msg,"")==0)
            {
                break;
            }
             Torrents.push_back(Utilitis::my_strdup(msg));
             f.open(msg,ofstream::out);
             write (socket,"torname recived",15);
             total=0;
             while (1)
             {
                 msg=Utilitis::clear_and_realoc(msg,513);
                 aux=read(socket,msg,513);
                 msg[aux-1]=NULL;
                 msg[strlen(msg)-1]=NULL;
                 f<<msg;
                 total+=aux;
                 if (total>=512)
                     break;
             }
             write(socket,"pakage livred",15);
             read(socket,msg,10);
             if (strcmp(msg,"end")==0)
                 break;
             msg=Utilitis::clear_and_realoc(msg);

        }
        write(socket,"ACK",4);
        pthread_mutex_unlock(&torMutex);
        pthread_mutex_unlock(&mutexList);
        break;
    }
    case 1:
    {
        pthread_mutex_lock(&mutexList);
        clientList->renew(param->ipaddr);
        write(socket,"renew confirmed",20);
        pthread_mutex_unlock(&mutexList);

        break;
    }
    case 2:
    {
        pthread_mutex_lock(&mutexList);
        clientList->pop_up(param->ipaddr);
        pthread_mutex_unlock(&mutexList);
        break;
    }
    case 3:
    {
        write(socket,"request granted",20);
        read(socket,filesNames,90);
        int ok=0;

        std::string buffer="";
        std::stringstream ss;
        int sizeL=clientList->size();
        for (int i=0;i<sizeL;i++)
        {
            if  (clientList->Search(filesNames,i)==true)
            {
                if (ok==0)
                {
                    //const char *a=*clientList[i];
                    buffer=(*clientList)[i];
                    ss<<buffer;
                    ok=1;
                }
                else
                {
                    ss<< " ";
                    buffer=(*clientList)[i];
                    ss<<buffer;
                }
            }
        }
        buffer=ss.str();
        char *token;
        char *lenght=strstr(filesNames,".");
        token=new char[strlen(lenght)+1];
        strncpy(token,filesNames,strlen(filesNames)-strlen(lenght));
        token[strlen(filesNames)-strlen(lenght)]=NULL;
        string aux1=token;
        string aux2=".popeye";
        aux1=aux1+aux2;
        delete token;
        token=new char[aux1.length()+1];
        strcpy(token,aux1.c_str());
        token[aux1.length()]=NULL;
        char *alldata=Utilitis::read_file(token);
        char *size=strtok(alldata,"\n");
        char *hash=strtok(alldata,NULL);
        std::string ssize=size;
        std::string shash=hash;
        buffer=buffer+"\n";
        buffer=buffer+ssize;
        buffer=buffer+"\n";
        buffer=buffer+shash;
        int sizeW=strlen(buffer.c_str());
        //char *towrite=Utilitis::my_strdup(buffer.c_str());
        write(socket,buffer.c_str(),sizeW+1);
        break;
    }
    case 4:
    {
        pthread_mutex_lock(&mutexList);
        while (read(socket,msg,90)>0)
        {
            if (strcmp("<end of prot>",msg)==0)
                write(socket,"sendig files closed",20);
                break;
            clientList->addFile(param->ipaddr,msg);
            write (socket,"received",10);
            msg=Utilitis::clear_and_realoc(msg,90);
        }
        pthread_mutex_unlock(&mutexList);
        break;
    }
    default:
    {
        printf("%s comanda invalida",param->ipaddr);
        break;
    }
    }
    delete param;

}
int SuperTracker::str2int(char *buffer)
{
    if (strcmp(buffer,"startup")==0)
        return 0;
    if (strcmp(buffer,"alive")==0)
        return 1;
    if (strcmp(buffer,"close")==0)
        return 2;
    if (strcmp(buffer,"get torrent")==0)
        return 3;
    if (strcmp(buffer,"add file")==0)
        return 4;
    return 10;
}

void SuperTracker::Start()
{
    this->PrimeReceiver();
}
