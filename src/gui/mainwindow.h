#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QFileInfo>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QStringList>
#include <QTableView>
#include <QToolBar>
#include <atomic>
#include "mainwidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget* parent = nullptr);

  const QString READY = tr("Ready");

 signals:
  void newDXFFileSignal(QFileInfo);
  void newXMLFilesSignal(QVector<QFileInfo>);

 public slots:
  void onActionOpenXmls();
  void onActionOpenDxf();
  void onActionAbout();

  void onStartProcessing(int size);
  void onOneProcessed(int pos, int max);
  void onEndProcessingXMLs(QStringList);
  void onDBBeginSignal();
  void onDBEndSignal();

 private:
  QAction* actionOpenXmls_;
  QAction* actionOpenDxf_;
  QAction* actionAbout_;
  QAction* actionExit_;
  MainWidget* mainWidget_;
  QLabel* statusBarMessage_;
  QProgressBar* progressBar_;
  QLabel* dbIconLabel_;
  std::atomic<int> dbProcesses_ = 0;

 private:
  QToolBar* createTopToolBar();
  void createMenuBar();
  void initActions();
  void initStatusBar();
  void connectAll();
  void updateDBIcon();

  // QWidget interface
 protected:
  void closeEvent(QCloseEvent* event) override;
};

#endif  // MAINWINDOW_H
