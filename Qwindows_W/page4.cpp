#include "page4.h"
#include "ui_page4.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

page4::page4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page4)
{
    ui->setupUi(this);

    // 连接网络请求的(信号发出者，信号，接受者/槽函数所属对象，槽函数)
        connect(&m_manager, &QNetworkAccessManager::finished, this, &page4::do_finished);
}

page4::~page4()
{
    delete ui;
}

void page4::on_pushButton_clicked()
{
    // 设置请求 URL
    QNetworkRequest request(QUrl("http://v1.yiketianqi.com/free/day?appid=74849888&appsecret=b1blC1dr&unescape=1"));

    // 发送 GET 请求
    m_manager.get(request);
}

void page4::do_finished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc =  QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    ui->textEdit->append(obj.value("wea").toString());
}
