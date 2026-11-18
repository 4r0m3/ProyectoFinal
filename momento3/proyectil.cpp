#include "proyectil.h"

Proyectil::Proyectil(int x, int y, int direccion, int velocidadY, int gravedad, int rapidez)
    : x(x),
    y(y),
    velocidadX(direccion * rapidez),
    velocidadY(velocidadY),
    gravedad(gravedad)
{
}

void Proyectil::actualizar()
{
    x += velocidadX;
    velocidadY += gravedad;
    y += velocidadY;
}

bool Proyectil::fueraDePantalla(int anchoPantalla) const
{
    return x < 0 || x > anchoPantalla;
}

int Proyectil::getX() const { return x; }
int Proyectil::getY() const { return y; }

QRect Proyectil::getRect() const
{
    return QRect(x, y, 10, 4);
}