#include "filedialog.h"



FileDialog::FileDialog(QWidget* parent) :
    QDialog(parent),
    // ui(new Ui::FileDialog) ,
    personList(new QList<QLineEdit*>),
    keywordList(new QList<QLineEdit*>),
    doc(new QPdfDocument()),
    personLayout(new  QVBoxLayout()),
    keywordLayout(new  QVBoxLayout())
{
    qDebug() << "Begin File Dialog";
    //ui->setupUi(this);


    //QScrollArea* scroller = new QScrollArea(this);
    //scroller->setGeometry(0,0,575,325);



    QGridLayout* gridLayout = new QGridLayout;

    setLayout(gridLayout);

    // scroller->setWidget(layout->widget());
    this->resize(575, 325);

    QLabel* fileLabel = new QLabel("Datei :");
    gridLayout->addWidget(fileLabel, 0, 0);

    fileEdit = new QLineEdit();
    fileEdit->setEnabled(false);
    gridLayout->addWidget(fileEdit, 0, 1);


    QPushButton* fileButton = new QPushButton();
    fileButton->setText("Datei Oeffnen");
    connect(fileButton, &QPushButton::clicked, this, &FileDialog::on_fileOpenButton_clicked);
    gridLayout->addWidget(fileButton, 0, 2);

    QPdfView* pdfView = new QPdfView();
    pdfView->setDocument(doc);
    pdfView->setPageMode(QPdfView::MultiPage);
    pdfView->setZoomMode(QPdfView::FitInView);
    gridLayout->addWidget(pdfView, 0, 3, 5, 1);
    gridLayout->setColumnStretch(3, 2);

    QLabel* nameLabel = new QLabel("Name :");

    //nameLabel->setGeometry(10,10,47,13);
    gridLayout->addWidget(nameLabel, 1, 0);

    nameEdit = new QLineEdit();
    //nameEdit->setGeometry()
    nameEdit->setEnabled(false);
    gridLayout->addWidget(nameEdit, 1, 1, 1, 2);

    QLabel* dateLabel = new QLabel("Datum :");
    gridLayout->addWidget(dateLabel, 2, 0);

    dateTimeEdit = new QDateTimeEdit();
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setEnabled(false);
    gridLayout->addWidget(dateTimeEdit, 2, 1, 1, 2);

    personGroupBox = new QGroupBox("Personen");
    personGroupBox->setLayout(personLayout);
    gridLayout->addWidget(personGroupBox, 3, 0, 1, 2);

    personPlusButton = new QPushButton("+");
    personPlusButton->setEnabled(false);
    connect(personPlusButton, &QPushButton::clicked, this, &FileDialog::on_PersonsPlusButton_clicked);
    gridLayout->addWidget(personPlusButton, 3, 2, Qt::AlignTop);

    keywordGroupBox = new QGroupBox("Schlagworte");
    keywordGroupBox->setLayout(keywordLayout);
    gridLayout->addWidget(keywordGroupBox, 4, 0, 1, 2);

    keywordPlusButton = new QPushButton("+");
    keywordPlusButton->setEnabled(false);
    connect(keywordPlusButton, &QPushButton::clicked, this, &FileDialog::on_KeywordPlusButton_clicked);
    gridLayout->addWidget(keywordPlusButton, 4, 2, Qt::AlignTop);


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
    gridLayout->addWidget(buttonBox, 5, 0, 1, 4, Qt::AlignRight);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);



    /*   ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
       ui->dateTimeEdit->setCalendarPopup(true);

       ui->pdfView->setDocument(doc);
       ui->pdfView->setPageMode(QPdfView::MultiPage);
       ui->pdfView->setZoomMode(QPdfView::FitInView);

        qDebug() << "Before List append";
       QLineEdit* firstPersonLineEdit = new QLineEdit();
       firstPersonLineEdit->setGeometry(10,20,150,20);
       personLayout->addWidget(firstPersonLineEdit);
       delete ui->PersonGroupBox->layout();
       ui->PersonGroupBox->setLayout(personLayout);

       QLineEdit* firstKeywordLineEdit = new QLineEdit();
       firstKeywordLineEdit->setGeometry(10,20,150,20);
       keywordLayout->addWidget(firstKeywordLineEdit);
       delete ui->KeywordGroupBox->layout();
       ui->KeywordGroupBox->setLayout(keywordLayout);

       personList->append(firstPersonLineEdit);
       keywordList->append(firstKeywordLineEdit);
        qDebug() << "After List append";
       ui->scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
       */
}

