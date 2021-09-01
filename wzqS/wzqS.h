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
    //TCPServer 接收client的请求
    QTcpServer* tcpServer_player;

    //服务器大厅窗口
    ServerStatus* serverStatus;

    //初始化控件函数
    void widgetInit();

private slots:
    
    //开启连接按钮
    void on_startServer_server_BTN_clicked();
    //关闭服务
    void stopService();
};
