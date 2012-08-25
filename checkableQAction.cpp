#include "checkableQAction.h"
#include <QObject> 
#include <QAction> 

checkableQAction::checkableQAction(int num, const QString & text, QObject * parent) :
	QAction(text, parent), number(num)
{
	setCheckable(true);
	connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleDetect(bool)));
}

void checkableQAction::toggleDetect(bool checked){
	emit toggledNum(number, checked);
}
