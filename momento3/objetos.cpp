#include "objetos.h"
#include "doomslayer.h"

const int CANTIDAD_CURACION = 30;

Objetos::Objetos(TipoObjeto tipo, int x, int y)
    : tipo(tipo),
    usado(false)
{
    if (tipo == TipoObjeto::Puerta) {
        zona = QRect(x, y, 45, 80);
    } else if (tipo == TipoObjeto::Llave) {
        zona = QRect(x, y, 20, 20);
    } else {
        zona = QRect(x, y, 24, 24);
    }
}

bool Objetos::interactuar(DoomSlayer *doom)
{
    if (usado) {
        return false;
    }

    if (tipo == TipoObjeto::Llave) {
        doom->recogerLlave();
        usado = true;
        return true;
    }

    if (tipo == TipoObjeto::Curacion) {
        doom->curar(CANTIDAD_CURACION);
        usado = true;
        return true;
    }

    if (tipo == TipoObjeto::Furia) {
        doom->activarFuria();
        usado = true;
        return true;
    }

    if (doom->tieneLlave()) {
        usado = true;
        return true;
    }

    return false;
}

void Objetos::dibujar(QPainter &pintor)
{
    if (tipo == TipoObjeto::Llave) {
        pintor.fillRect(zona, QColor(255, 210, 0));
    } else if (tipo == TipoObjeto::Curacion) {
        pintor.fillRect(zona, QColor(60, 200, 90));
    } else if (tipo == TipoObjeto::Furia) {
        pintor.fillRect(zona, QColor(220, 30, 30));
    } else if (usado) {
        pintor.fillRect(zona, QColor(40, 120, 60));
    } else {
        pintor.fillRect(zona, QColor(150, 100, 50));
    }
}

QRect Objetos::getRect() const
{
    return zona;
}

TipoObjeto Objetos::getTipo() const
{
    return tipo;
}