#ifndef HEARTBEATTHREAD_H
#define HEARTBEATTHREAD_H

#include <QThread>

class ConnectToServer;


class HeartBeatThread : public QThread
{
    Q_OBJECT
public:
    HeartBeatThread();
    void run();
signals:
    void offline();
    void sendHeartBeat();

private:

};

#endif // HEARTBEATTHREAD_H
