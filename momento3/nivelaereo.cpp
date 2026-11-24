#include "nivelaereo.h"
#include <cmath>
#include <cstdlib>

const int TAM_BLOQUE = 40;
const int FILAS = 23;
const int COLUMNAS = 39;

const int TAM_JUGADOR = 26;
const double VELOCIDAD_JUGADOR = 4;
const int SALUD_MAXIMA = 100;
const int TICKS_INVULNERABLE = 30;
const int ESPERA_DISPARO_JUGADOR = 50;
const int DISPAROS_POR_RAFAGA = 1;
const int ESPERA_ENTRE_DISPAROS = 15;
const double VELOCIDAD_BALA_JUGADOR = 10;
const int DANO_BALA_JUGADOR = 10;

const int TAM_ENEMIGO = 28;
const int SALUD_ENEMIGO = 30;
const double VELOCIDAD_PATRULLA = 1.5;
const double VELOCIDAD_PERSECUCION = 2.2;
const int VISION = 320;
const int ESPERA_DISPARO_ENEMIGO = 100;
const double VELOCIDAD_BALA_ENEMIGA = 5;
const int DANO_BALA_ENEMIGA = 10;
const int DANO_CONTACTO = 15;

const double VELOCIDAD_VIGILANTE = 2.4;
const int ALCANCE_SENAL = 14;
const int TICKS_BUSCAR = 150;

const int PATRULLA = 0;
const int INVESTIGAR = 1;
const int PERSEGUIR = 2;
const int BUSCAR = 3;

const int CURACION = 30;

const int TAM_SANGRE = 50;
const int MAX_MANCHAS = 80;

const char *MAPA[FILAS] = {
    "#######################################",
    "#P..#.......#..E....#.....#.....#.....#",
    "###.#.#####.#.###.#.#.#.###.#.#.#.#####",
    "#...#.#..S#.#.#.#.#...#.#...#.#...#...#",
    "#.###.#.###.#.#.#.#######.###.#####.#.#",
    "#.#...#...#.#.#.#...#V....#.#.......#E#",
    "#.#.#####.#.#.#.###.#.#####.#########.#",
    "#.#.#.....#.......#..V....#.....#...D.#",
    "#.#.#.###.#####.#########.#####.#.#####",
    "#.#...#...#...#.#.......#.......#.....#",
    "#.###.#####.#.#.#.#####.#.###########.#",
    "#...#.#E....#.#.#...#.#.#H#.........#.#",
    "###.#.#.#####.#####.#.#.###.#######.#.#",
    "#...#.#.#L.V#.....#.#.#...#.......#...#",
    "#.###.#.###.#####.#.#.###.#######.#####",
    "#.#...#.#...#.....#.#..H#.......#.#...#",
    "#.#####.#.###.#####D#.#########.#.#.#.#",
    "#E#.....#.....#..V..#.#.....#...#...#.#",
    "#.#.#####.#########.#.#.###.#.#######.#",
    "#...#.#...#...#.....#.#.#.#.#.#H......#",
    "#####.#.###.#.#.#####.#.#.#.#.###.###.#",
    "#H..........#..E#L........#.......#...#",
    "#######################################"
};

