#ifndef SUPERTRACKER_H
#define SUPERTRACKER_H
# include "csocket.h"
# include "clist.h"
# include "mastercsconstructor.h"
# include <pthread.h>
# include <sstream>
# include <fstream>
class SuperTracker
{
public:
    SuperTracker();
    void SetIpAddr(char *ip);
    void AutoIp();
    void SetPortComunicatii(int port);

    void Start();
private:
    //CSocket *socket=nullptr;
    char * ipAddr=nullptr;
    int portComunicatii=60000;
    static CList *clientList;
    void PrimeReceiver();
    static void* ComunicationProtocol(void *parameter);
    struct ThreadParameters
    {
        int socket;
        char* ipaddr=nullptr;
    };
    static pthread_mutex_t mutexList;
    static std::vector<char*> Torrents;
    static pthread_mutex_t torMutex;
    static int str2int (char *buffer);


};

#endif // SUPERTRACKER_H
