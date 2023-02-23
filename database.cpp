#include "database.h"
#include <QBuffer>
#include <QDateTime>

database::database(Documentizer* _mainWindow)
{
    mainWindow = _mainWindow;
        
    //QString path = (QDir::homePath()).append("/.QDocumentizer/Documents.db");
    
}

database::~database() {

    dbase.close();
}

int database::initDB(QString _path) {
    int ret = 0;
    if (QFile::exists(_path)) {
        //Dialog
        qDebug() << "File exists";
        bool first = true;
        bool wrongPassword = true;
        while (wrongPassword) {
            if (first) {
                first = false;
                if (connectDB(new newPasswordDir(_path, mainWindow->getPassword())) != 2) {
                    wrongPassword = false;
                    ret = 1;
                }
                    
            }
            else {
                if (connectDB(new newPasswordDir(_path, mainWindow->getPassword("Falsches Passwort!"))) != 2) {
                    wrongPassword = false;
                    ret = 1;
                }
                    
            }
        }
    }
    else {
        //Dialog
        ret = createDB(mainWindow->getNewPassword());
    }
    return ret;
}

int database::createDB(newPasswordDir* _passDir) {
    
    int ret = 0; 
    pragmaQry = "PRAGMA key='";
    pragmaQry.append(_passDir->password()).append("'");
    QString path = _passDir->dir();        //(QDir::homePath()).append("/.QDocumentizer/Documents.db");

    dbase = QSqlDatabase::addDatabase("QSQLCIPHER","db");
    dbase.setDatabaseName(path);

    if (dbase.open()) {
        qDebug() << "Database connection established";
        QSqlQuery pragmaQuery(pragmaQry,QSqlDatabase::database("db"));
        //pragmaQuery.prepare("PRAGMA key=':pw'");//Doesnt work bug in SQLClipher?
        //pragmaQuery.bindValue(":pw",password);
        pragmaQuery.exec();
        sqlError(pragmaQuery.lastError(), pragmaQuery.lastQuery());

        QStringList queries;
        queries << "CREATE TABLE files(id INTEGER PRIMARY KEY ASC, name TEXT, dateInsert INTEGER)"
            << "CREATE TABLE data(fileid INTEGER, pdf BLOB,dateDoc INTEGER, yearDoc INTEGER, monthDoc INTEGER, version REAL)" //"CREATE TABLE data(fileid INTEGER, pdf BLOB)"
            << "CREATE TABLE persons(fileid INTEGER, person TEXT)"
            << "CREATE TABLE keywords(fileid INTEGER, keyword TEXT)"
            
           ; // 


        for (const QString& qs : qAsConst(queries)) {
            pragmaQuery.exec(qs);
        }
        if(sqlError(pragmaQuery.lastError(), pragmaQuery.lastQuery()) == false) {
            mainWindow->updateConfig(path);
            ret = 1;
        }
        qDebug() << "Tables Created;";
    }
    else {
        qDebug() << "Database connection failed";
        ret = 3;
    }
    return ret;
}
int database::connectDB(newPasswordDir* _passDir) {
    int ret = 0;
    QString path = _passDir->dir();//(QDir::homePath()).append("/.QDocumentizer/Documents.db");
    pragmaQry = "PRAGMA key='";
    pragmaQry.append(_passDir->password()).append("'");

    dbase = (QSqlDatabase::addDatabase("QSQLCIPHER","db"));
    dbase.setDatabaseName(path);


    if (dbase.open()) {
        qDebug() << "Connection Established";
        //Todo : Only one Pragma or always?
        QSqlQuery pragmaQuery(pragmaQry,QSqlDatabase::database("db"));
        //pragmaQuery.prepare("PRAGMA key='test2'");
        //pragmaQuery.bindValue(":pw", pragmaQry);
        pragmaQuery.exec();
        
        if (pragmaQuery.lastError().isValid()) {
            sqlError(pragmaQuery.lastError(), pragmaQuery.lastQuery());
        }
        else {
            pragmaQuery.exec("SELECT COUNT(id) FROM files"); //test if you can access the db
            if (pragmaQuery.lastError().isValid()) {
                //sqlError(pragmaQuery.lastError(), pragmaQuery.lastQuery());
                qDebug() << pragmaQuery.lastError().type();
                ret = 2;
            }
            else
                ret = 1;
        }
    }
    else {
        QErrorMessage* erMsg = new QErrorMessage(mainWindow);
        erMsg->showMessage("Connection to SQLite Databas failed");
    }
    return ret;
}

