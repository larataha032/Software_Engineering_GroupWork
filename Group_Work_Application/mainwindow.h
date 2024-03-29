#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString logFilePath;

private slots:
    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

    void on_actionExit_triggered();

    void on_checkPassButton_clicked();

    void logThis(QString messageToLog);

    QString formatTime();

private:
    Ui::MainWindow *ui;

    void hashFunction(QString password);

    QSet<QString> commonHashes;

    void loadCommonPasswords();

    bool Requirements(QString password);
};
#endif // MAINWINDOW_H
