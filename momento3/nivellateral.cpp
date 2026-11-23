#include "nivellateral.h"
#include <QFont>
#include <QPen>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QUrl>

const int INICIO_X = 100;
const int ANCHO_NIVEL = 11600;
const int SUELO_ABISMO = 5000;
const int ALTO_DOOM = 60;
const int ALTO_ENEMIGO_NIVEL = 60;
const int MARGEN_APOYO = 15;
const int TOLERANCIA_PARED = 4;
const int TICKS_ENTRE_DANO = 10;
const int DANO_ESPINAS = 2;
const int DANO_CAIDA = 20;
const int PROBABILIDAD_CURACION = 10;
const int ANCHO_FONDO = 800;
const int REPETICIONES_PRIMER_FONDO = 3;
const int MARGEN_CAMARA = 350;
const int ALTO_IMAGEN_SUELO = 100;
const int ANCHO_SUELO1 = 400;
const int ANCHO_SUELO2 = 450;
const int DANO_BALA_ENEMIGA = 8;
const int SALA_JEFE_X = 9000;
const int ZONA_JEFE_X = 9080;
const int ANCHO_ZONA_JEFE = 40;
const int VELOCIDAD_CAMARA_JEFE = 12;
const int RADIO_EXPLOSION = 90;
const int TICKS_EXPLOSION = 20;
const int DANO_EXPLOSION = 15;
const int DANO_A_TANQUE = 15;
const int RADIO_GRANADA = 50;
const int DANO_GRANADA = 10;

