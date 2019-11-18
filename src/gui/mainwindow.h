#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>
#include "mainwidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget* parent = nullptr);

 signals:

 public slots:
  void slotNoImpl();

 private:
  QAction* actionOpenXmls_;
  QAction* actionOpenDxf_;
  QAction* actionAbout_;
  QAction* actionExit_;

  MainWidget* mainWidget_;

 private:
  QToolBar* createTopToolBar();
  void createMenuBar();
  void initActions();
};

#endif  // MAINWINDOW_H
