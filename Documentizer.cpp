#include "documentizer.h"

#include "database.h"
#include "filedialog.h"
#include "deletedialog.h"
#include "scandialog.h"
#include "NewPasswordDialog.h"
#include "PasswordDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtMath>
#include <QSplitter>
#include <QPrinter>
#include <QPrintDialog>
#include <qpdfview.h>
#include <QHBoxLayout>
#include <QToolBar> 
#include <QMenuBar>


Documentizer::Documentizer(QWidget* parent, QApplication* _app)
    : QMainWindow(parent)
     , doc(new QPdfDocument(this))
    , currentPDF(new QFile())
    , currentBuffer(new QBuffer()) {
    
    app = _app;
    dbase = new database(this);
    dbase->initDB(this->readConfig());
    QPdfView* pdfView = new QPdfView(this);
    pdfView->setZoomMode(QPdfView::ZoomMode::FitInView); //Lags out the Widget with certain pdfs at a certain height...????
    pdfView->setDocument(doc);
    pdfView->setPageMode(QPdfView::PageMode::MultiPage);
    pdfView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    pdfView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->pdfView->setZoomMode(QPdfView::CustomZoom);
    //ui->pdfView->setZoomFactor(0.7);

    fileView = new QTreeWidget(this);
    fileView->setSortingEnabled(true);
    fileView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fileView, &QWidget::customContextMenuRequested, this, &Documentizer::contextMenu);

    QSplitter* splitter = new QSplitter();
    QWidget* pdfViewParent = new QWidget(this);
    QHBoxLayout* pdfViewLayout = new QHBoxLayout();
    pdfViewParent->setLayout(pdfViewLayout);
    pdfViewLayout->addWidget(splitter);
    QToolBar* pdfViewToolBar = new QToolBar("pdfViewToolBar", this);
    pdfViewLayout->setMenuBar(pdfViewToolBar);

    pdfViewToolBar->addAction(*new QIcon(":/icons/D:/icons/icons-master/places/48/folder-documents-open.svg"), "Datei Hinzufuegen", this, &Documentizer::on_action_ffnen_triggered);
    pdfViewToolBar->addSeparator();

    pdfViewToolBar->addAction(*new QIcon(":/icons/D:/icons/icons8-eye-64.png"), "Alle Anzeigen", this, &Documentizer::refreshSlot);
    pdfViewToolBar->addSeparator();
    pdfViewToolBar->addAction(*new QIcon(":/icons//D:/icons/icons-master/devices/48/printer.svg"), "Drucken", this, &Documentizer::printPDF);
    pdfViewToolBar->addSeparator();
    pdfViewToolBar->addAction(*new QIcon(":/icons/D:/icons/icons8-save-64.png"), "Speichern", this, &Documentizer::savePDF);
    pdfViewToolBar->addSeparator();
    pdfViewToolBar->addAction(*new QIcon(":/icons/D:/icons/icons-master/devices/64/scanner.svg"), "Scannen", this, &Documentizer::scan);

    splitter->addWidget(fileView);
    splitter->addWidget(pdfView);

    QMenuBar* topMenuBar = new QMenuBar(this);
    setMenuBar(topMenuBar);

    QMenu* fileMenu = new QMenu("Datei", this);
    fileMenu->addAction("Hinzufuegen",this, &Documentizer::on_action_ffnen_triggered);
    fileMenu->addAction("Scannen", this, &Documentizer::scan);
    fileMenu->addAction("Drucken", this, &Documentizer::printPDF);
    fileMenu->addSeparator();
    fileMenu->addAction("Quit", app, &QCoreApplication::quit, Qt::QueuedConnection);
    topMenuBar->addMenu(fileMenu);

    dbMenu = new QMenu("Database");
    topMenuBar->addMenu(dbMenu);

    this->readRecent();
    this->updateRecentDB();


    personsMenu = new QMenu("Personen", this);
    topMenuBar->addMenu(personsMenu);

    keywordsMenu = new QMenu("Schlagwoerter", this);
    topMenuBar->addMenu(keywordsMenu);
    
    yearsMenu = new QMenu("Jahr", this);
    topMenuBar->addMenu(yearsMenu);

    this->setCentralWidget(pdfViewParent);
    refreshViews();
    QObject::connect(fileView, &QTreeWidget::currentItemChanged,
        this, &Documentizer::selection_Changed);

    this->resize(800, 600);
}

Documentizer::~Documentizer()
{
    delete dbase;
}

