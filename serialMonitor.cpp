#include <QMetaType>
#include <iostream>
#include <QList>
#include <QString>
#include "serialMonitor.h"
#include "ui_serialMonitor.h"
#include "serial.h"
#include "frame.h"

SerialMonitor::SerialMonitor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SerialMonitor),
	plotters()
{
	ui->setupUi(this);
	serial = new Serial("/dev/ttyUSB0", 9600);

	QList<QString> names;
	names.append(tr("x axis"));
	names.append(tr("y axis"));
	names.append(tr("z axis"));

	plotters.append(new Plotter(tr("Gyroscope"), names,this));
	plotters.append(new Plotter(tr("Accelerometer"), names,this));
	plotters.append(new Plotter(tr("Magnetometer"), names,this));

	for (QList<Plotter*>::const_iterator i = plotters.constBegin(); i != plotters.constEnd(); ++i)
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
