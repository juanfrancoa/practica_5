#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QString>

class Jugador : public QGraphicsItemGroup {
public:
    Jugador(int id, const QString &nombre, double xRival, double yPiesRival,
            double xCanon, double yCanon, int direccionDisparo);

    void dibujarCañonEnEscena(QGraphicsScene *scene) const;

    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    double getPosCanonX() const { return posCanonX; }
    double getPosCanonY() const { return posCanonY; }
    int getDireccionDisparo() const { return direccionDisparo; }

private:
    int id;
    QString nombre;
    double posCanonX;
    double posCanonY;
    int direccionDisparo; // +1 hacia la derecha, -1 hacia la izquierda

    void construirFiguraRival(double xCentro, double yPies);
};

#endif // JUGADOR_H