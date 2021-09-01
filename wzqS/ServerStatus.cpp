#include "ServerStatus.h"

ServerStatus::ServerStatus(QWidget *parent)
	: QDialog(parent), tcpServer_player(NULL)
{
	ui.setupUi(this);
	widgetInit();
}

ServerStatus::~ServerStatus()
{
    if (tcpServer_player)
    {
        QList<QTcpSocket*> allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
        for (int i = 0; i < allTcpSocket.size(); i++)
        {
            allTcpSocket[i]->close();
            allTcpSocket[i] = NULL;
        }
        tcpServer_player->close();
        tcpServer_player = NULL;
    }
}

void ServerStatus::closeEvent(QCloseEvent* event)
{
    if (!DataClass::checkDialog(QString::fromLocal8Bit("�˳�"), QString::fromLocal8Bit( "ȷ�Ϲرշ���")))
    {
        event->ignore();
        return;
    }
    emit stopService();
}

void ServerStatus::widgetInit()
{
    ui.curConnCount_server_LE->setText("0");
    updateGameInfo();
}

void ServerStatus::setPara(QTcpServer* _tcpServer_player)
{
    tcpServer_player = _tcpServer_player;
}

void ServerStatus::on_closeServer_server_BTN_clicked()
{
    if (!DataClass::checkDialog(QString::fromLocal8Bit("�˳�"), QString::fromLocal8Bit( "ȷ�Ϲرշ���")))
        return;
    emit stopService();
}

void ServerStatus::getNewConn()
{
    QTcpSocket* tcpSocket_player = tcpServer_player->nextPendingConnection();
    //	DataClass::dbgBox(1,tcpSocket_player->peerAddress().toString());

        //��ǰ����������1
    DataClass::curConnCount++;
    if (DataClass::curConnCount > DataClass::maxConnectionLimit)
    {
        //�������ﵽ�������������,�ջ�õ�QTcpSocketҪ�ر�
        DataClass::curConnCount--;
        tcpSocket_player->close();
        tcpSocket_player = NULL;
        return;
    }

    //��ǰ��������������ӷ������Ŀͻ�������
    ui.curConnCount_server_LE->setText(QString::number(DataClass::curConnCount));


    //��ͻ��˷��ʹ�����Ϣ�����������������
    sendGameInfo(tcpSocket_player);
    DataDisposalThread* dataDisposalThread = new DataDisposalThread(NULL);
    dataDisposalThread->setPara(tcpSocket_player);
    connect(dataDisposalThread, SIGNAL(recvMsg_updateCurConnCount()), this, SLOT(recvMsg_updateCurConnCount()));
    connect(dataDisposalThread, SIGNAL(recvMsg_createRoom(QString)), this, SLOT(recvMsg_createRoom(QString)));
    connect(dataDisposalThread, SIGNAL(recvMsg_quitRoom(QString, QString)), this, SLOT(recvMsg_quitRoom(QString, QString)));
    connect(dataDisposalThread, SIGNAL(recvMsg_joinRoom(QString, QString)), this, SLOT(recvMsg_joinRoom(QString, QString)));
    connect(dataDisposalThread, SIGNAL(recvMsg_lostConn(QString)), this, SLOT(recvMsg_lostConn(QString)));
    connect(tcpSocket_player, SIGNAL(readyRead()), dataDisposalThread, SLOT(start()));

    //�������ݹ���ʱ��Ӧ�ĺ���
    //connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(getNewData()));
}

void ServerStatus::recvMsg_updateCurConnCount()
{
    ui.curConnCount_server_LE->setText(QString::number(DataClass::curConnCount));
}

void ServerStatus::recvMsg_createRoom(QString clientAddr)
{
    QString qsNull = "-";
    playerFightInfo.push_back(qMakePair(clientAddr, qsNull));
    updateGameInfo();
    QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
    for (int i = 0; i < allTcpSocket.size(); i++)
        sendGameInfo(allTcpSocket[i]);
}

void ServerStatus::sendGameInfo(QTcpSocket* tcpSocket)
{
    QString data;
    for (int i = 0; i < playerFightInfo.size(); i++)
        data += playerFightInfo[i].first + " " + playerFightInfo[i].second + "_";

    DataClass::sendMsg(COMM_SERVER_GAMEINFO, data, tcpSocket);
}

