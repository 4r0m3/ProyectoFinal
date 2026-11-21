#include "enemigos.h"

const int ANCHO_ENEMIGO = 50;
const int ALTO_ENEMIGO = 60;

const int RANGO_DISPARO = 350;
const int DIFERENCIA_ALTURA = 70;
const int ESPERA_ENTRE_DISPAROS = 90;
const int RAPIDEZ_BALA_ENEMIGA = 6;

const int RANGO_GRANADA = 400;
const int DISTANCIA_MINIMA_GRANADA = 150;
const int ESPERA_ENTRE_GRANADAS = 150;
const int TICKS_VUELO_GRANADA = 30;
const int GRAVEDAD_GRANADA = 1;

Enemigos::Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho, bool lanzaGranadas)
    : Personaje(x, y, 30),
    velocidadX(2),
    limiteIzquierdo(limiteIzquierdo),
    limiteDerecho(limiteDerecho),
    miraHacia(1),
    esperaDisparo(60),
    lanzaGranadas(lanzaGranadas),
    esperaGranada(90)
{
}

void Enemigos::atacar()
{
}

void Enemigos::actualizar()
{
    if (esperaDisparo > 0) {
        esperaDisparo--;
    }

    if (esperaGranada > 0) {
        esperaGranada--;
    }

    x += velocidadX;

    if (x <= limiteIzquierdo) {
        x = limiteIzquierdo;
        velocidadX = -velocidadX;
    }

    if (x >= limiteDerecho) {
        x = limiteDerecho;
        velocidadX = -velocidadX;
    }

    miraHacia = (velocidadX > 0) ? 1 : -1;
}

Proyectil* Enemigos::disparar(const QRect &objetivo)
{
    if (esperaDisparo > 0) {
        return nullptr;
    }

    QRect cuerpo = getRect();
    int distanciaX = objetivo.center().x() - cuerpo.center().x();
    int distanciaY = objetivo.center().y() - cuerpo.center().y();

    if (qAbs(distanciaX) > RANGO_DISPARO || qAbs(distanciaY) > DIFERENCIA_ALTURA) {
        return nullptr;
    }

    miraHacia = (distanciaX >= 0) ? 1 : -1;
    esperaDisparo = ESPERA_ENTRE_DISPAROS;

    int xBala = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 10;
    int yBala = y + 25;

    return new Proyectil(xBala, yBala, miraHacia, 0, 0, RAPIDEZ_BALA_ENEMIGA);
}

Proyectil* Enemigos::lanzarGranada(const QRect &objetivo)
{
    if (!lanzaGranadas || esperaGranada > 0) {
        return nullptr;
    }

    QRect cuerpo = getRect();
    int distanciaX = objetivo.center().x() - cuerpo.center().x();

    if (qAbs(distanciaX) > RANGO_GRANADA || qAbs(distanciaX) < DISTANCIA_MINIMA_GRANADA) {
        return nullptr;
    }

    miraHacia = (distanciaX >= 0) ? 1 : -1;
    esperaGranada = ESPERA_ENTRE_GRANADAS;

    int xGranada = x + ANCHO_ENEMIGO / 2;
    int yGranada = y;

    int dx = objetivo.center().x() - xGranada;
    int dy = objetivo.center().y() - yGranada;

    int direccion = 1;
    if (dx < 0) {
        direccion = -1;
    }

    int rapidez = (qAbs(dx) + TICKS_VUELO_GRANADA / 2) / TICKS_VUELO_GRANADA;
    int velocidadY = (dy - GRAVEDAD_GRANADA * TICKS_VUELO_GRANADA * (TICKS_VUELO_GRANADA + 1) / 2) / TICKS_VUELO_GRANADA;

    return new Proyectil(xGranada, yGranada, direccion, velocidadY, GRAVEDAD_GRANADA, rapidez);
}

void Enemigos::dibujar(QPainter &pintor)
{
    QColor color(40, 160, 60);
    if (lanzaGranadas) {
        color = QColor(200, 120, 30);
    }

    pintor.fillRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO, color);

    int xCanon = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 14;
    pintor.fillRect(xCanon, y + 23, 14, 6, QColor(30, 30, 30));
}

QRect Enemigos::getRect() const
{
    return QRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO);
}