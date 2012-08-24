#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include <QMainWindow>
#include "plotter.h"
#include "serial.h"
#include "frame.h"

namespace Ui{
	class SerialMonitor;
}

class SerialMonitor : public QMainWindow
{
	Q_OBJECT

	public:
		SerialMonitor(QWidget *parent = 0);
		~SerialMonitor();

	private:
		Ui::SerialMonitor *ui;
		Serial *serial;
		QVector<Plotter*> plotters;
	
	private slots:
		void newFrame(Frame frame);
};

#endif
