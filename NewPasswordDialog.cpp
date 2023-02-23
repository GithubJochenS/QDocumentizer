#include "NewPasswordDialog.h"
#include <QLayout>
#include <qpushbutton.h>
#include <qdebug.h>
#include <qdir.h>
#include <QFileDialog>

NewPasswordDialog::NewPasswordDialog(QWidget* parent) : QDialog(parent) {
	QGridLayout* gridLayout = new QGridLayout(this);
	setLayout(gridLayout);

	errorMsgLabel = new QLabel();
	errorMsgLabel->setStyleSheet("QLabel {color : red}");
	gridLayout->addWidget(errorMsgLabel,0,0,1,2);

	dirEdit = new QLineEdit((QDir::homePath()).append("/.QDocumentizer"),this);
	dirEdit->setEnabled(false);
	gridLayout->addWidget(dirEdit, 1, 0);

	QPushButton* dirSelectButton = new QPushButton(*new QIcon(":/icons/D:/icons/icons-master/places/48/folder-documents-open.svg"),"",this);
	gridLayout->addWidget(dirSelectButton,1,1);
	connect(dirSelectButton, &QAbstractButton::clicked, this, &NewPasswordDialog::selectDir);

	gridLayout->addWidget(new QLabel("Passwort :"),2,0);
	pwOneEdit = new QLineEdit(this);
	pwOneEdit->setEchoMode(QLineEdit::Password);
	gridLayout->addWidget(pwOneEdit,2,1,1,2);
	
	gridLayout->addWidget(new QLabel("Passwort (wiederholen):"), 3, 0);
	pwTwoEdit = new QLineEdit(this);
	pwTwoEdit->setEchoMode(QLineEdit::Password);
	gridLayout->addWidget(pwTwoEdit, 3, 1,1,2);

	QPushButton* okButton = new QPushButton("OK", this);
	//QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	//buttonBox->setOrientation(Qt::Horizontal);
	gridLayout->addWidget(okButton, 4, 2, Qt::AlignRight);
	connect(okButton, &QAbstractButton::clicked, this, &NewPasswordDialog::validatePasswords);
	//connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString NewPasswordDialog::getPassword() {
	return pwOneEdit->text();
}

QString NewPasswordDialog::getDir() {
	return dirEdit->text();
}

void NewPasswordDialog::validatePasswords() {
	qDebug() << "validatePasswords() ";
	if (pwOneEdit->text() == "") {
		errorMsgLabel->setText("Passwort darf nicht leer sein");
	}
	else if (pwOneEdit->text() != pwTwoEdit->text()) {
		errorMsgLabel->setText("Passwoerter Stimmen nicht ueberein");
	}
	else 
		this->accept();
}

void NewPasswordDialog::selectDir() {
	QString dirStr = QFileDialog::getExistingDirectory(this,"Datenbank oeffnen",QDir::homePath().append("/.QDocumentizer/"));
	QFileInfo selectedDir(dirStr);

	if (selectedDir.isDir()) {
		if (selectedDir.isWritable()) {
			dirEdit->setText(dirStr);
		}
		else {
			errorMsgLabel->setText("Directory is not writable");
		}
	}
	else {
		errorMsgLabel->setText("You selected a File");
	}
}

void NewPasswordDialog::reject() {
	errorMsgLabel->setText("You need to enter a Password");
}
