#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil
{
public:
    Proyectil(int x, int y, int direccion, int gravedad = 0);

    void actualizar();
    bool fueraDePantalla(int anchoPantalla) const;

    int getX() const;
    int getY() const;

private:
    int x;
    int y;
    int velocidadX;
    int velocidadY;
    int gravedad;
};

#endif