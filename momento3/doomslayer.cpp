#include "doomslayer.h"

const int TAMANO = 30;
const int ESCALA = 2;

const int QUIETO_X = 4;
const int QUIETO_Y = 4;

const int CAMINAR_X = 4;
const int CAMINAR_Y = 34;
const int CUADROS_CAMINAR = 2;
const int TICKS_POR_CUADRO = 6;

const int DISPARO_X = 4;
const int DISPARO_Y = 67;
const int TICKS_SPRITE_DISPARO = 10;

const int CARTUCHOS_MAXIMOS = 2;
const int TICKS_DE_RECARGA = 60;

DoomSlayer::DoomSlayer(int x, int y)
    : Personaje(x, y, 100),
    velocidadX(0),
    velocidadY(0),
    sueloY(y),
    miraHacia(1),
    enElSuelo(true),
    cartuchos(CARTUCHOS_MAXIMOS),
    esperaRecarga(0),
    ticksDisparo(0),
    cuadroActual(0),
    contadorTicks(0)
{
    hoja.load(":/imagenes/sprite/Movimientos");
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

QList<Proyectil*> DoomSlayer::disparar()
{
    QList<Proyectil*> balas;

    if (cartuchos <= 0) {
        return balas;
    }

    cartuchos--;
    ticksDisparo = TICKS_SPRITE_DISPARO;

    if (cartuchos == 0) {
        esperaRecarga = TICKS_DE_RECARGA;
    }

    int xBala = (miraHacia > 0) ? x + TAMANO * ESCALA : x - 10;
    int yBala = y + 25;

    balas.append(new Proyectil(xBala, yBala, miraHacia));

    return balas;
}

void DoomSlayer::interactuar()
{
}

void DoomSlayer::actualizar()
{
    x += velocidadX;

    if (ticksDisparo > 0) {
        ticksDisparo--;
    }

    if (esperaRecarga > 0) {
        esperaRecarga--;
        if (esperaRecarga == 0) {
            cartuchos = CARTUCHOS_MAXIMOS;
        }
    }

    if (!enElSuelo) {
        velocidadY += 1;
        y += velocidadY;

        if (y >= sueloY) {
            y = sueloY;
            velocidadY = 0;
            enElSuelo = true;
        }
    }

    if (velocidadX != 0) {
        contadorTicks++;
        if (contadorTicks >= TICKS_POR_CUADRO) {
            contadorTicks = 0;
            cuadroActual = (cuadroActual + 1) % CUADROS_CAMINAR;
        }
    } else {
        cuadroActual = 0;
        contadorTicks = 0;
    }
}

void DoomSlayer::dibujar(QPainter &pintor)
{
    QRect origen;
    if (ticksDisparo > 0) {
        origen = QRect(DISPARO_X, DISPARO_Y, TAMANO, TAMANO);
    } else if (velocidadX == 0) {
        origen = QRect(QUIETO_X, QUIETO_Y, TAMANO, TAMANO);
    } else {
        origen = QRect(CAMINAR_X + cuadroActual * TAMANO, CAMINAR_Y, TAMANO, TAMANO);
    }

    int lado = TAMANO * ESCALA;

    if (miraHacia > 0) {
        pintor.drawPixmap(QRect(x, y, lado, lado), hoja, origen);
    } else {
        pintor.save();
        pintor.translate(x + lado, y);
        pintor.scale(-1, 1);
        pintor.drawPixmap(QRect(0, 0, lado, lado), hoja, origen);
        pintor.restore();
    }
}