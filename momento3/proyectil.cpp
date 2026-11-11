#include "proyectil.h"

Proyectil::Proyectil(int x, int y, int direccion, int gravedad)
    : x(x),
    y(y),
    velocidadX(direccion * 12),
    velocidadY(0),
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