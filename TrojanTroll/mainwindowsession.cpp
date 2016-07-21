#include "mainwindowsession.h"
#include "ui_mainwindowsession.h"

MainWindowSession::MainWindowSession(QWidget *parent, int clientSocket, struct sockaddr_in serverAddr, socklen_t addr_size) :
    QMainWindow(parent),
    ui(new Ui::MainWindowSession)
{
    ui->setupUi(this);
    this->clientSocket = clientSocket;
    this->serverAddr = serverAddr;
    this->addr_size = addr_size;
    console = new Console(this, clientSocket, serverAddr, addr_size);
    ui->gridLayout_6->addWidget(console);
}

MainWindowSession::~MainWindowSession()
{
    delete ui;
}

/* Send Text : 1_ */
void MainWindowSession::doSend()
{
    QString txt = ui->lineEdit->text();
    QString tmp = "1_";
    tmp.append(txt);
    /* Conversion de QString vers char* */
    QByteArray qba = tmp.toLatin1();
    const char* str = qba.data();

    /* Send message to server */
    sendto( clientSocket, str, BUF_SIZE, 0, (struct sockaddr *) &serverAddr, addr_size );

    /* Receive message from server */
    nBytes = recvfrom( clientSocket, buffer, BUF_SIZE, 0, NULL, NULL );

    qDebug() << "Sent: " << str << endl << "Received from server: " << buffer;
}
