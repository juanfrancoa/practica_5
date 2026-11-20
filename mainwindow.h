#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QShowEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include "escenariojuego.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btnDisparar_clicked();
    void on_btnReiniciar_clicked();
    void mostrarMensajeVictoria(const QString &mensaje);

private:
    Ui::MainWindow *ui;
    EscenarioJuego *escenario;
    QWidget *hudBar;

    void configurarInterfazPantallaCompleta();
    void ajustarVistaEscenario();
};

#endif // MAINWINDOW_H