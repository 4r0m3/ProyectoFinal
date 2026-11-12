#include "enemigos.h"

const int ANCHO_ENEMIGO = 50;
const int ALTO_ENEMIGO = 60;

Enemigos::Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho)
    : Personaje(x, y, 30),
    velocidadX(2),
    limiteIzquierdo(limiteIzquierdo),
    limiteDerecho(limiteDerecho)
{
}

void Enemigos::atacar()
{
}

void Enemigos::actualizar()
{
    x += velocidadX;

    if (x <= limiteIzquierdo) {
        x = limiteIzquierdo;
        velocidadX = -velocidadX;
    }

    if (x >= limiteDerecho) {
        x = limiteDerecho;
        velocidadX = -velocidadX;
    }
}

void Enemigos::dibujar(QPainter &pintor)
{
    pintor.fillRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO, QColor(40, 160, 60));
}

QRect Enemigos::getRect() const
{
    return QRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO);
}