NivelAereo::NivelAereo(int ancho, int alto)
    : ancho(ancho),
    alto(alto),
    jugadorX(0),
    jugadorY(0),
    inicioX(0),
    inicioY(0),
    salud(SALUD_MAXIMA),
    llaves(0),
    ticksInvulnerable(0),
    esperaDisparo(0),
    disparando(false),
    mouseX(0),
    mouseY(0),
    camaraX(0),
    camaraY(0),
    nivelCompleto(false),
    ticksMensaje(0),
    frameAnimacionJugador(0),
    ticksAnimacionJugador(0),
    direccionMiradaJugador(0),
    jugadorDisparando(false),
    ticksDisparoJugador(0)
{

    escalaPolicia = 2.5;
    escalaPoliciaAlterado = 2.5;
    escalaPoliciaBusqueda = 2.5;
    escalaJugador = 2.5;

    QPixmap spriteSheet(":/imagenes/sprite/Pasillos.png");
    if (!spriteSheet.isNull()) {
        int anchoTextura = spriteSheet.width() / 3;
        int altoTextura = spriteSheet.height();

        texPared = spriteSheet.copy(0, 0, anchoTextura, altoTextura).scaled(TAM_BLOQUE, TAM_BLOQUE, Qt::IgnoreAspectRatio);
        texSuelo = spriteSheet.copy(anchoTextura, 0, anchoTextura, altoTextura).scaled(TAM_BLOQUE, TAM_BLOQUE, Qt::IgnoreAspectRatio);
        texPuerta = spriteSheet.copy(anchoTextura * 2, 0, anchoTextura, altoTextura).scaled(TAM_BLOQUE, TAM_BLOQUE, Qt::IgnoreAspectRatio);
    }

    texPolicia = QPixmap(":/imagenes/sprite/PoliciaNivelAereo.png");
    texPoliciaAlterado = QPixmap(":/imagenes/sprite/PoliciaAlterado.png");
    texPoliciaBusqueda = QPixmap(":/imagenes/sprite/PoliciaBusqueda.png");
    texJugador = QPixmap(":/imagenes/sprite/Personaje.png");

    texSangre = QPixmap(":/imagenes/sprite/sangre-fondo.png");
    if (!texSangre.isNull()) {
        texSangre = texSangre.scaled(TAM_SANGRE, TAM_SANGRE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    sonidoDisparo.setSource(QUrl("qrc:/Sonido/sonidos/sound-gun-shot.wav"));
    sonidoDisparo.setVolume(0.6);
    sonidoDisparoEnemigo.setSource(QUrl("qrc:/Sonido/sonidos/sound-gun-shot.wav"));
    sonidoDisparoEnemigo.setVolume(0.3);
    sonidoRespawn.setSource(QUrl("qrc:/Sonido/sonidos/sound-respaw.wav"));
    sonidoAlerta.setSource(QUrl("qrc:/Sonido/sonidos/sound-enemy-alert.wav"));
    sonidoVictoria.setSource(QUrl("qrc:/Sonido/sonidos/sound-winner.wav"));
    sonidoMuerte.setSource(QUrl("qrc:/Sonido/sonidos/sound-enemy-dead.wav"));
    sonidoPuerta.setSource(QUrl("qrc:/Sonido/sonidos/sound-open-door.wav"));
    sonidoRecoger.setSource(QUrl("qrc:/Sonido/sonidos/sound-collet.wav"));
    sonidoRecoger.setVolume(0.6);

    SonidoFondo.setSource(QUrl("qrc:/Sonido/sonidos/sound-background.wav"));
    SonidoFondo.setVolume(0.35);
    SonidoFondo.setLoopCount(QSoundEffect::Infinite);
    SonidoFondo.play();

    for (int fila = 0; fila < FILAS; fila++) {
        QString linea = MAPA[fila];

        for (int columna = 0; columna < COLUMNAS; columna++) {
            if (linea[columna] == 'P') {
                inicioX = columna * TAM_BLOQUE + (TAM_BLOQUE - TAM_JUGADOR) / 2;
                inicioY = fila * TAM_BLOQUE + (TAM_BLOQUE - TAM_JUGADOR) / 2;
                linea[columna] = '.';
            }

            if (linea[columna] == 'E' || linea[columna] == 'V') {
                EnemigoAereo enemigo;
                enemigo.x = columna * TAM_BLOQUE + (TAM_BLOQUE - TAM_ENEMIGO) / 2;
                enemigo.y = fila * TAM_BLOQUE + (TAM_BLOQUE - TAM_ENEMIGO) / 2;
                enemigo.salud = SALUD_ENEMIGO;
                enemigo.dirX = 1;
                enemigo.dirY = 0;
                enemigo.espera = ESPERA_DISPARO_ENEMIGO;
                enemigo.inteligente = (linea[columna] == 'V');
                enemigo.estado = PATRULLA;
                enemigo.ticksEstado = 0;
                enemigo.objetivoColumna = columna;
                enemigo.objetivoFila = fila;

                enemigo.frameAnimacion = 0;
                enemigo.ticksAnimacion = 0;
                enemigo.direccionMirada = 0;
                enemigo.estaDisparando = false;
                enemigo.ticksDisparo = 0;

                enemigos.append(enemigo);
                linea[columna] = '.';
            }
        }
        mapa.append(linea);
    }

    jugadorX = inicioX;
    jugadorY = inicioY;
    actualizarCamara();
}

void NivelAereo::teclaPresionada(int tecla) { if (nivelCompleto) return; teclas.insert(tecla); }
void NivelAereo::teclaSoltada(int tecla) { teclas.remove(tecla); }
void NivelAereo::mouseMovido(int x, int y) { mouseX = x; mouseY = y; }
void NivelAereo::mousePresionado(int x, int y) { mouseX = x; mouseY = y; disparando = true; }
void NivelAereo::mouseSoltado() { disparando = false; }
bool NivelAereo::estaCompleto() const { return nivelCompleto; }

char NivelAereo::bloqueEn(int columna, int fila) const
{
    if (columna < 0 || columna >= COLUMNAS || fila < 0 || fila >= FILAS) return '#';
    return mapa[fila][columna].toLatin1();
}

bool NivelAereo::esSolido(char bloque) const { return bloque == '#' || bloque == 'D'; }

bool NivelAereo::chocaConPared(double x, double y, int tam) const
{
    int columnaInicial = (int)x / TAM_BLOQUE;
    int columnaFinal = (int)(x + tam - 1) / TAM_BLOQUE;
    int filaInicial = (int)y / TAM_BLOQUE;
    int filaFinal = (int)(y + tam - 1) / TAM_BLOQUE;

    for (int fila = filaInicial; fila <= filaFinal; fila++) {
        for (int columna = columnaInicial; columna <= columnaFinal; columna++) {
            if (esSolido(bloqueEn(columna, fila))) return true;
        }
    }
    return false;
}

bool NivelAereo::hayLineaLibre(double x1, double y1, double x2, double y2) const
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double distancia = std::sqrt(dx * dx + dy * dy);
    int pasos = (int)(distancia / 8) + 1;

    for (int i = 0; i <= pasos; i++) {
        double px = x1 + dx * i / pasos;
        double py = y1 + dy * i / pasos;
        if (esSolido(bloqueEn((int)px / TAM_BLOQUE, (int)py / TAM_BLOQUE))) return false;
    }
    return true;
}

void NivelAereo::abrirPuertas(double x, double y, int tam)
{
    int columnaInicial = (int)x / TAM_BLOQUE;
    int columnaFinal = (int)(x + tam - 1) / TAM_BLOQUE;
    int filaInicial = (int)y / TAM_BLOQUE;
    int filaFinal = (int)(y + tam - 1) / TAM_BLOQUE;

    for (int fila = filaInicial; fila <= filaFinal; fila++) {
        for (int columna = columnaInicial; columna <= columnaFinal; columna++) {
            if (bloqueEn(columna, fila) == 'D') {
                if (llaves > 0) {
                    llaves--;
                    mapa[fila][columna] = '.';
                    mostrarMensaje("Puerta abierta");
                    sonidoPuerta.play();
                } else {
                    mostrarMensaje("Necesitas una llave");
                }
            }
        }
    }
}

void NivelAereo::moverJugador(double dx, double dy)
{
    double nuevoX = jugadorX + dx;
    if (chocaConPared(nuevoX, jugadorY, TAM_JUGADOR)) abrirPuertas(nuevoX, jugadorY, TAM_JUGADOR);
    if (!chocaConPared(nuevoX, jugadorY, TAM_JUGADOR)) jugadorX = nuevoX;

    double nuevoY = jugadorY + dy;
    if (chocaConPared(jugadorX, nuevoY, TAM_JUGADOR)) abrirPuertas(jugadorX, nuevoY, TAM_JUGADOR);
    if (!chocaConPared(jugadorX, nuevoY, TAM_JUGADOR)) jugadorY = nuevoY;
}

bool NivelAereo::moverEnemigo(EnemigoAereo &enemigo, double dx, double dy)
{
    bool libre = true;
    if (!chocaConPared(enemigo.x + dx, enemigo.y, TAM_ENEMIGO)) { enemigo.x += dx; } else { libre = false; }
    if (!chocaConPared(enemigo.x, enemigo.y + dy, TAM_ENEMIGO)) { enemigo.y += dy; } else { libre = false; }
    return libre;
}

void NivelAereo::recogerObjetos()
{
    int columna = (int)(jugadorX + TAM_JUGADOR / 2) / TAM_BLOQUE;
    int fila = (int)(jugadorY + TAM_JUGADOR / 2) / TAM_BLOQUE;
    char bloque = bloqueEn(columna, fila);

    if (bloque == 'L') {
        llaves++;
        mapa[fila][columna] = '.';
        mostrarMensaje("Llave recogida");
        sonidoRecoger.play();
    }
    if (bloque == 'H') {
        if (salud < SALUD_MAXIMA) {
            salud += CURACION;
            if (salud > SALUD_MAXIMA) salud = SALUD_MAXIMA;
            mapa[fila][columna] = '.';
            sonidoRecoger.play();
        }
    }
    if (bloque == 'S' && !nivelCompleto) {
        nivelCompleto = true;
        sonidoVictoria.play();
    }
}

void NivelAereo::actualizarJugador()
{
    double dx = 0, dy = 0;
    if (teclas.contains(Qt::Key_A)) dx -= 1;
    if (teclas.contains(Qt::Key_D)) dx += 1;
    if (teclas.contains(Qt::Key_W)) dy -= 1;
    if (teclas.contains(Qt::Key_S)) dy += 1;

    if (dx != 0 && dy != 0) { dx *= 0.7071; dy *= 0.7071; }

    moverJugador(dx * VELOCIDAD_JUGADOR, dy * VELOCIDAD_JUGADOR);
    recogerObjetos();

    if (ticksInvulnerable > 0) ticksInvulnerable--;
    if (esperaDisparo > 0) esperaDisparo--;

    double centroX = jugadorX + TAM_JUGADOR / 2;
    double centroY = jugadorY + TAM_JUGADOR / 2;
    double apuntaX = mouseX + camaraX - centroX;
    double apuntaY = mouseY + camaraY - centroY;
    double distancia = std::sqrt(apuntaX * apuntaX + apuntaY * apuntaY);

    if (disparando && esperaDisparo == 0 && distancia > 1) {
        crearBalaJugador(centroX, centroY, apuntaX, apuntaY, distancia);
        disparosPendientes = DISPAROS_POR_RAFAGA - 1;
        esperaEntreDisparos = ESPERA_ENTRE_DISPAROS;
        esperaDisparo = ESPERA_DISPARO_JUGADOR;
        jugadorDisparando = true;
        ticksDisparoJugador = 25;
        frameAnimacionJugador = 0;
    }

    if (disparosPendientes > 0) {
        esperaEntreDisparos--;
        if (esperaEntreDisparos <= 0 && distancia > 1) {
            crearBalaJugador(centroX, centroY, apuntaX, apuntaY, distancia);
            disparosPendientes--;
            esperaEntreDisparos = ESPERA_ENTRE_DISPAROS;
            ticksDisparoJugador = 25;
        }
    }

    if (jugadorDisparando) {
        if (std::fabs(apuntaX) > std::fabs(apuntaY)) {
            direccionMiradaJugador = (apuntaX > 0) ? 2 : 3;
        } else {
            direccionMiradaJugador = (apuntaY > 0) ? 0 : 1;
        }

        ticksDisparoJugador--;
        if (ticksDisparoJugador <= 0) jugadorDisparando = false;

        ticksAnimacionJugador++;
        if (ticksAnimacionJugador > 3) {
            ticksAnimacionJugador = 0;
            frameAnimacionJugador = (frameAnimacionJugador + 1) % 5;
        }
    } else {
        if (std::fabs(dx) > 0.1 || std::fabs(dy) > 0.1) {
            if (std::fabs(dx) > std::fabs(dy)) {
                direccionMiradaJugador = (dx > 0) ? 2 : 3;
            } else {
                direccionMiradaJugador = (dy > 0) ? 0 : 1;
            }

            ticksAnimacionJugador++;
            if (ticksAnimacionJugador > 6) {
                ticksAnimacionJugador = 0;
                frameAnimacionJugador = (frameAnimacionJugador + 1) % 5;
            }
        } else {
            // Quieto
            frameAnimacionJugador = 0;
            ticksAnimacionJugador = 0;
        }
    }
}

void NivelAereo::crearBalaJugador(double centroX, double centroY, double apuntaX, double apuntaY, double distancia)
{
    BalaAerea bala;
    bala.x = centroX;
    bala.y = centroY;
    bala.vx = apuntaX / distancia * VELOCIDAD_BALA_JUGADOR;
    bala.vy = apuntaY / distancia * VELOCIDAD_BALA_JUGADOR;
    balasJugador.append(bala);
    emitirSenal(centroX, centroY);
    sonidoDisparo.play();
}

void NivelAereo::actualizarEnemigos()
{
    double jugadorCentroX = jugadorX + TAM_JUGADOR / 2;
    double jugadorCentroY = jugadorY + TAM_JUGADOR / 2;

    for (int i = 0; i < enemigos.size(); i++) {
        EnemigoAereo &enemigo = enemigos[i];

        double posXAnterior = enemigo.x;
        double posYAnterior = enemigo.y;

        if (enemigo.inteligente) {
            actualizarVigilante(enemigo);
        } else {
            double centroX = enemigo.x + TAM_ENEMIGO / 2;
            double centroY = enemigo.y + TAM_ENEMIGO / 2;
            double dx = jugadorCentroX - centroX;
            double dy = jugadorCentroY - centroY;
            double distancia = std::sqrt(dx * dx + dy * dy);

            if (enemigo.espera > 0) enemigo.espera--;

            bool ve = distancia < VISION && hayLineaLibre(centroX, centroY, jugadorCentroX, jugadorCentroY);

            if (ve) {
                moverEnemigo(enemigo, dx / distancia * VELOCIDAD_PERSECUCION, dy / distancia * VELOCIDAD_PERSECUCION);

                if (enemigo.espera == 0) {
                    BalaAerea bala;
                    bala.x = centroX;
                    bala.y = centroY;
                    bala.vx = dx / distancia * VELOCIDAD_BALA_ENEMIGA;
                    bala.vy = dy / distancia * VELOCIDAD_BALA_ENEMIGA;
                    balasEnemigas.append(bala);
                    enemigo.espera = ESPERA_DISPARO_ENEMIGO;

                    enemigo.estaDisparando = true;
                    sonidoDisparoEnemigo.play();
                    enemigo.ticksDisparo = 25;
                    enemigo.frameAnimacion = 0;
                }
            } else {
                patrullar(enemigo);
            }
        }

        double movX = enemigo.x - posXAnterior;
        double movY = enemigo.y - posYAnterior;

        if (enemigo.estaDisparando) {
            double dx = jugadorCentroX - (enemigo.x + TAM_ENEMIGO / 2);
            double dy = jugadorCentroY - (enemigo.y + TAM_ENEMIGO / 2);
            if (std::fabs(dx) > std::fabs(dy)) {
                enemigo.direccionMirada = (dx > 0) ? 2 : 3;
            } else {
                enemigo.direccionMirada = (dy > 0) ? 0 : 1;
            }

            enemigo.ticksDisparo--;
            if (enemigo.ticksDisparo <= 0) enemigo.estaDisparando = false;

            enemigo.ticksAnimacion++;
            if (enemigo.ticksAnimacion > 5) {
                enemigo.ticksAnimacion = 0;
                enemigo.frameAnimacion = (enemigo.frameAnimacion + 1) % 5;
            }
        } else {
            if (std::fabs(movX) > 0.1 || std::fabs(movY) > 0.1) {
                if (std::fabs(movX) > std::fabs(movY)) {
                    enemigo.direccionMirada = (movX > 0) ? 2 : 3;
                } else {
                    enemigo.direccionMirada = (movY > 0) ? 0 : 1;
                }
            }

            enemigo.ticksAnimacion++;
            if (enemigo.ticksAnimacion > 8) {
                enemigo.ticksAnimacion = 0;
                enemigo.frameAnimacion = (enemigo.frameAnimacion + 1) % 5;
            }
        }

        QRectF cuerpoEnemigo(enemigo.x, enemigo.y, TAM_ENEMIGO, TAM_ENEMIGO);
        QRectF cuerpoJugador(jugadorX, jugadorY, TAM_JUGADOR, TAM_JUGADOR);
        if (cuerpoEnemigo.intersects(cuerpoJugador)) recibirDano(DANO_CONTACTO);
    }
}

void NivelAereo::patrullar(EnemigoAereo &enemigo)
{
    bool libre = moverEnemigo(enemigo, enemigo.dirX * VELOCIDAD_PATRULLA, enemigo.dirY * VELOCIDAD_PATRULLA);

    if (!libre) {
        int direccion = rand() % 4;
        if (direccion == 0) { enemigo.dirX = 1; enemigo.dirY = 0; }
        else if (direccion == 1) { enemigo.dirX = -1; enemigo.dirY = 0; }
        else if (direccion == 2) { enemigo.dirX = 0; enemigo.dirY = 1; }
        else { enemigo.dirX = 0; enemigo.dirY = -1; }
    }
}

QList<QPoint> NivelAereo::calcularRuta(double xDesde, double yDesde, int columnaDestino, int filaDestino) const
{
    QList<QPoint> ruta;
    int columnaInicio = (int)(xDesde + TAM_ENEMIGO / 2) / TAM_BLOQUE;
    int filaInicio = (int)(yDesde + TAM_ENEMIGO / 2) / TAM_BLOQUE;

    if (esSolido(bloqueEn(columnaDestino, filaDestino))) return ruta;

    bool visitado[FILAS][COLUMNAS] = {false};
    QPoint padre[FILAS][COLUMNAS];

    QList<QPoint> cola;
    cola.append(QPoint(columnaInicio, filaInicio));
    visitado[filaInicio][columnaInicio] = true;

    int movimientosX[4] = {1, -1, 0, 0};
    int movimientosY[4] = {0, 0, 1, -1};
    int cabeza = 0;
    bool encontrado = false;

    while (cabeza < cola.size() && !encontrado) {
        QPoint actual = cola[cabeza];
        cabeza++;

        for (int k = 0; k < 4; k++) {
            int columna = actual.x() + movimientosX[k];
            int fila = actual.y() + movimientosY[k];

            if (esSolido(bloqueEn(columna, fila)) || visitado[fila][columna]) continue;

            visitado[fila][columna] = true;
            padre[fila][columna] = actual;
            cola.append(QPoint(columna, fila));

            if (columna == columnaDestino && fila == filaDestino) encontrado = true;
        }
    }

    if (!encontrado) return ruta;

    QPoint paso(columnaDestino, filaDestino);
    while (paso.x() != columnaInicio || paso.y() != filaInicio) {
        ruta.prepend(paso);
        paso = padre[paso.y()][paso.x()];
    }

    return ruta;
}

void NivelAereo::seguirRuta(EnemigoAereo &enemigo)
{
    if (enemigo.ruta.isEmpty()) return;

    double destinoX = enemigo.ruta[0].x() * TAM_BLOQUE + (TAM_BLOQUE - TAM_ENEMIGO) / 2;
    double destinoY = enemigo.ruta[0].y() * TAM_BLOQUE + (TAM_BLOQUE - TAM_ENEMIGO) / 2;
    double dx = destinoX - enemigo.x;
    double dy = destinoY - enemigo.y;

    if (std::fabs(dx) <= VELOCIDAD_VIGILANTE && std::fabs(dy) <= VELOCIDAD_VIGILANTE) {
        enemigo.x = destinoX;
        enemigo.y = destinoY;
        enemigo.ruta.removeFirst();
        return;
    }

    if (dx > VELOCIDAD_VIGILANTE) dx = VELOCIDAD_VIGILANTE;
    if (dx < -VELOCIDAD_VIGILANTE) dx = -VELOCIDAD_VIGILANTE;
    if (dy > VELOCIDAD_VIGILANTE) dy = VELOCIDAD_VIGILANTE;
    if (dy < -VELOCIDAD_VIGILANTE) dy = -VELOCIDAD_VIGILANTE;

    moverEnemigo(enemigo, dx, dy);
}

void NivelAereo::emitirSenal(double x, double y)
{
    int columnaSenal = (int)x / TAM_BLOQUE;
    int filaSenal = (int)y / TAM_BLOQUE;

    for (int i = 0; i < enemigos.size(); i++) {
        EnemigoAereo &enemigo = enemigos[i];
        if (!enemigo.inteligente || enemigo.estado == PERSEGUIR) continue;

        QList<QPoint> ruta = calcularRuta(enemigo.x, enemigo.y, columnaSenal, filaSenal);
        if (!ruta.isEmpty() && ruta.size() <= ALCANCE_SENAL) {
            enemigo.ruta = ruta;
            enemigo.objetivoColumna = columnaSenal;
            enemigo.objetivoFila = filaSenal;
            enemigo.estado = INVESTIGAR;
        }
    }
}

void NivelAereo::actualizarVigilante(EnemigoAereo &enemigo)
{
    double jugadorCentroX = jugadorX + TAM_JUGADOR / 2;
    double jugadorCentroY = jugadorY + TAM_JUGADOR / 2;
    double centroX = enemigo.x + TAM_ENEMIGO / 2;
    double centroY = enemigo.y + TAM_ENEMIGO / 2;
    double dx = jugadorCentroX - centroX;
    double dy = jugadorCentroY - centroY;
    double distancia = std::sqrt(dx * dx + dy * dy);

    if (enemigo.espera > 0) enemigo.espera--;

    bool ve = distancia < VISION && hayLineaLibre(centroX, centroY, jugadorCentroX, jugadorCentroY);

    if (ve) {
        if (enemigo.estado != PERSEGUIR) {
            enemigo.estado = PERSEGUIR;
            sonidoAlerta.play();
            emitirSenal(jugadorCentroX, jugadorCentroY);
        }
        enemigo.objetivoColumna = (int)jugadorCentroX / TAM_BLOQUE;
        enemigo.objetivoFila = (int)jugadorCentroY / TAM_BLOQUE;
    }

    if (enemigo.estado == PATRULLA) {
        patrullar(enemigo);
    } else if (enemigo.estado == INVESTIGAR) {
        seguirRuta(enemigo);
        if (enemigo.ruta.isEmpty()) {
            enemigo.estado = BUSCAR;
            enemigo.ticksEstado = TICKS_BUSCAR;
        }
    } else if (enemigo.estado == BUSCAR) {
        enemigo.ticksEstado--;
        if (enemigo.ticksEstado <= 0) enemigo.estado = PATRULLA;
    } else if (enemigo.estado == PERSEGUIR) {
        if (ve) {
            moverEnemigo(enemigo, dx / distancia * VELOCIDAD_VIGILANTE, dy / distancia * VELOCIDAD_VIGILANTE);
            if (enemigo.espera == 0) {
                BalaAerea bala;
                bala.x = centroX;
                bala.y = centroY;
                bala.vx = dx / distancia * VELOCIDAD_BALA_ENEMIGA;
                bala.vy = dy / distancia * VELOCIDAD_BALA_ENEMIGA;
                balasEnemigas.append(bala);
                enemigo.espera = ESPERA_DISPARO_ENEMIGO;

                enemigo.estaDisparando = true;
                sonidoDisparoEnemigo.play();
                enemigo.ticksDisparo = 25;
                enemigo.frameAnimacion = 0;
            }
        } else {
            enemigo.ruta = calcularRuta(enemigo.x, enemigo.y, enemigo.objetivoColumna, enemigo.objetivoFila);
            enemigo.estado = INVESTIGAR;
        }
    }
}

void NivelAereo::actualizarBalas()
{
    for (int i = balasJugador.size() - 1; i >= 0; i--) {
        balasJugador[i].x += balasJugador[i].vx;
        balasJugador[i].y += balasJugador[i].vy;
        bool eliminar = false;

        if (esSolido(bloqueEn((int)balasJugador[i].x / TAM_BLOQUE, (int)balasJugador[i].y / TAM_BLOQUE))) eliminar = true;

        for (int j = enemigos.size() - 1; j >= 0 && !eliminar; j--) {
            QRectF cuerpo(enemigos[j].x, enemigos[j].y, TAM_ENEMIGO, TAM_ENEMIGO);
            if (cuerpo.contains(balasJugador[i].x, balasJugador[i].y)) {
                enemigos[j].salud -= DANO_BALA_JUGADOR;
                eliminar = true;
                if (enemigos[j].salud <= 0) {
                    manchas.append(QPoint((int)enemigos[j].x + TAM_ENEMIGO / 2, (int)enemigos[j].y + TAM_ENEMIGO / 2));
                    if (manchas.size() > MAX_MANCHAS) manchas.removeFirst();
                    sonidoMuerte.play();
                    enemigos.removeAt(j);
                }
            }
        }
        if (eliminar) balasJugador.removeAt(i);
    }

    for (int i = balasEnemigas.size() - 1; i >= 0; i--) {
        balasEnemigas[i].x += balasEnemigas[i].vx;
        balasEnemigas[i].y += balasEnemigas[i].vy;
        bool eliminar = false;

        if (esSolido(bloqueEn((int)balasEnemigas[i].x / TAM_BLOQUE, (int)balasEnemigas[i].y / TAM_BLOQUE))) eliminar = true;

        QRectF cuerpoJugador(jugadorX, jugadorY, TAM_JUGADOR, TAM_JUGADOR);
        if (!eliminar && cuerpoJugador.contains(balasEnemigas[i].x, balasEnemigas[i].y)) {
            recibirDano(DANO_BALA_ENEMIGA);
            eliminar = true;
        }
        if (eliminar) balasEnemigas.removeAt(i);
    }
}

void NivelAereo::recibirDano(int dano)
{
    if (ticksInvulnerable > 0) return;
    salud -= dano;
    ticksInvulnerable = TICKS_INVULNERABLE;
    if (salud <= 0) reaparecer();
}

void NivelAereo::reaparecer()
{
    jugadorX = inicioX;
    jugadorY = inicioY;
    salud = SALUD_MAXIMA;
    balasEnemigas.clear();
    balasJugador.clear();
    disparosPendientes = 0;
    sonidoRespawn.play();
    mostrarMensaje("Has muerto");
}

void NivelAereo::mostrarMensaje(const QString &texto)
{
    mensaje = texto;
    ticksMensaje = 90;
}

void NivelAereo::actualizarCamara()
{
    camaraX = (int)jugadorX + TAM_JUGADOR / 2 - ancho / 2;
    camaraY = (int)jugadorY + TAM_JUGADOR / 2 - alto / 2;

    int maximoX = COLUMNAS * TAM_BLOQUE - ancho;
    int maximoY = FILAS * TAM_BLOQUE - alto;

    if (camaraX < 0) camaraX = 0;
    if (camaraY < 0) camaraY = 0;
    if (camaraX > maximoX) camaraX = maximoX;
    if (camaraY > maximoY) camaraY = maximoY;
}

void NivelAereo::actualizar()
{
    if (nivelCompleto) {
        if (SonidoFondo.isPlaying()) SonidoFondo.stop();
        return;
    }
    actualizarJugador();
    actualizarEnemigos();
    actualizarBalas();
    actualizarCamara();
    if (ticksMensaje > 0) ticksMensaje--;
}

void NivelAereo::dibujar(QPainter &pintor)
{
    pintor.fillRect(0, 0, ancho, alto, QColor(25, 20, 20));

    int columnaInicial = camaraX / TAM_BLOQUE;
    int columnaFinal = (camaraX + ancho) / TAM_BLOQUE;
    int filaInicial = camaraY / TAM_BLOQUE;
    int filaFinal = (camaraY + alto) / TAM_BLOQUE;

    pintor.save();
    pintor.translate(-camaraX, -camaraY);

    for (int fila = filaInicial; fila <= filaFinal; fila++) {
        for (int columna = columnaInicial; columna <= columnaFinal; columna++) {
            char bloque = bloqueEn(columna, fila);
            int x = columna * TAM_BLOQUE;
            int y = fila * TAM_BLOQUE;

            if (bloque != '#' && !texSuelo.isNull()) {
                pintor.drawPixmap(x, y, texSuelo);
            }

            if (bloque == '#') {
                if (!texPared.isNull()) {
                    pintor.drawPixmap(x, y, texPared);
                } else {
                    pintor.fillRect(x, y, TAM_BLOQUE, TAM_BLOQUE, QColor(70, 70, 90));
                    pintor.setPen(QColor(50, 50, 65));
                    pintor.drawRect(x, y, TAM_BLOQUE - 1, TAM_BLOQUE - 1);
                }
            } else if (bloque == 'D') {
                if (!texPuerta.isNull()) {
                    pintor.drawPixmap(x, y, texPuerta);
                } else {
                    pintor.fillRect(x, y, TAM_BLOQUE, TAM_BLOQUE, QColor(150, 100, 30));
                    pintor.fillRect(x + 16, y + 16, 8, 8, QColor(240, 200, 60));
                }
            } else if (bloque == 'L') {
                pintor.setPen(Qt::NoPen);
                pintor.setBrush(QColor(240, 200, 60));
                pintor.drawEllipse(x + 12, y + 12, 16, 16);
            } else if (bloque == 'H') {
                pintor.fillRect(x + 8, y + 16, 24, 8, QColor(60, 200, 80));
                pintor.fillRect(x + 16, y + 8, 8, 24, QColor(60, 200, 80));
            } else if (bloque == 'S') {
                pintor.fillRect(x + 4, y + 4, TAM_BLOQUE - 8, TAM_BLOQUE - 8, QColor(60, 200, 230));
            }
        }
    }

    if (!texSangre.isNull()) {
        for (int i = 0; i < manchas.size(); i++) {
            pintor.save();
            pintor.translate(manchas[i].x(), manchas[i].y());
            pintor.rotate((manchas[i].x() * 7 + manchas[i].y() * 13) % 360);
            pintor.drawPixmap(-TAM_SANGRE / 2, -TAM_SANGRE / 2, texSangre);
            pintor.restore();
        }
    }

    pintor.setPen(Qt::NoPen);

    for (int i = 0; i < enemigos.size(); i++) {

        QPixmap* texActual = nullptr;
        double escalaActual = 1.0;

        if (enemigos[i].inteligente) {
            if (enemigos[i].estado == PERSEGUIR && !texPoliciaAlterado.isNull()) {
                texActual = &texPoliciaAlterado;
                escalaActual = escalaPoliciaAlterado;
            } else if ((enemigos[i].estado == INVESTIGAR || enemigos[i].estado == BUSCAR) && !texPoliciaBusqueda.isNull()) {
                texActual = &texPoliciaBusqueda;
                escalaActual = escalaPoliciaBusqueda;
            } else if (!texPolicia.isNull()) {
                texActual = &texPolicia;
                escalaActual = escalaPolicia;
            }
        } else {
            if (!texPolicia.isNull()) {
                texActual = &texPolicia;
                escalaActual = escalaPolicia;
            }
        }

        if (texActual != nullptr) {
            int anchoFrame = texActual->width() / 5;
            int altoFrame = texActual->height() / 4;
            int filaSprite = 0;

            if (enemigos[i].estaDisparando) {
                filaSprite = 3;
            } else {
                if (enemigos[i].direccionMirada == 0) filaSprite = 0;
                else if (enemigos[i].direccionMirada == 1) filaSprite = 1;
                else filaSprite = 2;
            }

            int colSprite = enemigos[i].frameAnimacion;
            QRect rectOrigen(colSprite * anchoFrame, filaSprite * altoFrame, anchoFrame, altoFrame);
            QPixmap frame = texActual->copy(rectOrigen);

            if (enemigos[i].direccionMirada == 2 && (filaSprite == 2 || filaSprite == 3)) {
                frame = frame.transformed(QTransform().scale(-1, 1));
            }

            int tamDibujo = TAM_ENEMIGO * escalaActual;
            int offset = (tamDibujo - TAM_ENEMIGO) / 2;

            pintor.drawPixmap((int)enemigos[i].x - offset, (int)enemigos[i].y - offset, tamDibujo, tamDibujo, frame);
        } else {
            if (enemigos[i].inteligente) pintor.setBrush(QColor(60, 160, 200));
            else pintor.setBrush(QColor(160, 60, 160));
            pintor.drawEllipse((int)enemigos[i].x, (int)enemigos[i].y, TAM_ENEMIGO, TAM_ENEMIGO);
        }

        // Barra de salud
        int largo = TAM_ENEMIGO * enemigos[i].salud / SALUD_ENEMIGO;
        pintor.fillRect((int)enemigos[i].x, (int)enemigos[i].y - 6, TAM_ENEMIGO, 3, QColor(60, 0, 0));
        pintor.fillRect((int)enemigos[i].x, (int)enemigos[i].y - 6, largo, 3, QColor(220, 30, 30));
    }

    for (int i = 0; i < balasJugador.size(); i++) {
        pintor.setBrush(QColor(255, 220, 80));
        pintor.drawEllipse((int)balasJugador[i].x - 3, (int)balasJugador[i].y - 3, 6, 6);
    }

    for (int i = 0; i < balasEnemigas.size(); i++) {
        pintor.setBrush(QColor(255, 90, 200));
        pintor.drawEllipse((int)balasEnemigas[i].x - 4, (int)balasEnemigas[i].y - 4, 8, 8);
    }

    bool parpadeo = ticksInvulnerable > 0 && (ticksInvulnerable / 5) % 2 == 0;
    if (!parpadeo) {
        if (!texJugador.isNull()) {
            int anchoFrame = texJugador.width() / 5;
            int altoFrame = texJugador.height() / 4;
            int filaSprite = 0;

            if (jugadorDisparando) {
                filaSprite = 3;
            } else {
                if (direccionMiradaJugador == 0) filaSprite = 0;
                else if (direccionMiradaJugador == 1) filaSprite = 1;
                else filaSprite = 2; // Izquierda o Derecha
            }

            int colSprite = frameAnimacionJugador;
            QRect rectOrigen(colSprite * anchoFrame, filaSprite * altoFrame, anchoFrame, altoFrame);
            QPixmap frame = texJugador.copy(rectOrigen);

            if (direccionMiradaJugador == 2 && (filaSprite == 2 || filaSprite == 3)) {
                frame = frame.transformed(QTransform().scale(-1, 1));
            }

            int tamDibujo = TAM_JUGADOR * escalaJugador;
            int offset = (tamDibujo - TAM_JUGADOR) / 2;

            pintor.drawPixmap((int)jugadorX - offset, (int)jugadorY - offset, tamDibujo, tamDibujo, frame);
        } else {

            pintor.setBrush(QColor(200, 30, 30));
            pintor.drawEllipse((int)jugadorX, (int)jugadorY, TAM_JUGADOR, TAM_JUGADOR);
        }
    }

    pintor.restore();

    pintor.setPen(Qt::white);
    pintor.drawLine(mouseX - 8, mouseY, mouseX + 8, mouseY);
    pintor.drawLine(mouseX, mouseY - 8, mouseX, mouseY + 8);

    pintor.fillRect(10, 10, 200, 14, QColor(60, 0, 0));
    pintor.fillRect(10, 10, 200 * salud / SALUD_MAXIMA, 14, QColor(220, 30, 30));
    pintor.setPen(Qt::white);
    pintor.drawText(10, 45, QString("Llaves: %1").arg(llaves));
    pintor.drawText(10, 65, QString("Enemigos: %1").arg(enemigos.size()));
    pintor.drawText(10, 85, "WASD: mover  Mouse: apuntar y disparar");

    if (ticksMensaje > 0) {
        pintor.drawText(QRect(0, 100, ancho, 30), Qt::AlignCenter, mensaje);
    }

    if (nivelCompleto) {
        pintor.fillRect(0, 0, ancho, alto, QColor(0, 0, 0, 160));
        pintor.setPen(Qt::white);
        pintor.setFont(QFont("Arial", 28, QFont::Bold));
        pintor.drawText(QRect(0, 0, ancho, alto), Qt::AlignCenter, "Nivel completado");
    }
}