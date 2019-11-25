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
#include "updater.h"
#include "xmltreeview.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  Updater().startUpdateQuery();

  setWindowIcon(QIcon(":/icons/rrt.ico"));
  setWindowTitle("Rosreestr Tools");

  mainWidget_ = new MainWidget();
  setCentralWidget(mainWidget_);

  initActions();
  createMenuBar();
  initStatusBar();

  connectAll();
}

void MainWindow::onActionOpenXmls() {
  QVector<QString> fileName =
      QFileDialog::getOpenFileNames(this, tr("Open XMLs"), "", "XML (*.xml)")
          .toVector();
  QVector<QFileInfo> res;
  for (auto& f : fileName) {
    res.push_back(QFileInfo(f));
  }
  emit newXMLFilesSignal(res);
}

void MainWindow::onActionOpenDxf() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open DXF"), "",
                                                  "Autocad drawing (*.dxf)");
  emit newDXFFileSignal(QFileInfo(fileName));
}

void MainWindow::onActionAbout() {
  QMessageBox::about(
      this, tr("About"),
      tr(R"***(<div>This program is for Cadastral Engineers.</div>
<div>It can open, rename and export to dxf rosreestr xml files. Besides, it can check intersection of dxf files in them.</div>
<br>
<address>
If you see any bugs, please contact author via <a href="https://github.com/Niakr1s/rrt/issues">github</a>.<br></address>)***"));
}

void MainWindow::onStartProcessingXMLs(int size) {
  statusBarMessage_->setText(tr("Processing XMLs ..."));
  progressBar_->setRange(0, size);
  progressBar_->show();
}

void MainWindow::onOneXMLProcessed(int pos, int max) {
  BOOST_LOG_TRIVIAL(debug) << "MainWindow::onOneXMLProcessed: pos = " << pos
                           << ", max = " << max;
  if (++pos == max) {
    progressBar_->reset();
    progressBar_->hide();
    updateDBIcon();
  } else {
    progressBar_->setValue(pos);
  }
}

void MainWindow::onEndProcessingXMLs(QStringList) {
  updateDBIcon();
}

void MainWindow::onDBBeginSignal() {
  ++dbProcesses_;
  updateDBIcon();
}

void MainWindow::onDBEndSignal() {
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
  statusBarMessage_->setText(READY);
}

void MainWindow::connectAll() {
  connect(this, &MainWindow::newDXFFileSignal, mainWidget_->dxfLabel(),
          &DXFLabel::newDXFFileSignal);
  connect(this, &MainWindow::newXMLFilesSignal, mainWidget_->dxfLabel(),
          &DXFLabel::newXMLFilesSignal);

  connect(mainWidget_->treeView(), &XMLTreeView::startProcessingXMLsSignal,
          this, &MainWindow::onStartProcessingXMLs);
  connect(mainWidget_->treeView(), &XMLTreeView::oneXMLProcessedSignal, this,
          &MainWindow::onOneXMLProcessed);
  connect(mainWidget_->treeView(), &XMLTreeView::endProcessingXMLsSignal, this,
          &MainWindow::onEndProcessingXMLs);
  connect(mainWidget_->treeView(), &XMLTreeView::DBBeginSignal, this,
          &MainWindow::onDBBeginSignal);
  connect(mainWidget_->treeView(), &XMLTreeView::DBEndSignal, this,
          &MainWindow::onDBEndSignal);

  connect(actionExit_, &QAction::triggered, this, &QApplication::quit);

  connect(actionOpenXmls_, &QAction::triggered, this,
          &MainWindow::onActionOpenXmls);
  connect(actionOpenDxf_, &QAction::triggered, this,
          &MainWindow::onActionOpenDxf);
  connect(actionAbout_, &QAction::triggered, this, &MainWindow::onActionAbout);
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
  BOOST_LOG_TRIVIAL(debug) << "MainWindow::closeEvent";
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
