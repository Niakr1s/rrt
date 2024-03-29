#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QTranslator>
#include <boost/log/utility/setup/file.hpp>
#include "mainwindow.h"
#include "typedefs.h"

void registerMetaTypes() {
  qRegisterMetaType<DXFResult>("DXFResult");
  qRegisterMetaType<std::shared_ptr<DXFResult>>("std::shared_ptr<DXFResult>");
  qRegisterMetaType<rrt::xmlSpatials_t>("rrt::xmlSpatials_t");
  qRegisterMetaType<QVector<int>>("QVector<int>");
  qRegisterMetaType<QVector<QFileInfo>>("QVector<QFileInfo>");
}

void logToFile() {
  boost::log::add_file_log(boost::log::keywords::file_name = "rrt.log",
                           boost::log::keywords::auto_flush = true);
  boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                      boost::log::trivial::info);
}

int main(int argc, char* argv[]) {
#ifndef COUT_LOG
  logToFile();
#endif
  registerMetaTypes();

  QApplication app(argc, argv);
  QTranslator myappTranslator;
  myappTranslator.load("i10n/ru");

  app.installTranslator(&myappTranslator);

  MainWindow mw;
  mw.show();

  return app.exec();
}
