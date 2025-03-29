#include "page2.h"
#include "ui_page2.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

page2::page2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page2)
{
    ui->setupUi(this);

    //初始化类对象
    m_ioDevice = NULL;

    QAudioFormat format;
    format.setByteOrder(QAudioFormat::LittleEndian); //小端序
    format.setChannelCount(1); //单声道
    format.setCodec("audio/pcm"); //音频本身的格式 pcm原始数据格式
    format.setSampleRate(16000); //采样的频率 vosk要求16KHz
    format.setSampleSize(16); //位深
    format.setSampleType(QAudioFormat::SignedInt); //数据格式

    m_audioInput = new QAudioInput(format); //实例化音频输入对象

    //加载模型 传入模型文件的路径
    m_model = vosk_model_new("D:\\vosk\\vosk-model-cn-0.22");
    if(m_model == NULL){
        qDebug() << "model new error";
    }

    //初始化识别器
    m_recognizer = vosk_recognizer_new(m_model, 16000.0);
    if(m_recognizer ==  NULL){
        qDebug() << "recognizer new error";
    }

    // 初始化网络访问管理器
    networkManager = new QNetworkAccessManager(this);


    // 连接发送按钮的点击信号到槽函数
    connect(ui->pushButton, &QPushButton::clicked, this, &page2::on_pushButton_clicked);
}

page2::~page2()
{
    delete ui;

}

void page2::on_pushButton_clicked()
{
    // 获取用户输入
    QString userMessage = ui->lineEdit->text();
    if (userMessage.isEmpty()) {
        return;  // 如果输入为空，直接返回
    }

    // 将用户消息添加到对话历史
    ui->textEdit->append("你: " + userMessage);
    ui->lineEdit->clear();  // 清空输入框

    // 调用 API 获取 AI 回复
    QString aiReply = getAiReply(userMessage);

    // 将 AI 回复添加到对话历史
    ui->textEdit->append("AI: " + aiReply);
}

QString page2::getAiReply(const QString &userMessage)
{
    // 设置 API 的 URL
    QUrl url("https://api.deepseek.com/chat/completions");

    // 创建请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    request.setRawHeader("Authorization", "sk-9864421d386644099a8a759286658c31");
    request.setRawHeader("Authorization", ("Bearer " + api_key).toUtf8());

    // 创建请求体
    QJsonObject requestBody;
    requestBody["model"] = "deepseek-chat";
    requestBody["messages"] = QJsonArray {
        QJsonObject {
            {"role", "system"},
            {"content", "You are a helpful assistant"}
        },
        QJsonObject {
            {"role", "user"},
            {"content", userMessage}
        }
    };
    requestBody["max_tokens"] = 2048;
    requestBody["temperature"] = 1;
    requestBody["top_p"] = 1;
    requestBody["stream"] = false;

    // 发送 POST 请求
    QNetworkReply *reply = networkManager->post(request, QJsonDocument(requestBody).toJson());

    // 等待响应
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 处理响应
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();
        QString aiReply = jsonObject["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
        return aiReply;
    } else {
        return "请求失败: " + reply->errorString();
    }
}


void page2::on_pushButton_2_pressed()
{
    //长按按钮 -- 开始录音
    m_audioData.clear(); //清空缓存区
    m_ioDevice = m_audioInput->start(); //开始录音
    if(m_ioDevice){
        // m_ioDevice 在采集音频数据之后，产生一个readyRead信号
        // lambda表达式 this作用：在函数内部，可以使用类中的成员以及成员方法
        connect(m_ioDevice, &QIODevice::readyRead, [this](/*参数列表*/){
            //代码块
            //将采集好的音频数据放入一个缓存区
            m_audioData.append(m_ioDevice->readAll());
        });
    }
}

void page2::on_pushButton_2_released()
{
    //松开按钮 -- 结束，并且调用VOSK模型识别
    m_audioInput->stop(); //停止录音
    m_ioDevice->disconnect();//解绑信号与槽
    m_ioDevice = NULL;

    //测试有没有音频数据
    //qDebug() << m_audioData;

    if(!m_audioData.isEmpty()){
        //判断m_audioData是否有数据
        //开始识别
        vosk_recognizer_accept_waveform(m_recognizer, m_audioData.constData(), m_audioData.size());
        //获取识别结果，追加显示到文本编辑器
        //测试 显示所有的结果
        //ui->textEdit->append( vosk_recognizer_final_result(m_recognizer));

        const char *result = vosk_recognizer_final_result(m_recognizer);
        QJsonDocument doc = QJsonDocument::fromJson(result);
        QJsonObject obj = doc.object();
        //解析出最终的识别结果，并且通过remove去除空格
        //ui->textEdit->append( obj.value("text").toString().remove(" "));
        QString vosk_text = obj.value("text").toString().remove(" ");
//        ui->textEdit->append(vosk_text);
        ui->textEdit->append("你: " + vosk_text);

        dp_start(vosk_text);
    }

}

void page2::dp_start(QString vosk_text)
{
    // 调用 API 获取 AI 回复
    QString aiReply = getAiReply(vosk_text);

    // 将 AI 回复添加到对话历史
    ui->textEdit->append("AI: " + aiReply);
}

