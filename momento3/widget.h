#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QSet>
#include <QList>
#include <QKeyEvent>
#include <QPaintEvent>
#include "doomslayer.h"
#include "proyectil.h"
#include "enemigos.h"
#include "plataforma.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;
    void paintEvent(QPaintEvent *evento) override;

private slots:
    void actualizarJuego();

private:
    void actualizarMovimiento();
    void actualizarProyectiles();
    void actualizarSuelo();
    void revisarColisiones();

    Ui::Widget *ui;
    QTimer *temporizador;
    DoomSlayer *doom;
    QList<Proyectil*> proyectiles;
    QList<Enemigos*> enemigos;
    QList<Plataforma*> plataformas;
    QSet<int> teclas;
};

#endif