FileDialog::FileDialog(QWidget* parent, QString name, QByteArray* pdf, QStringList* persons, QStringList* keywords, QDateTime* date) :
    QDialog(parent),
    // ui(new Ui::FileDialog) ,
    personList(new QList<QLineEdit*>),
    keywordList(new QList<QLineEdit*>),
    doc(new QPdfDocument()),
    personLayout(new  QVBoxLayout()),
    keywordLayout(new  QVBoxLayout())
{
    qDebug() << "Begin Edit Dialog";
    //ui->setupUi(this);
   QGridLayout* gridLayout = new QGridLayout;

    setLayout(gridLayout);

    // scroller->setWidget(layout->widget());
    this->resize( 575, 325);

    QLabel* fileLabel = new QLabel("Datei :");
    gridLayout->addWidget(fileLabel, 0, 0);

    fileEdit = new QLineEdit();
    fileEdit->setEnabled(false);
    gridLayout->addWidget(fileEdit, 0, 1);

    QPushButton* fileButton = new QPushButton();
    fileButton->setText("Datei Oeffnen");
    fileButton->setEnabled(false);
    connect(fileButton, &QPushButton::clicked, this, &FileDialog::on_fileOpenButton_clicked);
    gridLayout->addWidget(fileButton, 0, 2);

    QPdfView* pdfView = new QPdfView();
    pdfView->setDocument(doc);
    pdfView->setPageMode(QPdfView::MultiPage);
    pdfView->setZoomMode(QPdfView::FitInView);
    gridLayout->addWidget(pdfView, 0, 3, 5, 1);
    gridLayout->setColumnStretch(3, 2);
    QBuffer* buffer = new QBuffer();
    buffer->open(QIODevice::ReadWrite);
    QDataStream* out = new QDataStream(buffer);
    *out << *pdf;
    doc->load(buffer);

    QLabel* nameLabel = new QLabel("Name :");

    //nameLabel->setGeometry(10,10,47,13);
    gridLayout->addWidget(nameLabel, 1, 0);

    nameEdit = new QLineEdit();
    //nameEdit->setGeometry()
    nameEdit->setEnabled(true);
    nameEdit->setText(name);
    gridLayout->addWidget(nameEdit, 1, 1, 1, 2);

    QLabel* dateLabel = new QLabel("Datum :");
    gridLayout->addWidget(dateLabel, 2, 0);

    dateTimeEdit = new QDateTimeEdit();
    
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setEnabled(true);
    dateTimeEdit->setDateTime(*date);
    gridLayout->addWidget(dateTimeEdit, 2, 1, 1, 2);

    personGroupBox = new QGroupBox("Personen");
    personGroupBox->setLayout(personLayout);
    gridLayout->addWidget(personGroupBox, 3, 0, 1, 2);

    personPlusButton = new QPushButton("+");
    personPlusButton->setEnabled(true);
    connect(personPlusButton, &QPushButton::clicked, this, &FileDialog::on_PersonsPlusButton_clicked);
    gridLayout->addWidget(personPlusButton, 3, 2, Qt::AlignTop);

    keywordGroupBox = new QGroupBox("Schlagworte");
    keywordGroupBox->setLayout(keywordLayout);
    gridLayout->addWidget(keywordGroupBox, 4, 0, 1, 2);

    keywordPlusButton = new QPushButton("+");
    keywordPlusButton->setEnabled(true);
    connect(keywordPlusButton, &QPushButton::clicked, this, &FileDialog::on_KeywordPlusButton_clicked);
    gridLayout->addWidget(keywordPlusButton, 4, 2, Qt::AlignTop);

    if (persons->count() > 0) {
        for (int i = 0; i < persons->count(); i++) {
            QLineEdit* newLineEdit = new QLineEdit();
            newLineEdit->setText(persons->at(i));
            personList->append(newLineEdit);
            personLayout->addWidget(newLineEdit);
        }
    }
    else {
        QLineEdit* firstPersonLineEdit = new QLineEdit();
        firstPersonLineEdit->setEnabled(true);
        //firstPersonLineEdit->setGeometry(10,20,150,20);
        personLayout->addWidget(firstPersonLineEdit, Qt::AlignTop);
        personLayout->setDirection(QBoxLayout::TopToBottom);
        personList->append(firstPersonLineEdit);
    }

    if (keywords->count() > 0) {
        for (int i = 0; i < keywords->count(); i++) {
            QLineEdit* newLineEdit = new QLineEdit();
            newLineEdit->setText(keywords->at(i));
            keywordList->append(newLineEdit);
            keywordLayout->addWidget(newLineEdit);
        }
    }
    else {
        QLineEdit* firstKeywordLineEdit = new QLineEdit();
        firstKeywordLineEdit->setEnabled(true);
        keywordLayout->addWidget(firstKeywordLineEdit, Qt::AlignTop);
        //firstKeywordLineEdit->setGeometry(10,20,150,20);
        keywordList->append(firstKeywordLineEdit);
    }


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setOrientation(Qt::Horizontal);
    gridLayout->addWidget(buttonBox, 5, 0, 1, 4, Qt::AlignRight);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

FileDialog::~FileDialog()
{
    //delete ui;
    doc->close();
}

void FileDialog::on_fileOpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::homePath(),
        tr("PDF (*.pdf)"));
    if (!fileName.isNull()) {
        doc->load(fileName);

        fileEdit->setText(fileName);

        dateTimeEdit->setDateTime(doc->metaData(QPdfDocument::CreationDate).toDateTime());
        nameEdit->setEnabled(true);
        dateTimeEdit->setEnabled(true);
        personPlusButton->setEnabled(true);
        keywordPlusButton->setEnabled(true);
        keywordList->first()->setEnabled(true);
        personList->first()->setEnabled(true);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);


    }
}


