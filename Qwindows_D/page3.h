#ifndef PAGE3_H
#define PAGE3_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>


namespace Ui {
class page3;
}

class page3 : public QWidget
{
    Q_OBJECT

public:
    explicit page3(QWidget *parent = nullptr);
    ~page3();

private:
    Ui::page3 *ui;

    QCamera *m_camera;
    QCameraViewfinder *m_viewFinder;
};

#endif // PAGE3_H
