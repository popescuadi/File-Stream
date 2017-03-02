//#include <QCoreApplication>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "csocket.h"
# include "supertracker.h"
using namespace std;

int main(int argc, char *argv[])
{
    int x;
    x=0;
    //printf ("radadada\n");
    SuperTracker server;
    server.SetIpAddr("127.0.0.1");
    server.Start();
    return 0;
}
