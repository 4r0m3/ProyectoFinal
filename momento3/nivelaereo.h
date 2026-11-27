#ifndef NIVELAEREO_H
#define NIVELAEREO_H

#include <QList>
#include <QSet>
#include <QString>
#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QTransform>
#include <QSoundEffect>
#include "nivel.h"

struct EnemigoAereo {
    double x;
    double y;
    int salud;
    int dirX;
    int dirY;
    int espera;
    bool inteligente;
    int estado;
    int ticksEstado;
    int objetivoColumna;
    int objetivoFila;
    QList<QPoint> ruta;


    int frameAnimacion;
    int ticksAnimacion;
    int direccionMirada;
    bool estaDisparando;
    int ticksDisparo;
};

struct BalaAerea {
    double x;
    double y;
    double vx;
    double vy;
};

class NivelAereo : public Nivel
{
public:
    NivelAereo(int ancho, int alto, int nivelDificultad = 0);

    void teclaPresionada(int tecla) override;
    void teclaSoltada(int tecla) override;
    void actualizar() override;
    void dibujar(QPainter &pintor) override;
    bool estaCompleto() const override;
    bool estaPerdido() const;
    bool terminoSonidoFinal() const;

    void mouseMovido(int x, int y) override;
    void mousePresionado(int x, int y) override;
    void mouseSoltado() override;

private:
    char bloqueEn(int columna, int fila) const;
    bool esSolido(char bloque) const;
    bool chocaConPared(double x, double y, int tam) const;
    bool hayLineaLibre(double x1, double y1, double x2, double y2) const;
    void abrirPuertas(double x, double y, int tam);
    void moverJugador(double dx, double dy);
    bool moverEnemigo(EnemigoAereo &enemigo, double dx, double dy);
    void recogerObjetos();
    void actualizarJugador();
    void crearBalaJugador(double centroX, double centroY, double apuntaX, double apuntaY, double distancia);
    void actualizarEnemigos();
    void actualizarBalas();
    void patrullar(EnemigoAereo &enemigo);
    void actualizarVigilante(EnemigoAereo &enemigo);
    void emitirSenal(double x, double y, int alcance);
    bool dentroDelCono(const EnemigoAereo &enemigo, double dx, double dy, double distancia) const;
    QList<QPoint> calcularRuta(double xDesde, double yDesde, int columnaDestino, int filaDestino) const;
    void seguirRuta(EnemigoAereo &enemigo);
    void recibirDano(int dano);
    void reaparecer();
    void actualizarCamara();
    void mostrarMensaje(const QString &texto);

    int ancho;
    int alto;
    QList<QString> mapa;
    QList<EnemigoAereo> enemigos;
    QList<BalaAerea> balasJugador;
    QList<BalaAerea> balasEnemigas;
    QSet<int> teclas;

    double jugadorX;
    double jugadorY;
    double inicioX;
    double inicioY;
    int salud;
    int llaves;
    int ticksInvulnerable;
    int esperaDisparo;
    bool disparando;
    int disparosPendientes = 0;
    int esperaEntreDisparos = 0;
    int mouseX;
    int mouseY;
    int camaraX;
    int camaraY;
    bool nivelCompleto;
    int dificultad = 0;
    int vidas = 3;
    bool jugadorPerdido = false;
    QString mensaje;
    int ticksMensaje;

    int frameAnimacionJugador;
    int ticksAnimacionJugador;
    int direccionMiradaJugador;
    bool jugadorDisparando;
    int ticksDisparoJugador;

    QPixmap texPared;
    QPixmap texSuelo;
    QPixmap texPuerta;
    QPixmap texPolicia;
    QPixmap texPoliciaAlterado;
    QPixmap texPoliciaBusqueda;
    QPixmap texJugador;
    QPixmap texSangre;
    QList<QPoint> manchas;
    QSoundEffect sonidoDisparo;
    QSoundEffect sonidoDisparoEnemigo;
    QSoundEffect sonidoRespawn;
    QSoundEffect sonidoAlerta;
    QSoundEffect sonidoVictoria;
    QSoundEffect sonidoMuerte;
    QSoundEffect sonidoPuerta;
    QSoundEffect SonidoFondo;
    QSoundEffect sonidoRecoger;
    QSoundEffect sonidoGameOver;
    bool sonidoFinalIniciado = false;
    bool sonidoFinalTerminado = false;
    int ticksSonidoFinal = 0;


    double escalaPolicia;
    double escalaPoliciaAlterado;
    double escalaPoliciaBusqueda;
    double escalaJugador;
};

#endif