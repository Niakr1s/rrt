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
  qRegisterMetaType<rrt::XML::xmlSpatials_t>("rrt::XML::xmlSpatials_t");
}

void logToFile() {
  boost::log::add_file_log(boost::log::keywords::file_name = "logs/rrt.log");
  boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                      boost::log::trivial::debug);
}

int main(int argc, char* argv[]) {
  //  logToFile();
  registerMetaTypes();

  QApplication app(argc, argv);
  QTranslator myappTranslator;
  myappTranslator.load("i10n/ru");

  app.installTranslator(&myappTranslator);

  MainWindow mw;
  mw.show();
  return app.exec();
}
