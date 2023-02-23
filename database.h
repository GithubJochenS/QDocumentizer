#ifndef DATABASE_H
#define DATABASE_H
#include "documentizer.h"
#include <QtSql>
#include <QErrorMessage>
#include "fileitem.h"

class database
{
private:
    Documentizer* mainWindow;
    QString pragmaQry;
    QSqlDatabase dbase;
    int createDB(newPasswordDir* _passDir);
    int connectDB(newPasswordDir* _passDir);
    bool sqlError(QSqlError error, QString query);

public:
    database(Documentizer* _mainWindow);
    ~database();
    QList<FileItem*>* getItems(QSqlQuery*);
    QStringList* getPersons(int id);
    QStringList* getPersons();
    QStringList* getKeywords(int id);
    QStringList* getKeywords();
    void addFile(QString file, QString name, QDateTime date, QStringList* persons, QStringList* keywords);
    void edit(int id, QString name, QDateTime date, QStringList* persons, QStringList* keywords);
    void deleteFile(int id);
    QByteArray* getFile(int id);
    QStringList* getYears();
    void createNewDB();
    void connectToDB(QString _path);
    int initDB(QString _path);



};

#endif // DATABASE_H