void Documentizer::on_action_ffnen_triggered()
{
    FileDialog dialog(this);

    if (dialog.exec()  == QDialog::Accepted) {
        dbase->addFile(dialog.getFile(), dialog.getName(), dialog.getDate(), dialog.getPersons(), dialog.getKeywords());
        refreshViews();
    }
}

void Documentizer::refreshViews(QSqlQuery* selectQry, bool refreshMenu) {
    fileView->clear();
    QStringList headerStrings({ "Name","Datum","Personen","Schalgworte" });

    fileView->setHeaderLabels(headerStrings);
    fileView->addTopLevelItems(*(QList<QTreeWidgetItem*>*)dbase->getItems(selectQry));
    
    if (refreshMenu) {
        personsMenu->clear();
        keywordsMenu->clear();
        yearsMenu->clear();

        foreach(QString persons, *dbase->getPersons()) {
            QAction* action = new QAction(persons, personsMenu);
            action->setCheckable(true);

            connect(action, &QAction::triggered, this, &Documentizer::menuTriggered);
            personsMenu->addAction(action);
        }

        foreach(QString keywords, *dbase->getKeywords()) {
            QAction* action = new QAction(keywords, keywordsMenu);
            action->setCheckable(true);

            connect(action, &QAction::triggered, this, &Documentizer::menuTriggered);
            keywordsMenu->addAction(action);
        }

        foreach(QString year, *dbase->getYears()) {
            QAction* action = new QAction(year, yearsMenu);
            action->setCheckable(action);

            connect(action, &QAction::triggered, this, &Documentizer::menuTriggered);
            yearsMenu->addAction(action);
        }
    }
}

QTreeWidget* Documentizer::getTreeWidget() {
    return (fileView);
}

void Documentizer::selection_Changed(QTreeWidgetItem* newItem) {
    if (newItem)
    {
        doc->close();

        /*
         QString fileStr;
        fileStr = "tmp";
        fileStr.append(QString::number(QRandomGenerator::global()->generate()));
        fileStr.append(".pdf");

        if(currentPDF->exists()) {
            currentPDF->close();
            currentPDF->remove();
        }


        currentPDF = new QFile(fileStr);
        currentPDF->open(QIODevice::ReadWrite );
        QDataStream* out = new QDataStream(currentPDF);
        *out << bArray;

        doc->load(currentPDF);
        */
        currentBuffer->close();
        delete (currentBuffer);

        currentBuffer = new QBuffer();
        currentBuffer->open(QIODevice::ReadWrite);
        QDataStream* out = new QDataStream(currentBuffer);
        QByteArray* pdf = dbase->getFile(((FileItem*)newItem)->getID());
        
        if (pdf) {
            *out << *pdf;
            doc->load(currentBuffer);
        }
        //doc->load(fileStr);
    }
}

void Documentizer::menuTriggered() {
    QAction* action = (QAction*)this->sender();
    QMenu* par = (QMenu*)action->parent();
    QSqlQuery* newQry = new QSqlQuery(QSqlDatabase::database("db"));
    QString person = "";
    QString keyword = "";
    QString year = "";

    if (par == personsMenu) {
        if (action->isChecked()) {
            foreach(QAction * act, ((QMenu*)action->parent())->actions()) {
                if (act != action)
                    act->setChecked(false);
            }
        }
        
    }
    else if (par == keywordsMenu) {
        if (action->isChecked()) {
            foreach(QAction * act, ((QMenu*)action->parent())->actions()) {
                if (act != action)
                    act->setChecked(false);
            }
      
        }
    }
    else if (par == yearsMenu) {
        if (action->isChecked()) {
            foreach(QAction * act, ((QMenu*)action->parent())->actions()) {
                if (act != action)
                    act->setChecked(false);
            }

        }
    }

    foreach(QAction * act, keywordsMenu->actions()) {
        if (act->isChecked()) {
            keyword = act->text();
            break;
        }
    }
    
    foreach(QAction* act, personsMenu->actions()) {
        if (act->isChecked()) {
            person = act->text();
            break;
        }
    }

    foreach(QAction* act, yearsMenu->actions()) {
        if (act->isChecked()) {
            year = act->text();
            break;
        }
    }

    if (person != "" && keyword != "" && year != "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM persons WHERE person=:person) AND id IN (SELECT fileid FROM keywords WHERE keyword=:keyword) AND id IN (SELECT fileid FROM data WHERE yearDoc=:year)");
        newQry->bindValue(":person", person);
        newQry->bindValue(":keyword", keyword);
        newQry->bindValue(":year", year);
    }
    else if (person != "" && keyword != "" && year == "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM persons WHERE person=:person) AND id IN (SELECT fileid FROM keywords WHERE keyword=:keyword)");
        newQry->bindValue(":person", person);
        newQry->bindValue(":keyword",keyword);
    }
    else if (person != "" && keyword == "" && year != "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM persons WHERE person=:person) AND id IN (SELECT fileid FROM data WHERE yearDoc=:year)");
        newQry->bindValue(":person", person);
        newQry->bindValue(":year", year);
    }
    else if (person != "" && keyword == "" && year == "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM persons WHERE person=:person)");
        newQry->bindValue(":person", action->text());
    }
    else if (person == "" && keyword != "" && year != "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM keywords WHERE keyword=:keyword) AND id IN (SELECT fileid FROM data WHERE yearDoc=:year)");
        newQry->bindValue(":keyword", keyword );
        newQry->bindValue(":year", year);
    }
    else if (person == "" && keyword != "" && year =="") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM keywords WHERE keyword=:keyword)");
        newQry->bindValue(":keyword", keyword);
    }
    else if (person == "" && keyword == "" && year != "") {
        newQry->prepare("SELECT name,dateInsert,id FROM files where id IN(SELECT fileid FROM data WHERE yearDoc=:year)");
        newQry->bindValue(":year", year);
    }
    else {
        newQry->prepare("SELECT name,dateInsert,id FROM files");
    }
    refreshViews(newQry, false);
}




