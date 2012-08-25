#include "plotterCore.h"
#include <QWidget>
#include <QPen>
#include "checkableQAction.h"
#include <QString>
#include <QList>
#include <QCursor>

PlotterCore::PlotterCore(QWidget *parent) :
	QCustomPlot(parent),
	numOfGraphs(0),
	autoTrack(true),
	autoZoom(true),
	popup(new QMenu(this)),
	mainColors(3),
	maxY(std::numeric_limits<double>::min()),
	minY(std::numeric_limits<double>::max())
{
	//Añadimos los colores con los que se dibujarán las primeras curvas
	mainColors[0] = QColor(Qt::red);
	mainColors[1] = QColor(Qt::darkGreen);
	mainColors[2] = QColor(Qt::blue);
	
	//Activamos las interacciones para mover, hacer zoom y seleccionar los ejes.
	setInteractions(QCustomPlot::iRangeDrag |
			QCustomPlot::iRangeZoom |
			QCustomPlot::iSelectAxes);

	//Establecemos las direcciones en las que se pueden hacer zoom y arrastrar
	setRangeDrag(Qt::Horizontal|Qt::Vertical);
	setRangeZoom(Qt::Horizontal|Qt::Vertical);

	//xAxis->setRange(-8, 8);
	//yAxis->setRange(-300, 300);
	setupFullAxesBox();
	//setTitle(tr("Unnamed"));
	xAxis->setLabel(tr("x Axis"));
	yAxis->setLabel(tr("y Axis"));

	// make bottom and left axes transfer their ranges to top and right axes:
	connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
	connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));

	// connect slots that takes care that when an axis is selected,
	// only that direction can be dragged and zoomed:
	connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(this, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

	// setup policy and connect slot for context menu popup:
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

	connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseYCoord(QMouseEvent*)));
}

PlotterCore::~PlotterCore(){
	delete popup;
	delete lastYCoords;
	delete tracers;
}

void PlotterCore::newData(int graphNumber, double x, double y){
	//Añadimos el nuevo dato
	graph(graphNumber)->addData(x, y);
		
	//Guardamos las últimas coordenadas
	(*lastYCoords)[graphNumber] = y;

	//Si está activado el autoTrack, movemos el gráfico hasta que la curva esté en el centro
	if(autoTrack){
		double rangDiff = x - xAxis->range().center();
		xAxis->moveRange(rangDiff);
	}

	//Si está activado el autoZoom, ajustamos el zoom para abarcar los máximos y los mínimos
	if(autoZoom){
		//Comprobamos si sobrepasan los máximos
		if(y > maxY) maxY = y;
		if(y < minY) minY = y;

		//Establecemos el rango a un 10% más del los límites
		yAxis->setRange(minY - minY*0.1, maxY + maxY*0.1);
	}

	emit mouseYCoordChanged(getYCoords(mapFromGlobal(QCursor::pos()).x()));

	//Redibujamos
	replot();
}

void PlotterCore::contextMenuRequest(QPoint pos)
{
	//Abrimos el menú en la posición dónde se clickea
	popup->popup(mapToGlobal(pos));
}

void PlotterCore::actionClicked(int actionNumber, bool checked){
	//Si la curva está seleccionada la ponemos como visible
	if(checked){
		graph(actionNumber)->setVisible(true);
		tracers->value(actionNumber)->setVisible(true);
	}
	else{
		graph(actionNumber)->setVisible(false);
		tracers->value(actionNumber)->setVisible(false);
	}
}

