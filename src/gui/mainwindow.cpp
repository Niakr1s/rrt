#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>
#include <boost/log/trivial.hpp>
#include "version.h"
#include "xmltreeview.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  updater_ = new Updater(this);
  updater_->startUpdateQuery();

  setWindowIcon(QIcon(":/icons/rrt.ico"));
  setWindowTitle("Rosreestr Tools v" + QString(PROJECT_VER));

  mainWidget_ = new MainWidget();
  setCentralWidget(mainWidget_);

  initActions();
  createMenuBar();
  initStatusBar();

  connectAll();
}

void MainWindow::openXMLs() {
  QVector<QString> fileName =
      QFileDialog::getOpenFileNames(this, tr("Open XMLs"), "", "XML (*.xml)")
          .toVector();
  QVector<QFileInfo> res;
  for (auto& f : fileName) {
    res.push_back(QFileInfo(f));
  }
  emit newXMLs(res);
}

void MainWindow::openDXF() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open DXF"), "",
                                                  "Autocad drawing (*.dxf)");
  emit newDXF(QFileInfo(fileName));
}

void MainWindow::showAbout() {
  QMessageBox::about(
      this, tr("About"),
      tr(R"***(<div>This program is for Cadastral Engineers.</div>
<div>It can open, rename and export to dxf rosreestr xml files. Besides, it can check intersection of dxf files in them.</div>
<br>
<address>
If you see any bugs, please contact author via <a href="https://github.com/Niakr1s/rrt/issues">github</a>.<br></address>)***"));
}

void MainWindow::resetProgressBar(int size) {
  if (size == 0) {
    return;
  }
  statusBarMessage_->setText(tr("Processing ..."));
  progressBar_->setRange(0, size);
  progressBar_->show();
}

void MainWindow::tickProgressBar(int pos, int max) {
  if (++pos == max) {
    progressBar_->reset();
    progressBar_->hide();
    updateDBIcon();
  } else {
    progressBar_->setValue(pos);
  }
}

void MainWindow::plusDBConnection() {
  ++dbProcesses_;
  updateDBIcon();
}

void MainWindow::minusDBConnection() {
  --dbProcesses_;
  updateDBIcon();
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
}

void MainWindow::initStatusBar() {
  progressBar_ = new QProgressBar();
  progressBar_->setAlignment(Qt::AlignRight);
  progressBar_->setMaximumSize(150, 15);
  progressBar_->setTextVisible(false);
  statusBar()->addPermanentWidget(progressBar_);
  progressBar_->hide();

  dbIconLabel_ = new QLabel();
  dbIconLabel_->setPixmap(QPixmap(":/icons/database.svg")
                              .scaledToHeight(statusBar()->height() / 2));
  statusBar()->addPermanentWidget(dbIconLabel_);
  dbIconLabel_->hide();

  statusBarMessage_ = new QLabel();
  statusBar()->addWidget(statusBarMessage_);
  statusBarMessage_->setText(tr("Initializing..."));
}

void MainWindow::connectAll() {
  connect(this, &MainWindow::newDXF, mainWidget_->dxfLabel(),
          &DXFLabel::openDXF);
  connect(this, &MainWindow::newXMLs, mainWidget_->dxfLabel(),
          &DXFLabel::newXMLs);

  connect(mainWidget_->treeView()->xmlModel(), &XMLTreeModel::startProcessing,
          this, &MainWindow::resetProgressBar);
  connect(mainWidget_->treeView()->xmlModel(), &XMLTreeModel::oneProcessed,
          this, &MainWindow::tickProgressBar);

  connect(mainWidget_->treeView()->xmlModel(), &XMLTreeModel::DBBegin, this,
          &MainWindow::plusDBConnection);
  connect(mainWidget_->treeView()->xmlModel(), &XMLTreeModel::DBEnd, this,
          &MainWindow::minusDBConnection);

  connect(actionExit_, &QAction::triggered, this, &MainWindow::close);

  connect(actionOpenXmls_, &QAction::triggered, this, &MainWindow::openXMLs);
  connect(actionOpenDxf_, &QAction::triggered, this, &MainWindow::openDXF);
  connect(actionAbout_, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::updateDBIcon() {
  if (dbProcesses_ == 0) {
    dbIconLabel_->hide();
    dbIconLabel_->setToolTip(READY);
    statusBarMessage_->setText(READY);
  } else {
    dbIconLabel_->show();
    auto txt = tr("DB: %1 to go").arg(dbProcesses_);
    statusBarMessage_->setText(txt);
    dbIconLabel_->setToolTip(txt);
  }
}

void MainWindow::closeEvent(QCloseEvent* event) {
  if (dbProcesses_ == 0) {
    return QMainWindow::closeEvent(event);
  }
  auto btn = QMessageBox::warning(
      this, tr("Are you sure?"),
      tr("Application is still updating DB, please wait until DB indicator in "
         "toolbar turns off. Closing app will cause DB inconsistence. "
         "Are you still want to exit?"),
      QMessageBox::Button::Ok | QMessageBox::Button::Cancel);
  if (btn == QMessageBox::StandardButton::Ok) {
    return QMainWindow::closeEvent(event);
  }
  event->ignore();
}
