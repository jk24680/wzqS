#pragma once

#include <QThread>
#include "DataClass.h"


class DataDisposalThread : public QThread
{
	Q_OBJECT

public:
	DataDisposalThread(QObject *parent);
	~DataDisposalThread();
	//传入参数
	void setPara(QTcpSocket* tcpSocket_player);


signals:
    //更新当前连接数
    void recvMsg_updateCurConnCount();
    //有客户端请求创建主机
    void recvMsg_createRoom(QString);
    //客户端退出主机
    void recvMsg_quitRoom(QString, QString);
    //客户端加入主机
    void recvMsg_joinRoom(QString, QString);
    //玩家发来消息，丢失对方的连接
    void recvMsg_lostConn(QString);

protected:
    void run();


private:
    //TCPSocket 接受到了数据信息
    QTcpSocket* tcpSocket_player;

    //数据包申明
    msg_request_struct* msg_req_struct;

};