QList<FileItem*>* database::getItems(QSqlQuery* selectQry) { //Todo : Vielleicht Query übergeben wegen Sicherheit?
    /*QSqlQuery pragmaQuery;
    pragmaQuery.prepare("PRAGMA key=':pw'");
    pragmaQuery.bindValue(":pw", password);
    pragmaQuery.exec();
    sqlError(pragmaQuery.lastError(), pragmaQuery.lastQuery());*/

    QList<FileItem*>* returnList = new QList<FileItem*>();
    selectQry->exec();
    while (selectQry->next()) {
        //QByteArray* bArray = new QByteArray(selectQry->value(0).toByteArray()); //Todo : Vielleicht erst Abfragen wenn man die PDF braucht wegen Memory
        QDateTime* date = new QDateTime(QDateTime::fromString(selectQry->value(1).toString(), Qt::ISODate));
        returnList->append(new FileItem( selectQry->value(0).toString(),
            getPersons(selectQry->value(2).toInt()),
            getKeywords(selectQry->value(2).toInt()),
            date,
            selectQry->value(2).toInt(),
            mainWindow->getTreeWidget()));
    }
    sqlError(selectQry->lastError(), selectQry->lastQuery());

    return returnList;
}

QStringList* database::getPersons(int id) {
    QStringList* personList = new QStringList();

    QSqlQuery personsQry(QSqlDatabase::database("db"));
    personsQry.prepare("SELECT DISTINCT person FROM persons WHERE fileid=:id ORDER BY Person");
    personsQry.bindValue(":id", id);
    personsQry.exec();

    QString str = personsQry.executedQuery();

    while (personsQry.next()) {
        personList->append(personsQry.value(0).toString());
    }

    sqlError(personsQry.lastError(), personsQry.lastQuery());
    return personList;
}

QStringList* database::getPersons() {
    QStringList* personList = new QStringList();
    QSqlQuery personsQry("SELECT DISTINCT person FROM persons ORDER BY Person", QSqlDatabase::database("db"));

    personsQry.exec();
    while (personsQry.next()) {
        personList->append(personsQry.value(0).toString());
    }

    sqlError(personsQry.lastError(), personsQry.lastQuery());
    return personList;
}


QStringList* database::getKeywords(int id) {
    QStringList* keywordList = new QStringList();

    QSqlQuery keywordQry(QSqlDatabase::database("db"));
    keywordQry.prepare("SELECT DISTINCT keyword FROM keywords WHERE fileid=:id ORDER BY keyword");
    keywordQry.bindValue(":id", id);
    keywordQry.exec();
    while (keywordQry.next()) {
        keywordList->append(keywordQry.value(0).toString());
    }

    sqlError(keywordQry.lastError(), keywordQry.lastQuery());
    return keywordList;
}

QStringList* database::getKeywords() {
    QStringList* keywordList = new QStringList();

    QSqlQuery keywordQry("SELECT DISTINCT keyword FROM keywords ORDER BY keyword", QSqlDatabase::database("db"));

    keywordQry.exec();
    while (keywordQry.next()) {
        keywordList->append(keywordQry.value(0).toString());
    }

    sqlError(keywordQry.lastError(), keywordQry.lastQuery());
    return keywordList;
}

