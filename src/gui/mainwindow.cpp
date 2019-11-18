#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowIcon(QIcon(":/icons/rrt.png"));
  setWindowTitle("Rosreestr Tools");

  initActions();

  createMenuBar();

  mainWidget_ = new MainWidget();
  setCentralWidget(mainWidget_);
}

void MainWindow::slotNoImpl() {}

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

  addToolBar(Qt::TopToolBarArea, createTopToolBar());
}

void MainWindow::initActions() {
  actionOpenXmls_ =
      new QAction(QIcon(":/icons/xml.svg"), tr("Add &XMLs"), this);
  actionOpenDxf_ = new QAction(QIcon(":/icons/dxf.svg"), tr("Open &DXF"), this);
  actionAbout_ = new QAction(QIcon(":/icons/about.svg"), tr("&About"), this);

  actionExit_ = new QAction(QIcon(":/icons/exit.svg"), tr("&Exit"), this);
  connect(actionExit_, &QAction::triggered, this, &QApplication::quit);
}
