#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Echo mode determines how the text entered into the widget
    // is displayed. In this case its displayed as dots instead
    // of plain text.
    ui->PasswordBox->setEchoMode(QLineEdit::Password);

    loadCommonPasswords();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// This is used to store common hashed passwords.
QSet <QString> commonHashes;

void MainWindow::loadCommonPasswords()
{

    // Open the file that was downloaded from Github.
    QFile file("C:/SE/Software_Engineering_GroupWork/Group_Work_Application/10-million-password-list-top-100000.txt");

    // This is used to open the file.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open common passwords file";
        return;
    }

    qDebug() << "Common passwords file opened successfully";

    // This used to read the file line by line and hash each password.
        QTextStream in(&file);

        // This while loop keeps reading the file line by line until it reaches the end.
        while (!in.atEnd())
        {
            // This reads a line from the file, removes any trailing whitespace,
            // and stores the resulting string in the variable line.
            QString line = in.readLine().trimmed();

            // This converts the strings to ts UTF-8 representation and hashes it using the SHA-256 algorithm.
            QByteArray hash = QCryptographicHash::hash(line.toUtf8(), QCryptographicHash::Sha256);

            // This converts it to a hexadecimal string.
            QString hashString = QString(hash.toHex());

            // This inserts the hash value as a string into the commonHashes created above this function.
            commonHashes.insert(hashString);

        }

        file.close();
    }

void MainWindow::hashFunction(QString password)
{
    //QByteArray is a class that represents a byte array.
    //QCryptographicHash is a class that computes cryptographic hashes.
    //The line below first converts the password to UTF-8 representation, and then
    //hashes the password using SHA-256 algorthim which is used to generate a fixed
    //size output of 256 bits, 32 bytes.
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    // The hash is then converted to hexadecimal string.
    QString hashString = QString(hash.toHex());

    // This if statement checks if the entered password matches any of the common hashes.
    if (commonHashes.contains(hashString))
    {
       QMessageBox::warning(this, tr("Result"), tr("The entered password is a common password."));

    } else
    {
        QMessageBox::information(this, tr("Result"), tr("The entered password is not a common password."));
    }
}

void MainWindow::on_actionAbout_triggered()
{
   QMessageBox::information(this, tr("About"), tr("Version 5.6"));
}


void MainWindow::on_actionHelp_triggered()
{
    // Used to display a help box when clicked on the help button.
    QMessageBox::information(this, "Help", "Contact us at: WeAreHereToHelp@gmail.com");
}


void MainWindow::on_actionExit_triggered()
{
    // Used to exit the program.
    close();
}


void MainWindow::on_checkPassButton_clicked()
{
    QString password = ui->PasswordBox->text();

    hashFunction(password);
}

