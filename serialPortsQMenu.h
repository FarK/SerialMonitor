#ifndef SERIAL_PORTS_QMENU_H
#define SERIAL_PORTS_QMENU_H

#include <QMenu>

class serialPortsQMenu : public QMenu
{
	Q_OBJECT

	public:
		serialPortsQMenu(QWidget *parent = 0);

		QString getDefaultPort();

	signals:
		void portChanged(QString);

	private slots:
		void refresh();
		void actionChecked(QAction *action);

	private:
		QString selectedPort;
};

#endif
