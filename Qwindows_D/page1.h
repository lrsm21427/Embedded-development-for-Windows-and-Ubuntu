#ifndef PAGE1_H
#define PAGE1_H

#include <QWidget>
#include <QMqttClient>
#include <QTimer>

#include <FSMP/fsmpLed.h>
#include <FSMP/fsmpBeeper.h>
#include <FSMP/fsmpTempHum.h>
#include <FSMP/fsmpLight.h>
#include <FSMP/fsmpEvents.h>
#include <FSMP/fsmpVibrator.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define PUBNISH_TOPIC "1742784837713/APP2AIOTSIM"

namespace Ui {
class page1;
}

class page1 : public QWidget
{
    Q_OBJECT

public:
    explicit page1(QWidget *parent = nullptr);
    ~page1();

private slots:
    void on_pushButton_led_clicked();

    void on_pushButton_beer_clicked();

private:
    Ui::page1 *ui;

    QMqttClient m_client;
    QMqttClient m_client_2;
    fsmpLeds m_leds;
    fsmpBeeper m_beep;
    fsmpTempHum m_temHum;
    fsmpLight m_light;
    fsmpVibrator m_vibrator;
    fsmpEvents m_event;


    QTimer m_timer;
};

#endif // PAGE1_H
