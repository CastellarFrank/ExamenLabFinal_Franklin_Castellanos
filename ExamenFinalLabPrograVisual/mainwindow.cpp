#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Procesador de Texto - Industrias Buen Record");
    ui->btnLD->setChecked(true);
    this->ejecutar=false;
    process.setProcessChannelMode(QProcess::MergedChannels);
    path="";
    ui->plainTextEdit->setReadOnly(true);
    this->esconderElementos(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCargarExe_clicked()
{
    this->path=QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath(),
                                              "Archivos ejecutables()");
    if(path==""){
        QMessageBox::warning(this,"Seleccion invalida","No se ha cargado ningun archivo.");
        this->ejecutar=false;
    }else{
        this->ejecutar=true;
    }

}

void MainWindow::on_btnLD_clicked()
{
    ui->btnLD->setChecked(true);
    ui->btnSD->setChecked(false);
}

void MainWindow::on_btnSD_clicked()
{
    ui->btnLD->setChecked(false);
    ui->btnSD->setChecked(true);
}

void MainWindow::on_btnOrigen_clicked()
{
    QString dir=QFileDialog::getOpenFileName(this,"Select a file to read",QDir::currentPath(),
                                              "File for read()");
    ui->txtOrigen->setText(dir);
}

void MainWindow::on_btnDestino_clicked()
{
    QString dir=QFileDialog::getSaveFileName(this,"Select a destination",QDir::currentPath());
    ui->txtDestino->setText(dir);

}

void MainWindow::on_btnEJECUTAR_clicked()
{
    if(!this->ejecutar){
        QMessageBox::warning(this,"Imposible","Debe cargarse un ejecutable primero.");
        return;
    }else if(ui->txtOrigen->text().isEmpty()){
        QMessageBox::warning(this,"Imposible","Debe seleccionar un archivo a leer primero.");
        return;
    }else if(ui->txtDestino->text().isEmpty()){
        QMessageBox::warning(this,"Imposible","Debe seleccionar una ubicacion de destino primero.");
        return;
    }
    QStringList lista;
    QString tipo=(ui->btnLD->isChecked()?"-ld":"-sd");
    lista.append(tipo);
    lista.append(ui->txtOrigen->text());
    lista.append(ui->txtDestino->text());
    this->process.start(this->path,lista);

    if( !process.waitForStarted())
    {
        QMessageBox::warning(this,"Error","El Proceso no pudo comenzar.");
        return;
    }
    if(!process.waitForFinished())
    {
        QMessageBox::warning(this,"Error","El proceso no termino correctamente.");
        return;
    }
    QString resultado=process.readAll();
    ui->plainTextEdit->setPlainText(resultado);
}

void MainWindow::on_btnCargarDatosEstadist_clicked()
{
    QString dir=QFileDialog::getOpenFileName(this,"Select a Data File",QDir::currentPath());
    QFile File(dir);
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Error al leer el archivo","No se pudo leer el archivo seleccionado.");
        return;
    }
    QString texto=File.readAll();
    File.close();
    QStringList partes=texto.split("\n_ | _\n");

    QString tipo=partes.at(0).mid(16);
    if(tipo=="-ld"){
        if(partes.size()!=8){
            this->errorArchivo();
            return;
        }
        this->esconderElementos(true);
        ui->txtTipoArchivo->setText("Long Details");
    }else if(tipo=="-sd"){
        if(partes.size()!=4){
            this->errorArchivo();
            return;
        }
        this->esconderElementos(false);
        ui->txtTipoArchivo->setText("Short Details");
    }else{
        this->errorArchivo();
        return;
    }
    tipo=partes.at(1);
    tipo.remove("\n*Frecuencia de Caracteres: \n\t");
    tipo.remove("\t");
    tipo.replace("Caracter","CHAR");
    tipo.replace(" Frecuencia","\tFREC");
    tipo.replace("CHAR:   \tFREC","CHAR: space FREC");
    ui->txtAllCharacters->setPlainText(tipo);

    tipo=partes.at(2);
    tipo.remove("\n*Caracteres con la menor Frecuencia: \nFrecuencias: ");
    tipo.replace("\t","CHAR: ");
    QString val;
    val=tipo;
    int pos=val.indexOf("\n");
    val.remove(pos,(val.length()-pos));
    val="Menor Frec.("+val+")";
    tipo.remove(0,pos+1);
    tipo.replace("CHAR:  ","CHAR: space");
    ui->lblMenorFrecuencia->setText(val);
    ui->txtMenorFrecuencia->setPlainText(tipo);

    tipo=partes.at(3);
    tipo.remove("\n*Caracteres con la mayor Frecuencia: \nFrecuencias: ");
    if(!ui->gridLayoutWidget->isVisible()){
        tipo.remove("\n\n\n\t CopyRight By Industrias Record\n\n");
    }
    tipo.replace("\t","CHAR: ");
    val=tipo;
    pos=val.indexOf("\n");
    val.remove(pos,(val.length()-pos));
    val="Mayor Frec.("+val+")";
    tipo.remove(0,pos+1);
    tipo.replace("CHAR:  ","CHAR: space");
    ui->lblMayorFrecuencia->setText(val);
    ui->txtMayorFrecuencia->setPlainText(tipo);

    if(ui->gridLayoutWidget->isVisible()){
        tipo=partes.at(4);
        tipo.remove("\n*Cantidad Total de Letras: ");
        ui->txtCantLetras->setText(tipo);

        tipo=partes.at(5);
        tipo.remove("\n*Cantidad Total de Numeros: ");
        ui->txtCantNumeros->setText(tipo);

        tipo=partes.at(6);
        tipo.remove("\n*Cantidad Total de Caracteres Especiales: ");
        ui->txtCaracteresEspeciales->setText(tipo);

        tipo=partes.at(7);
        tipo.remove("\n*Suma Total de los Numeros: ");
        tipo.remove("\n\n\n\t CopyRight By Industrias Record\n\n");
        ui->txtSumaNumeros->setText(tipo);
    }
}
void MainWindow::esconderElementos(bool valor){
    ui->gridLayoutWidget->setVisible(valor);
}
void MainWindow::errorArchivo(){
    QMessageBox::warning(this,"Imposible leer","El archivo seleccionado no es un formato compatible.");
}
