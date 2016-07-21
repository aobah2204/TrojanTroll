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

    mws = new MainWindowSession(this, clientSocket, serverAddr, addr_size);
    this->close();
    mws->show();
}
