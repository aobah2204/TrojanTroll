#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->connected_txt->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doConnect()
{
    int port = ui->spinBox->value();
    QString address = ui->lineEdit->text();
    /* Conversion de QString vers char* */
    QByteArray qba = address.toLatin1();
    const char* adr = qba.data();
//    QString password = ui->lineEdit_2->text();

    /* Create UDP socket */
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /* Configure settings in address struct */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(adr);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /* Initialize size variable to be used later on */
    addr_size = sizeof serverAddr;

    ui->connected_txt->show();
}

void MainWindow::doSend()
{
    QString txt = ui->lineEdit_3->text();
    /* Conversion de QString vers char* */
    QByteArray qba = txt.toLatin1();
    const char* str = qba.data();

    nBytes = strlen(buffer) + 1;

    /* Send message to server */
    sendto( clientSocket, str, nBytes, 0, (struct sockaddr *) &serverAddr, addr_size );

    /* Receive message from server */
    nBytes = recvfrom( clientSocket, buffer, BUF_SIZE, 0, NULL, NULL );

    qDebug() << "Received from server: %s\n" << buffer;
}
