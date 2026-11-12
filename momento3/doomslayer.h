#ifndef DOOMSLAYER_H
#define DOOMSLAYER_H

#include <QPixmap>
#include <QPainter>
#include <QList>
#include "personaje.h"
#include "proyectil.h"

class DoomSlayer : public Personaje
{
public:
    DoomSlayer(int x, int y);

    void correr(int direccion);
    void saltar();
    QList<Proyectil*> disparar();
    void interactuar();
    void actualizar();
    void dibujar(QPainter &pintor);

private:
    int velocidadX;
    int velocidadY;
    int sueloY;
    int miraHacia;
    bool enElSuelo;

    int cartuchos;
    int esperaRecarga;
    int ticksDisparo;

    QPixmap hoja;
    int cuadroActual;
    int contadorTicks;
};

#endif