#ifndef MAINWIGET_H
#define MAINWIGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <QRegExp>

enum CUR_VALUE{
    VOLTage_DC,
    VOLTage_AC,
    CURRent_DC,
    CURRent_AC,
    RESistance,
    FRESistance,
    PERiod,
    VOLTage_DC_RATio
};
enum BOOL{
    FAULSE,
    TURE
};

namespace Ui {
class MainWiget;
}

class MainWiget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWiget(QWidget *parent = nullptr);
    ~MainWiget();

private slots:
    void on_ON_pb_clicked();

    void on_OFF_pb_clicked();

    void on_DC_PB_clicked();

    void on_CV_pb_clicked();

    void on_CR_tb_clicked();

    void on_CW_pb_clicked();

    void readyRead_Slot();

    void connected_Slot();

    void timeout_Slot();


private:
    Ui::MainWiget *ui;

    int currentValue = 0;
    int receiveNum = 0;
    bool receiveFinish=0;
    QTcpSocket *tcpSocket;
    QTimer *timer;

    void queryVoltageDC();
    void queryCurrentDC();
    void queryVoltageDCRatio();

    void Delay_MSec(int msec);
};

#endif // MAINWIGET_H
