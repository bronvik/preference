#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    createUi(this);
}

void Widget::createUi(QWidget *parent)
{
    this->setFixedSize(800,600);
    this->setWindowTitle("Potissimum");

    //создание аватара
    QPixmap avatar;
    avatar.load(":/avatars/default.jpg");
    ava = new QLabel();
    ava -> resize(avatar.size());
    ava -> setPixmap(avatar);

    //полоска апдейта
    up_date = new QDateTime(QDateTime::currentDateTime());
    QString str = up_date->toString(Qt::LocalDate);

    update_button = new QPushButton("last update");
    update_label = new QLabel(str);
    update_button->setFlat(true);

    //полоски вертикальные
    frame = new QFrame();
    frame -> setFrameStyle(QFrame::VLine | QFrame::Raised);

    drawGrid(this);
    drawtoolBar(this);
}

void Widget::drawGrid(QWidget *parent)
{
    //Разметка приложения

    //Сетка аватара
    ava_grid = new QHBoxLayout();
    ava_grid -> addWidget(ava);
    ava_grid -> addStretch(2);

    //Сетка обновления

    upd_grid = new QHBoxLayout();
    upd_grid -> addWidget(update_button);
    upd_grid -> addWidget(update_label);

    // Левая панель
    lv_grid = new QVBoxLayout();
    lv_grid -> addStretch(1);
    lv_grid -> addLayout(ava_grid);
    lv_grid -> addStretch(5);
    lv_grid -> addLayout(upd_grid);

    //правая панель
    rv_grid = new QVBoxLayout();

    //основная сетка
    basicgrid = new QHBoxLayout(this);
    basicgrid -> addLayout(lv_grid);
    basicgrid -> addWidget(frame);
    basicgrid -> addStretch(5);
    basicgrid -> addLayout(rv_grid);
}

void Widget::drawtoolBar(QWidget *parent)
{
    mainbar = new QMenuBar(this);

    bullet = new QMenu("Пуля");
    bullet -> addAction("Показать");

    mainbar->addMenu(bullet);
    mainbar->show();
}

Widget::~Widget()
{

}
