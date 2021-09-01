#pragma once

#include <QDialog>
#include "ui_ServerStatus.h"
#include "DataClass.h"
#include "DataDisposalThread.h"


class ServerStatus : public QDialog
{
	Q_OBJECT

public:
	ServerStatus(QWidget *parent = Q_NULLPTR);
	~ServerStatus();

	//�������
	void setPara(QTcpServer* _tcpServer_player);


protected:
	//��Ӧ�ر��¼�
	void closeEvent(QCloseEvent* event);

private slots:
    void on_closeServer_server_BTN_clicked();

    //�յ�������
    void getNewConn();

    //���µ�ǰ�������
    void recvMsg_updateCurConnCount();
    //�ͻ������󴴽�����
    void recvMsg_createRoom(QString);
    //�ͻ����˳�����
    void recvMsg_quitRoom(QString, QString);
    //�ͻ��˼�������
    void recvMsg_joinRoom(QString, QString);
    //��ҷ�����Ϣ����ʧ�Է�������
    void recvMsg_lostConn(QString);

signals:
    void stopService();

private:
	Ui::ServerStatus ui;

    //���ݽṹ
    msg_request_struct* msg_req_struct;

    //������Ҷ�ս״̬
    QList<QPair<QString, QString> >playerFightInfo;

    //TCPServer ����client������
    QTcpServer* tcpServer_player;

    //��ʼ���ؼ�����
    void widgetInit();

    //���ʹ�����Ϣ
    void sendGameInfo(QTcpSocket* tcpSocket);

    //���´�����Ϣ
    void updateGameInfo();
};
