#include "menu.h"
#include "nivellateral.h"
#include "nivelaereo.h"
#include <QUrl>

const int EN_MENU = 0;
const int JUGANDO = 1;
const int VIENDO_CONTROLES = 2;
const int EN_CINEMATICA = 3;

QStringList textosDeEscena(int escena)
{
    QStringList textos;

    if (escena == 1) {
        textos.append("MEDELLIN\nUna ciudad entre montañas,\ncansada de vivir con miedo");
        textos.append("Anselmo Quintero, 'El Patron',\njefe del Cartel de la Montaña,\nestaba preso en La Fortaleza");
        textos.append("Una noche, sus hombres ofrecieron\nplata o plomo a los guardias\nTodos eligieron la plata");
        textos.append("Pero El Patron no se conformo\ncon escapar. Hizo un pacto\ncon el infierno para no ser atrapado");
        textos.append("Los demonios le abrieron un portal\ny los policias sobornados\nse convirtieron en sus soldados");
        textos.append("Entonces el Consejo recordo\nal unico que ya habia vencido\nal infierno por dentro\n\nDOOM SLAYER");
        textos.append("Tu mision\nCruzar la montaña, romper el bloqueo\ne ingresar a La Fortaleza");
        textos.append("No dudes en disparar\nQuienes visten ese uniforme\nya no responden a la ley");
    } else if (escena == 2) {
        textos.append("El tanque blindado cayo\nEntre sus restos encontraste la llave\nque abre las puertas de La Fortaleza");
        textos.append("Por dentro, la carcel cambio\nLos pasillos se retuercen como un laberinto\ny el infierno gotea por las paredes");
        textos.append("Los centinelas del Patron\nvigilan cada corredor\nEl ruido de tus disparos los atrae");
        textos.append("Busca llaves, abre las puertas\ny encuentra la salida que lleva\nal refugio de El Patron");
    } else if (escena == 3) {
        textos.append("Subes la ultima escalera\nLa sala de El Patron esta vacia\nSolo queda un portal ardiendo");
        textos.append("Sobre la mesa, una nota\n'Si quieres atraparme, ven a buscarme\nal lugar de donde vienen mis socios'");
        textos.append("Doom Slayer sonrie\nOtra vez lo invitan al infierno");
        textos.append("Medellin respira de nuevo\npero la caceria apenas empieza\n\nFIN DEL PRIMER ACTO");
    }

    return textos;
}

Menu::Menu(int ancho, int alto)
    : ancho(ancho),
    alto(alto),
    estado(EN_MENU),
    seleccion(0),
    salir(false),
    nivelActual(nullptr),
    nivel2Desbloqueado(false),
    nivelJugando(0),
    paginaCinematica(0),
    opacidadCinematica(0.0),
    nivelPendiente(0)
{
    opciones.append("Nivel 1");
    opciones.append("Nivel 2");
    opciones.append("Controles");
    opciones.append("Salir");

    sonidoTransicion.setSource(QUrl("qrc:/Sonido/sonidos/sound-transicion.wav"));
    sonidoTransicion.setVolume(0.5);
}

Menu::~Menu()
{
    delete nivelActual;
}

