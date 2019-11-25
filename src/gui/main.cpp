#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"
#include "typedefs.h"

void registerMetaTypes() {
  qRegisterMetaType<DXFResult>("DXFResult");
  qRegisterMetaType<std::shared_ptr<DXFResult>>("std::shared_ptr<DXFResult>");
  qRegisterMetaType<rrt::XML::xmlSpatials_t>("rrt::XML::xmlSpatials_t");
}

int main(int argc, char* argv[]) {
  registerMetaTypes();

  QApplication app(argc, argv);
  QTranslator myappTranslator;
  myappTranslator.load("i10n/ru");

  app.installTranslator(&myappTranslator);

  MainWindow mw;
  mw.show();
  return app.exec();
}
