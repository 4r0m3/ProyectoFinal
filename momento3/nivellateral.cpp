#include "nivellateral.h"
#include <QFont>
#include <QPen>
#include <QKeyEvent>
#include <QRandomGenerator>

const int ANCHO_NIVEL = 2400;
const int Y_SUELO = 400; //altura del suelo
const int ALTO_SUELO = 100;
const int SUELO_DOOM = 340;
const int SUELO_ABISMO = 5000;
const int PROFUNDIDAD_TRINCHERA = 30;
const int TICKS_ENTRE_DANO = 10;
const int DANO_ESPINAS = 2;
const int DANO_CAIDA = 20;
const int PROBABILIDAD_CURACION = 5;
//camara
const int REPETICIONES_FONDO = 4;
const int MARGEN_CAMARA = 350;
//suelo
const int ALTO_IMAGEN_SUELO = 100;
const int ANCHO_SUELO1 = 400;
const int ANCHO_SUELO2 = 450;


NivelLateral::NivelLateral(int ancho, int alto)
    : ancho(ancho),
    alto(alto),
    anchoNivel(ANCHO_NIVEL),
    camaraX(0),
    repeticionesFondo(REPETICIONES_FONDO),
    anchoFondo(0)
{
    fondo.load(":/imagenes/sprite/FondoNivel1");
    if (!fondo.isNull()) {
        anchoFondo = (anchoNivel + repeticionesFondo - 1) / repeticionesFondo;
        fondo = fondo.scaled(anchoFondo, alto, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    suelo1.load(":/imagenes/sprite/Suelo1");
    suelo2.load(":/imagenes/sprite/Suelo2");

    if (!suelo1.isNull()) {
        suelo1 = suelo1.scaled(ANCHO_SUELO1, ALTO_IMAGEN_SUELO, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    if (!suelo2.isNull()) {
        suelo2 = suelo2.scaled(ANCHO_SUELO2, ALTO_IMAGEN_SUELO, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    doom = new DoomSlayer(100, SUELO_DOOM);

    enemigos.append(new Enemigos(400, SUELO_DOOM, 330, 450));
    enemigos.append(new Enemigos(660, SUELO_DOOM, 640, 690));
    enemigos.append(new Enemigos(1100, SUELO_DOOM, 1050, 1250));
    enemigos.append(new Enemigos(1900, SUELO_DOOM, 1850, 2050));
    enemigos.append(new Enemigos(2150, SUELO_DOOM, 2100, 2250));

    plataformas.append(new Plataforma(150, 350, 120, 15));
    plataformas.append(new Plataforma(320, 280, 120, 15));
    plataformas.append(new Plataforma(500, 210, 120, 15));
    plataformas.append(new Plataforma(640, 300, 110, 15, 50, 0, 0.03));
    plataformas.append(new Plataforma(10, 330, 80, 15, 0, 50, 0.03));
    plataformas.append(new Plataforma(850, 320, 120, 15));
    plataformas.append(new Plataforma(1020, 260, 120, 15));
    plataformas.append(new Plataforma(1380, 320, 100, 15, 40, 0, 0.03));
    plataformas.append(new Plataforma(1600, 3320, 100, 15, 0, 40, 0.03));


    fosas.append(new Fosa(190, Y_SUELO, 110, PROFUNDIDAD_TRINCHERA, TipoFosa::Trinchera));
    fosas.append(new Fosa(520, Y_SUELO, 100, ALTO_SUELO, TipoFosa::Abismo));
    fosas.append(new Fosa(1350, Y_SUELO, 110, ALTO_SUELO, TipoFosa::Abismo));
    fosas.append(new Fosa(1700, Y_SUELO, 110, PROFUNDIDAD_TRINCHERA, TipoFosa::Trinchera));


    objetos.append(new Objetos(TipoObjeto::Curacion, 200, 326));
    objetos.append(new Objetos(TipoObjeto::Furia, 370, 256));
    objetos.append(new Objetos(TipoObjeto::Llave, 550, 180));
    objetos.append(new Objetos(TipoObjeto::Furia, 900, 396));
    objetos.append(new Objetos(TipoObjeto::Curacion, 1060, 236));
    objetos.append(new Objetos(TipoObjeto::Puerta, ANCHO_NIVEL - 80, Y_SUELO - 80));

    actualizarCamara();
}

NivelLateral::~NivelLateral()
{
    qDeleteAll(proyectiles);
    qDeleteAll(enemigos);
    qDeleteAll(plataformas);
    qDeleteAll(fosas);
    qDeleteAll(objetos);
    delete doom;
}

void NivelLateral::teclaPresionada(int tecla)
{
    if (nivelCompleto) return;

    teclas.insert(tecla);
    actualizarMovimiento();

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

const Fosa* NivelLateral::buscarFosa(const QRect &cuerpo) const
{
    int centro = cuerpo.center().x();

    for (const Fosa *fosa : fosas) {
        QRect zona = fosa->getRect();
        if (centro >= zona.left() && centro <= zona.right()) {
            return fosa;
        }
    }

    return nullptr;
}

void NivelLateral::actualizarSuelo()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();
    int suelo = SUELO_DOOM;
    Plataforma *apoyo = nullptr;

    const Fosa *fosa = buscarFosa(cuerpo);
    bool enTrinchera = fosa != nullptr && fosa->getTipo() == TipoFosa::Trinchera;

    if (enTrinchera) {
        suelo = fosa->getFondo() - cuerpo.height();
    } else if (fosa != nullptr || pies > Y_SUELO) {
        suelo = SUELO_ABISMO;
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

void NivelLateral::limitarEnTrinchera()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();

    if (pies <= Y_SUELO) return;

    const Fosa *fosa = buscarFosa(cuerpo);
    if (fosa == nullptr || fosa->getTipo() != TipoFosa::Trinchera) return;

    QRect zona = fosa->getRect();

    if (cuerpo.left() < zona.left()) {
        doom->moverX(zona.left() - cuerpo.left());
    }

    if (cuerpo.right() > zona.right()) {
        doom->moverX(zona.right() - cuerpo.right());
    }
}

void NivelLateral::aplicarDanoEspinas()
{
    QRect cuerpo = doom->getRect();
    int pies = cuerpo.y() + cuerpo.height();

    const Fosa *fosa = buscarFosa(cuerpo);
    bool dentro = pies > Y_SUELO && fosa != nullptr && fosa->getTipo() == TipoFosa::Trinchera;

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

    if (teclas.contains(Qt::Key_W)) doom->saltar();

    for (Plataforma *plataforma : plataformas) {
        plataforma->actualizar();
    }

    actualizarSuelo();
    doom->actualizar();
    limitarEnNivel();
    limitarEnTrinchera();
    aplicarDanoEspinas();

    if (doom->getY() > alto) {
        doom->recibirDano(DANO_CAIDA);
        doom->reaparecer(100, SUELO_DOOM);
    }

    for (Enemigos *enemigo : enemigos) {
        enemigo->actualizar();
    }

    actualizarProyectiles();
    revisarColisiones();
    actualizarCamara();
}

void NivelLateral::dibujar(QPainter &pintor)
{
    pintor.fillRect(0, 0, ancho, alto, QColor(30, 30, 30));

    pintor.save();
    pintor.translate(-camaraX, 0);

    if (!fondo.isNull()) {
        for (int i = 0; i < repeticionesFondo; i++) {
            pintor.drawPixmap(i * anchoFondo, 0, fondo);
        }
    }

    if (!suelo1.isNull() && !suelo2.isNull()) {
        int posicionX = 0;
        int indice = 0;

        while (posicionX < anchoNivel) {
            const QPixmap &imagen = (indice % 2 == 0) ? suelo1 : suelo2;
            pintor.drawPixmap(posicionX, Y_SUELO, imagen);
            posicionX += imagen.width();
            indice++;
        }
    } else {
        pintor.fillRect(0, Y_SUELO, anchoNivel, ALTO_SUELO, QColor(90, 60, 40));
    }

    for (Fosa *fosa : fosas) {
        fosa->dibujar(pintor);
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

    pintor.restore();

    pintor.setPen(Qt::white);
    pintor.drawText(10, 20, QString("Salud: %1").arg(doom->getSalud()));
    pintor.drawText(10, 40, QString("Llave: %1").arg(doom->tieneLlave() ? "Si" : "No"));
    pintor.drawText(10, 60, "E: interactuar");

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