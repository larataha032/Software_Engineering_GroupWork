#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>

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

    ui->Password_Length->setValue(15);//inital length of 15 characters

    QDir logFileDirectory;
    logFilePath = logFileDirectory.absoluteFilePath("C:/SE_EasyAccessFiles/PasswordVerificationLog.txt");
    logThis(" - ACTION -- Application started.\n");
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
    QFile file("C:/Software_Engineering/Software_Engineering_GroupWork/Group_Work_Application/10-million-password-list-top-100000.txt");

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

    if (Requirements(password) == true){
        QMessageBox::information(this,tr("temp"),tr("If you are reading this, the password is solid :)"));
    }
}

bool MainWindow::Requirements(QString password) {

    int length = ui->Password_Length->value();//stores the required length of the password into a variable
    int passlength = password.length(); //stores the actual length of the password into a variable

    if (passlength < length){//checks if the password length is less than the required length
        QMessageBox::warning(this, tr("Uh-Oh :("), tr("The password you entered is too short.\n"
                                                     "Please enter a valid password."));
        return false;
    }

    //Below is a list of regular expressions for capital letters, lowercase letters, numbers, and special characters.
    //By creating these variables, we can check to see if the password contains any of these expressions. If it does,
    //the function will return ture, if it doesnt, it will return false.
    QRegularExpression Capital("[A-Z]");
    QRegularExpression Lower("[a-z]");
    QRegularExpression Number("\\d");
    QRegularExpression SpecialCharacter("[!@#$%&*]");

    bool Cap = password.contains(Capital);
    bool Low = password.contains(Lower);
    bool Num = password.contains(Number);
    bool Special = password.contains(SpecialCharacter);

    if (Cap == false || Low == false || Num == false || Special == false){
        QMessageBox::warning(this, tr("Oopsies :("), tr("The password you entered did not meet the specified"
                                             "requirements.\n''Please try again"));
        return false;
    }

    return true;
}


void MainWindow::logThis(QString messageToLog)
{
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::Append)){
        QTextStream logMessage(&logFile);
        logMessage << formatTime() << messageToLog;
        logFile.close();
    }
}


QString MainWindow::formatTime()
{
    // Called at the beginning of each log entry
    // retrieves current time, formats it into standard format, and returns as a string
    QDateTime currentDT = QDateTime::currentDateTime();
    QString formattedDateTime = currentDT.toString("yyyyMMdd_HH:mm:ss");
    return formattedDateTime;
}
