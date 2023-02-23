#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QtPdfWidgets>


namespace Ui {
    class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget* parent = nullptr);
    FileDialog(QWidget* parent, QString name, QByteArray* pdf, QStringList* persons, QStringList* keywords, QDateTime* date);
    ~FileDialog();
    QStringList* getKeywords();
    QStringList* getPersons();
    QString getName();
    QDateTime getDate();
    QString getFile();

private slots:
    void on_fileOpenButton_clicked();
    void on_KeywordPlusButton_clicked();
    void on_PersonsPlusButton_clicked();

private:
    QList<QLineEdit*>* personList;
    QList<QLineEdit*>* keywordList;
    QPdfDocument* doc;
    QVBoxLayout* personLayout;
    QVBoxLayout* keywordLayout;
    QLineEdit* nameEdit;
    QLineEdit* fileEdit;
    QDateTimeEdit* dateTimeEdit;
    QGroupBox* personGroupBox;
    QGroupBox* keywordGroupBox;
    QPushButton* personPlusButton;
    QPushButton* keywordPlusButton;
    QDialogButtonBox* buttonBox;

    

};

#endif // FILEDIALOG_H
