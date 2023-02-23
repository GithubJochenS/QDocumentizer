#include "deletedialog.h"

DeleteDialog::DeleteDialog(QWidget* parent, QByteArray* pdf) : QDialog(parent)
{
    resize(300, 450);
    QVBoxLayout* vBoxlayout = new QVBoxLayout();
    setLayout(vBoxlayout);

    QPdfDocument* doc = new QPdfDocument();
    QPdfView* pdfView = new QPdfView();
    pdfView->setDocument(doc);
    pdfView->setPageMode(QPdfView::MultiPage);
    //pdfView->setZoomMode(QPdfView::FitInView); //Lags the Widget with certin PDFs at 450 Height but not on 350 <---???????
    vBoxlayout->addWidget(pdfView, 4);
    QBuffer* buffer = new QBuffer();
    buffer->open(QIODevice::ReadWrite);
    QDataStream* out = new QDataStream(buffer);
    *out << *pdf;
    doc->load(buffer);

    QLabel* confirmLabel = new QLabel("Wollen Sie die Datei wirklich loeschen?");
    vBoxlayout->addWidget(confirmLabel);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setOrientation(Qt::Horizontal);
    vBoxlayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    pdfView->setZoomMode(QPdfView::CustomZoom);
    pdfView->setZoomFactor(0.4);

}


