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

/* Send Text : 1_ */
void MainWindow::doSend()
{
    QString txt = ui->lineEdit_3->text();
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

/* Send Command : 2_ */
void MainWindow::doSendCommand()
{
    QString requete = ui->lineEdit_4->text();
    QString tmp = "2_";
    tmp.append(requete);
    /* Conversion de QString vers char* */
    QByteArray qba = tmp.toLatin1();
    const char* str = qba.data();

    if( sendto(clientSocket, str, strlen(str)+1, 0, (struct sockaddr *) &serverAddr, addr_size) < 0 ) {
        perror("Envoi requete");
        exit(1);
    }

    if( (longueur_reponse = recvfrom( clientSocket, buffer, BUF_SIZE, 0, NULL, NULL )) < 0 ) {
        perror("Attente réponse");
        exit(1);
    }
    qDebug() << "REPONSE> " << buffer;
}
