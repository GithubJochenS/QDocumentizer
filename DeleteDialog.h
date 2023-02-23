#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>
#include <QtPdfWidgets>

class DeleteDialog : public QDialog
{
public:
    DeleteDialog(QWidget* parent, QByteArray* pdf);

};

#endif // DELETEDIALOG_H
