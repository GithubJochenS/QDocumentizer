#include "ScanDialog.h"


#include <qdebug.h>
#include <qdatetimeedit.h>
#include <qpushbutton.h>
#include <QScrollArea>
#include <QPrinter>
#include <QPainter>
#include <QPageSize>
#include <QMutex> 
#include <QResizeEvent>




#pragma comment(lib, "comsuppw.lib")


template<class T, size_t N>
constexpr size_t sizeAry(T(&)[N]) { return N; }

ScanDialog::ScanDialog(QWidget* parent) :   QDialog(parent),
                                            personList(new QList<QLineEdit*>),
                                            keywordList(new QList<QLineEdit*>),
                                            personLayout(new  QVBoxLayout()),
                                            keywordLayout(new  QVBoxLayout()),
                                            scanImageList(new QList<QImage>()),
                                            scanLabelList(new QList<ScanLabel*>()),
                                            scanPixmapList(new QList<QPixmap>()){
	
    //scanImageList();
	QGridLayout* gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    QLabel* nameLabel = new QLabel("Name :");

    this->resize(475, 325);

    //nameLabel->setGeometry(10,10,47,13);
    gridLayout->addWidget(nameLabel, 0, 0);

    nameEdit = new QLineEdit();
    //nameEdit->setGeometry()
    nameEdit->setEnabled(false);
    gridLayout->addWidget(nameEdit, 0, 1, 1, 2);

    QLabel* dateLabel = new QLabel("Datum :");
    gridLayout->addWidget(dateLabel, 1, 0);

    dateTimeEdit = new QDateTimeEdit();
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setEnabled(false);
    gridLayout->addWidget(dateTimeEdit, 1, 1, 1, 2);

    personGroupBox = new QGroupBox("Personen");
    personGroupBox->setLayout(personLayout);
    gridLayout->addWidget(personGroupBox, 2, 0, 1, 2);

    personPlusButton = new QPushButton("+");
    personPlusButton->setEnabled(false);
    connect(personPlusButton, &QPushButton::clicked, this, &ScanDialog::on_PersonsPlusButton_clicked);
    gridLayout->addWidget(personPlusButton, 2, 2, Qt::AlignTop);

    keywordGroupBox = new QGroupBox("Schlagworte");
    keywordGroupBox->setLayout(keywordLayout);
    gridLayout->addWidget(keywordGroupBox, 3, 0, 1, 2);

    keywordPlusButton = new QPushButton("+");
    keywordPlusButton->setEnabled(false);
    connect(keywordPlusButton, &QPushButton::clicked, this, &ScanDialog::on_KeywordPlusButton_clicked);
    gridLayout->addWidget(keywordPlusButton, 3, 2, Qt::AlignTop);


    QLineEdit* firstPersonLineEdit = new QLineEdit();
    firstPersonLineEdit->setEnabled(false);
    //firstPersonLineEdit->setGeometry(10,20,150,20);
    personLayout->addWidget(firstPersonLineEdit, Qt::AlignTop);
    personLayout->setDirection(QBoxLayout::TopToBottom);
    personList->append(firstPersonLineEdit);

    QLineEdit* firstKeywordLineEdit = new QLineEdit();
    firstKeywordLineEdit->setEnabled(false);
    keywordLayout->addWidget(firstKeywordLineEdit, Qt::AlignTop);
    //firstKeywordLineEdit->setGeometry(10,20,150,20);
    keywordList->append(firstKeywordLineEdit);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->setOrientation(Qt::Horizontal);
    gridLayout->addWidget(buttonBox, 4, 0, 1, 4, Qt::AlignRight);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    //QScrollArea* scanArea = new QScrollArea(this);

    //QWidget* scrollWidget = new QWidget(this);
    scanLayout = new QHBoxLayout(this);
    //scrollWidget->setLayout(scanLayout);
    //scanArea->setWidget(scrollWidget);

    //gridLayout->addWidget(scrollWidget, 0, 3, 4, 1);
    gridLayout->addLayout(scanLayout, 0, 3, 4, 1);
    gridLayout->setColumnStretch(3, 2);

    firstScanLabel = setupNewScanLabel();
    

    //scanLayout->addWidget(firstScanLabel);
  
}

ScanDialog::~ScanDialog() {
    
    
}

