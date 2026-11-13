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
#include "fosa.h"
#include "objetos.h"

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
    void revisarColisiones();
    void limitarEnTrinchera();
    void aplicarDanoEspinas();
    void interactuar();
    const Fosa* buscarFosa(const QRect &cuerpo) const;

    QPixmap fondo;
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
    QList<Enemigos*> enemigos;
    QList<Plataforma*> plataformas;
    QList<Fosa*> fosas;
    QList<Objetos*> objetos;
    QSet<int> teclas;
    int ticksEspinas = 0;
    bool nivelCompleto = false;
    QString mensaje;
    int ticksMensaje = 0;
};

#endif