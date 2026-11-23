#ifndef OBJETOS_H
#define OBJETOS_H

#include <QPainter>
#include <QRect>

class DoomSlayer;

enum class TipoObjeto
{
    Llave,
    Curacion,
    Furia,
    Puerta,
    ZonaJefe
};

class Objetos
{
public:
    Objetos(TipoObjeto tipo, int x, int y, int ancho = 0, int alto = 0);

    bool interactuar(DoomSlayer *doom);
    bool detectarContacto(const QRect &cuerpo);
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    TipoObjeto getTipo() const;

private:
    TipoObjeto tipo;
    QRect zona;
    bool usado;
};

#endif