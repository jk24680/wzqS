#include "DataClass.h"


//初始化最大连接数
int DataClass::maxConnectionLimit = 0;
//初始化当前连接数
int DataClass::curConnCount = 0;

//初始化正则表达式
const QRegExp DataClass::regPort = QRegExp("([1-9])|([1-9]\\d{3})|([1-5]\\d{4})|(6[1-4]\\d{3})|(65[1-4]\\d{2})|(655[1-2]\\d)|(6553[1-5])");
const QRegExp DataClass::regIP = QRegExp("([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}");
const QRegExp DataClass::regConnLimit = QRegExp("([0-9]|[1-9][0-9]{2})");

int DataClass::connPort = -1;

bool DataClass::checkDialog(QString title, QString context)
{
    QMessageBox msgBox(title, context, QMessageBox::Question,
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape,
        QMessageBox::NoButton);
    if (msgBox.exec() == QMessageBox::No)
        return false;
    else
        return true;
}

void DataClass::sendMsg(comm_request_type type, QString context, QTcpSocket* _tcpSocket)
{
    msg_request_struct msg_req_struct;
    msg_req_struct.request = type;    //请求类型
    msg_req_struct.data = context;    //请求数据

    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out << msg_req_struct.request << msg_req_struct.data;
    _tcpSocket->write(buff);  //发送给客户端
}

void DataClass::dbgBox(int lPara, int rPara)
{
    QMessageBox::information(NULL, QString::number(lPara), QString::number(rPara));
}

void DataClass::dbgBox(int lPara, QString rPara)
{
    QMessageBox::information(NULL, QString::number(lPara), rPara);
}

void DataClass::dbgBox(QString lPara, QString rPara)
{
    QMessageBox::information(NULL, lPara, rPara);
}
