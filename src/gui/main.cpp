#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QTranslator myappTranslator;
  myappTranslator.load("i10n/ru");

  app.installTranslator(&myappTranslator);

  MainWindow mw;
  mw.show();
  return app.exec();
}
