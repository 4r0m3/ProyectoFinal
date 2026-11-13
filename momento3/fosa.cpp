#include "fosa.h"

Fosa::Fosa(int x, int ySuelo, int ancho, int profundidad, TipoFosa tipo)
    : zona(x, ySuelo, ancho, profundidad),
    tipo(tipo)
{
}

void Fosa::dibujar(QPainter &pintor)
{
    if (tipo == TipoFosa::Abismo) {
        pintor.fillRect(zona, QColor(0, 0, 0));
    } else {
        pintor.fillRect(zona, QColor(120, 30, 30));
    }
}

QRect Fosa::getRect() const
{
    return zona;
}

TipoFosa Fosa::getTipo() const
{
    return tipo;
}

int Fosa::getFondo() const
{
    return zona.y() + zona.height();
}