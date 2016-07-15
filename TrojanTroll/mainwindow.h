#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <QMainWindow>
#include <QDebug>
#include <QString>

#define BUF_SIZE 1024

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int clientSocket;
    int portNum;
    int nBytes;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

private:
    Ui::MainWindow *ui;

private slots:
    void doConnect();
    void doSend();
};

#endif // MAINWINDOW_H