void database::addFile(QString file, QString name, QDateTime date, QStringList* persons, QStringList* keywords) {
    /*QSqlQuery pragmaQuery(QSqlDatabase::database("db"));
    pragmaQuery.prepare("PRAGMA key=\':pw\'");
    pragmaQuery.bindValue(":pw", password);
    pragmaQuery.exec();*/
    
    QSqlQuery insertQuery( QSqlDatabase::database("db"));
    insertQuery.prepare("INSERT INTO files (name, dateInsert ) VALUES (:name ,CURRENT_TIMESTAMP)");
    insertQuery.bindValue(":name", name);
    insertQuery.exec();

    sqlError(insertQuery.lastError(), insertQuery.lastQuery());
    int lastId = insertQuery.lastInsertId().toInt();

    QFile pdf(file);
    pdf.open(QIODevice::ReadOnly);
    QSqlQuery insertFileQuery(QSqlDatabase::database("db"));
    insertFileQuery.prepare("INSERT INTO data (fileid,pdf,dateDoc, yearDoc, monthDoc,version) VALUES (:id, :file,:date, :year,:month,:version )"); // ,file, dateDoc, yearDoc, monthDoc :date, :year,:month ,:file
    insertFileQuery.bindValue(":id", lastId);
    insertFileQuery.bindValue(":file", pdf.readAll());
    insertFileQuery.bindValue(":date", (date.date().dayOfYear() * 10000) + date.date().year());
    insertFileQuery.bindValue(":year", date.date().year());
    insertFileQuery.bindValue(":month", date.date().month());
    insertFileQuery.bindValue(":version", 1.0);
    insertFileQuery.exec();
    pdf.close();
    
    sqlError(insertFileQuery.lastError(), insertFileQuery.lastQuery());

    foreach(QString person, *persons) {
        QSqlQuery insertPersonQuery(QSqlDatabase::database("db"));
        insertPersonQuery.prepare("INSERT INTO persons (fileid,person) VALUES (:id,:person)");
        insertPersonQuery.bindValue(":id", lastId);
        insertPersonQuery.bindValue(":person", person);
        insertPersonQuery.exec();

        sqlError(insertPersonQuery.lastError(), insertPersonQuery.lastQuery());
    }

    foreach(QString keyword, *keywords) {
        QSqlQuery insertKeywordQuery(QSqlDatabase::database("db"));
        insertKeywordQuery.prepare("INSERT INTO keywords (fileid,keyword) VALUES (:id,:keyword)");
        insertKeywordQuery.bindValue(":id", lastId);
        insertKeywordQuery.bindValue(":keyword", keyword);
        insertKeywordQuery.exec();

        sqlError(insertKeywordQuery.lastError(), insertKeywordQuery.lastQuery());
    }
}

void database::edit(int id, QString name, QDateTime date, QStringList* persons, QStringList* keywords) {
    qDebug() << "edit()";
    QSqlQuery editQuery(QSqlDatabase::database("db"));
    editQuery.prepare("UPDATE files SET name=:name WHERE id=:id");
    editQuery.bindValue(":name", name);
    editQuery.bindValue(":id", id);
    editQuery.exec();

    sqlError(editQuery.lastError(), editQuery.lastQuery());

    editQuery.prepare("UPDATE data SET dateDoc=:date, yearDoc=:year, monthDoc=:month WHERE fileid=:id");
    editQuery.bindValue(":date", (date.date().dayOfYear() * 10) + date.date().year());
    editQuery.bindValue(":year", date.date().year());
    editQuery.bindValue(":month", date.date().month());
    editQuery.bindValue(":id", id);
    editQuery.exec();

    sqlError(editQuery.lastError(), editQuery.lastQuery());

    QSqlQuery deletePersonsQuery(QSqlDatabase::database("db"));
    deletePersonsQuery.prepare("DELETE FROM persons WHERE fileid=:id");
    deletePersonsQuery.bindValue(":id", id);
    deletePersonsQuery.exec();

    sqlError(deletePersonsQuery.lastError(), deletePersonsQuery.lastQuery());

    foreach(QString person, *persons) {
        QSqlQuery insertPersonQuery(QSqlDatabase::database("db"));
        insertPersonQuery.prepare("INSERT INTO persons (fileid,person) VALUES (:id,:person)");
        insertPersonQuery.bindValue(":id", id);
        insertPersonQuery.bindValue(":person", person);
        insertPersonQuery.exec();

        sqlError(insertPersonQuery.lastError(), insertPersonQuery.lastQuery());
    }

    QSqlQuery deleteKeywordQuery(QSqlDatabase::database("db"));
    deleteKeywordQuery.prepare("DELETE FROM keywords WHERE fileid=:id");
    deleteKeywordQuery.bindValue(":id", id);
    deleteKeywordQuery.exec();

    sqlError(deleteKeywordQuery.lastError(), deleteKeywordQuery.lastQuery());

    foreach(QString keyword, *keywords) {
        QSqlQuery insertKeywordQuery(QSqlDatabase::database("db"));
        insertKeywordQuery.prepare("INSERT INTO keywords (fileid,keyword) VALUES (:id,:keyword)");
        insertKeywordQuery.bindValue(":id", id);
        insertKeywordQuery.bindValue(":keyword", keyword);
        insertKeywordQuery.exec();

        sqlError(insertKeywordQuery.lastError(), insertKeywordQuery.lastQuery());
    }
}

