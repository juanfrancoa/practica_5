#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsEllipseItem>
#include <QRectF>

class Proyectil : public QGraphicsEllipseItem {
public:
    Proyectil(double radio, double masa, double altoEscena);

    void lanzar(double x0, double y0, double v0, double anguloGrados, int direccion);
    void actualizarMovimiento(double dt, double gravedad);
    bool verificarColisionParedes(double anchoEscena, double altoEscena);
    bool verificarColisionBloque(const QRectF &rectBloque, double coefRestitucion);
    void detener();

    double getRapidez() const;
    double getMomentoLineal() const;
    double getMasa() const { return masa; }
    bool estaActivo() const { return activo; }

private:
    double posX;
    double posY; // Coordenada física (eje Y positivo hacia arriba)
    double velX;
    double velY;
    double masa;
    double radio;
    double altoEscena;
    bool activo;

    void sincronizarPosicionGrafica();
};

#endif // PROYECTIL_H