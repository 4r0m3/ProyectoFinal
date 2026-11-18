#include "nivellateral.h"
#include <QFont>
#include <QPen>
#include <QKeyEvent>
#include <QRandomGenerator>

const int INICIO_X = 4100;
const int ANCHO_NIVEL = 10000;
const int SUELO_ABISMO = 5000;
const int ALTO_DOOM = 60;
const int ALTO_ENEMIGO_NIVEL = 60;
const int MARGEN_APOYO = 15;
const int TOLERANCIA_PARED = 4;
const int TICKS_ENTRE_DANO = 10;
const int DANO_ESPINAS = 2;
const int DANO_CAIDA = 20;
const int PROBABILIDAD_CURACION = 5;
//camara
const int ANCHO_FONDO = 800;
const int REPETICIONES_PRIMER_FONDO = 3;
const int MARGEN_CAMARA = 350;
//suelo
const int ALTO_IMAGEN_SUELO = 100;
const int ANCHO_SUELO1 = 400;
const int ANCHO_SUELO2 = 450;
const int DANO_BALA_ENEMIGA = 8;

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
    //Parte 1 Plataformeo - controles
    //                    x    ancho   y    espinas
    tramos.append(Tramo{    0,  450, 400, false });
    tramos.append(Tramo{  550,  250, 360, false });
    tramos.append(Tramo{  800,  200, 320, false });
    tramos.append(Tramo{ 1130,  270, 400, false });
    tramos.append(Tramo{ 1400,  110, 440, false });
    tramos.append(Tramo{ 1510,  490, 400, false });
    tramos.append(Tramo{ 2000,  400, 360, false });
    tramos.append(Tramo{ 4000,  500, 320, false });
    tramos.append(Tramo{ 4500,  100, 400, false});
    plataformas.append(new Plataforma(2500, 330, 100, 10, 80, 0));
    plataformas.append(new Plataforma(2800, 330, 100, 10, 0, 80, 0.05));
    plataformas.append(new Plataforma(3000, 310, 100, 10, 0, 30, 0.08));
    plataformas.append(new Plataforma(3200, 300, 90, 10, 0, 60, 0.08));
    plataformas.append(new Plataforma(3400, 380, 100, 10, 50, 50, 0.05));
    plataformas.append(new Plataforma(3600, 360, 100, 10, -50, 50, 0.05));
    plataformas.append(new Plataforma(3850, 360, 150, 10));

    //parte 2 carcel - enemigos
    tramos.append(Tramo{ 4600,  100, 450, false});
    tramos.append(Tramo{ 4700,  80,  450, false });
    tramos.append(Tramo{ 4850,  4000, 450, false});
    auto enemigoEn = [this](int x, int izq, int der) {
        enemigos.append(new Enemigos(x, sueloEn(x + 25) - ALTO_ENEMIGO_NIVEL, izq, der));
    };
    plataformas.append(new Plataforma(5000, 360, 100, 10));
    plataformas.append(new Plataforma(5250, 340, 100, 10));
    plataformas.append(new Plataforma(5550, 320, 100, 10));
    plataformas.append(new Plataforma(6000, 300, 100, 10));
    enemigoEn(5000, 4950, 5150);
    enemigoEn(5400, 5300, 5500);
    enemigoEn(5800, 5700, 5900);
    //objetos.append(new Objetos(TipoObjeto::Curacion, 200, 306));    // sobre plataforma (150, 330)



    //aparición del personaje
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
    delete doom;
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

    teclas.insert(tecla);
    actualizarMovimiento();

    if (tecla == Qt::Key_W) {
        doom->saltar();
    }

    if (tecla == Qt::Key_Space) {
        proyectiles.append(doom->disparar());
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

void NivelLateral::actualizarCamara()
{
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

// Si los pies de Doom estan por debajo de la superficie de un tramo
// y se solapan horizontalmente con el, el tramo actua como pared.
void NivelLateral::resolverParedes()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();

    for (int i = 0; i < tramos.size(); i++) {
        const Tramo &t = tramos.at(i);

        if (pies <= t.y + TOLERANCIA_PARED) continue;   // va por encima
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

void NivelLateral::actualizar()
{
    if (nivelCompleto) return;

    if (ticksMensaje > 0) ticksMensaje--;

    for (Plataforma *plataforma : plataformas) {
        plataforma->actualizar();
    }

    actualizarSuelo();
    doom->actualizar();
    limitarEnNivel();
    resolverParedes();
    aplicarDanoEspinas();

    // checkpoint: ultimo punto seguro (tramo sin espinas, cuerpo completo dentro)
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
        }
    }

    actualizarProyectiles();
    actualizarProyectilesEnemigos();
    revisarColisiones();
    actualizarCamara();
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

    // abismos: relleno negro en los huecos entre tramos
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

    for (Proyectil *bala : proyectiles) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 220, 0));
    }

    for (Proyectil *bala : proyectilesEnemigos) {
        pintor.fillRect(bala->getX(), bala->getY(), 10, 4, QColor(255, 90, 40));
    }

    pintor.restore();

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