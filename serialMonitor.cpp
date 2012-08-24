#include <QMetaType>
#include <iostream>
#include <QVector>
#include <QString>
#include "serialMonitor.h"
#include "ui_serialMonitor.h"
#include "serial.h"
#include "frame.h"

SerialMonitor::SerialMonitor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialMonitor),
	plotters(3)
{
	ui->setupUi(this);
	serial = new Serial("/dev/ttyUSB0", 9600);

	QVector<QString> names(3);
	names[0] = (tr("x axis"));
	names[1] = (tr("y axis"));
	names[2] = (tr("z axis"));

	plotters[0] = (new Plotter(tr("Gyroscope"), names,this));
	plotters[1] = (new Plotter(tr("Accelerometer"), names,this));
	plotters[2] = (new Plotter(tr("Magnetometer"), names,this));

	for (QVector<Plotter*>::const_iterator i = plotters.constBegin(); i != plotters.constEnd(); ++i)
		ui->mdiArea->addSubWindow(*i);

	qRegisterMetaType<Frame>("Frame");
	connect(serial, SIGNAL(newFrame(Frame)), this, SLOT(newFrame(Frame)));

	serial->start();
}

SerialMonitor::~SerialMonitor(){
	delete ui;
	delete serial;
}

void SerialMonitor::newFrame(Frame frame){
	QVector<double> ys(3);

	ys[0] = frame.gx;
	ys[1] = frame.gy;
	ys[2] = frame.gz;
	plotters[0]->newData(frame.time, ys);

	ys[0] = frame.ax;
	ys[1] = frame.ay;
	ys[2] = frame.az;
	plotters[1]->newData(frame.time, ys);

	ys[0] = frame.mx;
	ys[1] = frame.my;
	ys[2] = frame.mz;
	plotters[2]->newData(frame.time, ys);
}
