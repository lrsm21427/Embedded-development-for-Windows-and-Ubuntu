#ifndef PAGE2_H
#define PAGE2_H

#include <QWidget>
#include <QNetworkAccessManager>

#include <QAudioInput> //音频输入设备类
#include <QAudioFormat> //音频格式类
#include <QIODevice> //输入输出设备类

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDebug>

#include "vosk_api.h"

namespace Ui {
class page2;
}

class page2 : public QWidget
{
    Q_OBJECT

public:
    explicit page2(QWidget *parent = nullptr);
    ~page2();

    void dp_start(QString vosk_text);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

private:
    Ui::page2 *ui;

    QAudioInput *m_audioInput;
    QIODevice *m_ioDevice;
    QByteArray m_audioData;

    VoskModel *m_model; //加载模型
    VoskRecognizer *m_recognizer; //识别器

    QString api_key = "sk-9864421d386644099a8a759286658c31"; // 替换为你的API Key

    QNetworkAccessManager *networkManager;  // 网络访问管理器
    QString getAiReply(const QString &userMessage);  // 调用 API 获取 AI 回复
};

#endif // PAGE2_H
