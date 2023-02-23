#pragma once

#include <QtWidgets/QMainWindow>


#include <QMainWindow>
#include <QtPdf>
#include <QTreeWidget>
#include <QtSql>
#include "fileitem.h"
#include "Wia.h"


class database;
class newPasswordDir {
    public :
        newPasswordDir(QString _dir, QString _password);
        QString password();
        QString dir();

    private :
        QString dirStr;
        QString passwordStr;
};

class Documentizer : public QMainWindow
{
    Q_OBJECT

public:
    Documentizer(QWidget* parent , QApplication* _app);
    ~Documentizer();
    newPasswordDir* getNewPassword();
    QString getPassword(QString errMsg = "");
    QTreeWidget* getTreeWidget();
    void updateConfig(QString _path);
    void updateRecentDB();
    enum connection { OK=1, UNKOWN=0,  };
    

private slots:
    void on_action_ffnen_triggered();
    void selection_Changed(QTreeWidgetItem* newItem);
    void menuTriggered();
    void contextMenu(const QPoint& pos);
    void edit();
    void deleteFile();
    void printPDF();
    void savePDF();
    void refreshSlot();
    void scan();
    void openDB();
    void createNewDB();
    void triggerDBMenu(int i);
  
private:
    database* dbase;
    QPdfDocument* doc;
    QFile* currentPDF;
    QBuffer* currentBuffer;
    FileItem* selectedItem = nullptr;
    QMenu* dbMenu;
    QMenu* personsMenu;
    QMenu* keywordsMenu;
    QMenu* yearsMenu;
    QTreeWidget* fileView;
    void refreshViews(QSqlQuery* selectQry = new QSqlQuery("SELECT name,dateInsert,id FROM files", QSqlDatabase::database("db")), bool refreshMenu = true);
    QString readConfig();
    QApplication* app;
    QStringList recentFiles;
    void readRecent();
    
};