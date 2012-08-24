#ifndef PLOTTER_CORE_H
#define PLOTTER_CORE_H

#include <QtGui>
#include <QWidget>
#include <QString>
#include <QList>
#include "qcustomplot.h"
#include "serial.h"

class PlotterCore : public QCustomPlot
{
	Q_OBJECT

	//MÉTODOS
	public:
		PlotterCore(QWidget *parent = 0);
		~PlotterCore();

		//Añade un nuevo punto (x,y) al gráfico con índice "graphNumber"
		void newData(int graphNumber, double x, double y);

		//Solo para ser llamada una vez. Establece el número de
		//curvas del gráfico
		void setNumOfGraphs(QVector<QString> &names);
	
	private:
		//Devuelve la coordenadas en Y de todas la curvas, para la
		//coordenada X del ratón 
		QVector<double> getYCoords(double xMouse);

	public slots:
		//Establece el autoTrack al valor de bool
		void setAutoTrack(bool);
		//Establece el autoZoom al valor de bool
		void setAutoZoom(bool);

	private slots:
		//Crea el menú del botón derecho
		void contextMenuRequest(QPoint pos);

		//Oculta o muestra el gráfico con índice "graphNumber",
		//según lo establezca el checkbox de la acción que emite la señal
		void actionClicked(int actionNumber, bool checked);

		//Controla los ejes del zoom según el eje seleccionado
		void mouseWheel();

		//Controla los ejes de arrastre según el eje seleccionado
		void mousePress();

		void mouseYCoord(QMouseEvent *event);

	signals:
		//Se envía cuándo cambia el valor de autoTrack
		void autoTrackToggled(bool);
		//Se envía cuándo cambia el valor de autoZoom
		void autoZoomToggled(bool);
		//Se envía cuando el ratón apunta a otras coordenadas
		//Envía una lista con las coordenadas en Y de todas las curvas
		//Si la coordenada Y no existe, devolvera +inf
		void mouseYCoordChanged(QVector<double> yCoords);


	//ATRIBUTOS
	public:
		//Número de curvas del gráfico
		int numOfGraphs;

		//Si es true se sigue el extremo derecho de la curva más alejada
		bool autoTrack;

		//Si es true se calcula el zoom según el máximo y el mínimo
		bool autoZoom;

	private:
		//Menú del botón secundario
		QMenu *popup;

		//Los colores que se asignaran por orden a los primeras curvas
		//que se dibujen
		QVector<QColor> mainColors;

		//Valores máximos y mínimos que toman las curvas en el eje Y
		double maxY, minY;

		QVector<double> *lastYCoords;
		QVector<QCPItemTracer*> *tracers;
};

#endif
