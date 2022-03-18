#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    MainWiget * mw = new MainWiget();
    ui->tabWidget->addTab(mw,QString::fromUtf8("设备").append(QString::number(this->tab_num)));
    ui->tabWidget->setCurrentIndex(this->tab_num);
    this->tab_num++;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_addTab_clicked()
{
    MainWiget * mw = new MainWiget();
    ui->tabWidget->addTab(mw,QString::fromUtf8("设备").append(QString::number(this->tab_num)));

    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex()+1);
    this->tab_num++;

}

void Widget::on_rmTab_clicked()
{
    ui->tabWidget->currentWidget()->~QWidget();
//    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

}
