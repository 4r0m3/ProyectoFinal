#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QTransform>
#include "personaje.h"
#include "proyectil.h"

class Enemigos : public Personaje
{
public:
    Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho, bool lanzaGranadas = false);
    ~Enemigos();

    void atacar();
    void actualizar();
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    Proyectil* disparar(const QRect &objetivo);
    Proyectil* lanzarGranada(const QRect &objetivo);
    Proyectil* tomarGranada();

private:
    Proyectil* crearProyectilGranada(const QRect &objetivo);

    int velocidadX;
    int limiteIzquierdo;
    int limiteDerecho;
    int miraHacia;
    int esperaDisparo;
    bool lanzaGranadas;
    int esperaGranada;

    QPixmap spritePolicia;
    int frameActual;
    int ticksAnimacion;
    int ticksAnimacionDisparo;
    int ticksAnimacionGranada;

    Proyectil *granadaLista;
    bool tieneObjetivoGranada;
    QRect ultimoObjetivoGranada;
};

#endif