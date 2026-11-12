#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QRect>

class Proyectil
{
public:
    Proyectil(int x, int y, int direccion, int velocidadY = 0, int gravedad = 0);

    void actualizar();
    bool fueraDePantalla(int anchoPantalla) const;

    int getX() const;
    int getY() const;
    QRect getRect() const;

private:
    int x;
    int y;
    int velocidadX;
    int velocidadY;
    int gravedad;
};

#endif