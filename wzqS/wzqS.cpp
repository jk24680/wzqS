#include "wzqS.h"


wzqS::wzqS(QWidget *parent)
    : QMainWindow(parent),tcpServer_player(NULL),serverStatus(NULL)
{
    ui.setupUi(this);

    widgetInit();

}

//界面UI初始化
void wzqS::widgetInit()
{
    ui.connPort_server_LE->setValidator(new QRegExpValidator(DataClass::regPort, this));
    ui.connLimit_server_LE->setValidator(new QRegExpValidator(DataClass::regConnLimit, this));
}

void wzqS::stopService()
{
    DataClass::curConnCount = 0;
    if (serverStatus)
    {
        delete serverStatus;
        serverStatus = NULL;
    }
    this->show();
}

void wzqS::on_startServer_server_BTN_clicked()
{
    //判断IP和端口是否输入为空
    if (ui.connPort_server_LE->text() == "" || ui.connLimit_server_LE->text() == "")
    {
        QMessageBox::information(this, "错误", "请先输入相关信息！");
        return;
    }

    //建立服务器监听对象
    tcpServer_player = new QTcpServer(this);

    //设置监听端口并判断是否监听成功
    if (!(tcpServer_player->listen(QHostAddress::Any, ui.connPort_server_LE->text().toInt())))
    {
        QMessageBox::information(this, "错误", "服务器监听端口失败！");
        return;
    }

    //获取服务器最大连接数
    DataClass::maxConnectionLimit = ui.connLimit_server_LE->text().toInt();
    //获取服务器监听的端口号
    DataClass::connPort = ui.connPort_server_LE->text().toInt();
    //建立服务器大厅对象
    serverStatus = new ServerStatus();
    //将监听的服务器server传入服务器大厅对象
    serverStatus->setPara(tcpServer_player);
    //接受到新的连接
    connect(tcpServer_player, SIGNAL(newConnection()), serverStatus, SLOT(getNewConn()));
    connect(serverStatus, SIGNAL(stopService()), this, SLOT(stopService()));
    serverStatus->show();
    this->hide();
}
