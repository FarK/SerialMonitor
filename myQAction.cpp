#include "myQAction.h"
#include <QObject> 
#include <QAction> 

myQAction::myQAction(int num, const QString & text, QObject * parent) :
	QAction(text, parent), number(num)
{
	connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleDetect(bool)));
}

void myQAction::toggleDetect(bool checked){
	emit toggledNum(number, checked);
}
