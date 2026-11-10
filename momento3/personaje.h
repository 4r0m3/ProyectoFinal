// personaje.h
#ifndef PERSONAJE_H
#define PERSONAJE_H

class Personaje
{
public:
    Personaje(int x, int y, int salud);

    int getX() const;
    int getY() const;
    int getSalud() const;
    void recibirDano(int cantidad);

protected:
    int x;
    int y;
    int salud;
};

#endif // PERSONAJE_H