#include "page3.h"
#include "ui_page3.h"

page3::page3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page3)
{
    ui->setupUi(this);

}

page3::~page3()
{
    delete ui;
}
