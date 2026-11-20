#include "proyectil.h"
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <cmath>

Proyectil::Proyectil(double radio, double masa, double altoEscena)
    : QGraphicsEllipseItem(-radio, -radio, radio * 2.0, radio * 2.0),
    posX(0.0), posY(0.0), velX(0.0), velY(0.0),
    masa(masa), radio(radio), altoEscena(altoEscena), activo(false) {

    setPen(QPen(Qt::darkRed, 2));
    setBrush(QBrush(Qt::red));
    setVisible(false);
}

void Proyectil::lanzar(double x0, double y0, double v0, double anguloGrados, int direccion) {
    double rad = qDegreesToRadians(anguloGrados);
    posX = x0;
    posY = y0;
    velX = direccion * v0 * qCos(rad);
    velY = v0 * qSin(rad);
    activo = true;

    sincronizarPosicionGrafica();
    setVisible(true);
}

void Proyectil::actualizarMovimiento(double dt, double gravedad) {
    if (!activo) return;

    posX += velX * dt;
    posY += velY * dt - 0.5 * gravedad * dt * dt;
    velY -= gravedad * dt;

    sincronizarPosicionGrafica();
}

bool Proyectil::verificarColisionParedes(double anchoEscena, double altoEscena) {
    bool reboto = false;

    if (posX - radio <= 0.0) {
        posX = radio;
        velX = -velX;
        reboto = true;
    } else if (posX + radio >= anchoEscena) {
        posX = anchoEscena - radio;
        velX = -velX;
        reboto = true;
    }

    if (posY + radio >= altoEscena) {
        posY = altoEscena - radio;
        velY = -velY;
        reboto = true;
    } else if (posY - radio <= 0.0) {
        posY = radio;
        velY = -velY;
        reboto = true;
    }

    if (reboto) {
        sincronizarPosicionGrafica();
    }
    return reboto;
}

bool Proyectil::verificarColisionBloque(const QRectF &rectBloque, double coefRestitucion) {
    double sceneX = posX;
    double sceneY = altoEscena - posY;

    double closestX = qBound(rectBloque.left(), sceneX, rectBloque.right());
    double closestY = qBound(rectBloque.top(), sceneY, rectBloque.bottom());
    double dx = sceneX - closestX;
    double dy = sceneY - closestY;

    if ((dx * dx + dy * dy) > (radio * radio)) {
        return false;
    }

    bool impactoHorizontal = false;
    if (dx == 0.0 && dy == 0.0) {
        double penIzq = qAbs(sceneX - rectBloque.left());
        double penDer = qAbs(rectBloque.right() - sceneX);
        double penSup = qAbs(sceneY - rectBloque.top());
        double penInf = qAbs(rectBloque.bottom() - sceneY);
        impactoHorizontal = (qMin(penIzq, penDer) < qMin(penSup, penInf));
    } else {
        impactoHorizontal = (qAbs(dx) > qAbs(dy));
    }

    if (impactoHorizontal) {
        velX = -coefRestitucion * velX;
        if (sceneX < rectBloque.center().x()) {
            posX = rectBloque.left() - radio - 0.5;
        } else {
            posX = rectBloque.right() + radio + 0.5;
        }
    } else {
        velY = -coefRestitucion * velY;
        if (sceneY < rectBloque.center().y()) {
            posY = altoEscena - (rectBloque.top() - radio - 0.5);
        } else {
            posY = altoEscena - (rectBloque.bottom() + radio + 0.5);
        }
    }

    sincronizarPosicionGrafica();
    return true;
}

void Proyectil::detener() {
    activo = false;
    velX = 0.0;
    velY = 0.0;
    setVisible(false);
}

double Proyectil::getRapidez() const {
    return std::hypot(velX, velY);
}

double Proyectil::getMomentoLineal() const {
    return masa * getRapidez();
}

void Proyectil::sincronizarPosicionGrafica() {
    setPos(posX, altoEscena - posY);
}
