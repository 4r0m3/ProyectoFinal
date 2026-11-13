#ifndef DOOMSLAYER_H
#define DOOMSLAYER_H

#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QRect>
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
    void setSuelo(int nuevoSuelo);
    bool estaEnElSuelo() const;
    void moverX(int cantidad);
    void moverY(int cantidad);
    void reaparecer(int nuevaX, int nuevaY);
    void recogerLlave();
    bool tieneLlave() const;
    void curar(int cantidad);
    void activarFuria();
    int getTicksFuria() const;

    void golpeado(int direccion, int dano);
    QRect getRect() const;

private:
    int velocidadX;
    int velocidadY;
    int direccionX;
    int sueloY;
    int miraHacia;
    bool enElSuelo;

    int cartuchos;
    int esperaRecarga;
    int ticksDisparo;
    int ticksInvulnerable;
    bool llave;
    int ticksFuria;

    QPixmap hoja;
    int cuadroActual;
    int contadorTicks;
};

#endif