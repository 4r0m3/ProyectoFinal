#include "objetos.h"
#include "doomslayer.h"

const int CANTIDAD_CURACION = 30;
const bool MOSTRAR_ZONA_JEFE = true;   // true para ver la zona mientras pruebas

Objetos::Objetos(TipoObjeto tipo, int x, int y, int ancho, int alto)
    : tipo(tipo),
    usado(false)
{
    if (tipo == TipoObjeto::ZonaJefe) {
        zona = QRect(x, y, ancho, alto);
    } else if (tipo == TipoObjeto::Puerta) {
        zona = QRect(x, y, 45, 80);
    } else if (tipo == TipoObjeto::Llave) {
        zona = QRect(x, y, 20, 20);
    } else {
        zona = QRect(x, y, 24, 24);
    }
}

bool Objetos::interactuar(DoomSlayer *doom)
{
    if (usado || tipo == TipoObjeto::ZonaJefe) {
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

// Se activa una sola vez cuando el cuerpo toca la zona (sin teclado).
bool Objetos::detectarContacto(const QRect &cuerpo)
{
    if (tipo != TipoObjeto::ZonaJefe || usado) {
        return false;
    }

    if (!zona.intersects(cuerpo)) {
        return false;
    }

    usado = true;
    return true;
}

void Objetos::dibujar(QPainter &pintor)
{
    if (tipo == TipoObjeto::ZonaJefe) {
        if (MOSTRAR_ZONA_JEFE) {
            pintor.fillRect(zona, QColor(255, 0, 255, usado ? 20 : 60));
        }
        return;
    }

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