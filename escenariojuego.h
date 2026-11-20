#ifndef ESCENARIOJUEGO_H
#define ESCENARIOJUEGO_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include <QString>
#include "proyectil.h"
#include "obstaculo.h"
#include "jugador.h"

class EscenarioJuego : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EscenarioJuego(QObject *parent = nullptr);

    void iniciarPartida();
    void ejecutarDisparo(double anguloGrados, double velocidadInicial);

signals:
    void turnoCambiado(const QString &textoTurno);
    void estadoActualizado(const QString &textoEstado);
    void disparoHabilitado(bool habilitado);
    void partidaFinalizada(const QString &mensajeGanador);

private slots:
    void avanzarPasoSimulacion();

private:
    // Parámetros físicos y dimensiones de la caja
    static constexpr double ANCHO_ESCENA = 900.0;
    static constexpr double ALTO_ESCENA = 500.0;
    static constexpr double DT = 0.016;               // Paso discreto fino para colisiones exactas (s)
    static constexpr int SUBPASOS_FISICA = 5;        // Multiplicador de velocidad de simulación (5x)
    static constexpr int INTERVALO_TIMER_MS = 16;    // Refresco gráfico (~60 FPS)
    static constexpr double GRAVEDAD = 9.8;          // Gravedad (m/s^2)
    static constexpr double COEF_RESTITUCION = 0.65; // Coeficiente e en bloques
    static constexpr double FACTOR_DANIO = 0.25;     // Constante de proporcionalidad de daño
    static constexpr double MASA_PROYECTIL = 5.0;    // Masa del proyectil (kg)
    static constexpr double RADIO_PROYECTIL = 10.0;  // Radio del proyectil (px)
    static constexpr int MAX_REBOTES = 6;            // Límite de rebotes por turno

    QTimer *timerSimulacion;
    Proyectil *proyectil;
    Jugador *jugador1;
    Jugador *jugador2;
    QList<Obstaculo*> obstaculos;

    int turnoActual;
    bool juegoTerminado;
    int contadorRebotes;
    double tiempoVuelo;

    void construirInfraestructura();
    void procesarColisionesObstaculos();
    void verificarCondicionVictoria();
    void finalizarTurno();
};

#endif // ESCENARIOJUEGO_H