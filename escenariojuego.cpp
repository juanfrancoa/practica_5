#include "escenariojuego.h"
#include <QPixmap>

EscenarioJuego::EscenarioJuego(QObject *parent)
    : QGraphicsScene(0, 0, ANCHO_ESCENA, ALTO_ESCENA, parent),
    proyectil(nullptr), jugador1(nullptr), jugador2(nullptr),
    turnoActual(1), juegoTerminado(false), contadorRebotes(0), tiempoVuelo(0.0) {

    timerSimulacion = new QTimer(this);
    connect(timerSimulacion, &QTimer::timeout, this, &EscenarioJuego::avanzarPasoSimulacion);
}

void EscenarioJuego::iniciarPartida() {
    timerSimulacion->stop();
    obstaculos.clear();
    clear();

    turnoActual = 1;
    juegoTerminado = false;
    contadorRebotes = 0;
    tiempoVuelo = 0.0;

    // 1. Crear jugadores (posiciones de rival y disparo físico)
    double yDisparoFisico = ALTO_ESCENA - 195.0;
    jugador1 = new Jugador(1, "Jugador 1", 162.0, 482.0, 48.0, yDisparoFisico, 1);
    jugador2 = new Jugador(2, "Jugador 2", 737.0, 482.0, ANCHO_ESCENA - 48.0, yDisparoFisico, -1);

    addItem(jugador1);
    addItem(jugador2);
    jugador1->dibujarCañonEnEscena(this);
    jugador2->dibujarCañonEnEscena(this);

    // 2. Construir búnkeres con sprites
    construirInfraestructura();

    // 3. Crear proyectil
    proyectil = new Proyectil(RADIO_PROYECTIL, MASA_PROYECTIL, ALTO_ESCENA);
    addItem(proyectil);

    emit turnoCambiado("Turno: Jugador 1 (Izquierda)");
    emit estadoActualizado("Configure ángulo y velocidad para disparar | ESC: Salir pantalla completa");
    emit disparoHabilitado(true);
}

void EscenarioJuego::construirInfraestructura() {
    QPixmap hojaSprites(":/sprites/obstaculo.png");
    QPixmap spritePared;
    QPixmap spriteTecho;

    if (!hojaSprites.isNull()) {
        int anchoCelda = hojaSprites.width() / 2;
        int altoCelda = hojaSprites.height() / 5;
        spritePared = hojaSprites.copy(0, 0, anchoCelda, altoCelda);
        spriteTecho = hojaSprites.copy(anchoCelda, 0, anchoCelda, altoCelda);
    }

    // Búnker Jugador 1 (columnas 200, techo 100)
    obstaculos << new Obstaculo(55,  350, 55,  150, 200, 1, spritePared);
    obstaculos << new Obstaculo(215, 350, 55,  150, 200, 1, spritePared);
    obstaculos << new Obstaculo(55,  290, 215, 60,  100, 1, spriteTecho);

    // Búnker Jugador 2 (columnas 200, techo 100)
    obstaculos << new Obstaculo(630, 350, 55,  150, 200, 2, spritePared);
    obstaculos << new Obstaculo(790, 350, 55,  150, 200, 2, spritePared);
    obstaculos << new Obstaculo(630, 290, 215, 60,  100, 2, spriteTecho);

    for (Obstaculo *obs : obstaculos) {
        addItem(obs);
    }
}

void EscenarioJuego::ejecutarDisparo(double anguloGrados, double velocidadInicial) {
    if (proyectil->estaActivo() || juegoTerminado) return;

    Jugador *atacante = (turnoActual == 1) ? jugador1 : jugador2;
    contadorRebotes = 0;
    tiempoVuelo = 0.0;

    proyectil->lanzar(atacante->getPosCanonX(),
                      atacante->getPosCanonY(),
                      velocidadInicial,
                      anguloGrados,
                      atacante->getDireccionDisparo());

    emit disparoHabilitado(false);
    emit estadoActualizado("Proyectil en trayectoria...");
    timerSimulacion->start(INTERVALO_TIMER_MS);
}

void EscenarioJuego::avanzarPasoSimulacion() {
    for (int paso = 0; paso < SUBPASOS_FISICA; ++paso) {
        if (!proyectil->estaActivo() || juegoTerminado) return;

        proyectil->actualizarMovimiento(DT, GRAVEDAD);
        tiempoVuelo += DT;

        if (proyectil->verificarColisionParedes(ANCHO_ESCENA, ALTO_ESCENA)) {
            contadorRebotes++;
        }

        procesarColisionesObstaculos();
        if (juegoTerminado) return;

        verificarCondicionVictoria();
        if (juegoTerminado) return;

        if (contadorRebotes >= MAX_REBOTES || tiempoVuelo >= 25.0 || proyectil->getRapidez() < 3.0) {
            finalizarTurno();
            return;
        }
    }
}

void EscenarioJuego::procesarColisionesObstaculos() {
    for (Obstaculo *obs : obstaculos) {
        double momentoImpacto = proyectil->getMomentoLineal();

        if (proyectil->verificarColisionBloque(obs->sceneBoundingRect(), COEF_RESTITUCION)) {
            contadorRebotes++;

            if (obs->getJugadorDueno() != turnoActual) {
                double danio = FACTOR_DANIO * momentoImpacto;
                obs->recibirDanio(danio);

                emit estadoActualizado(QString("¡Impacto! Daño infligido: %1")
                                           .arg(QString::number(danio, 'f', 1)));

                if (obs->estaDestruido()) {
                    removeItem(obs);
                    obstaculos.removeOne(obs);
                    delete obs;
                }
            }
            break;
        }
    }
}

void EscenarioJuego::verificarCondicionVictoria() {
    Jugador *objetivo = (turnoActual == 1) ? jugador2 : jugador1;

    if (proyectil->collidesWithItem(objetivo)) {
        timerSimulacion->stop();
        proyectil->detener();
        juegoTerminado = true;

        QString mensaje = QString("¡Victoria para el Jugador %1!").arg(turnoActual);
        emit turnoCambiado(mensaje);
        emit estadoActualizado("Presione 'Reiniciar Partida' para jugar de nuevo");
        emit partidaFinalizada(mensaje);
    }
}

void EscenarioJuego::finalizarTurno() {
    timerSimulacion->stop();
    proyectil->detener();

    turnoActual = (turnoActual == 1) ? 2 : 1;
    if (turnoActual == 1) {
        emit turnoCambiado("Turno: Jugador 1 (Izquierda)");
    } else {
        emit turnoCambiado("Turno: Jugador 2 (Derecha)");
    }
    emit estadoActualizado("Configure ángulo y velocidad para disparar");
    emit disparoHabilitado(true);
}
