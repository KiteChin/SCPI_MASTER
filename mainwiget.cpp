#include "mainwiget.h"
#include "ui_mainwiget.h"

MainWiget::MainWiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWiget)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    timer = new QTimer(this);
}

MainWiget::~MainWiget()
{
    delete ui;
}
 
void MainWiget::readyRead_Slot()
{
    this->receiveNum++;
    static QString buf_1;
    static QString buf_2;


    //SCPI指令解析返回参数
    QRegExp rx("(\\d)+(\\.\\d+)?");
    QRegExp rx_alone("^[1-9]\\d*$");

    if(this->currentValue == VOLTage_DC_RATio)
    {
        buf_1 = tcpSocket->readAll();
        ui->output_tb->append(buf_1);
        if(rx_alone.indexIn(buf_1,0) != -1 && this->receiveNum == 1)
        {
           ui->RATIO_1_TB->setText(rx_alone.cap(0));
           ui->RATIO_2_TB->setText(rx_alone.cap(0));
        }
        receiveFinish = TURE;

    }else
    {
        if(this->receiveNum == 1)
        {
            buf_1 = tcpSocket->readAll();
            ui->output_tb->append(buf_1);
        }
        if(this->receiveNum == 2)
        {
            buf_2 = (tcpSocket->readAll());
            ui->output_tb->append(buf_2);
        }
        if(rx.indexIn(buf_1,0)!=-1 && this->receiveNum == 1)
        {
            switch (this->currentValue) {
            case VOLTage_DC: ui->VOLT_DC_1_TB->setText(rx.cap(0));
                             break;
            case CURRent_DC: ui->CUR_DC_1_TB->setText(rx.cap(0));
                             break;
    //        case VOLTage_AC: ui->VOLT_AC_1_TB->setText(rx.cap(0));
    //                         break;
    //        case CURRent_AC: ui->CUR_AC_1_TB->setText(rx.cap(0));
    //                         break;
    //        case RESistance: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case FRESistance: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case PERiod: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case VOLTage_DC_RATio: ui->VOLT_AC_TB->setText(rx.cap(1));break;

            }
        }

        if(rx.indexIn(buf_2,0)!=-1 && receiveNum == 2)
        {
            switch (this->currentValue) {
            case VOLTage_DC: ui->VOLT_DC_2_TB->setText(rx.cap(0));
                                 break;
            case CURRent_DC: ui->CUR_DC_2_TB->setText(rx.cap(0));
                             break;
    //        case VOLTage_AC: ui->VOLT_AC_2_TB->setText(rx.cap(0));
    //                             break;
    //        case CURRent_AC: ui->CUR_AC_2_TB->setText(rx.cap(0));
    //                         break;
    //        case RESistance: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case FRESistance: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case PERiod: ui->VOLT_AC_TB->setText(rx.cap(1));break;
    //        case VOLTage_DC_RATio: ui->VOLT_AC_TB->setText(rx.cap(1));break;

            }
        }
        receiveFinish = TURE;
    }

}

void MainWiget::connected_Slot()
{
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout_Slot()),Qt::UniqueConnection);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()),Qt::UniqueConnection);
    ui->output_tb->append("connected!");
    //timer start
//    currentValue = VOLTage_DC;
//    receiveNum = 0;
//    tcpSocket->write("MEASure:VOLTage:DC? 1,1;");
    timer->start(1000);

}

void MainWiget::timeout_Slot()
{
    this->queryVoltageDC();
    Delay_MSec(300);
    this->queryCurrentDC();
    Delay_MSec(300);
    this->queryVoltageDCRatio();
    Delay_MSec(300);

//    cur_value = VOLTage_AC;
//    rev_num = 0;
//    tcpSocket->write("MEASure:VOLTage:AC? 1,1;");
//    Delay_MSec(100);
//    cur_value = CURRent_AC;
//    rev_num = 0;
//    tcpSocket->write("MEASure:CURRent:AC? 1,1;");
//    Delay_MSec(100);
//    cur_value = RESistance;
//    rev_num = 0;
//    tcpSocket->write("MEASure:RESistance?");
//    Delay_MSec(100);
//    cur_value = FRESistance;
//    rev_num = 0;
//    tcpSocket->write("MEASure:FRESistance?");
//    Delay_MSec(100);
//    cur_value = PERiod;
//    rev_num = 0;
//    tcpSocket->write("MEASure:PERiod?");
//    Delay_MSec(100);
}

void MainWiget::on_ON_pb_clicked()
{
    tcpSocket->connectToHost(ui->IP_Edit->toPlainText(),ui->PORT_Edit->toPlainText().toUShort());
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connected_Slot()),Qt::UniqueConnection);
}

void MainWiget::on_OFF_pb_clicked()
{
    if(tcpSocket->state() == tcpSocket->ConnectedState)
    {
        tcpSocket->close();
        ui->output_tb->append("close!");
        timer->stop();
    }
}


void MainWiget::on_DC_PB_clicked()
{
    if(tcpSocket->state() == tcpSocket->ConnectedState)
    {
        QString str;
        str = "CONFigure:VOLTage:DC ";
        str.append(ui->DC_TE->toPlainText()).append(";");
        tcpSocket->write(str.toLocal8Bit());
        ui->output_tb->append(str);
    }

}

void MainWiget::on_CV_pb_clicked()
{
    tcpSocket->write(ui->CV_te->toPlainText().toLocal8Bit());
}

void MainWiget::on_CR_tb_clicked()
{
    tcpSocket->write(ui->CR_te->toPlainText().toLocal8Bit());
}

void MainWiget::on_CW_pb_clicked()
{
    tcpSocket->write(ui->CW_te->toPlainText().toLocal8Bit());
}

void MainWiget::Delay_MSec(int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWiget::queryVoltageDC()
{
    currentValue = VOLTage_DC;
    receiveNum = 0;
    tcpSocket->write("MEASure:VOLTage:DC? 1,1;");
    while(this->receiveFinish != TURE);
    receiveFinish = FAULSE;
}
void MainWiget::queryCurrentDC()
{
    currentValue = CURRent_DC;
    receiveNum = 0;
    tcpSocket->write("MEASure:CURRent:DC? 1,1;");
    while(this->receiveFinish != TURE);
    receiveFinish = FAULSE;
}
void MainWiget::queryVoltageDCRatio()
{
    currentValue = VOLTage_DC_RATio;
    receiveNum = 0;
    tcpSocket->write("MEASure:VOLTage:DC:RATio?");
    while(this->receiveFinish != TURE);
    receiveFinish = FAULSE;
}
