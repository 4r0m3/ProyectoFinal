#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(800, 500);
    setFocusPolicy(Qt::StrongFocus);

    doom = new DoomSlayer(100, 400);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Widget::actualizarJuego);
    temporizador->start(16);
}

Widget::~Widget()
{
    qDeleteAll(proyectiles);
    delete doom;
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    teclas.insert(evento->key());
    actualizarMovimiento();

    if (evento->key() == Qt::Key_Space) {
        proyectiles.append(doom->disparar());
    }
}

void Widget::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    teclas.remove(evento->key());
    actualizarMovimiento();
}

void Widget::actualizarMovimiento()
{
    int direccion = 0;
    if (teclas.contains(Qt::Key_A)) direccion -= 1;
    if (teclas.contains(Qt::Key_D)) direccion += 1;
    doom->correr(direccion);
}

void Widget::actualizarProyectiles()
{
    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        proyectiles[i]->actualizar();

        if (proyectiles[i]->fueraDePantalla(width())) {
            delete proyectiles[i];
            proyectiles.removeAt(i);
        }
    }
}

void Widget::actualizarJuego()
{
    if (teclas.contains(Qt::Key_W)) doom->saltar();
    doom->actualizar();
    actualizarProyectiles();
    update();
}

void Widget::paintEvent(QPaintEvent *evento)
{
    Q_UNUSED(evento);
    QPainter pintor(this);

    pintor.fillRect(rect(), QColor(30, 30, 30));
    pintor.fillRect(0, 460, width(), 40, QColor(90, 60, 40));
    doom->dibujar(pintor);

    for (Proyectil *bala : proyectiles) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 220, 0));
    }
}