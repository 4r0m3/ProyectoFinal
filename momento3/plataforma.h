#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <QPainter>
#include <QRect>

class Plataforma
{
public:
    Plataforma(int x, int y, int ancho, int alto, int amplitudX = 0, int amplitudY = 0, double velocidad = 0.03);

    void actualizar();
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    int getDesplazamientoX() const;
    int getDesplazamientoY() const;

private:
    int x;
    int y;
    int ancho;
    int alto;

    int xInicial;
    int yInicial;
    int amplitudX;
    int amplitudY;
    double angulo;
    double velocidad;
    int desplazamientoX;
    int desplazamientoY;
};

#endif