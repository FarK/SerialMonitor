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

	public slots:
		void connectSerial();
		void disconnectSerial();
		void setSerialPort(const QString &serialPort);
		void setBaudRate(int baudRate);
	
	private slots:
		void newFrame(Frame frame);
		void connectedSerial();
		void disconnectedSerial();

	private:
		Ui::SerialMonitor *ui;
		Serial serial;
		QVector<QMdiSubWindow*> subWindows;
		QVector<QString> axesNames;
		QString serialPort;
		int baudRate;
};

#endif