void Documentizer::contextMenu(const QPoint& pos) {
    QTreeWidgetItem* selected = fileView->itemAt(pos);
    if (selected) {
        selectedItem = (FileItem*)selected;
        QAction* editAction = new QAction(tr("&Edit"));
        connect(editAction, &QAction::triggered, this, &Documentizer::edit);

        QAction* deleteAction = new QAction(tr("&Delete"));
        connect(deleteAction, &QAction::triggered, this, &Documentizer::deleteFile);

        QMenu menu(this);
        menu.addAction(editAction);
        menu.addAction(deleteAction);
        menu.exec(fileView->mapToGlobal(pos));
    }
}

void Documentizer::edit() {
    if (selectedItem) {
        FileDialog dialog(this, selectedItem->getName(), dbase->getFile(selectedItem->getID()), selectedItem->getPersons(), selectedItem->getKeywords(), selectedItem->getDate());
        
        if (dialog.exec() == QDialog::Accepted) {
            qDebug() << "edit Accepeted";
            dbase->edit(selectedItem->getID(), dialog.getName(), dialog.getDate(), dialog.getPersons(), dialog.getKeywords());
            refreshViews();
        }
        selectedItem = nullptr;
    }
    selectedItem = nullptr;
}

void Documentizer::deleteFile() {
    if (selectedItem) {
        DeleteDialog dialog(this, dbase->getFile(selectedItem->getID()));
        
        if (dialog.exec() == QDialog::Accepted) {
            dbase->deleteFile(selectedItem->getID());
            refreshViews();
        }
    }
    selectedItem = nullptr;
}

void Documentizer::printPDF() {
    if (doc->status() == QPdfDocument::Status::Ready) {
        QPrinter printer;
        QPrintDialog dlg(&printer, this);
        if (dlg.exec() == QDialog::Accepted) {
            for (int i = 0; i < doc->pageCount(); i++) {
                QImage img(doc->render(i, printer.pageRect(QPrinter::DevicePixel).size().toSize()));
                QPainter painter(&printer);
                painter.drawImage(QPoint(0, 0), img);
                painter.end();
            }
        }
    }
}

void Documentizer::savePDF() {
    if (!fileView->selectedItems().empty()) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Speichern unter"),
            QDir::homePath(),
            tr("PDF (*.pdf)"));

        QFile* saveFile = new QFile(fileName);
        if (saveFile->exists()) {
            //Todo Overrite Dialog
        }
        if (saveFile->open(QIODevice::ReadWrite)) {
            QDataStream* out = new QDataStream(saveFile);
            *out << *dbase->getFile(((FileItem*)fileView->selectedItems().first())->getID());
            saveFile->close();
        }
        else {
            //todo Fehler Dialog

        }
    }
}

void Documentizer::refreshSlot() {
    foreach(QAction * act, personsMenu->actions()) {
        act->setChecked(false);
    }
    foreach(QAction * act, keywordsMenu->actions()) {
        act->setChecked(false);
    }
    refreshViews();
}

void Documentizer::scan() {
    ScanDialog dialog(this);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
            dbase->addFile(dialog.getFile(), dialog.getName(), dialog.getDate(), dialog.getPersons(), dialog.getKeywords());
            QFile file(dialog.getFile());
            file.remove();
            refreshViews();
    }
}

