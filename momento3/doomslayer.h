#ifndef DOOMSLAYER_H
#define DOOMSLAYER_H

#include "personaje.h"

class DoomSlayer : public Personaje
{
public:
    DoomSlayer(int x, int y);

    void correr(int direccion);
    void saltar();
    void disparar();
    void interactuar();
    void actualizar();

private:
    int velocidadX;
    int velocidadY;
    int sueloY;
    bool enElSuelo;
};

#endif