void ScanDialog::on_PersonsPlusButton_clicked()
{
    QLineEdit* newLineEdit = new QLineEdit();
    //newLineEdit->setGeometry(personList->last()->x(),personList->last()->x()+30, personList->last()->width(),personList->last()->height());
    personList->append(newLineEdit);
    personLayout->addWidget(newLineEdit);

    //ui->PersonGroupBox->setGeometry(ui->PersonGroupBox->x(),ui->PersonGroupBox->y(),ui->PersonGroupBox->width(),ui->PersonGroupBox->height()+50);
    //ui->KeywordGroupBox->setGeometry(ui->KeywordGroupBox->x(), ui->KeywordGroupBox->y()+50,ui->KeywordGroupBox->width(),ui->KeywordGroupBox->height() );

}

void ScanDialog::on_KeywordPlusButton_clicked()
{
    QLineEdit* newLineEdit = new QLineEdit();
    //newLineEdit->setGeometry(personList->last()->x(),personList->last()->x()+30, personList->last()->width(),personList->last()->height());
    keywordList->append(newLineEdit);
    keywordLayout->addWidget(newLineEdit);


    //ui->PersonGroupBox->setGeometry(ui->PersonGroupBox->x(),ui->PersonGroupBox->y(),ui->PersonGroupBox->width(),ui->PersonGroupBox->height()+50);
    //ui->KeywordGroupBox->setGeometry(ui->KeywordGroupBox->x(), ui->KeywordGroupBox->y()+50,ui->KeywordGroupBox->width(),ui->KeywordGroupBox->height() );

}

QStringList* ScanDialog::getPersons() {
    QStringList* persons = new QStringList();
    for (int i = 0; i < personList->count(); i++) {
        if (personList->at(i)->text() != "")
            persons->append(personList->at(i)->text());
    }
    return persons;
}


QStringList* ScanDialog::getKeywords() {
    QStringList* keywords = new QStringList();
    for (int i = 0; i < keywordList->count(); i++) {
        if (keywordList->at(i)->text() != "")
            keywords->append(keywordList->at(i)->text());
    }
    return keywords;
}

QString ScanDialog::getName() {
    return nameEdit->text();
}

QString ScanDialog::getFile() {
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QDir::tempPath().append("printer.pdf"));
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setResolution(300);

    QPainter painter;

    if (!painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return nullptr;
    }

    for (int i = 0; i < scanImageList->count(); i++) {
        painter.drawImage(QPoint(0, 0), scanImageList->at(i).scaledToWidth(printer.pageRect(QPrinter::DevicePixel).width(), Qt::SmoothTransformation));
        painter.drawImage(printer.pageRect(QPrinter::DevicePixel), scanImageList->at(i), printer.pageRect(QPrinter::DevicePixel), Qt::NoFormatConversion);
        painter.resetTransform();
        if (i != scanImageList->count()-1)
            printer.newPage();
    }
    painter.end();

    return QDir::tempPath().append("printer.pdf");
}

QDateTime ScanDialog::getDate() {
    return dateTimeEdit->dateTime();
}