newPasswordDir* Documentizer::getNewPassword() {
    newPasswordDir* ret = nullptr;

    NewPasswordDialog newPasswordDialog(this);
    if (newPasswordDialog.exec() == QDialog::Accepted) {
        ret = new newPasswordDir(newPasswordDialog.getDir().append("/Documents.db"), newPasswordDialog.getPassword());
    }

    return ret;
}

QString Documentizer::getPassword(QString errMsg) {
    QString ret ;

    PasswordDialog passworDialog(errMsg, this);
    if (passworDialog.exec() == QDialog::Accepted) {
        ret = passworDialog.getPassword();
    }

    return ret;
}

QString Documentizer::readConfig() {
    QString retStr;

    QFile configFile((QDir::homePath()).append("/.QDocumentizer/.config"));

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&configFile);
        retStr = in.readAll();
        configFile.close();
        if (retStr.isNull()) {
            qDebug() << "ConfigFile String is NULL";
            //Todo Fehler Dialog
        }
    }
    else {
        //Todo Fehler Dialog
        qDebug() << "File could not be opened";
        configFile.close();
    }
    return retStr;
}

void Documentizer::updateConfig(QString _path) {
    QDir homeDir = QDir::home();
    if (!homeDir.exists(".QDocumentizer")) {
        if (!homeDir.mkdir(".QDocumentizer")) {
            qDebug() << "HomeDir/QDocmenitzer konnte nicht angelegt werden";
            //Todo:: FehlerDialog
            return;
        }
    }
    QFile configFile((QDir::homePath()).append("/.QDocumentizer/.config"));

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&configFile);
        out << _path;
    }
    configFile.close();
    QFile recentFile((QDir::homePath()).append("/.QDocumentizer/.recent"));

    QStringList buffer;
    if (recentFile.open(QIODevice::ReadOnly | QIODevice::Text )) {//| QIODevice::Truncate

        QTextStream* in = new QTextStream(&recentFile);
        
        QString rLine;

        while (in->readLineInto(&rLine)) {
            buffer.append(rLine);
        }
        delete in;
    } 
    recentFile.close();
    if (recentFile.open(QIODevice::WriteOnly | QIODevice::Text)) {//
        buffer.prepend(_path);
        buffer.removeDuplicates();

        if (buffer.count() > 5) {
            buffer.removeLast();
        }
        QTextStream out(&recentFile);

        for (int i = 0; i < buffer.count(); i++) {
            out << buffer.at(i) << "\n";
        }
        recentFiles = buffer;
        this->updateRecentDB();
    }//Todo Fehler Dialog
    else
        qDebug() << "recentFile could not be opened";

    recentFile.close();
}

void Documentizer::openDB() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::homePath(),
        tr("Database (*.db)"));
    QFileInfo fileInfo(fileName);
    if (fileInfo.isWritable()) {
        dbase->connectToDB(fileName);
        refreshViews();
    }
    else {
        qDebug() << "File Not Writeable";
    }
}

void Documentizer::createNewDB() {
    dbase->createNewDB();
    refreshViews();
}

void Documentizer::readRecent() {
    QFile recentFile((QDir::homePath()).append("/.QDocumentizer/.recent"));
    if (recentFile.open(QIODevice::ReadOnly | QIODevice::Text)) {//| QIODevice::Truncate

        QTextStream* in = new QTextStream(&recentFile);
        QString rLine;

        while (in->readLineInto(&rLine)) {
            recentFiles.append(rLine);
        }
        delete in;
    }
    recentFile.close();
}

void Documentizer::updateRecentDB() {
    dbMenu->clear();
    dbMenu->addAction("Datenbank Oeffnen", this, &Documentizer::openDB);
    dbMenu->addAction("Neue Datenbank erstellen", this, &Documentizer::createNewDB);
    dbMenu->addSeparator();
    for (int i = 0; i < recentFiles.count(); i++) {
        dbMenu->addAction(recentFiles.at(i), this, [=]() { this->triggerDBMenu(i); });
    }
}

void Documentizer::triggerDBMenu(int i) {
    dbase->connectToDB(recentFiles.at(i));
    refreshViews();
}

newPasswordDir::newPasswordDir(QString _dir, QString _password) {
    dirStr = _dir;
    passwordStr = _password;
}

QString newPasswordDir::dir() {
    return dirStr;
}

QString newPasswordDir::password() {
    return passwordStr;
}


