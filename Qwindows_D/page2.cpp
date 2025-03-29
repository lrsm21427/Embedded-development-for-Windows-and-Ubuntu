#include "page2.h"
#include "ui_page2.h"

page2::page2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page2)
{
    ui->setupUi(this);
}

page2::~page2()
{
    delete ui;
}
