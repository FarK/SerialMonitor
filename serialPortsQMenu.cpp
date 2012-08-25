#include "serialPortsQMenu.h"
#include <iostream>
#include <QDir>

serialPortsQMenu::serialPortsQMenu(QWidget *parent) :
	QMenu(parent)
{
	refresh();
	connect(this, SIGNAL(aboutToShow()), this, SLOT(refresh()));
	connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionChecked(QAction*)));
}

QString serialPortsQMenu::getDefaultPort(){
	refresh();
	if(actions().isEmpty())
		return "";
	else{
		actions().first()->setChecked(true);
		selectedPort = actions().first()->data().toString();
		return actions().first()->data().toString();
	}
}

void serialPortsQMenu::refresh(){
	//Eliminamos las acciones del menú anterior
	clear();

	//Creamos el directorio con el filtro adecuado
	QDir dir("/dev");
	QStringList nameFilters;
	nameFilters << "ttyUSB*" << "ttyACM*";
	dir.setNameFilters(nameFilters);
	dir.setFilter(QDir::System);

	//Recorremos el listado de ficheros y creamos las nuevas acciones y las
	//añadimos al menu
	QStringList files = dir.entryList();
	QStringList::const_iterator file = files.begin();
	for(; file != files.end() ; ++file){
		QAction *action = new QAction(*file, this);
		action->setData(dir.filePath(*file));
		action->setCheckable(true);
		if(*file == selectedPort)
			action->setChecked(true);
		addAction(action);
	}
}

void serialPortsQMenu::actionChecked(QAction *checkedAction){
	QList<QAction*>::Iterator action = actions().begin();
	for(; action != actions().end() ; ++action)
		(*action)->setChecked(false);

	checkedAction->setChecked(true);
	selectedPort = 	checkedAction->data().toString();

	emit portChanged(selectedPort);
}
