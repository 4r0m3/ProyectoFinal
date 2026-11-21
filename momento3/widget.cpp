#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(800, 500);
    setFocusPolicy(Qt::StrongFocus);

    menu = new Menu(800, 500);

    temporizador = new QTimer(this);
    temporizador->setTimerType(Qt::PreciseTimer);
    connect(temporizador, &QTimer::timeout, this, &Widget::actualizarJuego);
    temporizador->start(16);
}

Widget::~Widget()
{
    delete menu;
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;
    menu->teclaPresionada(evento->key());
}

void Widget::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;
    menu->teclaSoltada(evento->key());
}

void Widget::actualizarJuego()
{
    menu->actualizar();

    if (menu->quiereSalir()) {
        close();
        return;
    }

    update();
}

void Widget::paintEvent(QPaintEvent *evento)
{
    Q_UNUSED(evento);
    QPainter pintor(this);
    menu->dibujar(pintor);
}