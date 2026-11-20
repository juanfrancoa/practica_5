#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPixmap>

class Obstaculo : public QGraphicsPixmapItem {
public:
    Obstaculo(double x, double y, double ancho, double alto,
              double resistenciaInicial, int jugadorDueno, const QPixmap &tileSprite);

    void recibirDanio(double danio);
    double getResistencia() const { return resistencia; }
    int getJugadorDueno() const { return jugadorDueno; }
    bool estaDestruido() const { return resistencia <= 0.0; }

private:
    double resistencia;
    int jugadorDueno;
    QGraphicsRectItem *fondoTexto;
    QGraphicsSimpleTextItem *etiquetaResistencia;

    void actualizarTexto();
};

#endif // OBSTACULO_H