#include "client.h"
char * Client::ipAddr=nullptr;
char * Client::SvAddr=nullptr;
pthread_mutex_t Client::printingMutex;
pthread_mutex_t Client::TorrentsMutex;
std::vector <char*> Client::peddingTorrents;
Client::Client()
{
    this->ipAddr=IpUtilitis::GetIpETH0();
}
void Client::AddFile(char *file)
{
    char *aux=nullptr;
            bool unique=true;
    for (auto i=this->files.begin();i<files.end();i++)
    {
        aux=*i;
        if (strcmp(aux,file)==0)
        {
            unique=false;
            break;
        }
    }
    if (unique==true)
    {
        files.push_back(file);
        CreateTorrent(file);
    }
}
void Client::CreateTorrent(char *file)
{
    FILE *f;
    long int fsize=Utilitis::FileSize(file);
    f=fopen(file,"rb");
    char *token;
    char *lenght=strstr(file,".");
    token=new char[strlen(lenght)+1];
    strncpy(token,file,strlen(file)-strlen(lenght));
    token[strlen(file)-strlen(lenght)]=NULL;
    std::string first=token;
    std::string second=".popeye";
    std::string torrent=first+second;
    ofstream g;
    string aux;
    g.open(torrent,fstream::out);
    g<<fsize-1<<"\n";
    char buffer[1001];
    int nrb;

    if (f!=NULL)
    {
      while (!feof(f))
      {
       nrb=fread(buffer,1,1000,f);
       buffer[1000]=NULL;
       aux=Hashes::Get_sha512(buffer);
       g<<aux.c_str();
       if (nrb==1000)
           g<<" ";
      }
    }
    char *pointre=new char[torrent.length()+1];
    strcpy(pointre,torrent.c_str());
    pointre[torrent.length()]=NULL;
    char *fuck=Utilitis::my_strdup(pointre);
    this->torentsV.push_back(fuck);
}

void Client::AskForFile(char *file)
{
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&thread,&attr,GetTorrent,file);
}

void * Client::GetTorrent(void *param)
{
    char *file=(char*)param;
    CSocket *sock=MasterCSConstructor::CreateClSocket(SvAddr,60000,ipAddr,60100);
    bool unique=true;
    pthread_mutex_lock(&TorrentsMutex);
    for (auto i=peddingTorrents.begin();i<peddingTorrents.end();i++)
        if (strcmp(*i,file)==0)
        {
            unique=false;
            break;
        }
    if (unique)
        peddingTorrents.push_back(Utilitis::my_strdup(file));
    pthread_mutex_unlock(&TorrentsMutex);
    if (unique==true)
    {
        int socket_d=sock->Connect();
        write (socket_d,"get torrent",15);
        char msg[30];
        read (socket_d,msg,30);
        if (strcmp(msg,"request granted")==0)
        {
            pthread_mutex_lock(&printingMutex);
            printf ("getting torrent for %s\n",file);
            pthread_mutex_unlock(&printingMutex);
            char *torrentFile=new char [65000];
            write(socket_d,file,30);
            read(socket_d,torrentFile,65000);
            write (socket_d,"received",10);

                ofstream f;
                char *token;
                char *lenght=strstr(file,".");
                token=new char[strlen(lenght)+1];
                strncpy(token,file,strlen(file)-strlen(lenght));
                token[strlen(file)-strlen(lenght)]=NULL;
                string aux1=token;
                string aux2=".popey";
                aux1=aux1+aux2;
                f.open(aux1.c_str());
                if (f.is_open())
                {
                    f<<torrentFile;
                    pthread_mutex_lock(&printingMutex);
                    printf ("torrent for %s downloaded\n",file);
                    pthread_mutex_unlock(&printingMutex);
                }

        }
    }
    else
    {
        pthread_mutex_lock(&printingMutex);
        printf ("%s torrent is currently in progres\n",file);
        pthread_mutex_unlock(&printingMutex);
    }
    //close(socket_d);
    delete sock;
}
void Client::ConnetToTracker()
{
    CSocket *sock=MasterCSConstructor::CreateClSocket(SvAddr,this->SvPort,this->ipAddr,this->SPerrPort);
    int socket_d=sock->Connect();
    write(socket_d,"startup",10);
    char msg[30];
    read(socket_d,msg,10);
    if (strcmp(msg,"registred")==0)
    {
        for (auto i=files.begin();i<files.end();i++)
        {
            write(socket_d,*i,strlen(*i)+1);
            read(socket_d,msg,30);
            if (strcmp(msg,"file received")!=0)
                printf ("%s nu a putut fi adaugata\n",*i);
        }
        write(socket_d,"no file",10);
        read (socket_d,msg,30);
        if (strcmp("no file confirmed",msg)!=0)
            printf ("adding files failed\n");
        FILE *f;
        char msgTor[513];
        int total_SB=0;
        int max,aux;
        for (auto i=this->torentsV.begin();i<torentsV.end();i++)
        {
            f=fopen(*i,"rb");
            if (f!=NULL)
            {
                write(socket_d,*i,30);
                read(socket_d,msg,30);
                while (!feof(f))
                {
                    max=fread(msgTor,1,512,f);

                    total_SB=0;
                    while (1)
                    {
                        aux=write(socket_d,msgTor,513);
                        total_SB+=aux;
                        if (total_SB>=max)
                        {
                            break;
                        }
                    }
                    read(socket_d,msg,10);
                }
                write(socket_d,"end",30);
                read(socket_d,msg,30);
            }
        }
    }
    else
        printf ("conectarea si inregistrarea la server a esuat");
    close(socket_d);
    delete sock;
}