bool ScanDialog::scan(ScanLabel* label) {
    bool ret = false;
    QMutex mutex;

    mutex.lock();
    IWiaDevMgr2* devManager = nullptr;
    
    HRESULT hr = CoCreateInstance(CLSID_WiaDevMgr2, NULL, CLSCTX_LOCAL_SERVER, IID_IWiaDevMgr2, (void**)&devManager);
    
    if (hr != S_OK) {
        qDebug() << "DevManger Init failed";
       
    }
    else {
       
        BSTR deviceID;

        IWiaItem2* rootItem = nullptr;
        HRESULT hr = devManager->SelectDeviceDlg((HWND)this->winId(), StiDeviceTypeScanner, 0, &deviceID, &rootItem);

        if (hr != S_OK) {
            qDebug() << "SelectDeviceDlg Init failed";
            
        }
        else {
            

            QString qPathStr = (QDir::homePath()).append("/.QDocumentizer/tmp/");
            if (QFileInfo::exists(qPathStr)) {
                QDir tmpDir(qPathStr);
                if (!tmpDir.removeRecursively()) {
                    qDebug() << "Tempdir couldnt be removed";
                    rootItem->Release();
                    devManager->Release();
                    return false;
                }
                    
            }
            QDir dir;
            if (dir.mkpath(qPathStr)) {
                std::string path(qPathStr.toStdString());

                CComBSTR bstrResult(path.c_str());
                BSTR folder = bstrResult.Detach();

                BSTR file(SysAllocString(L"tmp"));
                BSTR* fileAry = new BSTR[0];
                LONG count = 0;
                hr = rootItem->DeviceDlg(0, (HWND)this->winId(), folder, file, &count, &fileAry, &rootItem);

                if (hr != S_OK) {
                    qDebug() << "DeviceDlg failed";
                }
                else {
                    
                    
                        
                    QDir tmpDir(qPathStr);
                    foreach(QFileInfo files, tmpDir.entryInfoList(QDir::Files, QDir::Time | QDir::Reversed)) {
                        QString fileStr(files.absoluteFilePath());
                        QImage img(fileStr);  
                        QPixmap pixmap(fileStr);
                        
                        scanImageList->append(img);
                        scanPixmapList->append(pixmap);

                        /*if (pixmapWidth == 0)
                            pixmapWidth = label->width();*/

                        label->setPixmap(pixmap.scaledToWidth(firstScanLabel->width()-2, Qt::SmoothTransformation)); //
                        //label->setScaledContents(true);
                        label->setToolBarVisible(true);

                        ScanLabel* newScanLabel = setupNewScanLabel();
                        //scanLayout->addWidget(newScanLabel);
                        newScanLabel->setMinimumWidth(firstScanLabel->width() - 2);
                        label = newScanLabel;

                        nameEdit->setEnabled(true);
                        dateTimeEdit->setEnabled(true);
                        personPlusButton->setEnabled(true);
                        keywordPlusButton->setEnabled(true);
                        keywordList->first()->setEnabled(true);
                        personList->first()->setEnabled(true);
                        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

                        QFile file(fileStr);
                        
                        file.remove();
                        ret = true;
                        
                    }
                    
                    //int fileAryLength = sizeof(fileAry) / sizeof(fileAry[0]);
                    

                    //QString fileStr(_com_util::ConvertBSTRToString(fileAry[0]));

                    //qDebug() << fileStr;

                    if (!tmpDir.removeRecursively()) {
                        qDebug() << "Tempdir couldnt be removed";
                    }
                }

            }
            else {
                qDebug() << "Temp Dir Couldn be created";
            }
            
            }
            rootItem->Release();
        }
        devManager->Release();
        mutex.unlock();
        return(ret);

    //label->setText("clickAble = true");
}

ScanLabel* ScanDialog::setupNewScanLabel() {
   
    QFrame* scanTopWidget = new QFrame(this); // need Widget because Tells the geometry manager to place the menu bar widget at the top of parentWidget(), outside QWidget::contentsMargins(). All child widgets are placed below the bottom edge of the menu bar.
    QToolBar* scanLabelToolBar = new QToolBar("scanLabelToolBar",scanTopWidget);
    ScanLabel* newScanLabel = new ScanLabel(scanTopWidget, this, 0,scanLabelToolBar);
    scanTopWidget->setFrameShape(QFrame::Panel);
    scanTopWidget->setFrameShadow(QFrame::Sunken);
    scanTopWidget->setLineWidth(2);
    scanTopWidget->setMidLineWidth(2);

    QHBoxLayout* scanLabelLayout = new QHBoxLayout(scanTopWidget);
    
    scanTopWidget->setLayout(scanLabelLayout); 

    scanLayout->addWidget(scanTopWidget);
   
    //QToolBar* scanLabelToolBar = new QToolBar("scanLabelToolBar");
    scanLabelLayout->setMenuBar(scanLabelToolBar);

    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/close.png"), "Bild entfernen", newScanLabel, &ScanLabel::deleteImage);
    scanLabelToolBar->addSeparator();
    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/rotate-right_2.svg"), "90° nach Rechts rotieren", newScanLabel, &ScanLabel::rotateRight);
    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/rotate-left_2.svg"), "90° nach Links rotieren", newScanLabel, &ScanLabel::rotateLeft);
    scanLabelToolBar->setIconSize(QSize(12, 12));
    scanLabelToolBar->setVisible(false);

       
    scanLabelLayout->addWidget(newScanLabel);
    scanLabelList->append(newScanLabel);
    //scanLayout->addWidget(newScanLabel);
        return(newScanLabel);
}

void ScanDialog::transformImage(int imageIterator, ScanLabel* label, QTransform transform) {
    
    scanPixmapList->replace(imageIterator,scanPixmapList->at(imageIterator).transformed(transform, Qt::SmoothTransformation) );
    scanImageList->replace(imageIterator, scanImageList->at(imageIterator).transformed(transform, Qt::SmoothTransformation));
    
    label->setPixmap(scanPixmapList->at(imageIterator).scaledToWidth(label->width()-2, Qt::SmoothTransformation) );
}

