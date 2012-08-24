#include "plotter.h"
#include "ui_plotterDesign.h"
#include <QVector>
#include <QString>
#include <QList>

Plotter::Plotter(QString title, QVector<QString> graphsNames, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Plotter)
{
	//Establecemos el diseño
	ui->setupUi(this);
	setWindowTitle(title);

	//Definimos el número de curvas que tendrá el gráfico pasándole los
	//nombres de cada una
	ui->plotterCore->setNumOfGraphs(graphsNames);

	//Creamos un lineEdit con su label por cada curva del gráfico
	QVector<QString>::const_iterator name = graphsNames.constBegin();
	for (int i = 0; name != graphsNames.constEnd(); ++name, ++i){
		QLabel *label = new QLabel(ui->scrollAreaContents);

		//label->setObjectName(*name);
		label->setText(*name);
		ui->formLayout->setWidget(i, QFormLayout::LabelRole, label);

		QLineEdit *lineEdit = new QLineEdit(ui->scrollAreaContents);
		//lineEdit->setObjectName(*name);
		lineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		lineEdit->setText("??");
		lineEdit->setReadOnly(true);
		ui->formLayout->setWidget(i, QFormLayout::FieldRole, lineEdit);
	}

	//Establece el valor de los line edit
	connect(ui->plotterCore, SIGNAL(mouseYCoordChanged(QVector<double>)),
		this, SLOT(changeLinesEdit(QVector<double>)));
}

Plotter::~Plotter(){
	delete ui;
}

void Plotter::newData(double x, const QVector<double> ys){
	//Mandamos los nuevos datos a plotterCore
	for(int i = 0 ; i < ui->plotterCore->numOfGraphs ; ++i)
		ui->plotterCore->newData(i, x, ys[i]);
}

void Plotter::changeLinesEdit(QVector<double> yCoords){
	//Recorremos los items del fromLayout cambiando el texto de sus
	//lineEdit por las coordenadas correspondientes
	QVector<double>::const_iterator yCoord = yCoords.constBegin();
	for(int i = 0 ; yCoord != yCoords.constEnd(); ++yCoord, ++i)
		((QLineEdit*)ui->formLayout->itemAt(i,QFormLayout::FieldRole)->widget())
			->setText(QString::number(*yCoord));
}
