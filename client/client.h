#ifndef CLIENT_H
#define CLIENT_H
# include "mastercsconstructor.h"
# include <vector>
# include <pthread.h>
# include "iputilitis.h"
# include <fstream>
# include <stdio.h>
# include <iostream>
# include "hashes.h"
using namespace std;

class Client
{
public:
    Client();
    void Start();
    void AddFile(char *file);
    void RemoveFile(char *file);
    void SetServer (char *ip);
    void AskForFile(char *file);
    void DownloadTorrent (char *torrent,char *filename);
private:
    std::vector<char*>files;
    void CreateTorrent(char *file);
    void ConnetToTracker();
    //void * GetTorrent(void *parameter);
    void * ServerPeer();
    void * SendToSpecificClient (void *parameterUpload);
    int SPerrPort=60100;
    static char *ipAddr;
    static char *SvAddr;
    int SvPort=60000;
    int TorPort=62000;
    void AutoIp();
    void* ClientPeer (void *downloadParam);
    void* Download (void *downloadFile);
    static void* GetTorrent(void *param);
   // static void* AdFileToDb(void *param);
    bool CreateFile (char* filename,int size);
   static std::vector <char*> peddingTorrents;
    static pthread_mutex_t TorrentsMutex;
   static pthread_mutex_t printingMutex;
    int *FileDescriptors=nullptr;
    std::vector<char *> torentsV;

    //server & client download side
private:
    static void* ServerPeer (void *param);
    static void* ServerProtocol(void *param);
    int ServerPeerPort=61000;
    struct ThreadParam
    {
        char *filename=nullptr;
        int socket=0;
    };
    static void * PrelucateTorrent(void *param);
    struct TorParam
    {
        char *filename=nullptr;
        char *torname=nullptr;
    };
    static void * GetData(void *param);
    struct DataParam
    {
        FILE * file;
        int offset=0;
        int sizeOfRead=0;
        char *context=nullptr;
        pthread_mutex_t *lock;
        char *ip;
        char*filename;
    };

};

#endif // CLIENT_H