void Client::SetServer(char *ip)
{
    if (this->SvAddr!=nullptr)
    {
        delete SvAddr;
        SvAddr=nullptr;
    }
    SvAddr=Utilitis::my_strdup(ip);
}
void Client::Start()
{
    this->ConnetToTracker();
}
void *Client::ServerPeer(void *param)
{
    char *ip=(char *)param;
    CSocket *sock=MasterCSConstructor::CreateSvSocket(ip,61000);
    while (1)
    {
        int socket=sock->Accept();
        ThreadParam *param=new ThreadParam();
        param->socket=socket;
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        pthread_create(&thread,&attr,ServerProtocol,param);
    }
}
void * Client::ServerProtocol(void *param)
{
    ThreadParam *parameter=(ThreadParam*)param;
    int socket_s=parameter->socket;
    char filename[30];
    read(socket_s,filename,30);
    write(socket_s,"confirmed",10);
    char msg[30];
    int offset=0;
    int sizeOfRead=0;
    read(socket_s,msg,10);
    offset=atoi(msg);
    write (socket_s,"offset received",20);
    read(socket_s,msg,10);
    sizeOfRead=atoi(msg);
    write (socket_s,"size received",20);
    if (offset>=0 &&sizeOfRead>0&&offset<sizeOfRead)
    {
        FILE *file;
        char buffer[1001];
        file=fopen(filename,"rb");
        if (file!=NULL)
        {
            if (offset!=0)
            {
                int readBytes=0;
                while (readBytes!=offset &&!feof(file))
                {
                    fread(buffer,1,1000,file);
                    readBytes++;
                }
                while (offset!=sizeOfRead && !feof(file))
                {
                    fread(buffer,1,1000,file);
                    buffer[1000]=NULL;
                    int writedBytes=0;
                    while (writedBytes<=1000)
                    {
                        writedBytes+=write(socket_s,buffer,1001);
                    }
                    read (socket_s,msg,10);

                }
                write(socket_s,"end of file",20);
            }
        }
    }
}
void Client::DownloadTorrent(char *torrent, char *filename)
{
    TorParam *param=new TorParam();
    param->filename=Utilitis::my_strdup(filename);
    param->torname=Utilitis::my_strdup(torrent);
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&thread,&attr,PrelucateTorrent,param);
}

void * Client::PrelucateTorrent(void *param)
{
    TorParam *parameter=(TorParam*)param;
    DataParam *data;
    char *buffer=Utilitis::read_file(parameter->torname);
    char *token,*hash;
    token=strtok(buffer,"\n");
    long int size=atoi(token);
    ofstream file;
    file.open(parameter->filename,ofstream::out);
    long int sizeB=size/1000;
    int sizeb=size%1000;
    char * bytebuffer=new char[1001];
    for (int i=0;i<=999;i++)
        bytebuffer[i]='a';
    bytebuffer[1000]=NULL;
    int contor=0;
    while (contor!=sizeB)
    {
        file<<bytebuffer;
        contor++;
    }
    contor=0;
    while (contor!=sizeb)
    {
        file<<"a";
        contor++;
    }
    file.close();
    FILE *f;
    f=fopen(parameter->filename,"rb");
    if (f!=NULL)
    {
        pthread_mutex_t locker;
        token=strtok(NULL,"\n");
        hash=strtok(NULL,"\n");
        int nrPeers=1;
        int n=strlen(token);
        for (int i=0;i<n;i++)
            if (token[i]==' ')
                nrPeers++;
        pthread_t *threaduri=new pthread_t[nrPeers];
        int unit=sizeB/nrPeers;
       /* if (unit==0)
            unit++;*/
        int indexUnit=0;
        char *ipToken=strtok(token," ");
        while (ipToken!=NULL&&indexUnit<nrPeers)
        {
            data=new DataParam();
            data->file=f;
            data->lock=&locker;
            data->context=hash;
            data->sizeOfRead=unit*(indexUnit+1);
            data->offset=unit*indexUnit;
            data->ip=Utilitis::my_strdup(ipToken);
            data->filename=parameter->filename;
            if (indexUnit+1==nrPeers)
            {
                //unit=unit+sizeB%nrPeers;
                data->sizeOfRead=unit*(indexUnit+1)+sizeB%nrPeers;
                if (sizeb!=0)
                    data->sizeOfRead++;

            }
            //pthread_t aux=
            pthread_create(&threaduri[indexUnit],NULL,GetData,data);

            data=NULL;
            indexUnit++;
            ipToken=strtok(NULL," ");
        }
        for (int i=0;i<nrPeers;i++)
            pthread_join(threaduri[i],NULL);
    }
}

void *Client::GetData(void *param)
{
    DataParam* data=(DataParam*)param;
    CSocket *sock=MasterCSConstructor::CreateClSocket(data->ip,61000,ipAddr,61001);
    int socket=sock->Connect();
    char msg[30];
    char buffer[1001];
    write(socket,data->filename,30);
    read(socket,msg,30);
     snprintf(msg, sizeof(msg), "%d", data->offset);
    write(socket,msg,10);
    read(socket,msg,30);
    snprintf(msg, sizeof(msg), "%d", data->sizeOfRead);
    write(socket,msg,10);
    read(socket,msg,30);
    int location=data->offset;
    while (location!=data->sizeOfRead)
    {
        read(socket,buffer,1001);
        if (strcmp(buffer,"end of file")==0)
            break;
        write(socket,"confirmed",10);
        buffer[1000]=NULL;
        pthread_mutex_lock(data->lock);
        fseek(data->file,SEEK_SET,location*1000);
        fwrite(buffer,1,1000,data->file);
        pthread_mutex_unlock(data->lock);
        location++;

    }

}
