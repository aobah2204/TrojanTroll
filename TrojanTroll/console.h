#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QDebug>
#include <QTextEdit>
#include <QMessageBox>
#include <QKeyEvent>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

namespace Ui {
class Console;
}

class Console : public QTextEdit
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent, int clientSocket, sockaddr_in serverAddr, socklen_t addr_size);
    ~Console();
    void upCursor();
    int nBytes;
    char buffer[BUF_SIZE];
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

private:
    Ui::Console *ui;

protected:
    virtual bool eventFilter(QObject* _o, QEvent* _e);
};

#endif // CONSOLE_H