void Menu::teclaPresionada(int tecla)
{
    // --- CONTROLES MIENTRAS SE JUEGA ---
    if (estado == JUGANDO) {
        if (tecla == Qt::Key_Escape) {
            cerrarNivel();
            return;
        }

        // Si el nivel se completó con éxito
        if (nivelActual->estaCompleto()) {
            if (tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
                // Si acabamos de pasar el nivel 1, desbloqueamos el 2
                if (nivelJugando == 1) {
                    nivel2Desbloqueado = true;
                }

                int nivelTerminado = nivelJugando;
                cerrarNivel();

                if (nivelTerminado == 2) {
                    textosCinematica = textosDeEscena(3);
                    estado = EN_CINEMATICA;
                    paginaCinematica = 0;
                    opacidadCinematica = 0.0;
                    nivelPendiente = 0;
                    sonidoTransicion.play();
                }
            }
            return;
        }

        nivelActual->teclaPresionada(tecla);

        NivelLateral *lateral = dynamic_cast<NivelLateral*>(nivelActual);
        if (lateral != nullptr && lateral->debeVolverAlMenu()) {
            if (tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
                cerrarNivel();
            }
        }
        return;
    }

    // --- CONTROLES EN LA PANTALLA DE CONTROLES ---
    if (estado == VIENDO_CONTROLES) {
        if (tecla == Qt::Key_Escape || tecla == Qt::Key_Return || tecla == Qt::Key_Enter) {
            estado = EN_MENU;
        }
        return;
    }

    // --- CONTROLES EN LA CINEMÁTICA ---
    if (estado == EN_CINEMATICA) {
        bool saltar = (tecla == Qt::Key_Escape);
        bool avanzar = (tecla == Qt::Key_Space || tecla == Qt::Key_Return || tecla == Qt::Key_Enter);

        if (saltar || avanzar) {
            if (saltar) {
                paginaCinematica = textosCinematica.size();
            } else {
                paginaCinematica++;
            }
            opacidadCinematica = 0.0;

            if (paginaCinematica >= textosCinematica.size()) {
                if (nivelPendiente == 0) {
                    estado = EN_MENU;
                } else {
                    iniciarNivel(nivelPendiente);
                }
            } else if (avanzar) {
                sonidoTransicion.play();
            }
        }
        return;
    }

    // --- CONTROLES EN EL MENÚ PRINCIPAL ---
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

void Menu::mouseMovido(int x, int y)
{
    if (estado == JUGANDO) {
        nivelActual->mouseMovido(x, y);
    }
}

void Menu::mousePresionado(int x, int y)
{
    if (estado == JUGANDO) {
        nivelActual->mousePresionado(x, y);
    }
}

void Menu::mouseSoltado()
{
    if (estado == JUGANDO) {
        nivelActual->mouseSoltado();
    }
}

void Menu::actualizar()
{
    if (estado == JUGANDO) {
        nivelActual->actualizar();
    }
    // Actualizar el efecto de "fade-in" en la cinemática
    else if (estado == EN_CINEMATICA) {
        if (opacidadCinematica < 255.0) {
            opacidadCinematica += 2.5; // Velocidad de aclarado (ajústala a tu gusto)
            if (opacidadCinematica > 255.0) opacidadCinematica = 255.0;
        }
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
    if (seleccion == 0) { // Nivel 1
        textosCinematica = textosDeEscena(1);
        estado = EN_CINEMATICA;
        paginaCinematica = 0;
        opacidadCinematica = 0.0;
        nivelPendiente = 1;
        sonidoTransicion.play();
    } else if (seleccion == 1) { // Nivel 2
        // Solo entra si está desbloqueado
        if (nivel2Desbloqueado) {
            textosCinematica = textosDeEscena(2);
            estado = EN_CINEMATICA;
            paginaCinematica = 0;
            opacidadCinematica = 0.0;
            nivelPendiente = 2;
            sonidoTransicion.play();
        }
    } else if (seleccion == 2) {
        estado = VIENDO_CONTROLES;
    } else if (seleccion == 3) {
        salir = true;
    }
}

void Menu::iniciarNivel(int numero)
{
    delete nivelActual;
    nivelActual = nullptr;
    nivelJugando = numero; // Guardamos qué nivel se está jugando

    if (numero == 1) {
        nivelActual = new NivelLateral(ancho, alto);
    } else if (numero == 2) {
        nivelActual = new NivelAereo(ancho, alto);
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

    if (estado == EN_CINEMATICA) {
        dibujarCinematica(pintor);
        return;
    }

    dibujarMenu(pintor);
}

void Menu::dibujarCinematica(QPainter &pintor)
{
    // Fondo completamente negro
    pintor.fillRect(0, 0, ancho, alto, Qt::black);

    if (paginaCinematica < textosCinematica.size()) {
        // Dibujamos el texto actual ajustando su "Alfa" (transparencia) para el efecto fade-in
        pintor.setPen(QColor(255, 255, 255, (int)opacidadCinematica));
        pintor.setFont(QFont("Arial", 22, QFont::Bold));

        // Centramos el texto en toda la pantalla
        pintor.drawText(QRect(0, 0, ancho, alto), Qt::AlignCenter, textosCinematica[paginaCinematica]);

        // Mostrar mensaje para avanzar cuando el texto ya esté 100% visible
        if (opacidadCinematica >= 255.0) {
            pintor.setPen(QColor(120, 120, 120));
            pintor.setFont(QFont("Arial", 12));
            pintor.drawText(QRect(0, alto - 50, ancho, 30), Qt::AlignCenter, "Espacio: continuar     Esc: saltar");
        }
    }
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
        QString textoMostrado = opciones[i];

        // Lógica visual para mostrar que el Nivel 2 está bloqueado
        bool esNivelBloqueado = (i == 1 && !nivel2Desbloqueado);
        if (esNivelBloqueado) {
            textoMostrado += " (Bloqueado)";
        }

        if (i == seleccion) {
            pintor.fillRect(zona, QColor(120, 20, 20));
            // Si está seleccionado pero bloqueado, texto rojizo oscuro
            if (esNivelBloqueado) {
                pintor.setPen(QColor(180, 100, 100));
            } else {
                pintor.setPen(Qt::white);
            }
        } else {
            // Si no está seleccionado y está bloqueado, se ve más oscuro
            if (esNivelBloqueado) {
                pintor.setPen(QColor(80, 80, 80));
            } else {
                pintor.setPen(QColor(170, 170, 170));
            }
        }

        pintor.drawText(zona, Qt::AlignCenter, textoMostrado);
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
    pintor.drawText(QRect(0, 300, ancho, 30), Qt::AlignCenter, "Nivel 2: mouse apunta, clic dispara, WASD mueve");
    pintor.drawText(QRect(0, 340, ancho, 30), Qt::AlignCenter, "Esc: volver al menu");

    pintor.setPen(QColor(120, 120, 120));
    pintor.setFont(QFont("Arial", 11));
    pintor.drawText(QRect(0, alto - 40, ancho, 25), Qt::AlignCenter, "Enter o Esc: volver");
}
