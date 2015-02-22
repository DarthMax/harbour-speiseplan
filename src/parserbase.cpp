#include "parserbase.h"

#include <QDomDocument>
#include <QDomNodeList>

#include <QRegExp>

#include <QtXml>
#include <QtXmlPatterns/QXmlQuery>

#include <QDebug>

ParserBase::ParserBase(QObject *parent)
    : QObject(parent)
    , m_url("http://www.studentenwerk-magdeburg.de/mensen-cafeterien/mensa-unicampus/")
{

}

void ParserBase::getSiteContent()
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(networkReplyFinishedSlot(QNetworkReply*)));
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
//        qDebug() << "parseHTML = " << html_string;
    }
    // Some http error received
    else
    {
       //TODO handle errors here
    }
    delete reply;
}


bool ParserBase::calcSubstringBorders(int & begin, int & end, QString beginString, QString endString, QString contentString, int startPos)
{
    begin  = contentString.indexOf(beginString, startPos);
    begin += beginString.length();
    end    = contentString.indexOf(endString, startPos+beginString.length());

    if (begin < end)
    {
        return true;
    }

    return false;
}

void ParserBase::parseHTML(QString const & content)
{
    Menu speiseplanObererSaal;


    int begin(0);
    int end(0);

    // get table "oberer saal"
    calcSubstringBorders(begin, end, "<div class='mensa'>", "</table></div>", content);
    QString currentMenu = content.mid(begin, end-begin);

    // one menu
    {
        // get date
        calcSubstringBorders(begin, end, "<td colspan='3'>", "</td></tr>", currentMenu);
        QString date = currentMenu.mid(end-10, 10); //< we have format "dd.mm.yyyy", so a fixed count of 10 chars


        qDebug() << "currentMenu=" << currentMenu;
        qDebug() << "date=" << date;

        // TODO! Fix loop ... (first item occures two times!)

        bool lastElement(false);
        int currentPos(0);

        QString currentFoodLine("");
        QString foodName("");
        QString price("");

        do
        {
            //get food name, separeted by <tr>...</tr>
            if (!calcSubstringBorders(begin, end, "<tr><td style='width:64%;'>", "</td></tr>", currentMenu, currentPos))
            {
                lastElement = true;
                break;
            }
            currentFoodLine = currentMenu.mid(begin, end-begin);

            qDebug() << "currentFoodLine=" << currentFoodLine;

            currentPos = end;

            //get food name
            calcSubstringBorders(begin, end, "<strong>","<br /><span class='grau'>", currentFoodLine);
            foodName = currentFoodLine.mid(begin, end-begin);

            // get price
            calcSubstringBorders(begin, end, "</strong><br />"," |", currentFoodLine);
            price = currentFoodLine.mid(begin, end-begin); //< hard coded 'student price'

            qDebug() << "currentPos=" << currentPos;
            qDebug() << "foodName=" << foodName;
            qDebug() << "price=" << price;

        } while( !lastElement );
    }
}
