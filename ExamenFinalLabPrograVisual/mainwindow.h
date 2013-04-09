#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool ejecutar;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString path;
    QProcess process;
    void esconderElementos(bool valor);
    void errorArchivo();

private slots:
    void on_btnCargarDatosEstadist_clicked();
    void on_btnEJECUTAR_clicked();
    void on_btnDestino_clicked();
    void on_btnOrigen_clicked();
    void on_btnSD_clicked();
    void on_btnLD_clicked();
    void on_btnCargarExe_clicked();
};

#endif // MAINWINDOW_H
