#ifndef MENU_H
#define MENU_H

#include <QPainter>
#include <QStringList>
#include <QSoundEffect>
#include "nivel.h"

class Menu
{
public:
    Menu(int ancho, int alto);
    ~Menu();

    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    void actualizar();
    void dibujar(QPainter &pintor);
    void mouseMovido(int x, int y);
    void mousePresionado(int x, int y);
    void mouseSoltado();
    bool quiereSalir() const;

private:
    void moverSeleccion(int cambio);
    void elegirOpcion();
    void iniciarNivel(int numero);
    void cerrarNivel();
    void dibujarMenu(QPainter &pintor);
    void dibujarControles(QPainter &pintor);
    void dibujarCinematica(QPainter &pintor);

    int ancho;
    int alto;
    int estado;
    int seleccion;
    bool salir;
    QStringList opciones;
    Nivel *nivelActual;

    bool nivel2Desbloqueado;
    int nivelJugando;

    QStringList textosCinematica;
    int paginaCinematica;
    double opacidadCinematica;
    int nivelPendiente;

    QSoundEffect sonidoTransicion;
};

#endif