#include "plotter.h"
#include "ui_plotterDesign.h"
#include <QVector>
#include <QString>
#include <QList>

Plotter::Plotter(QString title, QList<QString> graphsNames, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Plotter)
{
	ui->setupUi(this);
	setWindowTitle(title);
	ui->plotterCore->setNumOfGraphs(graphsNames);

	QList<QString>::const_iterator name = graphsNames.constBegin();
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
}

Plotter::~Plotter(){
	delete ui;
}

void Plotter::newData(double x, const QVector<double> ys){
	for(int i = 0 ; i < ui->plotterCore->numOfGraphs ; ++i){
		ui->plotterCore->newData(i, x, ys[i]);

		((QLineEdit*)ui->formLayout-> itemAt(i,QFormLayout::FieldRole)-> widget())
			->setText(QString::number(ys[i]));
	}
}
