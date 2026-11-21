#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QPainter>
#include <QRect>
#include "personaje.h"
#include "proyectil.h"

class Enemigos : public Personaje
{
public:
    Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho, bool lanzaGranadas = false);

    void atacar();
    void actualizar();
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    Proyectil* disparar(const QRect &objetivo);
    Proyectil* lanzarGranada(const QRect &objetivo);

private:
    int velocidadX;
    int limiteIzquierdo;
    int limiteDerecho;
    int miraHacia;
    int esperaDisparo;
    bool lanzaGranadas;
    int esperaGranada;
};

#endif