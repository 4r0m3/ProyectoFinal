#include "plataforma.h"
#include <cmath>

Plataforma::Plataforma(int x, int y, int ancho, int alto, int amplitudX, int amplitudY, double velocidad)
    : x(x),
    y(y),
    ancho(ancho),
    alto(alto),
    xInicial(x),
    yInicial(y),
    amplitudX(amplitudX),
    amplitudY(amplitudY),
    angulo(0),
    velocidad(velocidad),
    desplazamientoX(0),
    desplazamientoY(0)
{
}

void Plataforma::actualizar()
{
    int xAnterior = x;
    int yAnterior = y;

    angulo += velocidad;
    double seno = std::sin(angulo);

    x = xInicial + static_cast<int>(std::lround(amplitudX * seno));
    y = yInicial + static_cast<int>(std::lround(amplitudY * seno));

    desplazamientoX = x - xAnterior;
    desplazamientoY = y - yAnterior;
}

void Plataforma::dibujar(QPainter &pintor)
{
    bool esMovil = (amplitudX != 0 || amplitudY != 0);
    QColor color = esMovil ? QColor(70, 130, 200) : QColor(120, 120, 130);
    pintor.fillRect(x, y, ancho, alto, color);
}

QRect Plataforma::getRect() const
{
    return QRect(x, y, ancho, alto);
}

int Plataforma::getDesplazamientoX() const
{
    return desplazamientoX;
}

int Plataforma::getDesplazamientoY() const
{
    return desplazamientoY;
}