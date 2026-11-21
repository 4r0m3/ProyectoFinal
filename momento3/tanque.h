#ifndef TANQUE_H
#define TANQUE_H

#include <QPainter>
#include <QRect>
#include "enemigos.h"
#include "proyectil.h"

const int ANCHO_TANQUE = 100;
const int ALTO_TANQUE = 50;
const int SALUD_TANQUE = 150;

class Tanque : public Enemigos
{
public:
    Tanque(int x, int y, int limiteIzquierdo, int limiteDerecho);

    Proyectil* actualizar(const QRect &jugador);
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    Proyectil* tomarBala();

private:
    Proyectil* crearBomba(int objetivoX, int objetivoY);
    bool jugadorEnElSuelo(const QRect &jugador) const;

    int limiteIzq;
    int limiteDer;
    int direccion;
    int lado;
    int estado;
    int ticksPatrulla;
    int contadorMuestra;
    int sumaX;
    int sumaY;
    int cantidadMuestras;
    int objetivoX;
    int objetivoY;
    int disparosRestantes;
    int esperaBomba;
    int ticksRafaga;
    int esperaBala;
    int esperaRafaga;
    Proyectil *balaListo;
};

#endif