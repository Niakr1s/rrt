#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/icons/rrt.png"));
  setWindowTitle("Rosreestr Tools");

  mainWidget_ = new MainWidget();
  setCentralWidget(mainWidget_);

  initActions();
  createMenuBar();

  connect(this, &MainWindow::newDXFFileSignal, mainWidget_->dxfLabel(),
          &DXFLabel::newDXFFileSignal);
  connect(this, &MainWindow::newXMLFilesSignal, mainWidget_->dxfLabel(),
          &DXFLabel::newXMLFilesSignal);
}

void MainWindow::onActionOpenXmls() {
  QVector<QString> fileName = QFileDialog::getOpenFileNames(
                                  this, tr("Open XMLs"), "", tr("XML (*.xml)"))
                                  .toVector();
  QVector<QFileInfo> res;
  for (auto& f : fileName) {
    res.push_back(QFileInfo(f));
  }
  emit newXMLFilesSignal(res);
}

void MainWindow::onActionOpenDxf() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open DXF"), "", tr("Autocad drawing (*.dxf *.dwg)"));
  emit newDXFFileSignal(QFileInfo(fileName));
}

QToolBar* MainWindow::createTopToolBar() {
  QToolBar* ttb = new QToolBar();
  ttb->addAction(actionOpenXmls_);
  ttb->addAction(actionOpenDxf_);
  ttb->setMovable(false);
  return ttb;
}

void MainWindow::createMenuBar() {
  QMenu* menuFile_ = new QMenu(tr("&File"));
  menuFile_->addAction(actionOpenXmls_);
  menuFile_->addAction(actionOpenDxf_);
  menuFile_->addSeparator();
  menuFile_->addAction(actionExit_);
  menuBar()->addMenu(menuFile_);

  QMenu* menuHelp_ = new QMenu(tr("&Help"));
  menuHelp_->addAction(actionAbout_);
  menuBar()->addMenu(menuHelp_);
}

void MainWindow::initActions() {
  actionOpenXmls_ =
      new QAction(QIcon(":/icons/xml.svg"), tr("Add &XMLs"), this);
  actionOpenDxf_ = new QAction(QIcon(":/icons/dxf.svg"), tr("Open &DXF"), this);
  actionAbout_ = new QAction(QIcon(":/icons/about.svg"), tr("&About"), this);

  actionExit_ = new QAction(QIcon(":/icons/exit.svg"), tr("&Exit"), this);
  connect(actionExit_, &QAction::triggered, this, &QApplication::quit);

  connect(actionOpenXmls_, &QAction::triggered, this,
          &MainWindow::onActionOpenXmls);
  connect(actionOpenDxf_, &QAction::triggered, this,
          &MainWindow::onActionOpenDxf);
}
