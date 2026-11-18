#ifndef NIVELLATERAL_H
#define NIVELLATERAL_H

#include <QPixmap>
#include <QList>
#include <QSet>
#include <QRect>
#include <QString>
#include <QPainter>
#include "doomslayer.h"
#include "proyectil.h"
#include "enemigos.h"
#include "plataforma.h"
#include "objetos.h"

// Un tramo de suelo: una franja horizontal con su propia altura.
// Los huecos entre tramos son abismos.
struct Tramo {
    int x;          // inicio
    int ancho;
    int y;          // altura de la superficie
    bool espinas;   // true = hace dano mientras Doom esta encima
};

class NivelLateral
{
public:
    NivelLateral(int ancho, int alto);
    ~NivelLateral();

    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    void actualizar();
    void dibujar(QPainter &pintor);
    bool estaCompleto() const;

private:
    void actualizarMovimiento();
    void actualizarProyectiles();
    void actualizarSuelo();
    void actualizarCamara();
    void limitarEnNivel();
    void resolverParedes();
    void revisarColisiones();
    void aplicarDanoEspinas();
    void interactuar();
    void dibujarTramo(QPainter &pintor, const Tramo &tramo);
    int sueloEn(int x) const;
    void actualizarProyectilesEnemigos();
    bool tocaSuelo(const QRect &zona) const;

    QPixmap fondo;
    QPixmap fondo2;
    QPixmap suelo1;
    QPixmap suelo2;
    int repeticionesFondo;
    int anchoFondo;

    int ancho;
    int alto;
    int anchoNivel;
    int camaraX;
    DoomSlayer *doom;
    QList<Proyectil*> proyectiles;
    QList<Proyectil*> proyectilesEnemigos;
    QList<Enemigos*> enemigos;
    QList<Plataforma*> plataformas;
    QList<Objetos*> objetos;
    QList<Tramo> tramos;
    const Tramo *tramoApoyo = nullptr;
    int seguroX = 100;
    int seguroY = 0;
    QSet<int> teclas;
    int ticksEspinas = 0;
    bool nivelCompleto = false;
    QString mensaje;
    int ticksMensaje = 0;
};

#endif