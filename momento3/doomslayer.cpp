#include "doomslayer.h"

DoomSlayer::DoomSlayer(int x, int y)
    : Personaje(x, y, 100),
    velocidadX(0),
    velocidadY(0),
    sueloY(y),
    miraHacia(1),
    enElSuelo(true)
{
}

void DoomSlayer::correr(int direccion)
{
    velocidadX = direccion * 5;
    if (direccion != 0) {
        miraHacia = direccion;
    }
}

void DoomSlayer::saltar()
{
    if (enElSuelo) {
        velocidadY = -15;
        enElSuelo = false;
    }
}

Proyectil* DoomSlayer::disparar()
{
    int xBala = (miraHacia > 0) ? x + 40 : x - 10;
    int yBala = y + 25;
    return new Proyectil(xBala, yBala, miraHacia);
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