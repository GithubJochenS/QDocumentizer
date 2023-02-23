#pragma once
#include <qdialog.h>
#include <QLineEdit>
#include <QLabel>


class PasswordDialog :
    public QDialog
{
    public :
        PasswordDialog(QString errMsg = "", QWidget* parent = nullptr);
        QString getPassword();
        void reject();
    private :
        QLineEdit* pwLineEdit;
        QLabel* errorMsgLabel;

    private slots :
        void validatePassword();



};