void ServerStatus::recvMsg_quitRoom(QString plyRole, QString clientAddr)
{
    if (plyRole == "GUEST")
    {
        for (int i = 0; i < playerFightInfo.size(); i++)
        {
            if (playerFightInfo[i].second == clientAddr)
            {
                playerFightInfo[i].second = "-";
                updateGameInfo();
                //�Ż���ֻ���͸�����������ð����еĶ�����ȥ
                QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
                for (int i = 0; i < allTcpSocket.size(); i++)
                    sendGameInfo(allTcpSocket[i]);
            }
        }
    }
    else if (plyRole == "HOST")
    {
        for (int i = 0; i < playerFightInfo.size(); i++)
        {
            if (playerFightInfo[i].first == clientAddr)
            {
                playerFightInfo.removeAt(i);
                updateGameInfo();
                //�Ż���ֻ���͸�����������ð����еĶ�����ȥ
                QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
                for (int i = 0; i < allTcpSocket.size(); i++)
                    sendGameInfo(allTcpSocket[i]);
            }
        }
    }
}

void ServerStatus::recvMsg_joinRoom(QString hostAddr, QString guestAddr)
{
    for (int i = 0; i < playerFightInfo.size(); i++)
    {
        if (playerFightInfo[i].first == hostAddr)
        {
            playerFightInfo[i].second = guestAddr;
            updateGameInfo();//��������ʾ��ÿһ�����ݸ���
            QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
            for (int i = 0; i < allTcpSocket.size(); i++)
            {
                sendGameInfo(allTcpSocket[i]);
                if (allTcpSocket[i]->peerAddress().toString() == hostAddr)
                    DataClass::sendMsg(COMM_CLIENT_JOIN, guestAddr, allTcpSocket[i]);
            }
        }
    }
}

void ServerStatus::recvMsg_lostConn(QString msg)
{
    DataClass::curConnCount--;
    recvMsg_updateCurConnCount();
    if (msg[0] == 'H')
    {
        QString plyAddr = msg.mid(5, msg.size() - 5);
        for (int i = 0; i < playerFightInfo.size(); i++)
        {
            if (playerFightInfo[i].first == plyAddr)
            {
                playerFightInfo.removeAt(i);
                updateGameInfo();
                //�Ż���ֻ���͸�����������ð����еĶ�����ȥ
                QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
                for (int i = 0; i < allTcpSocket.size(); i++)
                {
                    //code here
                    //��Ϊ��������Ļ���IP����127.0.0.1������������ܲ���
// 					if(allTcpSocket[i]->peerAddress().toString() == plyAddr)
// 					{
// 						allTcpSocket[i]->close();
// 						allTcpSocket[i] = NULL;
// 					}
// 					else
                    sendGameInfo(allTcpSocket[i]);
                }
            }
        }
    }
    else if (msg[0] == 'G')
    {
        QString plyAddr = msg.mid(6, msg.size() - 6);
        for (int i = 0; i < playerFightInfo.size(); i++)
        {
            if (playerFightInfo[i].second == plyAddr)
            {
                playerFightInfo[i].second = "-";
                updateGameInfo();
                //�Ż���ֻ���͸�����������ð����еĶ�����ȥ
                QList<QTcpSocket* > allTcpSocket = tcpServer_player->findChildren<QTcpSocket*>();
                for (int i = 0; i < allTcpSocket.size(); i++)
                {
                    //code here
                    //��Ϊ��������Ļ���IP����127.0.0.1������������ܲ���
// 					if(allTcpSocket[i]->peerAddress().toString() == plyAddr)
// 					{
// 						allTcpSocket[i]->close();
// 						allTcpSocket[i] = NULL;
// 					}
// 					else
                    sendGameInfo(allTcpSocket[i]);
                }
            }
        }
    }
}

//����������ÿ����Ϣ����
void ServerStatus::updateGameInfo()
{
    ui.clientStatus_server_TBW->setRowCount(0);
    for (int i = 0; i < playerFightInfo.size(); i++)
    {
        QTableWidgetItem* player1, * player2, * playStatus;
        int row = ui.clientStatus_server_TBW->rowCount();
        ui.clientStatus_server_TBW->setRowCount(row + 1);
        player1 = new QTableWidgetItem(playerFightInfo[i].first);
        player1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter | Qt::AlignCenter);
        ui.clientStatus_server_TBW->setItem(row, 0, player1);

        player2 = new QTableWidgetItem(playerFightInfo[i].second);
        player2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter | Qt::AlignCenter);
        ui.clientStatus_server_TBW->setItem(row, 1, player2);

        playStatus = new QTableWidgetItem(playerFightInfo[i].second == "-" ? "�ȴ����" : "��ս��");
        playStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter | Qt::AlignCenter);
        ui.clientStatus_server_TBW->setItem(row, 2, playStatus);
    }
}