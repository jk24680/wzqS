#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_wzqS.h"
#include <QTcpServer>
#include "ServerStatus.h"
#include "DataClass.h"


class wzqS : public QMainWindow
{
    Q_OBJECT

public:
    wzqS(QWidget *parent = Q_NULLPTR);

private:
    Ui::wzqSClass ui;
    //TCPServer ����client������
    QTcpServer* tcpServer_player;

    //��������������
    ServerStatus* serverStatus;

    //��ʼ���ؼ�����
    void widgetInit();

private slots:
    
    //�������Ӱ�ť
    void on_startServer_server_BTN_clicked();
    //�رշ���
    void stopService();
};
