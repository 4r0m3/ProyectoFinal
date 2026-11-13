#ifndef FOSA_H
#define FOSA_H

#include <QPainter>
#include <QRect>

enum class TipoFosa
{
    Abismo,
    Trinchera
};

class Fosa
{
public:
    Fosa(int x, int ySuelo, int ancho, int profundidad, TipoFosa tipo);

    void dibujar(QPainter &pintor);
    QRect getRect() const;
    TipoFosa getTipo() const;
    int getFondo() const;

private:
    QRect zona;
    TipoFosa tipo;
};

#endif