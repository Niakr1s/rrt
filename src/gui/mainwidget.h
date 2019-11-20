#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLabel>
#include <QString>
#include <QTableView>
#include <QTreeView>
#include <QVector>
#include <QWidget>
#include "dxflabel.h"
#include "xmltreeview.h"

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MainWidget(QWidget* parent = nullptr);

 signals:

 public slots:
  void onErrXMLs(QVector<QString> errXMlPaths);

 private:
  XMLTreeView* treeView_;
  DXFLabel* dxfLabel_;
};

#endif  // MAINWIDGET_H
