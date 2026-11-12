#include "plataforma.h"

Plataforma::Plataforma(int x, int y, int ancho, int alto)
    : x(x),
    y(y),
    ancho(ancho),
    alto(alto)
{
}

void Plataforma::dibujar(QPainter &pintor)
{
    pintor.fillRect(x, y, ancho, alto, QColor(120, 120, 130));
}

QRect Plataforma::getRect() const
{
    return QRect(x, y, ancho, alto);
}