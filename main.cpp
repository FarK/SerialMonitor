#include <QtGui/QApplication>
#include "serialMonitor.h"

int main(int argc, char *argv[])
{
  QApplication::setGraphicsSystem("raster");
  QApplication a(argc, argv);
  SerialMonitor sm;
  sm.show();
  
  return a.exec();
}
