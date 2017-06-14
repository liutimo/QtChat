#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H

#include <QObject>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

class HttpConnect : public QObject
{
    Q_OBJECT
public:
    explicit HttpConnect(QObject *parent = nullptr);
    void loadFileFormUrl(const QString& url);
    QString getFilePath();
signals:
    void urlInvalid();
    void loadCompleted();
public slots:
    void httpFinished();
    void httpReadyRead();
private:
    void startRequest(const QUrl &url);
    QNetworkReply *reply;
    QNetworkAccessManager m;

    QFile *file;
    QString filepath;
};
#endif // HTTPCONNECT_H
