#ifndef DOOMSLAYER_H
#define DOOMSLAYER_H

#include "personaje.h"
#include "proyectil.h"

class DoomSlayer : public Personaje
{
public:
    DoomSlayer(int x, int y);

    void correr(int direccion);
    void saltar();
    Proyectil* disparar();
    void interactuar();
    void actualizar();

private:
    int velocidadX;
    int velocidadY;
    int sueloY;
    int miraHacia;
    bool enElSuelo;
};

#endif