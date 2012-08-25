#include <QMetaType>
#include <iostream>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <boost/asio.hpp>
#include <boost/exception/all.hpp>
#include "checkableQAction.h"
#include "serialMonitor.h"
#include "ui_serialMonitor.h"
#include "serial.h"
#include "frame.h"
#include "serialPortsQMenu.h"

SerialMonitor::SerialMonitor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialMonitor),
	subWindows(3),
	axesNames(3),
	baudRate(9600)
{
	ui->setupUi(this);

	//Establecemos el primer puerto encontrado como puerto de conexión
	setSerialPort(ui->menuSerialPorts->getDefaultPort());

	//El QtDesigner no la crea automáticamente
	connect(ui->menuSerialPorts, SIGNAL(portChanged(QString)), this, SLOT(setSerialPort(QString)));

	//Inicializamos los nombres de los ejes de cada curva
	axesNames[0] = (tr("x axis"));
	axesNames[1] = (tr("y axis"));
	axesNames[2] = (tr("z axis"));

	//Creamos las sub-ventanas del mdi
	subWindows[0] = new QMdiSubWindow;
	subWindows[1] = new QMdiSubWindow;
	subWindows[2] = new QMdiSubWindow;

	//Creamos las gráficas
	subWindows[0]->setWidget(new Plotter(tr("Gyroscope"), axesNames,this));
	subWindows[1]->setWidget(new Plotter(tr("Accelerometer"), axesNames,this));
	subWindows[2]->setWidget(new Plotter(tr("Magnetometer"), axesNames,this));
	//TODO: No se pueden cerrar
	//Desabilitamos cerrar, junto con minimizar y maximizar
	//subWindows[0]->setWindowFlags(Qt::CustomizeWindowHint);
	//subWindows[1]->setWindowFlags(Qt::CustomizeWindowHint);
	//subWindows[2]->setWindowFlags(Qt::CustomizeWindowHint);

	//Añadimos las gráficas al mdi y al menu
	for(int i = 0 ; i < subWindows.size() ; ++i){
		ui->mdiArea->addSubWindow(subWindows[i]);

		//Creamos la acción y la añadimos al menu
		checkableQAction *a = new checkableQAction(i, subWindows[i]->windowTitle(), this);
		a->setChecked(true);
		//Conectamos la señal de la acción
		connect(a, SIGNAL(toggledNum(int,bool)), this, SLOT(menuWindowsAction(int,bool)));
		ui->menuWindows->addAction(a);

		subWindows[i]->show();
	}

	//Conectamos las señales del hilo de Serial
	connect(&serial, SIGNAL(started()), this, SLOT(connectedSerial()));
	connect(&serial, SIGNAL(finished()), this, SLOT(disconnectedSerial()));
	connect(&serial, SIGNAL(terminated()), this, SLOT(disconnectedSerial()));
	connect(&serial, SIGNAL(readException(QString)), this, SLOT(readException(QString)));
	//Conectamos la señal de nueva trama para recoger las tramas
	qRegisterMetaType<Frame>("Frame");
	connect(&serial, SIGNAL(newFrame(Frame)), this, SLOT(newFrame(Frame)));
}

SerialMonitor::~SerialMonitor(){
	delete ui;
}

//Desconectamos el puerto serie antes de salir
void SerialMonitor::closeEvent(QCloseEvent *event){
	serial.disconnect();
	while(serial.isRunning());
	event->accept();
}

void SerialMonitor::connectSerial(){
	//Conectando
	ui->connectButton->setDisabled(true);
	ui->connectButton->setText(tr("Connecting..."));

	//Intentamos conectar
	try{
		serial.connect(serialPort.toStdString(), baudRate);
	}
	catch(std::exception &e){
		QMessageBox(QMessageBox::Warning,
				tr("unable to connect to port %1").arg(serialPort),
				e.what(),
				QMessageBox::Ok
			   ).exec();
	}
	catch(...){
		QMessageBox(QMessageBox::Warning,
				tr("unable to connect to port %1").arg(serialPort),
				tr("unknown error"),
				QMessageBox::Ok
			   ).exec();
	}

	if(serial.port.is_open()){
		try{
			serial.start();
		}
		catch(...){
			std::cout << "Desconectado" << std::endl;
		}
	}
	else{
		//Desconectado
		ui->connectButton->setEnabled(true);
		setSerialPort(serialPort);
	}
}

void SerialMonitor::disconnectSerial(){
	serial.disconnect();
	while(serial.isRunning());
}

void SerialMonitor::setSerialPort(QString sPort){
	serialPort = sPort;

	if(sPort == ""){
		ui->connectButton->setText(tr("No port"));
		ui->connectButton->setDisabled(true);
	}
	else{
		ui->connectButton->setText(tr("Connect to %1").arg(serialPort));
		ui->connectButton->setEnabled(true);
	}
}

void SerialMonitor::setBaudRate(int bRate){
	baudRate = bRate;
}

void SerialMonitor::readException(QString what){
	disconnectSerial();
	setSerialPort(ui->menuSerialPorts->getDefaultPort());

	QMessageBox(QMessageBox::Critical,
			tr("Read error"),
			what,
			QMessageBox::Ok
		   ).exec();
}

void SerialMonitor::newFrame(Frame frame){
	QVector<double> ys(3);

	ys[0] = frame.gx;
	ys[1] = frame.gy;
	ys[2] = frame.gz;
	((Plotter*)(subWindows[0]->widget()))->newData(frame.time, ys);

	ys[0] = frame.ax;
	ys[1] = frame.ay;
	ys[2] = frame.az;
	((Plotter*)(subWindows[1]->widget()))->newData(frame.time, ys);

	ys[0] = frame.mx;
	ys[1] = frame.my;
	ys[2] = frame.mz;
	((Plotter*)(subWindows[2]->widget()))->newData(frame.time, ys);
}

void SerialMonitor::connectedSerial(){
	//Conectado
	disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectSerial()));
	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectSerial()));

	ui->connectButton->setEnabled(true);
	ui->connectButton->setText(tr("Disconnect"));
}

void SerialMonitor::disconnectedSerial(){
	//Desconectado
	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectSerial()));
	disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectSerial()));

	setSerialPort(serialPort);
}

void SerialMonitor::menuWindowsAction(int numAction, bool checked){
	subWindows[numAction]->setVisible(checked);
	if(checked)
		subWindows[numAction]->widget()->show();
}
