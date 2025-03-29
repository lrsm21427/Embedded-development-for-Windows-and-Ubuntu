#ifndef PAGE4_H
#define PAGE4_H

#include <QWidget>
#include <QNetworkAccessManager>  //发送GET POST请求
#include <QNetworkRequest> //封装请求
#include <QNetworkReply> //接受请求结果

#include <QJsonDocument> //文本转json
#include <QJsonArray> //解析json数组
#include <QJsonObject> //封装解析键值对

#include <QTextEdit>

namespace Ui {
class page4;
}

class page4 : public QWidget
{
    Q_OBJECT

public:
    explicit page4(QWidget *parent = nullptr);
    ~page4();

private slots:
    void on_pushButton_clicked();
    void do_finished(QNetworkReply *reply);

private:
    Ui::page4 *ui;

    QNetworkAccessManager m_manager;
};

#endif // PAGE4_H
