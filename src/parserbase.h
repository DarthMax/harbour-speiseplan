#ifndef PARSERBASE_H
#define PARSERBASE_H

#include <QDate>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

struct Food{
    QString name;           //< native name
    QString nameEnglish;    //< translated name
    QString price;          //< usually price for a student, if there is difference between guest/student/etc.
};

struct Menu{
    QDate day;
    QList<Food> foodList;
};


class ParserBase : public QObject
{
    Q_OBJECT
public:
    explicit ParserBase(QObject *parent = 0);

    void getSiteContent();

    bool calcSubstringBorders(int & begin, int & end, QString beginString, QString endString, QString contentString, int startPos = 0);
signals:

public slots:


private slots:
    void networkReplyFinishedSlot(QNetworkReply *reply);
    void parseHTML(const QString & content);

private:
    QUrl m_url;


};

#endif // PARSERBASE_H
