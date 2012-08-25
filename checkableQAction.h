#ifndef CHECKABLE_QACTION_H
#define CHECKABLE_QACTION_H

#include <QObject> 
#include <QAction> 

class checkableQAction : public QAction{
	Q_OBJECT

	public:
		checkableQAction(int number, const QString & text, QObject * parent);

		int number;
	
	private slots:
		void toggleDetect(bool checked);

	signals:
		void toggledNum(int number, bool checked);
};

#endif
