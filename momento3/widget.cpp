#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(800, 500);
    setFocusPolicy(Qt::StrongFocus);

    nivel = new NivelLateral(800, 500);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Widget::actualizarJuego);
    temporizador->start(16);
}

Widget::~Widget()
{
    delete nivel;
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;
    nivel->teclaPresionada(evento->key());
}

void Widget::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;
    nivel->teclaSoltada(evento->key());
}

void Widget::actualizarJuego()
{
    nivel->actualizar();
    update();
}

void Widget::paintEvent(QPaintEvent *evento)
{
    Q_UNUSED(evento);
    QPainter pintor(this);
    nivel->dibujar(pintor);
}