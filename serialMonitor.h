#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include <QMainWindow>
#include <QAction>
#include "serialPortsQMenu.h"
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

		void closeEvent(QCloseEvent *event);
	
	public slots:
		void connectSerial();
		void disconnectSerial();
		void setSerialPort(QString serialPort);
		void setBaudRate(int baudRate);
		void readException(QString what);
	
	private slots:
		void newFrame(Frame frame);
		void connectedSerial();
		void disconnectedSerial();
		void menuWindowsAction(int numAction, bool checked);

	private:
		Ui::SerialMonitor *ui;
		Serial serial;
		QVector<QMdiSubWindow*> subWindows;
		QVector<QString> axesNames;
		QString serialPort;
		unsigned int baudRate;
};

#endif
