#include "heartbeatthread.h"
#include "NetWork/connecttoserver.h"
HeartBeatThread::HeartBeatThread()
{

}

void HeartBeatThread::run()
{
    int i = 0;

    for(;;)
    {
        if(i > 3) {
            qDebug() << "离线";
            this->terminate();
        }

        i++;
        emit sendHeartBeat();
        sleep(5);
    }

    exec();
}
