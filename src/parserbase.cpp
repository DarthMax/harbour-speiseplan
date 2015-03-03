#include "parserbase.h"

#include <QRegExp>

#include <QDebug>

ParserBase::ParserBase(QObject *parent)
    : QObject(parent)
    , m_obererSaalModel(new MenuModel)
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
    // look for first searchString aka \beginString
    begin  = contentString.indexOf(beginString, startPos);
    if (begin == -1)
    {
        return false;
    }

    // set begin position to "inner border" (we don't want to have the \beginString in the resulting middle)
    begin += beginString.length();
    if (begin >= contentString.length())
    {
        return false;
    }

    // look for second searchString aka \endString
    end = contentString.indexOf(endString, begin);
    if (end == -1)
    {
        return false;
    }

    return true;
}

void ParserBase::parseHTML(QString const & content)
{
    int begin(0);
    int end(0);

    // get table "oberer saal"
    calcSubstringBorders(begin, end, "<div class='mensa'>", "</table></div>", content);
    QString currentMenu = content.mid(begin, end-begin);

    // one menu
    Menu menuObererSaal;
    {
        // get date
        calcSubstringBorders(begin, end, "<td colspan='3'>", "</td></tr>", currentMenu);
        QString date = currentMenu.mid(end-10, 10); //< we have format "dd.MM.yyyy", so a fixed count of 10 chars
        menuObererSaal.day = QDate::fromString(date, "dd.MM.yyyy");

//        qDebug() << "currentMenu=" << currentMenu;
        qDebug() << "date=" << date;        

        bool lastElement(false);
        int currentPos(0);

        QString currentFoodLine("");
        QString foodName("");
        QString price("");

        do
        {
            //get food name, separated by <tr>...</tr>
            if (!calcSubstringBorders(begin, end, "<tr><td style='width:64%;'>", "</td></tr>", currentMenu, currentPos))
            {
                lastElement = true;
                break;
            }
            currentFoodLine = currentMenu.mid(begin, end-begin);

            qDebug() << "currentFoodLine=" << currentFoodLine;

            currentPos = end;

            //get food name
            if (!calcSubstringBorders(begin, end, "<strong>","<br /><span class='grau'>", currentFoodLine))
            {
                continue;
            }
            foodName = currentFoodLine.mid(begin, end-begin);

            // get price
            if (!calcSubstringBorders(begin, end, "</strong><br />"," |", currentFoodLine))
            {
                continue;
            }
            price = currentFoodLine.mid(begin, end-begin); //< hard coded 'student price'

            qDebug() << "currentPos=" << currentPos;
            qDebug() << "foodName=" << foodName;
            qDebug() << "price=" << price;

            // save food-entry
            Food currentFood(foodName, QString(""), price);
            menuObererSaal.foodList << currentFood;
        } while( !lastElement );

        qDebug() << "FoodList of ObererSaal contains " << menuObererSaal.foodList.size() << " entries.";
    }
    m_obererSaalModel->setFoodList(menuObererSaal.foodList);

    QModelIndex modelIndex = m_obererSaalModel->index(0);
    qDebug() << "m_obererSaalModel at index 0 = " << m_obererSaalModel->data( modelIndex, Qt::DisplayRole );
}


//---------------------------------------------------------------------


MenuModel::MenuModel(QObject * parent)
    :QAbstractListModel(parent)
{
}

void MenuModel::setFoodList(QList<Food> & list)
{
    if(!foodList.empty())
    {
        beginRemoveRows(QModelIndex(),0,foodList.size() - 1);
        foodList.clear();
        endRemoveRows();
    }

    int counter = 0;
    foreach(Food food, list)
    {
        beginInsertRows(QModelIndex(), counter, counter);
        foodList.push_back(food);
        endInsertRows();

        QModelIndex index = createIndex(counter, counter);

        emit dataChanged(index, index);
    }
}

int MenuModel::rowCount(const QModelIndex & /* parent */) const
{
    return foodList.count();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return foodList.at(index.row()).name;
}

bool MenuModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(value.toString().isEmpty())
    {
        return false;
    }

    beginInsertRows(index, 0, 0);
    foodList.append(Food(value.toString(),"",""));
    endInsertRows();

    emit dataChanged(index, index);

    return true;
}

QHash<int, QByteArray> MenuModel::roleNames() const
{    
    QHash<int, QByteArray> hash;
    hash[Qt::DisplayRole] = "content";
    return hash;
}

Qt::ItemFlags MenuModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}
