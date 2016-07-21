#ifndef MAINWINDOWSESSION_H
#define MAINWINDOWSESSION_H

#include <QMainWindow>
#include <QDebug>
#include "console.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

namespace Ui {
class MainWindowSession;
}

class MainWindowSession : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowSession(QWidget *parent, int clientSocket, struct sockaddr_in serverAddr, socklen_t addr_size);
    ~MainWindowSession();
    int nBytes;
    char buffer[BUF_SIZE];
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

private:
    Ui::MainWindowSession *ui;
    Console *console;

private slots:
    void doSend();
};

#endif // MAINWINDOWSESSION_H
