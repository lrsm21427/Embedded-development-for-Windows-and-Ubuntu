                                                                                                                                                                    #include "page1.h"
#include "ui_page1.h"

#include <QDebug>
#include <QString>


page1::page1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page1)
{
    ui->setupUi(this);

    m_beep.setRate(10);

    // 设置震动参数（强度0xFFFF，持续时间2000ms）
    m_vibrator.setParameter(0x8888, 1000);

    //设置MQTT服务器的IP 与 端口Port
    m_client.setHostname("mqtt.yyzlab.com.cn");
    m_client.setPort(1883);
    m_client.connectToHost();

    m_client_2.setHostname("mqtt.yyzlab.com.cn");
    m_client_2.setPort(1883);
    m_client_2.connectToHost();

    //成功连接服务器，m_client产生connected信号
    connect(&m_client, &QMqttClient::connected, [this](){
       qDebug() << "connect to host success!!!";
       m_client.subscribe(QString("1742784837713/AIOTSIM2APP"));//订阅主题
    });

    connect(&m_client_2, &QMqttClient::connected, [this](){
       qDebug() << "connect to 2 host success!!!";
       m_client_2.subscribe(QString("1743035610958/Device2AIOTSIM"));//订阅主题
    });

    //m_client收到订阅的消息，产生messageReceived信号
    connect(&m_client, &QMqttClient::messageReceived, [this](const QByteArray &message, const QMqttTopicName &){
        if(message.contains("lamp")){ //在message中查看是否有指定的字符串
//            qDebug() <<message;
            bool led_statu = QJsonDocument::fromJson(message).object().value("lamp").toBool();
            int led_id = QJsonDocument::fromJson(message).object().value("id").toInt();
//            qDebug() << led_id;
            if(led_statu == true && led_id == 0){
                m_leds.on(fsmpLeds::LED1);
                ui->pushButton_led->setText("关灯");
            }else if(led_statu == false && led_id == 0){
                m_leds.off(fsmpLeds::LED1);
                ui->pushButton_led->setText("开灯");
            }

        }else if(message.contains("alarm")){
            bool beep_statu = QJsonDocument::fromJson(message).object().value("alarm").toBool();
            if(beep_statu){
                m_beep.start();
                ui->pushButton_beer->setText("关蜂鸣器");
            }else{
                m_beep.stop();
                ui->pushButton_beer->setText("开蜂鸣器");
            }
        }
    });

    connect(&m_timer, &QTimer::timeout, [this](){
        //上传蜂鸣器状态
        if(ui->pushButton_beer->text() == "开蜂鸣器"){
//            QString beep_json = "{\"alarm\":false,\"id\": 0 }";
//            m_client.publish(QString(PUBNISH_TOPIC), beep_json.toLatin1());
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"alarm\":false,\"id\": 0 }")
            );
        }else {
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"alarm\":true,\"id\": 0 }")
            );
        }

        //上传温湿度和光照数据数据
        float tem = m_temHum.temperature();
        ui->lineEdit_tem->setText(QString::number(tem));
        float hum = m_temHum.humidity();
        if(tem > 40){
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"alarm\":true,\"id\": 0 }")
            );
            m_beep.start();
            ui->pushButton_beer->setText("关蜂鸣器");
        }else{
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"alarm\":false,\"id\": 0 }")
            );
            m_beep.stop();
            ui->pushButton_beer->setText("开蜂鸣器");
        }
        ui->lineEdit_hum->setText(QString::number(hum));
        double light = m_light.getValue();
        ui->lineEdit_light->setText(QString::number(light));
        if(light>40){
            m_leds.on(fsmpLeds::LED2);
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"lamp\":true,\"id\": 1 }")
            );
        }else {
            m_leds.off(fsmpLeds::LED2);
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"lamp\":false,\"id\": 1 }")
            );
        }
        QString light_json = QString("{\"light\":%1,\"id\":0}").arg(light);
//        qDebug() << light_json;
        QString temHum_json = QString("{\"soiltem\":%1,\"soilhum\":%2,\"id\":0}").arg(tem).arg(hum);
        qDebug() << temHum_json;
        m_client.publish(QString("1743035610958/AIOTSIM2Device"), light_json.toLatin1());
        m_client.publish(QString("1743035610958/AIOTSIM2Device"), temHum_json.toLatin1());
        });
        m_timer.start(2000);

        // 连接人体红外信号
        connect(&m_event, &fsmpEvents::peopleDetected, this, [this](bool detected){
            if(detected) {
//                qDebug() << "检测到人体，启动震动";
//                m_vibrator.start();
            } else {
//                qDebug() << "人体离开，停止震动";
                m_vibrator.stop();
            }
        });
}

page1::~page1()
{
    delete ui;
}

void page1::on_pushButton_led_clicked()
{
    if (ui->pushButton_led->text() == "开灯") {
            m_leds.on(fsmpLeds::LED1);
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"lamp\":true,\"id\": 0 }")
            );
            ui->pushButton_led->setText("关灯");
        } else {
            m_leds.off(fsmpLeds::LED1);
            m_client.publish(
                QString("1742784837713/APP2AIOTSIM"),
                QByteArray("{\"lamp\":false,\"id\": 0 }")
            );
            ui->pushButton_led->setText("开灯");
        }
}

void page1::on_pushButton_beer_clicked()
{
    if(ui->pushButton_beer->text() == "开蜂鸣器"){
        m_client.publish(
            QString("1742784837713/APP2AIOTSIM"),
            QByteArray("{\"alarm\":true,\"id\": 0 }")
        );
        m_beep.start();
        ui->pushButton_beer->setText("关蜂鸣器");
    }else{
        m_client.publish(
            QString("1742784837713/APP2AIOTSIM"),
            QByteArray("{\"alarm\":false,\"id\": 0 }")
        );
        m_beep.stop();
        ui->pushButton_beer->setText("开蜂鸣器");
    }
}


