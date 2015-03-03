#ifndef PARSERBASE_H
#define PARSERBASE_H

#include <QAbstractListModel>
#include <QDate>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QVariant>


//---------------------------------------------------------------------


struct Food{
    Food(QString const name, QString const nameEnglish, QString const price)
        :name(name)
        ,nameEnglish(nameEnglish)
        ,price(price)
    {}

    QString name;           //< native name
    QString nameEnglish;    //< translated name
    QString price;          //< usually price for a student, if there is difference between guest/student/etc.
};

struct Menu{
    QDate day;
    QList<Food> foodList;
};


//---------------------------------------------------------------------


class MenuModel : public QAbstractListModel
{
public:
    MenuModel(QObject *parent = 0);

    void setFoodList(QList<Food> & list);
    int rowCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    QHash<int, QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex & index) const;

signals:

public slots:

private:
    QList<Food> foodList;
};


//---------------------------------------------------------------------


class ParserBase : public QObject
{
    Q_OBJECT
public:
    explicit ParserBase(QObject *parent = 0);

    void getSiteContent();

    bool calcSubstringBorders(int & begin, int & end, QString beginString, QString endString, QString contentString, int startPos = 0);

    MenuModel * m_obererSaalModel;
signals:

public slots:


private slots:
    void networkReplyFinishedSlot(QNetworkReply *reply);
    void parseHTML(const QString & content);

private:
    QUrl m_url;


};

#endif // PARSERBASE_H
