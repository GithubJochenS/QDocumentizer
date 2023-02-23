#include "PasswordDialog.h"
#include "qlayout.h"
#include <QPushButton>

PasswordDialog::PasswordDialog(QString errMsg, QWidget* parent) : QDialog(parent) {
	QGridLayout* gridLayout = new QGridLayout(this);

	errorMsgLabel = new QLabel(errMsg, this);
	errorMsgLabel->setStyleSheet("QLabel {color : red}");
	gridLayout->addWidget(errorMsgLabel, 0, 0, 1, 2);

	gridLayout->addWidget(new QLabel("Passwort :"), 1, 0);
	pwLineEdit = new QLineEdit(this);
	pwLineEdit->setEchoMode(QLineEdit::Password);
	gridLayout->addWidget(pwLineEdit, 1, 1);

	QPushButton* okButton = new QPushButton("OK", this);
	gridLayout->addWidget(okButton, 2, 1, Qt::AlignRight);
	connect(okButton, &QAbstractButton::clicked, this, &PasswordDialog::validatePassword);
}


QString PasswordDialog::getPassword() {
	return pwLineEdit->text();
}

void PasswordDialog::validatePassword() {
	if (pwLineEdit->text() == "") {
		errorMsgLabel->setText("Passwort darf nicht leer sein");
	}
	else
		this->accept();
}

void PasswordDialog::reject() {
	errorMsgLabel->setText("You need to enter a Password");
	//maybe Dialog to ask if you want to setup a new DB
}
