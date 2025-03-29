#ifndef PAGE1_H
#define PAGE1_H

#include <QWidget>
#include "qmqtt.h"

#include <QTimer>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


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
    void on_pushButton_clicked();

    void on_pushButton_beer_clicked();

private:
    Ui::page1 *ui;

    QMQTT::Client m_client;
    QMQTT::Client m_client_2;

    QTimer m_timer;
};

#endif // PAGE1_H
