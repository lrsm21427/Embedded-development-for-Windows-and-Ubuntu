#include "win_main.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile css_file(":/CSS/main.css");
    if(css_file.open(QIODevice::ReadOnly)){
        a.setStyleSheet(css_file.readAll());
    }else
    {
        qDebug()<<"error";
    }
    css_file.close();

    Win_main w;
    w.show();
    return a.exec();
}
