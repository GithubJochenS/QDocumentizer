#ifndef FILEITEM_H
#define FILEITEM_H

#include <QTreeWidgetItem>
#include <QBuffer>
#include <QDateTime>

class FileItem : public QTreeWidgetItem
{
public:
    FileItem( QString _name, QStringList* _persons, QStringList* _keywords, QDateTime* _date, int _id, QTreeWidget* parent = nullptr);
    QString getName();
    QStringList* getKeywords();
    QStringList* getPersons();
    QDateTime* getDate();
    int getID();
    void updateItem(QString _name, QStringList* _persons, QStringList* _keywords, QDateTime* _date);

private:
    QString name;
    QStringList* persons;
    QStringList* keywords;
    QDateTime* date;
    int id;


};

#endif // FILEITEM_H
