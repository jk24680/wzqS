#pragma once
#include <QtWidgets/QMainWindow>

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDataStream>



enum playerRole
{
    //用于记录某个棋子是谁下的
    HOST,
    GUEST
};

enum playerStatus
{
    //记录当前是正在游戏还是在等待游戏
    playingGame,
    readyToPlayingGame
};

enum comm_request_type
{
    //连接服务器失败
    COMM_SERVER_CONN_FAILED,
    //连接服务器成功
    COMM_SERVER_CONN_SUCCESSFUL,
    //服务器关闭
    COMM_SERVER_CLOSE,
    //服务器发送大厅信息
    COMM_SERVER_GAMEINFO,

    //退出游戏
    COMM_CLIENT_QUITGAME,
    //请求断开服务器
    COMM_CLIENT_DISCONN,
    //客户端请求连接服务器
    COMM_CLIENT_CONN,
    //客户端请求创建主机
    COMM_CLIENT_CREATE,
    //客户端请求加入游戏
    COMM_CLIENT_JOIN,
    //双方准备完毕，游戏开始
    COMM_CLIENT_GAMESTART,
    //某方玩家胜利，游戏结束
    COMM_CLIENT_GAMEOVER,
    //玩家游戏操作：落子
    COMM_CLIENT_GAMEOP,
    //玩家游戏操作：发送聊天信息
    COMM_CLIENT_CHAT,
    //玩家游戏操作：悔棋
    COMM_CLIENT_UNDO,
    //玩家游戏操作：悔棋回复yes
    COMM_CLIENT_UNDO_YES,
    //玩家游戏操作：悔棋回复no
    COMM_CLIENT_UNDO_NO,
    //玩家游戏操作：认输
    COMM_CLIENT_LOSE,
    //玩家发来消息，对方掉线
    COMM_CLIENT_LOSTCONN
};

struct msg_request_struct
{
    //请求类型
    qint8 request;
    //数据
    QString data;
};

class DataClass
{
public:
    //服务端接受的最大连接数
    static int maxConnectionLimit;

    //当前服务器连接数
    static int curConnCount;

    //服务端，客户端连接端口
    static int connPort;

    //正则表达式：端口，IP，整数
    static const QRegExp regPort;
    static const QRegExp regIP;
    static const QRegExp regConnLimit;

    //每个五子棋小方框的高宽
    static const int paneWidth;
    static const int paneHeight;

    //确认对话框
    static bool checkDialog(QString title, QString context);

    //debug messagebox
    static void dbgBox(int lPara, int rPara);
    static void dbgBox(int lPara, QString rPara);
    static void dbgBox(QString lPara, QString rPara);

    //发送数据
    static void sendMsg(comm_request_type type, QString context, QTcpSocket* _tcpSocket);
};
