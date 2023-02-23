#include "FileItem.h"

FileItem::FileItem(QString _name, QStringList* _persons, QStringList* _keywords, QDateTime* _date, int _id, QTreeWidget* parent) : QTreeWidgetItem(parent)
{

    name = _name;
    persons = _persons;
    keywords = _keywords;
    date = _date;
    id = _id;
    this->setText(0, name);
    this->setText(1, date->toString());
    this->setText(2, persons->join(", "));
    this->setText(3, keywords->join(", "));
}


QString FileItem::getName() {
    return name;
}

QStringList* FileItem::getPersons() {
    return persons;
}

QStringList* FileItem::getKeywords() {
    return keywords;
}

QDateTime* FileItem::getDate() {
    return date;
}

int FileItem::getID() {
    return id;
}

void FileItem::updateItem(QString _name, QStringList* _persons, QStringList* _keywords, QDateTime* _date) {
    _name = name;
    persons = _persons;
    keywords = _keywords;
    date = _date;
    this->setText(0, name);
    this->setText(1, date->toString());
    this->setText(2, persons->join(", "));
    this->setText(3, keywords->join(", "));
    emitDataChanged();
}
