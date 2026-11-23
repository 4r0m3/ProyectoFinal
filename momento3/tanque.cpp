#include "tanque.h"

const int RAPIDEZ_TANQUE = 2;
const int TICKS_PATRULLA = 180;
const int TICKS_ENTRE_MUESTRAS = 10;
const int NUMERO_DISPAROS = 3;
const int TICKS_APUNTANDO = 75;
const int TICKS_ENTRE_BOMBAS = 75;
const int DISPERSION = 20;
const int TICKS_VUELO = 40;
const int GRAVEDAD_BOMBA = 1;

const int DURACION_RAFAGA = 90;
const int TICKS_ENTRE_BALAS = 10;
const int ENFRIAMIENTO_RAFAGA = 120;
const int TOLERANCIA_SUELO = 15;
const int RAPIDEZ_BALA_TANQUE = 8;

const int PATRULLANDO = 0;
const int DISPARANDO = 1;
const int RAFAGA = 2;

// Configuración del spritesheet
const int ANCHO_CUADRO = 229;
const int ALTO_CUADRO = 284;
const int TOTAL_CUADROS_FILA = 5;
const int FILA_DISPARO = 0;
const int FILA_DESPLAZAMIENTO = 1;
const int TICKS_POR_CUADRO_MOVIMIENTO = 6;
const int TICKS_POR_CUADRO_DISPARO = 15;

Tanque::Tanque(int x, int y, int limiteIzquierdo, int limiteDerecho)
    : Enemigos(x, y, limiteIzquierdo, limiteDerecho),
    limiteIzq(limiteIzquierdo),
    limiteDer(limiteDerecho),
    direccion(-1),
    lado(-1),
    estado(PATRULLANDO),
    ticksPatrulla(0),
    contadorMuestra(0),
    sumaX(0),
    sumaY(0),
    cantidadMuestras(0),
    objetivoX(0),
    objetivoY(0),
    disparosRestantes(0),
    esperaBomba(0),
    ticksRafaga(0),
    esperaBala(0),
    esperaRafaga(60),
    esperaBombaRafaga(0),
    balaListo(nullptr),
    frameActual(0),
    ticksAnimacion(0),
    filaActiva(FILA_DESPLAZAMIENTO)
{
    salud = SALUD_TANQUE;
    spriteSheet.load(":/imagenes/sprite/Tanque");
}

void Tanque::actualizarAnimacion(bool estaMoviendose, bool estaDisparando)
{
    int nuevaFila = estaDisparando ? FILA_DISPARO : FILA_DESPLAZAMIENTO;
    int ticksPorCuadro = estaDisparando ? TICKS_POR_CUADRO_DISPARO : TICKS_POR_CUADRO_MOVIMIENTO;

    if (nuevaFila != filaActiva) {
        filaActiva = nuevaFila;
        frameActual = 0;
        ticksAnimacion = 0;
    }

    if (!estaMoviendose && !estaDisparando) {
        frameActual = 0;
        ticksAnimacion = 0;
        return;
    }

    ticksAnimacion++;
    if (ticksAnimacion >= ticksPorCuadro) {
        ticksAnimacion = 0;
        frameActual = (frameActual + 1) % TOTAL_CUADROS_FILA;
    }
}

