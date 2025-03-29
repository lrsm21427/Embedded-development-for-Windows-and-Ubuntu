#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Win_main; }
QT_END_NAMESPACE

class Win_main : public QWidget
{
    Q_OBJECT

public:
    Win_main(QWidget *parent = nullptr);
    ~Win_main();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Win_main *ui;
};
#endif // WIN_MAIN_H
