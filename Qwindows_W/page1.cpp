#include "page1.h"
#include "ui_page1.h"

#include <QDebug>

page1::page1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page1)
{
    ui->setupUi(this);

    //连接到MQTT服务器
    m_client.setHostName("mqtt.yyzlab.com.cn");
    m_client.setPort(1883);
    m_client.connectToHost();

    m_client_2.setHostName("mqtt.yyzlab.com.cn");
    m_client_2.setPort(1883);
    m_client_2.connectToHost();

    connect(&m_client, &QMQTT::Client::connected, [this](){
        qDebug() << "connect host success!!!";
        m_client.subscribe("1742784837713/AIOTSIM2APP");
    });

    connect(&m_client_2, &QMQTT::Client::connected, [this](){
        qDebug() << "connect host success!!!";
        m_client_2.subscribe("1743035610958/AIOTSIM2Device");
    });

    //m_client收到订阅的消息，产生messageReceived信号
    connect(&m_client, &QMQTT::Client::received, [this](const QMQTT::Message& message){

        QString msgStr = QString::fromUtf8(message.payload());
//        qDebug() << "Received message:" << msgStr;

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(msgStr.toUtf8(), &parseError);

        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains("lamp") == true && jsonObj.contains("id") == 0) {
            bool ledStatus = jsonObj["lamp"].toBool();
            ui->pushButton->setText(ledStatus ? "关灯" : "开灯");
        }
        else if (jsonObj.contains("alarm")) {
            bool beepStatus = jsonObj["alarm"].toBool();
            ui->pushButton_beer->setText(beepStatus ? "关蜂鸣器" : "开蜂鸣器");
        }
    });

    connect(&m_client_2, &QMQTT::Client::received, [this](const QMQTT::Message& message){
        QString msgStr = QString::fromUtf8(message.payload());
//        qDebug() << "Received message from device:" << msgStr;

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(msgStr.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON parse error:" << parseError.errorString();
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        // 处理温湿度数据
        if(jsonObj.contains("soiltem") && jsonObj.contains("soilhum")){
            double temperature = jsonObj["soiltem"].toDouble();
            double humidity = jsonObj["soilhum"].toDouble();

            // 更新UI显示
            ui->lineEdit_tem->setText(QString::number(temperature, 'f', 2) + "°C");
            ui->lineEdit_hum->setText(QString::number(humidity, 'f', 2) + "%");

//            qDebug() << "土壤温度:" << temperature << "°C, 土壤湿度:" << humidity << "%";
        }else if(jsonObj.contains("light")){
            double light = jsonObj["light"].toDouble();
            ui->lineEdit_light->setText(QString::number(light, 'f', 3));
        }
    });

    connect(&m_timer, &QTimer::timeout, [this](){
        //上传蜂鸣器状态
        if(ui->pushButton_beer->text() == "开蜂鸣器"){
            QMQTT::Message msg;
            msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
            msg.setPayload("{\"alarm\":false,\"id\": 0 }");//设置消息
            m_client.publish(msg);
        }else {
            QMQTT::Message msg;
            msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
            msg.setPayload("{\"alarm\":true,\"id\": 0 }");//设置消息
            m_client.publish(msg);
        }

    });
    m_timer.start(2000);
}

page1::~page1()
{
    delete ui;
}

void page1::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "开灯"){
        QMQTT::Message msg;
        msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
        msg.setPayload("{\"lamp\":true,\"id\": 0 }");//设置消息
        m_client.publish(msg);
        ui->pushButton->setText("关灯");
    }else{
        QMQTT::Message msg;
        msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
        msg.setPayload("{\"lamp\":false,\"id\": 0 }");//设置消息
        m_client.publish(msg);
        ui->pushButton->setText("开灯");
    }
}

void page1::on_pushButton_beer_clicked()
{
    if(ui->pushButton_beer->text() == "开蜂鸣器"){
        QMQTT::Message msg;
        msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
        msg.setPayload("{\"alarm\":true,\"id\": 0 }");//设置消息
        m_client.publish(msg);
        ui->pushButton_beer->setText("关蜂鸣器");
    }else {
        QMQTT::Message msg;
        msg.setTopic("1742784837713/APP2AIOTSIM");//设置主题
        msg.setPayload("{\"alarm\":false,\"id\": 0 }");//设置消息
        m_client.publish(msg);
        ui->pushButton_beer->setText("开蜂鸣器");
    }
}
