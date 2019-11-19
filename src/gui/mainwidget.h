#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLabel>
#include <QTableView>
#include <QTreeView>
#include <QWidget>
#include "dxflabel.h"

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MainWidget(QWidget* parent = nullptr);

 signals:

 public slots:

 private:
  QTreeView* treeView_;
  DXFLabel* dxfLabel_;

 private:
  QTreeView* createTreeView();
  DXFLabel* createDXFLabel();
};

#endif  // MAINWIDGET_H