void database::deleteFile(int id) {
    qDebug() << "deleteFile()";

    QSqlQuery deleteQuery(QSqlDatabase::database("db"));
    deleteQuery.prepare("DELETE FROM keywords WHERE fileid=:id");
    deleteQuery.bindValue(":id", id);
    deleteQuery.exec();

    if (sqlError(deleteQuery.lastError(), deleteQuery.lastQuery()) ) 
        return;

    deleteQuery.prepare("DELETE FROM persons WHERE fileid=:id");
    deleteQuery.bindValue(":id", id);
    deleteQuery.exec();

    if (sqlError(deleteQuery.lastError(), deleteQuery.lastQuery()))
        return;

    deleteQuery.prepare("DELETE FROM data WHERE fileid=:id");
    deleteQuery.bindValue(":id", id);
    deleteQuery.exec();

    if (sqlError(deleteQuery.lastError(), deleteQuery.lastQuery()))
        return;

    deleteQuery.prepare("DELETE FROM files WHERE id=:id");
    deleteQuery.bindValue(":id", id);
    deleteQuery.exec();

    if (sqlError(deleteQuery.lastError(), deleteQuery.lastQuery()))
        return;
}

QByteArray* database::getFile(int id) {
    
    QByteArray* pdf = nullptr;
    QSqlQuery fileQry(QSqlDatabase::database("db"));
    fileQry.prepare("SELECT pdf FROM data WHERE fileid=:id");
    fileQry.bindValue(":id", id);
    fileQry.exec();
    
    while (fileQry.next()) {
        pdf = new QByteArray(fileQry.value(0).toByteArray());
    }
    sqlError(fileQry.lastError(), fileQry.lastQuery());

    return pdf;
}

QStringList* database::getYears() {
    QStringList* yearsList = new QStringList();

    QSqlQuery yearsQry("SELECT DISTINCT yearDoc FROM data ORDER BY yearDoc", QSqlDatabase::database("db"));

    yearsQry.exec();
    while (yearsQry.next()) {
        yearsList->append(yearsQry.value(0).toString());
    }

    sqlError(yearsQry.lastError(), yearsQry.lastQuery());
    return yearsList;

}

bool database::sqlError(QSqlError error, QString query) {
    if (error.isValid()) {
        QErrorMessage* erMsg = new QErrorMessage(mainWindow);
        erMsg->showMessage(error.text().append("::").append(query));
    }
    return error.isValid();
}

void database::createNewDB() {
    dbase.close();
    createDB(mainWindow->getNewPassword());
}

void database::connectToDB(QString _path) {
    dbase.close();
    bool first = true;
    bool wrongPassword = true;
    while (wrongPassword) {
        if (first) {
            first = false;
            if (connectDB(new newPasswordDir(_path, mainWindow->getPassword())) != 2)
                wrongPassword = false;
        }
        else {
            if (connectDB(new newPasswordDir(_path, mainWindow->getPassword("Falsches Passwort!"))) != 2)
                wrongPassword = false;
        }
    }
    mainWindow->updateConfig(_path);
}