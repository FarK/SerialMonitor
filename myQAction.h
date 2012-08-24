#ifndef MYQACTION_H
#define MYQACTION_H

#include <QObject> 
#include <QAction> 

class myQAction : public QAction{
	Q_OBJECT

	public:
		myQAction(int number, const QString & text, QObject * parent);

		int number;
	
	private slots:
		void toggleDetect(bool checked);

	signals:
		void toggledNum(int number, bool checked);
};

#endif
