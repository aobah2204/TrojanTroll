#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent, int clientSocket, struct sockaddr_in serverAddr, socklen_t addr_size) :
    QTextEdit(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    ui->textEdit->setText("> ");
    upCursor();
    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);
    this->clientSocket = clientSocket;
    this->serverAddr = serverAddr;
    this->addr_size = addr_size;
}

Console::~Console()
{
    delete ui;
}

void Console::upCursor()
{
    ui->textEdit->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
    ui->textEdit->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
}

bool Console::eventFilter(QObject* _o, QEvent* _e)
{
    if(_e->type() == QEvent::KeyPress)
    {
        QKeyEvent* eventKey = static_cast<QKeyEvent*>(_e);
        if(eventKey->key() == Qt::Key_Return)
        {
            QString txt = ui->textEdit->toPlainText();
            QString type = "2_";
            QString temp = txt.mid(2);
            type.append(temp);

            /* Conversion de QString vers char* */
            QByteArray qba = type.toLatin1();
            const char* str = qba.data();

            /* Send message to server */
            sendto( clientSocket, str, BUF_SIZE, 0, (struct sockaddr *) &serverAddr, addr_size );
            QTextEdit::eventFilter(_o, _e);

            memset( buffer, 0, BUF_SIZE );
            /* Receive message from server */
            nBytes = recvfrom( clientSocket, buffer, BUF_SIZE, 0, NULL, NULL );
//            ui->textEdit->append("\n");
            ui->textEdit->append(buffer);
//            ui->textEdit->append("\n");
            ui->textEdit->append("> ");

            return true;
        }
    }
    return QTextEdit::eventFilter(_o, _e);
}
