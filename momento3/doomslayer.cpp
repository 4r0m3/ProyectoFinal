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

const int TICKS_INVULNERABLE = 60;
const int DURACION_FURIA = 300;
const int VELOCIDAD_NORMAL = 5;
const int VELOCIDAD_FURIA = 8;

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
    ticksInvulnerable(0),
    cuadroActual(0),
    contadorTicks(0),
    llave(false),
    ticksFuria(0),
    direccionX(0)
{
    hoja.load(":/imagenes/sprite/Movimientos");
}

void DoomSlayer::correr(int direccion)
{
    direccionX = direccion;
    velocidadX = direccion * VELOCIDAD_NORMAL;
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

    bool enFuria = ticksFuria > 0;

    if (!enFuria && cartuchos <= 0) {
        return balas;
    }

    ticksDisparo = TICKS_SPRITE_DISPARO;

    if (!enFuria) {
        cartuchos--;

        if (cartuchos == 0) {
            esperaRecarga = TICKS_DE_RECARGA;
        }
    }

    int xBala = (miraHacia > 0) ? x + TAMANO * ESCALA : x - 10;
    int yBala = y + 25;

    balas.append(new Proyectil(xBala, yBala, miraHacia));

    return balas;
}

void DoomSlayer::interactuar()
{
}

void DoomSlayer::golpeado(int direccion, int dano)
{
    if (ticksInvulnerable > 0) {
        return;
    }

    recibirDano(dano);
    ticksInvulnerable = TICKS_INVULNERABLE;

    x += direccion * 40;
    velocidadY = -8;
    enElSuelo = false;
}

QRect DoomSlayer::getRect() const
{
    return QRect(x, y, TAMANO * ESCALA, TAMANO * ESCALA);
}

void DoomSlayer::actualizar()
{
    int rapidez = (ticksFuria > 0) ? VELOCIDAD_FURIA : VELOCIDAD_NORMAL;
    x += direccionX * rapidez;

    if (ticksDisparo > 0) {
        ticksDisparo--;
    }

    if (ticksInvulnerable > 0) {
        ticksInvulnerable--;
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

    if (ticksFuria > 0) {
        ticksFuria--;
    }
}

void DoomSlayer::dibujar(QPainter &pintor)
{
    if (ticksInvulnerable > 0 && (ticksInvulnerable / 5) % 2 == 0) {
        return;
    }

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

void DoomSlayer::setSuelo(int nuevoSuelo)
{
    sueloY = nuevoSuelo;

    if (enElSuelo && y < sueloY) {
        enElSuelo = false;
        velocidadY = 0;
    }
}
bool DoomSlayer::estaEnElSuelo() const
{
    return enElSuelo;
}

void DoomSlayer::moverX(int cantidad)
{
    x += cantidad;
}
void DoomSlayer::moverY(int cantidad)
{
    y += cantidad;
}

void DoomSlayer::reaparecer(int nuevaX, int nuevaY)
{
    x = nuevaX;
    y = nuevaY;
    velocidadY = 0;
    sueloY = nuevaY;
    enElSuelo = true;
}

void DoomSlayer::recogerLlave()
{
    llave = true;
}

bool DoomSlayer::tieneLlave() const
{
    return llave;
}

void DoomSlayer::curar(int cantidad)
{
    salud += cantidad;
    if (salud > 100) {
        salud = 100;
    }
}

void DoomSlayer::activarFuria()
{
    ticksFuria = DURACION_FURIA;
    cartuchos = CARTUCHOS_MAXIMOS;
    esperaRecarga = 0;
}

int DoomSlayer::getTicksFuria() const
{
    return ticksFuria;
}