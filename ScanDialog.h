#pragma once
#include "windows.h"

#include <qdialog.h>
#include <QGridLayout>
#include <QLineEdit>
#include <qgroupbox.h>
#include <QLabel>
#include <Wia.h>
#include <Sti.h>
#include <atlbase.h>
#include <Tchar.h>
#include <comutil.h>
#include <stdio.h>
#include <qdir.h>
#include <QToolBar> 
#include <qdialogbuttonbox.h>

class ScanDialog;

class ScanLabel : public QLabel
{
    public :
        ScanLabel(QWidget* parent, ScanDialog* _scanDialog , int _imageListiterator, QToolBar* _toolBar);
        void setToolBarVisible(bool visible);
    protected :
        void mousePressEvent(QMouseEvent* event);

    private :
        bool clickAble = true;
        ScanDialog* scanDialog;
        int imageListIterator;
        QToolBar* scanLabelToolBar;

public slots:
    void rotateRight();
    void rotateLeft();
    void deleteImage();

};

class ScanDialog :
    public QDialog
{
    public :
        ScanDialog(QWidget* parent);
        ~ScanDialog();
        QStringList* getKeywords();
        QStringList* getPersons();
        QString getName();
        QDateTime getDate();
        QString getFile();
        void transformImage(int imageIterator, ScanLabel* label, QTransform transform);
        bool scan(ScanLabel* label);
        void deleteScanLabel(int imageIterator,ScanLabel* label);

    private slots:
        void on_KeywordPlusButton_clicked();
        void on_PersonsPlusButton_clicked();

    private:
        QList<QLineEdit*>* personList;
        QList<QLineEdit*>* keywordList;
        QVBoxLayout* personLayout;
        QVBoxLayout* keywordLayout;
        QLineEdit* nameEdit;
        QDateTimeEdit* dateTimeEdit;
        QGroupBox* personGroupBox;
        QGroupBox* keywordGroupBox;
        QPushButton* personPlusButton;
        QPushButton* keywordPlusButton;
        QHBoxLayout* scanLayout;
        QList<QImage>* scanImageList = nullptr;
        QList<QPixmap>* scanPixmapList = nullptr;
        QList<ScanLabel*>* scanLabelList = nullptr;
        ScanLabel* firstScanLabel;
        ScanLabel* setupNewScanLabel();
        QDialogButtonBox* buttonBox;

};



