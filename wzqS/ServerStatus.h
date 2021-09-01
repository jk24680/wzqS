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

	//传入参数
	void setPara(QTcpServer* _tcpServer_player);


protected:
	//响应关闭事件
	void closeEvent(QCloseEvent* event);

private slots:
    void on_closeServer_server_BTN_clicked();

    //收到新连接
    void getNewConn();

    //更新当前在线玩家
    void recvMsg_updateCurConnCount();
    //客户端请求创建主机
    void recvMsg_createRoom(QString);
    //客户端退出主机
    void recvMsg_quitRoom(QString, QString);
    //客户端加入主机
    void recvMsg_joinRoom(QString, QString);
    //玩家发来消息，丢失对方的连接
    void recvMsg_lostConn(QString);

signals:
    void stopService();

private:
	Ui::ServerStatus ui;

    //数据结构
    msg_request_struct* msg_req_struct;

    //保存玩家对战状态
    QList<QPair<QString, QString> >playerFightInfo;

    //TCPServer 接受client的请求
    QTcpServer* tcpServer_player;

    //初始化控件属性
    void widgetInit();

    //发送大厅信息
    void sendGameInfo(QTcpSocket* tcpSocket);

    //更新大厅信息
    void updateGameInfo();
};
