#ifndef PARSERBASE_H
#define PARSERBASE_H

#include <QObject>
#include <QUrl>

class ParserBase : public QObject
{
    Q_OBJECT
public:
    explicit ParserBase(QObject *parent = 0);

signals:
    void

public slots:


private slots:
    void networkReplyFinishedSlot(QNetworkReply *reply);
    void parseHTML(const QString *content);

private:
    QUrl m_url;

    void getSiteContent();

};

#endif // PARSERBASE_H
