#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QTableView>
#include <QTreeView>
#include <QVector>
#include <QWidget>
#include "dxflabel.h"
#include "xmltreebuttons.h"
#include "xmltreeview.h"

class MainWidget : public QWidget {
 public:
  explicit MainWidget(QWidget* parent = nullptr);

  DXFLabel* dxfLabel() const;
  XMLTreeView* treeView() const;

 private:
  XMLTreeButtons* treeViewBtns_;
  XMLTreeView* treeView_;
  DXFLabel* dxfLabel_;

 private:
  void connectAll();
};

#endif  // MAINWIDGET_H
