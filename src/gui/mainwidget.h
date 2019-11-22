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
  Q_OBJECT
 public:
  explicit MainWidget(QWidget* parent = nullptr);

  DXFLabel* dxfLabel() const;
  XMLTreeView* treeView() const;

 signals:

 public slots:
  void onEndProcessingXMLs(QVector<QString> errXMlPaths);
  void onErrDXF(QString errDXFPath);

 private:
  XMLTreeButtons* treeViewBtns_;
  XMLTreeView* treeView_;
  DXFLabel* dxfLabel_;

 private:
  void connectAll();
};

#endif  // MAINWIDGET_H
