#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), hudBar(nullptr) {
    ui->setupUi(this);

    escenario = new EscenarioJuego(this);
    configurarInterfazPantallaCompleta();

    // Conectar señales del escenario con los controles de la ventana
    connect(escenario, &EscenarioJuego::turnoCambiado, ui->lblTurno, &QLabel::setText);
    connect(escenario, &EscenarioJuego::estadoActualizado, ui->lblEstado, &QLabel::setText);
    connect(escenario, &EscenarioJuego::disparoHabilitado, ui->btnDisparar, &QPushButton::setEnabled);
    connect(escenario, &EscenarioJuego::partidaFinalizada, this, &MainWindow::mostrarMensajeVictoria);

    escenario->iniciarPartida();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::configurarInterfazPantallaCompleta() {
    if (ui->menubar) ui->menubar->hide();
    if (ui->statusbar) ui->statusbar->hide();

    // Ocultar contenedores previos del diseñador para evitar solapamientos
    for (QObject *child : ui->centralwidget->children()) {
        if (auto *widget = qobject_cast<QWidget*>(child)) {
            widget->hide();
        }
    }

    // Configurar graphicsView para ocupar toda la pantalla sin bordes ni scrollbars
    ui->graphicsView->setParent(ui->centralwidget);
    ui->graphicsView->setScene(escenario);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->show();

    // Barra flotante superior con los controles del juego
    hudBar = new QWidget(ui->centralwidget);
    hudBar->setObjectName("hudBar");
    hudBar->setStyleSheet(
        "#hudBar { background-color: rgba(25, 35, 45, 215); border-radius: 8px; }"
        "#hudBar QLabel { color: white; font-weight: bold; }"
        );

    auto *hudLayout = new QHBoxLayout(hudBar);
    hudLayout->setContentsMargins(14, 6, 14, 6);
    hudLayout->setSpacing(10);

    auto *txtAngulo = new QLabel("Ángulo (°):", hudBar);
    auto *txtVelocidad = new QLabel("Velocidad (m/s):", hudBar);

    hudLayout->addWidget(ui->lblTurno);
    hudLayout->addStretch(1);
    hudLayout->addWidget(ui->lblEstado);
    hudLayout->addStretch(1);
    hudLayout->addWidget(txtAngulo);
    hudLayout->addWidget(ui->spinAngulo);
    hudLayout->addWidget(txtVelocidad);
    hudLayout->addWidget(ui->spinVelocidad);
    hudLayout->addWidget(ui->btnDisparar);
    hudLayout->addWidget(ui->btnReiniciar);

    ui->lblTurno->show();
    ui->lblEstado->show();
    ui->spinAngulo->show();
    ui->spinVelocidad->show();
    ui->btnDisparar->show();
    ui->btnReiniciar->show();

    hudBar->show();
    hudBar->raise();
}

void MainWindow::ajustarVistaEscenario() {
    if (!escenario || !ui->graphicsView) return;

    int ancho = ui->centralwidget->width();
    int alto = ui->centralwidget->height();

    ui->graphicsView->setGeometry(0, 0, ancho, alto);
    ui->graphicsView->fitInView(escenario->sceneRect(), Qt::IgnoreAspectRatio);

    if (hudBar) {
        hudBar->setGeometry(12, 10, ancho - 24, 46);
        hudBar->raise();
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    ajustarVistaEscenario();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    ajustarVistaEscenario();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (isFullScreen()) {
            showNormal();
        } else {
            close();
        }
    } else if (event->key() == Qt::Key_F11) {
        isFullScreen() ? showNormal() : showFullScreen();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::on_btnDisparar_clicked() {
    escenario->ejecutarDisparo(ui->spinAngulo->value(), ui->spinVelocidad->value());
}

void MainWindow::on_btnReiniciar_clicked() {
    escenario->iniciarPartida();
}

void MainWindow::mostrarMensajeVictoria(const QString &mensaje) {
    QMessageBox::information(this, "Fin de la partida", mensaje);
}