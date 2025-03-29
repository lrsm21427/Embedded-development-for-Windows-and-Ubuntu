#include "page3.h"
#include "ui_page3.h"

page3::page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page3)
{
    ui->setupUi(this);

    // 检查并初始化摄像头
    QList<QCameraInfo> cam_list = QCameraInfo::availableCameras();
    qDebug() << cam_list;
    if(cam_list[0].isNull()) {
        qDebug() << "未检测到可用摄像头";
        m_camera = nullptr;
        m_viewFinder = nullptr;
    }else {
        m_camera = new QCamera(cam_list[0]);
        m_viewFinder = new QCameraViewfinder(this);

        m_camera->setViewfinder(m_viewFinder);

        ui->verticalLayout->addWidget(m_viewFinder);

        m_camera->start();
    }
}

page3::~page3()
{
    delete ui;
}
