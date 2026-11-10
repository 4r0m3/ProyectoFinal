#include "doomslayer.h"

DoomSlayer::DoomSlayer(int x, int y)
    : Personaje(x, y, 100),
    velocidadX(0),
    velocidadY(0),
    sueloY(y),
    enElSuelo(true)
{
}

void DoomSlayer::correr(int direccion)
{
    velocidadX = direccion * 5;
}

void DoomSlayer::saltar()
{
    if (enElSuelo) {
        velocidadY = -15;
        enElSuelo = false;
    }
}

void DoomSlayer::disparar()
{
}

void DoomSlayer::interactuar()
{
}

void DoomSlayer::actualizar()
{
    x += velocidadX;

    if (!enElSuelo) {
        velocidadY += 1;
        y += velocidadY;

        if (y >= sueloY) {
            y = sueloY;
            velocidadY = 0;
            enElSuelo = true;
        }
    }
}