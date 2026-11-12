#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <QPainter>
#include <QRect>

class Plataforma
{
public:
    Plataforma(int x, int y, int ancho, int alto);

    void dibujar(QPainter &pintor);
    QRect getRect() const;

private:
    int x;
    int y;
    int ancho;
    int alto;
};

#endif