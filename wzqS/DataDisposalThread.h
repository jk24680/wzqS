#pragma once

#include <QThread>
#include "DataClass.h"


class DataDisposalThread : public QThread
{
	Q_OBJECT

public:
	DataDisposalThread(QObject *parent);
	~DataDisposalThread();
	//�������
	void setPara(QTcpSocket* tcpSocket_player);


signals:
    //���µ�ǰ������
    void recvMsg_updateCurConnCount();
    //�пͻ������󴴽�����
    void recvMsg_createRoom(QString);
    //�ͻ����˳�����
    void recvMsg_quitRoom(QString, QString);
    //�ͻ��˼�������
    void recvMsg_joinRoom(QString, QString);
    //��ҷ�����Ϣ����ʧ�Է�������
    void recvMsg_lostConn(QString);

protected:
    void run();


private:
    //TCPSocket ���ܵ���������Ϣ
    QTcpSocket* tcpSocket_player;

    //���ݰ�����
    msg_request_struct* msg_req_struct;

};
