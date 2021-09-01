#include "wzqS.h"


wzqS::wzqS(QWidget *parent)
    : QMainWindow(parent),tcpServer_player(NULL),serverStatus(NULL)
{
    ui.setupUi(this);

    widgetInit();

}

//����UI��ʼ��
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
    //�ж�IP�Ͷ˿��Ƿ�����Ϊ��
    if (ui.connPort_server_LE->text() == "" || ui.connLimit_server_LE->text() == "")
    {
        QMessageBox::information(this, "����", "�������������Ϣ��");
        return;
    }

    //������������������
    tcpServer_player = new QTcpServer(this);

    //���ü����˿ڲ��ж��Ƿ�����ɹ�
    if (!(tcpServer_player->listen(QHostAddress::Any, ui.connPort_server_LE->text().toInt())))
    {
        QMessageBox::information(this, "����", "�����������˿�ʧ�ܣ�");
        return;
    }

    //��ȡ���������������
    DataClass::maxConnectionLimit = ui.connLimit_server_LE->text().toInt();
    //��ȡ�����������Ķ˿ں�
    DataClass::connPort = ui.connPort_server_LE->text().toInt();
    //������������������
    serverStatus = new ServerStatus();
    //�������ķ�����server�����������������
    serverStatus->setPara(tcpServer_player);
    //���ܵ��µ�����
    connect(tcpServer_player, SIGNAL(newConnection()), serverStatus, SLOT(getNewConn()));
    connect(serverStatus, SIGNAL(stopService()), this, SLOT(stopService()));
    serverStatus->show();
    this->hide();
}
