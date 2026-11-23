#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(800, 500);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    menu = new Menu(800, 500);

    temporizador = new QTimer(this);
    temporizador->setTimerType(Qt::PreciseTimer);
    connect(temporizador, &QTimer::timeout, this, &Widget::actualizarJuego);
    temporizador->start(16);
}

Widget::~Widget()
{
    delete nivelLateral;
    delete menu;
    delete ui;
}

void Widget::volverAlMenu()
{
    delete nivelLateral;
    nivelLateral = nullptr;
    setFocus();
    update();
}

void Widget::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    if (nivelLateral != nullptr) {
        nivelLateral->teclaPresionada(evento->key());
        return;
    }

    menu->teclaPresionada(evento->key());
}

void Widget::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    if (nivelLateral != nullptr) {
        nivelLateral->teclaSoltada(evento->key());
        return;
    }

    menu->teclaSoltada(evento->key());
}

void Widget::mouseMoveEvent(QMouseEvent *evento)
{
    if (nivelLateral != nullptr) return;
    menu->mouseMovido(evento->pos().x(), evento->pos().y());
}

void Widget::mousePressEvent(QMouseEvent *evento)
{
    if (nivelLateral != nullptr) return;
    if (evento->button() == Qt::LeftButton) {
        menu->mousePresionado(evento->pos().x(), evento->pos().y());
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *evento)
{
    if (nivelLateral != nullptr) return;
    if (evento->button() == Qt::LeftButton) {
        menu->mouseSoltado();
    }
}

void Widget::actualizarJuego()
{
    if (nivelLateral != nullptr) {
        nivelLateral->actualizar();

        if (nivelLateral->debeVolverAlMenu()) {
            volverAlMenu();
            return;
        }

        update();
        return;
    }

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

    if (nivelLateral != nullptr) {
        nivelLateral->dibujar(pintor);
        return;
    }

    menu->dibujar(pintor);
}