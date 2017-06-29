#include "httpconnect.h"
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>

HttpConnect::HttpConnect(QObject *parent) : QObject(parent)
  , reply(Q_NULLPTR)
  , file(Q_NULLPTR)
{
}
void HttpConnect::loadFileFormUrl(const QString& _url)
{
    const QUrl url = QUrl::fromUserInput(_url);
    if(url.isValid())
        emit urlInvalid();
    QString filename = url.fileName();
    if(filename.isEmpty())
        emit urlInvalid();
    file = new QFile(filename);
    if(!file->open(QIODevice::WriteOnly))
        emit urlInvalid();

    startRequest(url);
}
void HttpConnect::startRequest(const QUrl &url)
{
    reply = m.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &HttpConnect::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &HttpConnect::httpReadyRead);
}
void HttpConnect::httpFinished()
{
    if(file){
        QFileInfo f(*file);
        filepath = f.absoluteFilePath();
        file->close();
    }
    emit loadCompleted();
}
void HttpConnect::httpReadyRead()
{
    if(file)
        file->write(reply->readAll());
}
QString HttpConnect::getFilePath()
{
    return filepath;
}
