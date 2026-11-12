#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QPainter>
#include <QRect>
#include "personaje.h"

class Enemigos : public Personaje
{
public:
    Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho);

    void atacar();
    void actualizar();
    void dibujar(QPainter &pintor);
    QRect getRect() const;

private:
    int velocidadX;
    int limiteIzquierdo;
    int limiteDerecho;
};

#endif