void ScanDialog::deleteScanLabel(int imageIterator,ScanLabel* label) {
    int savedWidth = ((QWidget*)firstScanLabel->parent())->sizeHint().width();

    scanPixmapList->removeAt(imageIterator);
    scanImageList->removeAt(imageIterator);
    
    if (label != firstScanLabel) {
        scanLayout->removeWidget((QWidget*)label->parent());
        scanLabelList->removeAt(imageIterator);
        delete((QWidget*)label->parent());
    }
    else {
        if (scanLabelList->count() > 1) {
            scanLayout->removeWidget((QWidget*)scanLabelList->at(1)->parent());
            delete((QWidget*)scanLabelList->at(1)->parent());
            scanLabelList->removeAt(1);
        }
        firstScanLabel->setToolBarVisible(false);
        firstScanLabel->setPixmap(QPixmap(":/icons/D:/icons/icons8-plus-+-64.png"));
        firstScanLabel->setAlignment(Qt::AlignCenter);
        firstScanLabel->setScaledContents(false);

        nameEdit->setEnabled(false);
        dateTimeEdit->setEnabled(false);
        personPlusButton->setEnabled(false);
        keywordPlusButton->setEnabled(false);
        keywordList->first()->setEnabled(false);
        personList->first()->setEnabled(false);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    
    
    this->setMinimumWidth(this->sizeHint().width() - savedWidth);
    qDebug() << this->minimumSize();
    
    this->resize(this->sizeHint().width() - savedWidth, this->height());
    this->updateGeometry();
    
    
}



ScanLabel::ScanLabel(QWidget* parent, ScanDialog* _scanDialog, int _imageListiterator, QToolBar* _toolBar) : QLabel(parent) {
    imageListIterator = _imageListiterator;
    scanDialog = _scanDialog;
    scanLabelToolBar = _toolBar;
    
    /*QFrame* scanTopWidget = new QFrame(parent); // need Widget because Tells the geometry manager to place the menu bar widget at the top of parentWidget(), outside QWidget::contentsMargins(). All child widgets are placed below the bottom edge of the menu bar.
    scanTopWidget->setFrameShape(QFrame::Panel);
    scanTopWidget->setFrameShadow(QFrame::Sunken);
    scanTopWidget->setLineWidth(2);
    scanTopWidget->setMidLineWidth(2);
    
    QHBoxLayout* scanLabelLayout = new QHBoxLayout(parent);
    
    scanLayout->addWidget(scanTopWidget);
    scanTopWidget->setLayout(scanLabelLayout);*/

    /*scanLabelToolBar = new QToolBar("scanLabelToolBar");
    scanLabelLayout->setMenuBar(scanLabelToolBar);
    
    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/close.png"), "Bild entfernen", this, &ScanLabel::deleteImage);
    scanLabelToolBar->addSeparator();
    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/rotate-right_2.svg"), "90° nach Rechts rotieren", this, &ScanLabel::rotateRight); 
    scanLabelToolBar->addAction(*new QIcon(":/icons/D:/icons/rotate-left_2.svg"), "90° nach Links rotieren", this, &ScanLabel::rotateLeft);
    
    scanLabelToolBar->setVisible(false);*/

    //scanLabelLayout->addWidget(this);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Plain);
    setLineWidth(1);
    setMidLineWidth(1);
    //firstScanLabel->setText("Scannen");
    setPixmap(QPixmap(":/icons/D:/icons/icons8-plus-+-64.png"));
    setAlignment(Qt::AlignCenter);
    setScaledContents(false);
    //scanLayout->addWidget(this);
    //scanLabelLayout->addWidget(this);
}

void ScanLabel::mousePressEvent(QMouseEvent* ev) {
    if (clickAble) {
        
        if (parent()) {
            if (scanDialog->scan(this)) {
                clickAble = false;
                //scanLabelToolBar->setVisible(true);
            }
                
        }
    }
}

void ScanLabel::rotateRight() {
    qDebug() << "Rotate Right()";
    QTransform transform;
    transform.rotate(90);
    scanDialog->transformImage(imageListIterator, this, transform);
}
void ScanLabel::rotateLeft() {
    QTransform transform;
    transform.rotate(270);
    scanDialog->transformImage(imageListIterator, this, transform);
    qDebug() << "Rotate Left()";
}

void ScanLabel::deleteImage() {
    qDebug() << "Delete Image()";
    scanDialog->deleteScanLabel(imageListIterator,this);
}

void ScanLabel::setToolBarVisible(bool visible) {
    scanLabelToolBar->setVisible(visible);

}


