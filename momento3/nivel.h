#ifndef NIVEL_H
#define NIVEL_H

#include <QPainter>

class Nivel
{
public:
    virtual ~Nivel() {}

    virtual void teclaPresionada(int tecla) = 0;
    virtual void teclaSoltada(int tecla) = 0;
    virtual void actualizar() = 0;
    virtual void dibujar(QPainter &pintor) = 0;
    virtual bool estaCompleto() const = 0;

    virtual void mouseMovido(int x, int y) { Q_UNUSED(x); Q_UNUSED(y); }
    virtual void mousePresionado(int x, int y) { Q_UNUSED(x); Q_UNUSED(y); }
    virtual void mouseSoltado() {}
};

#endif