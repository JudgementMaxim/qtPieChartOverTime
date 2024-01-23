#include "widget.h"
#include "bulletpiechart.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    BulletPieChart bpc;

    qDebug() << bpc.gSOt();

}

Widget::~Widget()
{
}

