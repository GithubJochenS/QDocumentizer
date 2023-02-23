#pragma once
#include <QDialog>
#include <QLineEdit>
#include <Qlabel>


class NewPasswordDialog :
    public QDialog
{
    public :
        NewPasswordDialog(QWidget* parent = nullptr);
        QString getPassword();
        QString getDir();
        void reject();

    private :
        QLineEdit* dirEdit;
        QLineEdit* pwOneEdit;
        QLineEdit* pwTwoEdit;
        QLabel* errorMsgLabel;

private slots :
        void validatePasswords();
        void selectDir();
};

