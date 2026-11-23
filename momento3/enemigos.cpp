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

const int ANCHO_CUADRO_POLICIA = 307;
const int ALTO_CUADRO_POLICIA = 341;
const int TOTAL_CUADROS_FILA = 5;
const int FILA_CAMINATA = 0;
const int FILA_GRANADA = 1;
const int FILA_DISPARO = 2;
const int TICKS_POR_CUADRO_CAMINATA = 7;
const int TICKS_POR_CUADRO_GRANADA = 12;
const int TICKS_POR_CUADRO_DISPARO = 5;
const int DURACION_ANIMACION_GRANADA = TOTAL_CUADROS_FILA * TICKS_POR_CUADRO_GRANADA;
const int DURACION_ANIMACION_DISPARO = TOTAL_CUADROS_FILA * TICKS_POR_CUADRO_DISPARO;

const float ESCALA_VISUAL_POLICIA = 1.5f;
const int OFFSET_X_POLICIA = 0;
const int OFFSET_Y_POLICIA = 10;

Enemigos::Enemigos(int x, int y, int limiteIzquierdo, int limiteDerecho, bool lanzaGranadas)
    : Personaje(x, y, 30),
    velocidadX(2),
    limiteIzquierdo(limiteIzquierdo),
    limiteDerecho(limiteDerecho),
    miraHacia(1),
    esperaDisparo(60),
    lanzaGranadas(lanzaGranadas),
    esperaGranada(90),
    frameActual(0),
    ticksAnimacion(0),
    ticksAnimacionDisparo(0),
    ticksAnimacionGranada(0),
    granadaLista(nullptr),
    tieneObjetivoGranada(false),
    ultimoObjetivoGranada()
{
    spritePolicia.load(":/imagenes/sprite/Policias");
}

Enemigos::~Enemigos()
{
    delete granadaLista;
}

void Enemigos::atacar()
{
}

Proyectil* Enemigos::crearProyectilGranada(const QRect &objetivo)
{
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

void Enemigos::actualizar()
{
    if (esperaDisparo > 0) {
        esperaDisparo--;
    }

    if (esperaGranada > 0) {
        esperaGranada--;
    }

    int xAnterior = x;
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

    if (ticksAnimacionDisparo > 0) {
        ticksAnimacionDisparo--;
        int transcurridos = DURACION_ANIMACION_DISPARO - ticksAnimacionDisparo - 1;
        frameActual = (transcurridos / TICKS_POR_CUADRO_DISPARO) % TOTAL_CUADROS_FILA;
        return;
    }

    if (ticksAnimacionGranada > 0) {
        ticksAnimacionGranada--;
        int transcurridos = DURACION_ANIMACION_GRANADA - ticksAnimacionGranada - 1;
        frameActual = (transcurridos / TICKS_POR_CUADRO_GRANADA) % TOTAL_CUADROS_FILA;

        if (ticksAnimacionGranada == 0 && tieneObjetivoGranada) {
            miraHacia = (ultimoObjetivoGranada.center().x() >= getRect().center().x()) ? 1 : -1;
            delete granadaLista;
            granadaLista = crearProyectilGranada(ultimoObjetivoGranada);
            tieneObjetivoGranada = false;
            esperaGranada = ESPERA_ENTRE_GRANADAS;
            frameActual = 0;
            ticksAnimacion = 0;
        }
        return;
    }

    if (x != xAnterior) {
        ticksAnimacion++;
        if (ticksAnimacion >= TICKS_POR_CUADRO_CAMINATA) {
            ticksAnimacion = 0;
            frameActual = (frameActual + 1) % TOTAL_CUADROS_FILA;
        }
    } else {
        frameActual = 0;
        ticksAnimacion = 0;
    }
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
    ticksAnimacionGranada = 0;
    tieneObjetivoGranada = false;
    ticksAnimacionDisparo = DURACION_ANIMACION_DISPARO;
    frameActual = 0;

    int xBala = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 10;
    int yBala = y + 25;

    return new Proyectil(xBala, yBala, miraHacia, 0, 0, RAPIDEZ_BALA_ENEMIGA);
}

Proyectil* Enemigos::lanzarGranada(const QRect &objetivo)
{
    if (!lanzaGranadas || esperaGranada > 0 || ticksAnimacionGranada > 0) {
        return nullptr;
    }

    QRect cuerpo = getRect();
    int distanciaX = objetivo.center().x() - cuerpo.center().x();

    if (qAbs(distanciaX) > RANGO_GRANADA || qAbs(distanciaX) < DISTANCIA_MINIMA_GRANADA) {
        return nullptr;
    }

    miraHacia = (distanciaX >= 0) ? 1 : -1;
    ticksAnimacionDisparo = 0;
    ticksAnimacionGranada = DURACION_ANIMACION_GRANADA;
    tieneObjetivoGranada = true;
    ultimoObjetivoGranada = objetivo;
    frameActual = 0;

    // La granada se creará y devolverá vía tomarGranada() al terminar la animación
    return nullptr;
}

Proyectil* Enemigos::tomarGranada()
{
    Proyectil *granada = granadaLista;
    granadaLista = nullptr;
    return granada;
}

void Enemigos::dibujar(QPainter &pintor)
{
    if (!spritePolicia.isNull()) {
        int filaActiva = FILA_CAMINATA;
        if (ticksAnimacionDisparo > 0) {
            filaActiva = FILA_DISPARO;
        } else if (ticksAnimacionGranada > 0) {
            filaActiva = FILA_GRANADA;
        }

        QRect recorte(
            frameActual * ANCHO_CUADRO_POLICIA,
            filaActiva * ALTO_CUADRO_POLICIA,
            ANCHO_CUADRO_POLICIA,
            ALTO_CUADRO_POLICIA
            );
        QPixmap cuadro = spritePolicia.copy(recorte);

        if (miraHacia < 0) {
            cuadro = cuadro.transformed(QTransform().scale(-1, 1));
        }

        int altoVisual = static_cast<int>(ALTO_ENEMIGO * ESCALA_VISUAL_POLICIA);
        int anchoVisual = (altoVisual * ANCHO_CUADRO_POLICIA) / ALTO_CUADRO_POLICIA;
        int xVisual = x + ANCHO_ENEMIGO / 2 - anchoVisual / 2 + OFFSET_X_POLICIA;
        int yVisual = (y + ALTO_ENEMIGO) - altoVisual + OFFSET_Y_POLICIA;

        pintor.drawPixmap(xVisual, yVisual, anchoVisual, altoVisual, cuadro);
    } else {
        QColor color(40, 160, 60);
        if (lanzaGranadas) {
            color = QColor(200, 120, 30);
        }

        pintor.fillRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO, color);

        int xCanon = (miraHacia > 0) ? x + ANCHO_ENEMIGO : x - 14;
        pintor.fillRect(xCanon, y + 23, 14, 6, QColor(30, 30, 30));
    }
}

QRect Enemigos::getRect() const
{
    return QRect(x, y, ANCHO_ENEMIGO, ALTO_ENEMIGO);
}