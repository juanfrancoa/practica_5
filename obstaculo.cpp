#include "obstaculo.h"
#include <QPainter>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QtGlobal>

Obstaculo::Obstaculo(double x, double y, double ancho, double alto,
                     double resistenciaInicial, int jugadorDueno, const QPixmap &tileSprite)
    : QGraphicsPixmapItem(), resistencia(resistenciaInicial), jugadorDueno(jugadorDueno) {

    int w = static_cast<int>(ancho);
    int h = static_cast<int>(alto);
    QPixmap texturaFinal(w, h);

    if (!tileSprite.isNull()) {
        int ladoTile = qMin(w, h);
        QPixmap tileEscalado = tileSprite.scaled(ladoTile, ladoTile,
                                                 Qt::KeepAspectRatio, Qt::FastTransformation);
        QPainter painter(&texturaFinal);
        painter.drawTiledPixmap(0, 0, w, h, tileEscalado);
    } else {
        texturaFinal.fill(Qt::darkGray);
    }

    setPixmap(texturaFinal);
    setPos(x, y);

    fondoTexto = new QGraphicsRectItem(this);
    fondoTexto->setBrush(QBrush(QColor(0, 0, 0, 165)));
    fondoTexto->setPen(Qt::NoPen);

    etiquetaResistencia = new QGraphicsSimpleTextItem(this);
    etiquetaResistencia->setFont(QFont("Arial", 11, QFont::Bold));
    etiquetaResistencia->setBrush(QBrush(Qt::white));

    actualizarTexto();
}

void Obstaculo::recibirDanio(double danio) {
    resistencia -= danio;
    if (resistencia < 0.0) {
        resistencia = 0.0;
    }
    actualizarTexto();
}

void Obstaculo::actualizarTexto() {
    etiquetaResistencia->setText(QString::number(static_cast<int>(resistencia)));

    QRectF rectSprite = boundingRect();
    QRectF rectTexto = etiquetaResistencia->boundingRect();

    double posX = (rectSprite.width() - rectTexto.width()) / 2.0;
    double posY = (rectSprite.height() - rectTexto.height()) / 2.0;

    etiquetaResistencia->setPos(posX, posY);
    fondoTexto->setRect(posX - 5.0, posY - 2.0, rectTexto.width() + 10.0, rectTexto.height() + 4.0);
}