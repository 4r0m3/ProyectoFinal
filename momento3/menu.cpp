#include "menu.h"

const int EN_MENU = 0;
const int JUGANDO = 1;
const int VIENDO_CONTROLES = 2;

Menu::Menu(int ancho, int alto)
    : ancho(ancho),
    alto(alto),
    estado(EN_MENU),
    seleccion(0),
    salir(false),
    nivelActual(nullptr)
{
    opciones.append("Nivel 1");
    opciones.append("Controles");
    opciones.append("Salir");
}

Menu::~Menu()
{
    delete nivelActual;
}

void Menu::teclaPresionada(int tecla)
{
    if (estado == JUGANDO) {
        if (tecla == Qt::Key_Escape) {
            cerrarNivel();
            return;
        }

        if (nivelActual->estaCompleto()) {
            if (tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
                cerrarNivel();
            }
            return;
        }

        nivelActual->teclaPresionada(tecla);
        return;
    }

    if (estado == VIENDO_CONTROLES) {
        if (tecla == Qt::Key_Escape || tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
            estado = EN_MENU;
        }
        return;
    }

    if (tecla == Qt::Key_W || tecla == Qt::Key_Up) {
        moverSeleccion(-1);
    }

    if (tecla == Qt::Key_S || tecla == Qt::Key_Down) {
        moverSeleccion(1);
    }

    if (tecla == Qt::Key_Return || tecla == Qt::Key_Enter || tecla == Qt::Key_Space) {
        elegirOpcion();
    }
}

void Menu::teclaSoltada(int tecla)
{
    if (estado == JUGANDO) {
        nivelActual->teclaSoltada(tecla);
    }
}

void Menu::actualizar()
{
    if (estado == JUGANDO) {
        nivelActual->actualizar();
    }
}

void Menu::moverSeleccion(int cambio)
{
    seleccion = seleccion + cambio;

    if (seleccion < 0) {
        seleccion = opciones.size() - 1;
    }

    if (seleccion >= opciones.size()) {
        seleccion = 0;
    }
}

void Menu::elegirOpcion()
{
    if (seleccion == 0) {
        iniciarNivel(1);
    } else if (seleccion == 1) {
        estado = VIENDO_CONTROLES;
    } else if (seleccion == 2) {
        salir = true;
    }
}

void Menu::iniciarNivel(int numero)
{
    delete nivelActual;
    nivelActual = nullptr;

    if (numero == 1) {
        nivelActual = new NivelLateral(ancho, alto);
    }

    if (nivelActual != nullptr) {
        estado = JUGANDO;
    }
}

void Menu::cerrarNivel()
{
    delete nivelActual;
    nivelActual = nullptr;
    estado = EN_MENU;
}

bool Menu::quiereSalir() const
{
    return salir;
}

void Menu::dibujar(QPainter &pintor)
{
    if (estado == JUGANDO) {
        nivelActual->dibujar(pintor);

        if (nivelActual->estaCompleto()) {
            pintor.setPen(Qt::white);
            pintor.setFont(QFont("Arial", 14));
            pintor.drawText(QRect(0, alto - 80, ancho, 30), Qt::AlignCenter, "Enter: volver al menu");
        }
        return;
    }

    if (estado == VIENDO_CONTROLES) {
        dibujarControles(pintor);
        return;
    }

    dibujarMenu(pintor);
}

void Menu::dibujarMenu(QPainter &pintor)
{
    pintor.fillRect(0, 0, ancho, alto, QColor(20, 10, 10));

    pintor.setPen(QColor(220, 40, 40));
    pintor.setFont(QFont("Arial", 40, QFont::Bold));
    pintor.drawText(QRect(0, 60, ancho, 80), Qt::AlignCenter, "DOOM SLAYER");

    pintor.setFont(QFont("Arial", 22));

    for (int i = 0; i < opciones.size(); i++) {
        QRect zona(ancho / 2 - 150, 200 + i * 60, 300, 45);

        if (i == seleccion) {
            pintor.fillRect(zona, QColor(120, 20, 20));
            pintor.setPen(Qt::white);
        } else {
            pintor.setPen(QColor(170, 170, 170));
        }

        pintor.drawText(zona, Qt::AlignCenter, opciones[i]);
    }

    pintor.setPen(QColor(120, 120, 120));
    pintor.setFont(QFont("Arial", 11));
    pintor.drawText(QRect(0, alto - 40, ancho, 25), Qt::AlignCenter, "W / S: mover    Enter: elegir");
}

void Menu::dibujarControles(QPainter &pintor)
{
    pintor.fillRect(0, 0, ancho, alto, QColor(20, 10, 10));

    pintor.setPen(QColor(220, 40, 40));
    pintor.setFont(QFont("Arial", 30, QFont::Bold));
    pintor.drawText(QRect(0, 40, ancho, 60), Qt::AlignCenter, "Controles");

    pintor.setPen(Qt::white);
    pintor.setFont(QFont("Arial", 16));
    pintor.drawText(QRect(0, 140, ancho, 30), Qt::AlignCenter, "A / D: moverse");
    pintor.drawText(QRect(0, 180, ancho, 30), Qt::AlignCenter, "W: saltar");
    pintor.drawText(QRect(0, 220, ancho, 30), Qt::AlignCenter, "Espacio: disparar");
    pintor.drawText(QRect(0, 260, ancho, 30), Qt::AlignCenter, "E: interactuar");
    pintor.drawText(QRect(0, 300, ancho, 30), Qt::AlignCenter, "Esc: volver al menu");

    pintor.setPen(QColor(120, 120, 120));
    pintor.setFont(QFont("Arial", 11));
    pintor.drawText(QRect(0, alto - 40, ancho, 25), Qt::AlignCenter, "Enter o Esc: volver");
}