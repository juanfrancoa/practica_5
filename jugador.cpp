#include "jugador.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>

Jugador::Jugador(int id, const QString &nombre, double xRival, double yPiesRival,
                 double xCanon, double yCanon, int direccionDisparo)
    : QGraphicsItemGroup(), id(id), nombre(nombre),
    posCanonX(xCanon), posCanonY(yCanon), direccionDisparo(direccionDisparo) {

    construirFiguraRival(xRival, yPiesRival);
}

void Jugador::construirFiguraRival(double xCentro, double yPies) {
    QPen pen(Qt::black, 2);

    auto *cabeza = new QGraphicsEllipseItem(xCentro - 10, yPies - 75, 20, 20);
    cabeza->setPen(pen);

    auto *torso = new QGraphicsLineItem(xCentro, yPies - 55, xCentro, yPies - 25);
    torso->setPen(pen);

    auto *brazos = new QGraphicsLineItem(xCentro - 15, yPies - 45, xCentro + 15, yPies - 45);
    brazos->setPen(pen);

    auto *piernaIzq = new QGraphicsLineItem(xCentro, yPies - 25, xCentro - 12, yPies);
    piernaIzq->setPen(pen);

    auto *piernaDer = new QGraphicsLineItem(xCentro, yPies - 25, xCentro + 12, yPies);
    piernaDer->setPen(pen);

    auto *etiqueta = new QGraphicsSimpleTextItem(nombre);
    etiqueta->setFont(QFont("Arial", 9, QFont::Bold));
    QRectF rectTexto = etiqueta->boundingRect();
    etiqueta->setPos(xCentro - rectTexto.width() / 2.0, yPies + 2);

    addToGroup(cabeza);
    addToGroup(torso);
    addToGroup(brazos);
    addToGroup(piernaIzq);
    addToGroup(piernaDer);
    addToGroup(etiqueta);
}

void Jugador::dibujarCañonEnEscena(QGraphicsScene *scene) const {
    if (direccionDisparo == 1) {
        scene->addRect(0, 220, 45, 12, QPen(Qt::black), QBrush(Qt::lightGray));
        scene->addRect(10, 195, 25, 25, QPen(Qt::black), QBrush(Qt::darkBlue));
    } else {
        double anchoEscena = scene->width();
        scene->addRect(anchoEscena - 45, 220, 45, 12, QPen(Qt::black), QBrush(Qt::lightGray));
        scene->addRect(anchoEscena - 35, 195, 25, 25, QPen(Qt::black), QBrush(Qt::darkBlue));
    }
}