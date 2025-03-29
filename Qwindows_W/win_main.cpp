#include "win_main.h"
#include "ui_win_main.h"

Win_main::Win_main(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Win_main)
{
    ui->setupUi(this);

    ui->verticalLayout->setAlignment(Qt::AlignTop);

}

Win_main::~Win_main()
{
    delete ui;
}

//基本控件
void Win_main::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//语音对话
void Win_main::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//摄像头
void Win_main::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//NULL
void Win_main::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
