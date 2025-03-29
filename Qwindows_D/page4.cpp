#include "page4.h"
#include "ui_page4.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

page4::page4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page4)
{
    ui->setupUi(this);
    connect(&m_manager, &QNetworkAccessManager::finished, this, &page4::do_finished);
}

page4::~page4()
{
    delete ui;
}

void page4::on_pushButton_clicked()
{
    // 清空之前的内容
    ui->textEdit->clear();

    // 显示加载状态
    ui->textEdit->append("正在获取天气数据...");

    // 设置请求URL（添加城市参数）
    QNetworkRequest request(QUrl("http://v1.yiketianqi.com/free/day?appid=74849888&appsecret=b1blC1dr&unescape=1&cityid=101010100"));

    // 发送GET请求
    m_manager.get(request);
}

void page4::do_finished(QNetworkReply *reply)
{
    // 清空加载状态
    ui->textEdit->clear();

    // 检查网络错误
    if(reply->error() != QNetworkReply::NoError) {
        ui->textEdit->append("网络请求错误: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    // 读取并解析JSON数据
    QByteArray data = reply->readAll();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);

    if(jsonError.error != QJsonParseError::NoError) {
        ui->textEdit->append("JSON解析错误: " + jsonError.errorString());
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    // 显示完整的天气信息
    ui->textEdit->append(QString("城市: %1").arg(obj["city"].toString()));
    ui->textEdit->append(QString("日期: %1 %2").arg(obj["date"].toString()).arg(obj["week"].toString()));
    ui->textEdit->append(QString("天气: %1").arg(obj["wea"].toString()));
    ui->textEdit->append(QString("温度: %1°C (白天%2°C/夜间%3°C)")
                        .arg(obj["tem"].toString())
                        .arg(obj["tem_day"].toString())
                        .arg(obj["tem_night"].toString()));
    ui->textEdit->append(QString("湿度: %1").arg(obj["humidity"].toString()));
    ui->textEdit->append(QString("空气质量: %1").arg(obj["air"].toString()));
    ui->textEdit->append(QString("气压: %1 hPa").arg(obj["pressure"].toString()));
    ui->textEdit->append(QString("风速: %1 %2").arg(obj["win_speed"].toString()).arg(obj["win_meter"].toString()));
    ui->textEdit->append(QString("风向: %1").arg(obj["win"].toString()));

    // 根据天气情况设置不同颜色
    QString weather = obj["wea"].toString();
    if(weather.contains("晴")) {
        ui->textEdit->setTextColor(Qt::yellow);
    } else if(weather.contains("雨")) {
        ui->textEdit->setTextColor(Qt::blue);
    } else if(weather.contains("云")) {
        ui->textEdit->setTextColor(Qt::gray);
    } else {
        ui->textEdit->setTextColor(Qt::black);
    }

    reply->deleteLater();
}
