// personaje.cpp
#include "personaje.h"

Personaje::Personaje(int x, int y, int salud)
    : x(x), y(y), salud(salud)
{
}


int Personaje::getX() const {return x;}
int Personaje::getY() const { return y; }
int Personaje::getSalud() const {return salud; }

void Personaje::recibirDano(int cantidad)
{
    salud -= cantidad;
    if (salud < 0) salud = 0;
}