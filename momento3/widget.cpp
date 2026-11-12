#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

const int SUELO_DOOM = 400;

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(800, 500);
    setFocusPolicy(Qt::StrongFocus);

    doom = new DoomSlayer(100, SUELO_DOOM);

    enemigos.append(new Enemigos(400, 400, 300, 500));
    enemigos.append(new Enemigos(650, 400, 580, 740));

    plataformas.append(new Plataforma(150, 390, 120, 15));
    plataformas.append(new Plataforma(320, 320, 120, 15));
    plataformas.append(new Plataforma(500, 250, 120, 15));

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Widget::actualizarJuego);
    temporizador->start(16);
}

Widget::~Widget()
{
    qDeleteAll(proyectiles);
    qDeleteAll(enemigos);
    qDeleteAll(plataformas);
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

void Widget::actualizarSuelo()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();
    int suelo = SUELO_DOOM;

    for (Plataforma *plataforma : plataformas) {
        QRect zona = plataforma->getRect();

        bool estaEncima = cuerpo.right() > zona.left() && cuerpo.left() < zona.right();

        if (estaEncima && pies <= zona.top()) {
            int sueloPlataforma = zona.top() - cuerpo.height();
            if (sueloPlataforma < suelo) {
                suelo = sueloPlataforma;
            }
        }
    }

    doom->setSuelo(suelo);
}

void Widget::revisarColisiones()
{
    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        for (int j = enemigos.size() - 1; j >= 0; j--) {
            if (proyectiles[i]->getRect().intersects(enemigos[j]->getRect())) {
                enemigos[j]->recibirDano(15);

                if (enemigos[j]->getSalud() <= 0) {
                    delete enemigos[j];
                    enemigos.removeAt(j);
                }

                delete proyectiles[i];
                proyectiles.removeAt(i);
                break;
            }
        }
    }

    for (Enemigos *enemigo : enemigos) {
        if (doom->getRect().intersects(enemigo->getRect())) {
            int direccion = (doom->getX() < enemigo->getX()) ? -1 : 1;
            doom->golpeado(direccion, 10);
        }
    }
}

void Widget::actualizarJuego()
{
    if (teclas.contains(Qt::Key_W)) doom->saltar();

    actualizarSuelo();
    doom->actualizar();

    for (Enemigos *enemigo : enemigos) {
        enemigo->actualizar();
    }

    actualizarProyectiles();
    revisarColisiones();
    update();
}

void Widget::paintEvent(QPaintEvent *evento)
{
    Q_UNUSED(evento);
    QPainter pintor(this);

    pintor.fillRect(rect(), QColor(30, 30, 30));
    pintor.fillRect(0, 460, width(), 40, QColor(90, 60, 40));

    for (Plataforma *plataforma : plataformas) {
        plataforma->dibujar(pintor);
    }

    doom->dibujar(pintor);

    for (Enemigos *enemigo : enemigos) {
        enemigo->dibujar(pintor);
    }

    for (Proyectil *bala : proyectiles) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 220, 0));
    }

    pintor.setPen(Qt::white);
    pintor.drawText(10, 20, QString("Salud: %1").arg(doom->getSalud()));
}