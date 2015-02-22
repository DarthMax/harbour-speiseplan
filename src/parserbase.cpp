#include "parserbase.h"
#include <QtXmlPatterns/QXmlQuery>
#include <QDebug>

ParserBase::ParserBase(QObject *parent)
    : QObject(parent)
    , m_url("http://www.studentenwerk-magdeburg.de/mensen-cafeterien/mensa-unicampus/")
{

}

void ParserBase::getSiteContent()
{
    networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(networkReplyFinishedSlot(QNetworkReply*)));)
    networkManager->get(QNetworkRequest(m_url));
}


void ParserBase::networkReplyFinishedSlot(QNetworkReply* reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  // bytes
        QString html_string(bytes); // string
        parseHTML(html_string);

    }
    // Some http error received
    else
    {
       //TODO handle errors here
    }
    delete reply;
}


void ParserBase::parseHTML(QString const * content)
{
    QXmlQuery query;
    query.setQuery(content, "/body/div[@class='mensa']/table/tbody/tr");

    QString r;
    query.evaluateTo(&r);

    qDebug() << r;
}
