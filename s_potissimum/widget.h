#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QString>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QMenuBar *mainbar;

    QMenu *bullet;

    QHBoxLayout *basicgrid;
    QVBoxLayout *lv_grid;
    QHBoxLayout *ava_grid;
    QHBoxLayout *upd_grid;
    QVBoxLayout *rv_grid;

    QLabel *ava;
    QLabel *update_label;

    QPushButton *update_button;

    QFrame *frame;

    QDateTime *up_date;

    void createUi(QWidget *parent);
    void drawGrid(QWidget *parent);
    void drawtoolBar(QWidget *parent);
};


#endif // WIDGET_H
