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
  void newDXF(QFileInfo);
  void newXMLs(QVector<QFileInfo>);

 public slots:
  void openXMLs();
  void openDXF();
  void showAbout();

  void resetProgressBar(int size);
  void tickProgressBar(int pos, int max);
  void plusDBConnection();
  void minusDBConnection();

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
