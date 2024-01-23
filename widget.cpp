#include "widget.h"
#include "bulletpiechart.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    BulletPieChart bpc;
    bpc.getSellers();

}

Widget::~Widget()
{
}