void PlotterCore::setNumOfGraphs(QVector<QString> &names){
	numOfGraphs = names.size();

	//Inicializamos lastYCoords y tracers
	lastYCoords = new QVector<double>(numOfGraphs);
	tracers = new QVector<QCPItemTracer*>(3);

	//Creamos tantas acciones del menú secuendario como número de curvas haya
	QVector<QString>::const_iterator name = names.constBegin();
	for (int i = 0; name != names.constEnd(); ++name, ++i)
	{
		//Añadimos la curva y establecemos su nombre
		addGraph();
		graph()->setName(*name);

		//Añadimos el tracer
		(*tracers)[i] = new QCPItemTracer(this);
		tracers->value(i)->setGraph(graph());
		addItem(tracers->value(i));
		tracers->value(i)->setInterpolating(true);
		tracers->value(i)->setStyle(QCPItemTracer::tsCircle);
		tracers->value(i)->setSize(7);

		//Establecemos los colores de las primeras curvas y luego les
		//damos un valor aleatorio
		if(i < mainColors.size()){
			graph()->setPen(QPen(mainColors[i]));
			tracers->value(i)->setPen(QPen(mainColors[i]));
			tracers->value(i)->setBrush(mainColors[i]);
		}
		else{
			QColor randColor(rand()%245+10, rand()%245+10, rand()%245+10);

			graph()->setPen(*new QPen(randColor));
			tracers->value(i)->setPen(QPen(randColor));
			tracers->value(i)->setBrush(randColor);
		}


		//Creamos una nueva acción y la añadimos al menú
		checkableQAction *a = new checkableQAction(i,*name,popup);
		a->setChecked(true);

		popup->addAction(a);

		//Conectamos la acción del menú a su correspondiente slot
		connect(a, SIGNAL(toggledNum(int,bool)), this, SLOT(actionClicked(int,bool)));
	}
}

void PlotterCore::mouseWheel()
{
	//Zoom en el eje seleccionado, en los dos ejes si no hay seleccionado ninguno
	if (xAxis->selected().testFlag(QCPAxis::spAxis))
		setRangeZoom(Qt::Horizontal);

	else if (yAxis->selected().testFlag(QCPAxis::spAxis)){
		setRangeZoom(Qt::Vertical);

		//Desabilitamos el auto-zoom y reseteamos los límites
		autoZoom = false;
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::min();
		emit autoZoomToggled(false);
	} 
	else{
		setRangeZoom(Qt::Horizontal|Qt::Vertical);

		//Desabilitamos el auto-zoom y reseteamos los límites
		autoZoom = false;
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::min();
		emit autoZoomToggled(false);
	}
}

void PlotterCore::mousePress(){
	//Arrastre en el eje seleccionado, en los dos ejes si no hay seleccionado ninguno
	if (xAxis->selected().testFlag(QCPAxis::spAxis)){
		setRangeDrag(Qt::Horizontal);

		//Desabilitamos el auto-track
		autoTrack = false;
		emit autoTrackToggled(false);
	}
	else if (yAxis->selected().testFlag(QCPAxis::spAxis)){
		setRangeDrag(Qt::Vertical);

		//Desabilitamos el auto-zoom y reseteamos los límites
		autoZoom = false;
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::min();
		emit autoZoomToggled(false);
	}
	else{
		setRangeDrag(Qt::Horizontal|Qt::Vertical);

		//Desabilitamos el auto-track
		//autoTrack = false;
		//emit autoTrackToggled(false);

		//Desabilitamos el auto-zoom y reseteamos los límites
		autoZoom = false;
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::min();
		emit autoZoomToggled(false);
	}
}

void PlotterCore::setAutoTrack(bool checket){
	autoTrack = checket;
}

void PlotterCore::setAutoZoom(bool checket){
	autoZoom = checket;

	//Si lo estamos desactivando, reseteamos los límites
	if(!checket){
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::min();
	}
}

QVector<double> PlotterCore::getYCoords(double xMouse){
	QVector<double> coords(numOfGraphs);
	double xCoord = round(xAxis->pixelToCoord(xMouse));

	for(int i = 0 ; i < numOfGraphs ; ++i){
		if(graph(i)->data()->contains(xCoord))
			coords[i] = (graph(i)->data()->value(xCoord).value);
		else
			coords[i] = (*lastYCoords)[i];

		tracers->value(i)->setGraphKey(xCoord);
	}


	return coords;
}

void PlotterCore::mouseYCoord(QMouseEvent *event){
	emit mouseYCoordChanged(getYCoords(event->x()));
	replot();
}
