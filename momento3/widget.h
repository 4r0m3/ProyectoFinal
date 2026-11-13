#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPaintEvent>
#include "nivellateral.h"

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
    Ui::Widget *ui;
    QTimer *temporizador;
    NivelLateral *nivel;
};

#endif