Proyectil* Tanque::actualizar(const QRect &jugador)
{
    if (estado == PATRULLANDO) {
        int xAnterior = x;
        x += direccion * RAPIDEZ_TANQUE;

        if (x <= limiteIzq) {
            x = limiteIzq;
            direccion = 1;
        }

        if (x >= limiteDer) {
            x = limiteDer;
            direccion = -1;
        }

        lado = direccion;
        bool seMovio = (x != xAnterior);

        contadorMuestra++;
        if (contadorMuestra >= TICKS_ENTRE_MUESTRAS) {
            contadorMuestra = 0;
            sumaX += jugador.center().x();
            sumaY += jugador.center().y();
            cantidadMuestras++;
        }

        if (esperaRafaga > 0) {
            esperaRafaga--;
        }

        if (esperaRafaga == 0 && jugadorEnElSuelo(jugador)) {
            estado = RAFAGA;
            ticksRafaga = DURACION_RAFAGA;
            esperaBala = 0;
            esperaBombaRafaga = 0;
            actualizarAnimacion(false, true);
            return nullptr;
        }

        ticksPatrulla++;
        if (ticksPatrulla >= TICKS_PATRULLA) {
            // Precisión exacta al jugador
            objetivoX = jugador.center().x();
            objetivoY = jugador.center().y();
            estado = DISPARANDO;
            disparosRestantes = NUMERO_DISPAROS;
            esperaBomba = TICKS_APUNTANDO;

            if (objetivoX >= x + ANCHO_TANQUE / 2) {
                lado = 1;
            } else {
                lado = -1;
            }
            actualizarAnimacion(false, true);
            return nullptr;
        }

        actualizarAnimacion(seMovio, false);
        return nullptr;
    }

    if (estado == RAFAGA) {
        if (jugador.center().x() >= x + ANCHO_TANQUE / 2) {
            lado = 1;
        } else {
            lado = -1;
        }

        esperaBala--;
        if (esperaBala <= 0) {
            esperaBala = TICKS_ENTRE_BALAS;

            int xBala = x - 10;
            if (lado > 0) {
                xBala = x + ANCHO_TANQUE;
            }

            balaListo = new Proyectil(xBala, y + ALTO_TANQUE / 2, lado, 0, 0, RAPIDEZ_BALA_TANQUE);
        }

        esperaBombaRafaga--;
        if (esperaBombaRafaga <= 0) {
            esperaBombaRafaga = TICKS_ENTRE_BOMBAS;

            Proyectil *bomba = crearBomba(jugador.center().x(), jugador.center().y());

            ticksRafaga--;
            if (ticksRafaga <= 0) {
                estado = PATRULLANDO;
                esperaRafaga = ENFRIAMIENTO_RAFAGA;
                actualizarAnimacion(true, false);
            } else {
                actualizarAnimacion(false, true);
            }
            return bomba;
        }

        ticksRafaga--;
        if (ticksRafaga <= 0) {
            estado = PATRULLANDO;
            esperaRafaga = ENFRIAMIENTO_RAFAGA;
            actualizarAnimacion(true, false);
            return nullptr;
        }

        actualizarAnimacion(false, true);
        return nullptr;
    }

    actualizarAnimacion(false, true);

    esperaBomba--;
    if (esperaBomba > 0) {
        return nullptr;
    }

    int numero = NUMERO_DISPAROS - disparosRestantes;
    int desvio = (numero - 1) * DISPERSION;
    Proyectil *bomba = crearBomba(objetivoX + desvio, objetivoY);

    disparosRestantes--;
    esperaBomba = TICKS_ENTRE_BOMBAS;

    if (disparosRestantes == 0) {
        estado = PATRULLANDO;
        ticksPatrulla = 0;
        contadorMuestra = 0;
        sumaX = 0;
        sumaY = 0;
        cantidadMuestras = 0;
    }

    return bomba;
}

Proyectil* Tanque::crearBomba(int objetivoX, int objetivoY)
{
    int xBomba = x + ANCHO_TANQUE / 2;
    int yBomba = y;

    int dx = objetivoX - xBomba;
    int dy = objetivoY - yBomba;

    int direccionBomba = 1;
    if (dx < 0) {
        direccionBomba = -1;
    }

    int rapidez = (qAbs(dx) + TICKS_VUELO / 2) / TICKS_VUELO;
    int velocidadY = (dy - GRAVEDAD_BOMBA * TICKS_VUELO * (TICKS_VUELO + 1) / 2) / TICKS_VUELO;

    return new Proyectil(xBomba, yBomba, direccionBomba, velocidadY, GRAVEDAD_BOMBA, rapidez);
}

void Tanque::dibujar(QPainter &pintor)
{
    if (!spriteSheet.isNull()) {
        QRect recorte(frameActual * ANCHO_CUADRO, filaActiva * ALTO_CUADRO, ANCHO_CUADRO, ALTO_CUADRO);
        QPixmap cuadro = spriteSheet.copy(recorte);

        if (lado < 0) {
            cuadro = cuadro.transformed(QTransform().scale(-1, 1));
        }
        const float escalaVisual = 1.6f;
        const int offsetX = 0;
        const int offsetY = 50;
        int anchoVisual = static_cast<int>(ANCHO_TANQUE * escalaVisual);
        int altoVisual = (anchoVisual * ALTO_CUADRO) / ANCHO_CUADRO;

        int centroX = x + ANCHO_TANQUE / 2;
        int xVisual = centroX - anchoVisual / 2;
        int yVisual = (y + ALTO_TANQUE) - altoVisual + offsetY;

        pintor.drawPixmap(xVisual, yVisual, anchoVisual, altoVisual, cuadro);
    } else {
        pintor.fillRect(x, y, ANCHO_TANQUE, ALTO_TANQUE, QColor(70, 90, 60));
    }

    if (estado == DISPARANDO) {
        pintor.fillRect(objetivoX - 20, y + ALTO_TANQUE - 4, 40, 4, QColor(255, 0, 0, 160));
    }
}

QRect Tanque::getRect() const
{
    return QRect(x, y, ANCHO_TANQUE, ALTO_TANQUE);
}

Proyectil* Tanque::tomarBala()
{
    Proyectil *bala = balaListo;
    balaListo = nullptr;
    return bala;
}

bool Tanque::jugadorEnElSuelo(const QRect &jugador) const
{
    int diferencia = jugador.bottom() - getRect().bottom();
    return qAbs(diferencia) <= TOLERANCIA_SUELO;
}