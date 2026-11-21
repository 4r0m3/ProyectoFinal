#ifndef MENU_H
#define MENU_H

#include <QPainter>
#include <QStringList>
#include "nivellateral.h"

class Menu
{
public:
    Menu(int ancho, int alto);
    ~Menu();

    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    void actualizar();
    void dibujar(QPainter &pintor);
    bool quiereSalir() const;

private:
    void moverSeleccion(int cambio);
    void elegirOpcion();
    void iniciarNivel(int numero);
    void cerrarNivel();
    void dibujarMenu(QPainter &pintor);
    void dibujarControles(QPainter &pintor);

    int ancho;
    int alto;
    int estado;
    int seleccion;
    bool salir;
    QStringList opciones;
    NivelLateral *nivelActual;
};

#endif