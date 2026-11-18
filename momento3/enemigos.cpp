#include "enemigos.h"

const int ANCHO_ENEMIGO = 50;
const int ALTO_ENEMIGO = 60;

const int RANGO_DISPARO = 350;
const int DIFERENCIA_ALTURA = 70;
const int ESPERA_ENTRE_DISPAROS = 90;
const int RAPIDEZ_BALA_ENEMIGA = 6;

Enemigos::Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho)
    : Personaje(x, y, 30),
    velocidadX(2),
    limiteIzquierdo(limiteIzquierdo),
    limiteDerecho(limiteDerecho),
    miraHacia(1),
    esperaDisparo(60)
{
}

void Enemigos::atacar()
{
}

void Enemigos::actualizar()
{
    if (esperaDisparo > 0) {
        esperaDisparo--;
    }

    x += velocidadX;

    if (x <= limiteIzquierdo) {
        x = limiteIzquierdo;
        velocidadX = -velocidadX;
    }

    if (x >= limiteDerecho) {
        x = limiteDerecho;
        velocidadX = -velocidadX;
    }

    miraHacia = (velocidadX > 0) ? 1 : -1;
}

Proyectil* Enemigos::disparar(const QRect &objetivo)
{
    if (esperaDisparo > 0) {
        return nullptr;
    }

    QRect cuerpo = getRect();
    int distanciaX = objetivo.center().x() - cuerpo.center().x();
    int distanciaY = objetivo.center().y() - cuerpo.center().y();

    if (qAbs(distanciaX) > RANGO_DISPARO || qAbs(distanciaY) > DIFERENCIA_ALTURA) {
        return nullptr;
    }

    miraHacia = (distanciaX >= 0) ? 1 : -1;
    esperaDisparo = ESPERA_ENTRE_DISPAROS;

    int xBala = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 10;
    int yBala = y + 25;

    return new Proyectil(xBala, yBala, miraHacia, 0, 0, RAPIDEZ_BALA_ENEMIGA);
}

void Enemigos::dibujar(QPainter &pintor)
{
    pintor.fillRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO, QColor(40, 160, 60));

    int xCanon = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 14;
    pintor.fillRect(xCanon, y + 23, 14, 6, QColor(30, 30, 30));
}

QRect Enemigos::getRect() const
{
    return QRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO);
}