#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doSettings( char* address, int port )
{
    /* Create UDP socket */
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /* Configure settings in address struct */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(address);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /* Initialize size variable to be used later on */
    addr_size = sizeof serverAddr;
}

void MainWindow::doSend( char* message )
{
    nBytes = strlen(buffer) + 1;

    /* Send message to server */
    sendto( clientSocket, message, nBytes, 0, (struct sockaddr *) &serverAddr, addr_size );

    /* Receive message from server */
    nBytes = recvfrom( clientSocket, buffer, BUF_SIZE, 0, NULL, NULL );

    qDebug() << "Received from server: %s\n" << buffer;
}
