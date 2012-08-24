#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QList>
#include "plotterCore.h"
#include "ui_plotterDesign.h"

namespace Ui{
	class Plotter;
}

class Plotter : public QWidget
{
	Q_OBJECT

	public:
		Plotter(QString title, QVector<QString> graphsNames, QWidget *parent = 0);
		~Plotter();

		void newData(double x, const QVector<double> ys);

		void setTitle(const QString &title);
		void setXLabel(const QString &label);
		void setYLabel(const QString &label);

	private slots:
		void changeLinesEdit(QVector<double> yCoords);

	private:
		Ui::Plotter *ui;
};

#endif
