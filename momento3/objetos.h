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
    Puerta
};

class Objetos
{
public:
    Objetos(TipoObjeto tipo, int x, int y);

    bool interactuar(DoomSlayer *doom);
    void dibujar(QPainter &pintor);
    QRect getRect() const;
    TipoObjeto getTipo() const;

private:
    TipoObjeto tipo;
    QRect zona;
    bool usado;
};

#endif