void FileDialog::on_PersonsPlusButton_clicked()
{
    QLineEdit* newLineEdit = new QLineEdit();
    //newLineEdit->setGeometry(personList->last()->x(),personList->last()->x()+30, personList->last()->width(),personList->last()->height());
    personList->append(newLineEdit);
    personLayout->addWidget(newLineEdit);

    //ui->PersonGroupBox->setGeometry(ui->PersonGroupBox->x(),ui->PersonGroupBox->y(),ui->PersonGroupBox->width(),ui->PersonGroupBox->height()+50);
    //ui->KeywordGroupBox->setGeometry(ui->KeywordGroupBox->x(), ui->KeywordGroupBox->y()+50,ui->KeywordGroupBox->width(),ui->KeywordGroupBox->height() );

}

void FileDialog::on_KeywordPlusButton_clicked()
{
    QLineEdit* newLineEdit = new QLineEdit();
    //newLineEdit->setGeometry(personList->last()->x(),personList->last()->x()+30, personList->last()->width(),personList->last()->height());
    keywordList->append(newLineEdit);
    keywordLayout->addWidget(newLineEdit);


    //ui->PersonGroupBox->setGeometry(ui->PersonGroupBox->x(),ui->PersonGroupBox->y(),ui->PersonGroupBox->width(),ui->PersonGroupBox->height()+50);
    //ui->KeywordGroupBox->setGeometry(ui->KeywordGroupBox->x(), ui->KeywordGroupBox->y()+50,ui->KeywordGroupBox->width(),ui->KeywordGroupBox->height() );

}

QStringList* FileDialog::getPersons() {
    QStringList* persons = new QStringList();
    for (int i = 0; i < personList->count(); i++) {
        if (personList->at(i)->text() != "")
            persons->append(personList->at(i)->text());
    }
    return persons;
}


QStringList* FileDialog::getKeywords() {
    QStringList* keywords = new QStringList();
    for (int i = 0; i < keywordList->count(); i++) {
        if (keywordList->at(i)->text() != "")
            keywords->append(keywordList->at(i)->text());
    }
    return keywords;
}

QString FileDialog::getName() {
    return nameEdit->text();
}

QString FileDialog::getFile() {
    return fileEdit->text();
}

QDateTime FileDialog::getDate() {
    return dateTimeEdit->dateTime();
}