NivelLateral::NivelLateral(int ancho, int alto)
    : repeticionesFondo(0),
    anchoFondo(ANCHO_FONDO),
    ancho(ancho),
    alto(alto),
    anchoNivel(ANCHO_NIVEL),
    camaraX(0)
{
    repeticionesFondo = (anchoNivel + anchoFondo - 1) / anchoFondo;

    fondo.load(":/imagenes/sprite/FondoNivel1");
    if (!fondo.isNull()) {
        fondo = fondo.scaled(anchoFondo, alto, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    fondo2.load(":/imagenes/sprite/FondoNivel1_1");
    if (!fondo2.isNull()) {
        fondo2 = fondo2.scaled(anchoFondo, alto, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    suelo1.load(":/imagenes/sprite/Suelo1");
    suelo2.load(":/imagenes/sprite/Suelo2");

    if (!suelo1.isNull()) {
        suelo1 = suelo1.scaled(ANCHO_SUELO1, ALTO_IMAGEN_SUELO, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    if (!suelo2.isNull()) {
        suelo2 = suelo2.scaled(ANCHO_SUELO2, ALTO_IMAGEN_SUELO, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    sonidoDisparoJugador.setSource(QUrl("qrc:/Sonido/sonidos/sound-gun-shot.wav"));
    sonidoDisparoJugador.setVolume(0.6);

    sonidoDisparoEnemigo.setSource(QUrl("qrc:/Sonido/sonidos/sound-gun-shot.wav"));
    sonidoDisparoEnemigo.setVolume(0.3);

    sonidoRafagaTanque.setSource(QUrl("qrc:/Sonido/sonidos/sound-gun-shot.wav"));
    sonidoRafagaTanque.setVolume(0.15);
    sonidoRafagaTanque.setLoopCount(3);

    sonidoRecoleccion.setSource(QUrl("qrc:/Sonido/sonidos/sound-collet.wav"));
    sonidoRecoleccion.setVolume(0.6);

    sonidoExplosion.setSource(QUrl("qrc:/Sonido/sonidos/sound-explocion.wav"));
    sonidoExplosion.setVolume(0.75);

    tramos.append(Tramo{    0,  450, 400, false });
    tramos.append(Tramo{  550,  250, 360, false });
    tramos.append(Tramo{  800,  200, 320, false });
    tramos.append(Tramo{ 1130,  270, 400, false });
    tramos.append(Tramo{ 1400,  110, 440, false });
    tramos.append(Tramo{ 1510,  490, 400, false });
    tramos.append(Tramo{ 2000,  400, 360, false });
    tramos.append(Tramo{ 4100,  300, 360, false });
    tramos.append(Tramo{ 5600,  500, 320, false });
    tramos.append(Tramo{ 6100,  100, 400, false });
    plataformas.append(new Plataforma(2500, 330, 100, 10, 80, 0));
    plataformas.append(new Plataforma(2800, 330, 100, 10, 0, 80, 0.05));
    plataformas.append(new Plataforma(3000, 310, 100, 10, 0, 30, 0.08));
    plataformas.append(new Plataforma(3200, 300, 90, 10, 0, 60, 0.08));
    plataformas.append(new Plataforma(3400, 380, 100, 10, 50, 50, 0.05));
    plataformas.append(new Plataforma(3600, 360, 100, 10, -50, 50, 0.05));
    plataformas.append(new Plataforma(3850, 360, 150, 10));
    plataformas.append(new Plataforma(4550, 330, 100, 10, 80, 0));
    plataformas.append(new Plataforma(4850, 340, 100, 10, 0, 70, 0.05));
    plataformas.append(new Plataforma(5050, 300, 100, 10, 0, 40, 0.08));
    plataformas.append(new Plataforma(5250, 350, 100, 10, 60, 0, 0.05));
    plataformas.append(new Plataforma(5450, 360, 150, 10));

    tramos.append(Tramo{ 6200,  100, 450, false });
    tramos.append(Tramo{ 6300,  80,  450, false });
    tramos.append(Tramo{ 6450,  4000, 450, false });
    auto enemigoEn = [this](int x, int izq, int der) {
        enemigos.append(new Enemigos(x, sueloEn(x + 25) - ALTO_ENEMIGO_NIVEL, izq, der));
    };
    auto enemigoSobrePlataforma = [this](int x, int y, int izq, int der) {
        enemigos.append(new Enemigos(x, y - ALTO_ENEMIGO_NIVEL, izq, der));
    };
    auto granaderoSobrePlataforma = [this](int x, int y, int izq, int der) {
        enemigos.append(new Enemigos(x, y - ALTO_ENEMIGO_NIVEL, izq, der, true));
    };
    plataformas.append(new Plataforma(6600, 360, 100, 10));
    plataformas.append(new Plataforma(6850, 340, 100, 10));
    plataformas.append(new Plataforma(7150, 320, 100, 10));
    plataformas.append(new Plataforma(7600, 300, 100, 10));
    enemigoEn(6600, 6550, 6750);
    enemigoEn(7000, 6900, 7100);
    enemigoEn(7400, 7300, 7500);

    plataformas.append(new Plataforma(7750, 370, 160, 10));
    plataformas.append(new Plataforma(8050, 330, 160, 10));
    plataformas.append(new Plataforma(8350, 370, 160, 10));
    plataformas.append(new Plataforma(8650, 330, 160, 10));
    enemigoSobrePlataforma(7770, 370, 7750, 7860);
    granaderoSobrePlataforma(8070, 330, 8050, 8160);
    enemigoSobrePlataforma(8370, 370, 8350, 8460);
    granaderoSobrePlataforma(8670, 330, 8650, 8760);
    objetos.append(new Objetos(TipoObjeto::Curacion, 8720, 306));

    plataformas.append(new Plataforma(2440, 290, 70, 10));
    plataformas.append(new Plataforma(2580, 240, 70, 10));
    plataformas.append(new Plataforma(2740, 190, 70, 10));
    plataformas.append(new Plataforma(2900, 240, 70, 10));
    plataformas.append(new Plataforma(3050, 190, 160, 10));
    plataformas.append(new Plataforma(3290, 240, 70, 10));
    plataformas.append(new Plataforma(3430, 190, 70, 10));
    plataformas.append(new Plataforma(3570, 150, 70, 10));
    plataformas.append(new Plataforma(3710, 200, 70, 10));
    plataformas.append(new Plataforma(3850, 250, 70, 10));
    enemigoSobrePlataforma(3070, 190, 3050, 3160);
    objetos.append(new Objetos(TipoObjeto::Curacion, 3725, 176));

    plataformas.append(new Plataforma(4450, 280, 70, 10));
    plataformas.append(new Plataforma(4590, 230, 70, 10));
    plataformas.append(new Plataforma(4730, 180, 160, 10));
    plataformas.append(new Plataforma(4970, 230, 70, 10));
    plataformas.append(new Plataforma(5110, 180, 70, 10));
    plataformas.append(new Plataforma(5250, 220, 70, 10));
    plataformas.append(new Plataforma(5390, 270, 70, 10));
    granaderoSobrePlataforma(4750, 180, 4730, 4840);
    objetos.append(new Objetos(TipoObjeto::Curacion, 5405, 246));

    // zona jefe
    plataformas.append(new Plataforma(9150, 350, 80, 10));
    plataformas.append(new Plataforma(9150, 250, 80, 10));
    plataformas.append(new Plataforma(9350, 350, 80, 10));
    plataformas.append(new Plataforma(9350, 250, 80, 10));
    plataformas.append(new Plataforma(9550, 350, 80, 10));
    plataformas.append(new Plataforma(9550, 250, 80, 10));
    objetos.append(new Objetos(TipoObjeto::ZonaJefe, ZONA_JEFE_X, 0, ANCHO_ZONA_JEFE, alto));
    objetos.append(new Objetos(TipoObjeto::Puerta, 10300, 360, 20, 30));

    seguroX = INICIO_X;
    seguroY = sueloEn(INICIO_X) - ALTO_DOOM;
    doom = new DoomSlayer(seguroX, seguroY);
    actualizarCamara();
}

NivelLateral::~NivelLateral()
{
    qDeleteAll(proyectiles);
    qDeleteAll(enemigos);
    qDeleteAll(plataformas);
    qDeleteAll(objetos);
    qDeleteAll(proyectilesEnemigos);
    qDeleteAll(bombas);
    qDeleteAll(granadas);
    delete tanque;
    delete doom;
}

void NivelLateral::setModoTesteo(bool activo)
{
    modoTesteo = activo;
    if (modoTesteo) {
        jugadorMuerto = false;
    }
}

bool NivelLateral::estaEnModoTesteo() const
{
    return modoTesteo;
}

bool NivelLateral::debeVolverAlMenu() const
{
    return volverAlMenu;
}

int NivelLateral::sueloEn(int x) const
{
    for (const Tramo &t : tramos) {
        if (x >= t.x && x < t.x + t.ancho) return t.y;
    }
    return SUELO_ABISMO;
}

void NivelLateral::teclaPresionada(int tecla)
{
    if (nivelCompleto) return;

    if (jugadorMuerto) {
        if (tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
            volverAlMenu = true;
        }
        return;
    }

    teclas.insert(tecla);
    actualizarMovimiento();

    if (tecla == Qt::Key_W) {
        doom->saltar();
    }

    if (tecla == Qt::Key_Space) {
        QList<Proyectil*> nuevasBalas = doom->disparar();
        if (!nuevasBalas.isEmpty()) {
            proyectiles.append(nuevasBalas);
            sonidoDisparoJugador.play();
        }
    }

    if (tecla == Qt::Key_E) {
        interactuar();
    }
}

void NivelLateral::teclaSoltada(int tecla)
{
    teclas.remove(tecla);

    if (tecla == Qt::Key_W) {
        doom->cortarSalto();
    }

    actualizarMovimiento();
}

void NivelLateral::actualizarMovimiento()
{
    int direccion = 0;
    if (teclas.contains(Qt::Key_A)) direccion -= 1;
    if (teclas.contains(Qt::Key_D)) direccion += 1;
    doom->correr(direccion);
}

void NivelLateral::interactuar()
{
    for (int i = objetos.size() - 1; i >= 0; i--) {
        if (objetos[i]->getTipo() == TipoObjeto::ZonaJefe) {
            continue;
        }

        if (!doom->getRect().intersects(objetos[i]->getRect())) {
            continue;
        }

        bool exito = objetos[i]->interactuar(doom);

        if (objetos[i]->getTipo() == TipoObjeto::Puerta) {
            if (exito) {
                nivelCompleto = true;
            } else {
                mensaje = "Necesitas la llave";
                ticksMensaje = 120;
            }
        } else if (exito) {
            sonidoRecoleccion.play();
            delete objetos[i];
            objetos.removeAt(i);
        }

        break;
    }
}

void NivelLateral::actualizarProyectiles()
{
    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        proyectiles[i]->actualizar();

        int xBala = proyectiles[i]->getX();
        if (xBala < camaraX - 20 || xBala > camaraX + ancho + 20) {
            delete proyectiles[i];
            proyectiles.removeAt(i);
        }
    }
}

bool NivelLateral::tocaSuelo(const QRect &zona) const
{
    for (const Tramo &t : tramos) {
        if (zona.right() > t.x && zona.left() < t.x + t.ancho && zona.bottom() > t.y) {
            return true;
        }
    }

    return false;
}

void NivelLateral::actualizarProyectilesEnemigos()
{
    for (int i = proyectilesEnemigos.size() - 1; i >= 0; i--) {
        Proyectil *bala = proyectilesEnemigos[i];
        bala->actualizar();

        bool fuera = bala->getX() < camaraX - 20 || bala->getX() > camaraX + ancho + 20;
        bool impacto = false;

        if (!fuera && bala->getRect().intersects(doom->getRect())) {
            int direccion = (bala->getX() < doom->getX()) ? 1 : -1;
            doom->golpeado(direccion, DANO_BALA_ENEMIGA);
            impacto = true;
        }

        if (fuera || impacto || tocaSuelo(bala->getRect())) {
            delete bala;
            proyectilesEnemigos.removeAt(i);
        }
    }
}

void NivelLateral::actualizarSuelo()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();
    int suelo = SUELO_ABISMO;
    Plataforma *apoyo = nullptr;
    tramoApoyo = nullptr;

    for (int i = 0; i < tramos.size(); i++) {
        const Tramo &t = tramos.at(i);

        bool encima = cuerpo.right() - MARGEN_APOYO > t.x &&
                      cuerpo.left() + MARGEN_APOYO < t.x + t.ancho;

        if (encima && pies <= t.y) {
            int sueloTramo = t.y - cuerpo.height();
            if (sueloTramo < suelo) {
                suelo = sueloTramo;
                tramoApoyo = &t;
            }
        }
    }

    for (Plataforma *plataforma : plataformas) {
        QRect zona = plataforma->getRect();
        int techoAnterior = zona.top() - plataforma->getDesplazamientoY();
        bool estaEncima = cuerpo.right() > zona.left() && cuerpo.left() < zona.right();

        if (estaEncima && pies <= techoAnterior) {
            int sueloPlataforma = zona.top() - cuerpo.height();
            if (sueloPlataforma < suelo) {
                suelo = sueloPlataforma;
                apoyo = plataforma;
                tramoApoyo = nullptr;
            }
        }
    }

    if (apoyo != nullptr && doom->estaEnElSuelo()) {
        doom->moverX(apoyo->getDesplazamientoX());
        doom->moverY(apoyo->getDesplazamientoY());
    }

    doom->setSuelo(suelo);
}

void NivelLateral::actualizarZonasJefe()
{
    if (camaraJefe) return;

    for (Objetos *objeto : objetos) {
        if (objeto->detectarContacto(doom->getRect())) {
            camaraJefe = true;
            camaraJefeX = SALA_JEFE_X;

            int xTanque = SALA_JEFE_X + ancho - 200;
            int yTanque = sueloEn(xTanque) - ALTO_TANQUE;
            int izquierda = SALA_JEFE_X + 20;
            int derecha = SALA_JEFE_X + ancho - ANCHO_TANQUE - 20;
            tanque = new Tanque(xTanque, yTanque, izquierda, derecha);
            break;
        }
    }
}

void NivelLateral::limitarEnSala()
{
    if (!camaraJefe) return;

    QRect cuerpo = doom->getRect();
    int izquierda = camaraJefeX;
    int derecha = camaraJefeX + ancho;

    if (cuerpo.left() < izquierda) {
        doom->moverX(izquierda - cuerpo.left());
    }

    if (cuerpo.right() > derecha - 1) {
        doom->moverX(derecha - 1 - cuerpo.right());
    }
}

void NivelLateral::actualizarCamara()
{
    if (camaraJefe) {
        if (camaraX < camaraJefeX) {
            camaraX = qMin(camaraX + VELOCIDAD_CAMARA_JEFE, camaraJefeX);
        } else if (camaraX > camaraJefeX) {
            camaraX = qMax(camaraX - VELOCIDAD_CAMARA_JEFE, camaraJefeX);
        }
        return;
    }

    QRect cuerpo = doom->getRect();

    if (cuerpo.right() - camaraX > ancho - MARGEN_CAMARA) {
        camaraX = cuerpo.right() - (ancho - MARGEN_CAMARA);
    } else if (cuerpo.left() - camaraX < MARGEN_CAMARA) {
        camaraX = cuerpo.left() - MARGEN_CAMARA;
    }

    int maximo = anchoNivel - ancho;

    if (camaraX < 0) camaraX = 0;
    if (camaraX > maximo) camaraX = maximo;
}

void NivelLateral::limitarEnNivel()
{
    QRect cuerpo = doom->getRect();

    if (cuerpo.left() < 0) {
        doom->moverX(-cuerpo.left());
    }

    if (cuerpo.right() > anchoNivel) {
        doom->moverX(anchoNivel - cuerpo.right());
    }
}

void NivelLateral::resolverParedes()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();

    for (int i = 0; i < tramos.size(); i++) {
        const Tramo &t = tramos.at(i);

        if (pies <= t.y + TOLERANCIA_PARED) continue;
        if (cuerpo.right() < t.x || cuerpo.left() >= t.x + t.ancho) continue;

        if (cuerpo.center().x() < t.x + t.ancho / 2) {
            doom->moverX(t.x - 1 - cuerpo.right());
        } else {
            doom->moverX(t.x + t.ancho - cuerpo.left());
        }
        cuerpo = doom->getRect();
    }
}

void NivelLateral::aplicarDanoEspinas()
{
    bool dentro = tramoApoyo != nullptr && tramoApoyo->espinas && doom->estaEnElSuelo();

    if (!dentro) {
        ticksEspinas = 0;
        return;
    }

    ticksEspinas++;

    if (ticksEspinas >= TICKS_ENTRE_DANO) {
        ticksEspinas = 0;
        doom->recibirDano(DANO_ESPINAS);
    }
}

void NivelLateral::revisarColisiones()
{
    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        for (int j = enemigos.size() - 1; j >= 0; j--) {
            if (proyectiles[i]->getRect().intersects(enemigos[j]->getRect())) {
                enemigos[j]->recibirDano(15);

                if (enemigos[j]->getSalud() <= 0) {
                    QRect zona = enemigos[j]->getRect();

                    if (QRandomGenerator::global()->bounded(100) < PROBABILIDAD_CURACION) {
                        int xObjeto = zona.center().x() - 12;
                        int yObjeto = zona.y() + zona.height() - 24;
                        objetos.append(new Objetos(TipoObjeto::Curacion, xObjeto, yObjeto));
                    }

                    delete enemigos[j];
                    enemigos.removeAt(j);
                }

                delete proyectiles[i];
                proyectiles.removeAt(i);
                break;
            }
        }
    }

    for (Enemigos *enemigo : enemigos) {
        if (doom->getRect().intersects(enemigo->getRect())) {
            int direccion = (doom->getX() < enemigo->getX()) ? -1 : 1;
            doom->golpeado(direccion, 10);
        }
    }
}

void NivelLateral::actualizarTanque()
{
    if (tanque == nullptr) {
        return;
    }

    Proyectil *bomba = tanque->actualizar(doom->getRect());
    if (bomba != nullptr) {
        bombas.append(bomba);
    }

    Proyectil *bala = tanque->tomarBala();
    if (bala != nullptr) {
        proyectilesEnemigos.append(bala);
        sonidoRafagaTanque.play();
    }
}

void NivelLateral::crearExplosion(int x, int y, int radio, int dano)
{
    sonidoExplosion.play();

    Explosion e;
    e.x = x;
    e.y = y;
    e.radio = radio;
    e.ticks = TICKS_EXPLOSION;
    explosiones.append(e);

    QPoint centro = doom->getRect().center();
    int dx = centro.x() - x;
    int dy = centro.y() - y;

    if (dx * dx + dy * dy <= radio * radio) {
        int direccion = (dx >= 0) ? 1 : -1;
        doom->golpeado(direccion, dano);
    }
}

void NivelLateral::moverParabolicos(QList<Proyectil*> &lista, int radio, int dano)
{
    for (int i = lista.size() - 1; i >= 0; i--) {
        Proyectil *bomba = lista[i];
        int yAnterior = bomba->getY();
        bomba->actualizar();

        bool explota = false;
        QRect rectBomba = bomba->getRect();
        int altoBomba = rectBomba.height();

        for (Plataforma *plataforma : plataformas) {
            QRect zona = plataforma->getRect();
            bool solapaHorizontal = rectBomba.right() > zona.left() && rectBomba.left() < zona.right();
            int fondoAnterior = yAnterior + altoBomba;
            int fondoActual = rectBomba.bottom();

            if (solapaHorizontal && fondoAnterior <= zona.top() + 6 && fondoActual >= zona.top()) {
                explota = true;
                break;
            }
        }

        if (!explota && tocaSuelo(rectBomba)) {
            explota = true;
        }

        if (explota) {
            crearExplosion(bomba->getX(), bomba->getY(), radio, dano);
            delete bomba;
            lista.removeAt(i);
        } else if (bomba->getY() > alto + 200) {
            delete bomba;
            lista.removeAt(i);
        }
    }
}

void NivelLateral::actualizarBombas()
{
    moverParabolicos(bombas, RADIO_EXPLOSION, DANO_EXPLOSION);
    moverParabolicos(granadas, RADIO_GRANADA, DANO_GRANADA);

    for (int i = explosiones.size() - 1; i >= 0; i--) {
        explosiones[i].ticks--;
        if (explosiones[i].ticks <= 0) {
            explosiones.removeAt(i);
        }
    }
}

void NivelLateral::revisarTanque()
{
    if (tanque == nullptr) {
        return;
    }

    for (int i = proyectiles.size() - 1; i >= 0; i--) {
        if (proyectiles[i]->getRect().intersects(tanque->getRect())) {
            tanque->recibirDano(DANO_A_TANQUE);
            delete proyectiles[i];
            proyectiles.removeAt(i);
        }
    }

    if (doom->getRect().intersects(tanque->getRect())) {
        int direccion = (doom->getX() < tanque->getX()) ? -1 : 1;
        doom->golpeado(direccion, 10);
    }

    if (tanque->getSalud() <= 0) {
        QRect zona = tanque->getRect();
        objetos.append(new Objetos(TipoObjeto::Llave, zona.center().x(), zona.y() + zona.height() - 20));
        delete tanque;
        tanque = nullptr;
        camaraJefe = false;
    }
}

void NivelLateral::actualizar()
{
    if (nivelCompleto || jugadorMuerto) return;

    if (ticksMensaje > 0) ticksMensaje--;

    for (Plataforma *plataforma : plataformas) {
        plataforma->actualizar();
    }

    actualizarSuelo();
    doom->actualizar();
    limitarEnNivel();
    actualizarZonasJefe();
    limitarEnSala();
    resolverParedes();
    aplicarDanoEspinas();

    if (tramoApoyo != nullptr && !tramoApoyo->espinas && doom->estaEnElSuelo()) {
        QRect c = doom->getRect();
        if (c.left() >= tramoApoyo->x && c.right() <= tramoApoyo->x + tramoApoyo->ancho) {
            seguroX = doom->getX();
            seguroY = doom->getY();
        }
    }

    if (doom->getY() > alto) {
        doom->recibirDano(DANO_CAIDA);
        doom->reaparecer(seguroX, seguroY);
    }

    for (Enemigos *enemigo : enemigos) {
        enemigo->actualizar();

        Proyectil *bala = enemigo->disparar(doom->getRect());
        if (bala != nullptr) {
            proyectilesEnemigos.append(bala);
            sonidoDisparoEnemigo.play();
        }

        Proyectil *granada = enemigo->lanzarGranada(doom->getRect());
        if (granada != nullptr) {
            granadas.append(granada);
            sonidoDisparoEnemigo.play();
        }

        Proyectil *granadaLista = enemigo->tomarGranada();
        if (granadaLista != nullptr) {
            granadas.append(granadaLista);
            sonidoDisparoEnemigo.play();
        }
    }

    actualizarTanque();
    actualizarBombas();
    revisarTanque();

    actualizarProyectiles();
    actualizarProyectilesEnemigos();
    revisarColisiones();
    actualizarCamara();

    if (!modoTesteo && doom->getSalud() <= 0) {
        jugadorMuerto = true;
    }
}

void NivelLateral::dibujarTramo(QPainter &pintor, const Tramo &t)
{
    QRect zona(t.x, t.y, t.ancho, alto - t.y);

    pintor.save();
    pintor.setClipRect(zona);

    if (!suelo1.isNull() && !suelo2.isNull()) {
        pintor.fillRect(zona, t.espinas ? QColor(120, 30, 30) : QColor(60, 40, 30));

        int indice = 0;
        for (int px = t.x; px < t.x + t.ancho; indice++) {
            const QPixmap &img = (indice % 2 == 0) ? suelo1 : suelo2;
            pintor.drawPixmap(px, t.y, img);
            px += img.width();
        }
    } else {
        pintor.fillRect(zona, t.espinas ? QColor(120, 30, 30) : QColor(90, 60, 40));
    }

    pintor.restore();
}

void NivelLateral::dibujar(QPainter &pintor)
{
    pintor.fillRect(0, 0, ancho, alto, QColor(30, 30, 30));

    pintor.save();
    pintor.translate(-camaraX, 0);

    if (!fondo.isNull()) {
        for (int i = 0; i < repeticionesFondo; i++) {
            if (i < REPETICIONES_PRIMER_FONDO) {
                pintor.drawPixmap(i * anchoFondo, 0, fondo);
            } else {
                pintor.drawPixmap(i * anchoFondo, 0, fondo2);
            }
        }
    }

    for (int i = 0; i + 1 < tramos.size(); i++) {
        const Tramo &a = tramos.at(i);
        const Tramo &b = tramos.at(i + 1);
        int finA = a.x + a.ancho;
        int hueco = b.x - finA;

        if (hueco > 0) {
            int arriba = qMax(a.y, b.y);
            pintor.fillRect(finA, arriba, hueco, alto - arriba, QColor(0, 0, 0));
        }
    }

    for (int i = 0; i < tramos.size(); i++) {
        dibujarTramo(pintor, tramos.at(i));
    }

    for (Plataforma *plataforma : plataformas) {
        plataforma->dibujar(pintor);
    }

    for (Objetos *objeto : objetos) {
        objeto->dibujar(pintor);
    }

    doom->dibujar(pintor);

    if (doom->getTicksFuria() > 0) {
        pintor.setPen(QPen(QColor(220, 30, 30), 2));
        pintor.setBrush(Qt::NoBrush);
        pintor.drawRect(doom->getRect());
    }

    for (Enemigos *enemigo : enemigos) {
        enemigo->dibujar(pintor);
    }

    if (tanque != nullptr) {
        tanque->dibujar(pintor);
    }

    for (Proyectil *bala : proyectiles) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 220, 0));
    }

    for (Proyectil *bala : proyectilesEnemigos) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 90, 40));
    }

    for (Proyectil *bomba : bombas) {
        pintor.setPen(Qt::NoPen);
        pintor.setBrush(QColor(255, 200, 0));
        pintor.drawEllipse(bomba->getX(), bomba->getY(), 10, 10);
    }

    for (Proyectil *granada : granadas) {
        pintor.setPen(Qt::NoPen);
        pintor.setBrush(QColor(120, 220, 60));
        pintor.drawEllipse(granada->getX(), granada->getY(), 8, 8);
    }

    for (int i = 0; i < explosiones.size(); i++) {
        pintor.setPen(Qt::NoPen);
        pintor.setBrush(QColor(255, 140, 0, 130));
        pintor.drawEllipse(QPoint(explosiones[i].x, explosiones[i].y), explosiones[i].radio, explosiones[i].radio);
    }

    pintor.restore();

    if (tanque != nullptr) {
        pintor.fillRect(250, 20, 300, 12, QColor(60, 0, 0));
        int largo = 300 * tanque->getSalud() / SALUD_TANQUE;
        pintor.fillRect(250, 20, largo, 12, QColor(220, 30, 30));
    }

    pintor.setPen(Qt::white);
    pintor.drawText(10, 20, QString("Salud: %1").arg(doom->getSalud()));
    pintor.drawText(10, 40, QString("Llave: %1").arg(doom->tieneLlave() ? "Si" : "No"));
    pintor.drawText(10, 60, "E: interactuar");
    pintor.drawText(10, 80, "W: saltar  , A: izquierda , D: Derecha");
    pintor.drawText(10, 100, "Espacio: Dispara");

    if (doom->getTicksFuria() > 0) {
        pintor.setPen(QColor(255, 80, 80));
        double segundos = doom->getTicksFuria() * 0.016;
        pintor.drawText(10, 80, QString("FURIA: %1 s").arg(segundos, 0, 'f', 1));
    }

    if (ticksMensaje > 0) {
        pintor.setPen(Qt::white);
        pintor.drawText(330, 100, mensaje);
    }

    if (modoTesteo) {
        pintor.setPen(QColor(120, 220, 255));
        pintor.drawText(10, 120, "MODO TESTEO: ON");
    }

    if (jugadorMuerto) {
        pintor.fillRect(0, 0, ancho, alto, QColor(80, 0, 0, 185));
        pintor.setPen(Qt::white);
        pintor.setFont(QFont("Arial", 30, QFont::Bold));
        pintor.drawText(QRect(0, 0, ancho, alto - 40), Qt::AlignCenter, "HAS MUERTO");
        pintor.setFont(QFont("Arial", 16, QFont::Normal));
        pintor.drawText(QRect(0, 50, ancho, alto), Qt::AlignCenter, "Presiona ENTER para ir al menu");
    }

    if (nivelCompleto) {
        pintor.fillRect(0, 0, ancho, alto, QColor(0, 0, 0, 160));
        pintor.setPen(Qt::white);
        pintor.setFont(QFont("Arial", 28, QFont::Bold));
        pintor.drawText(QRect(0, 0, ancho, alto), Qt::AlignCenter, "Nivel 1 completado");
    }
}

bool NivelLateral::estaCompleto() const
{
    return nivelCompleto;
}