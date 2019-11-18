#include "mainwidget.h"

#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include "xmltreemodel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout();
  treeView_ = createTreeView();
  resultLabel_ = createresultLabel();

  vbox->addWidget(treeView_);
  vbox->addWidget(resultLabel_);

  setLayout(vbox);
}

QTreeView* MainWidget::createTreeView() {
  QTreeView* tv = new QTreeView();

  XMLTreeModel* model = new XMLTreeModel();

  tv->setModel(model);
  tv->setSortingEnabled(true);
  tv->setEditTriggers(QTableView::NoEditTriggers);

  tv->setMinimumHeight(600);
  tv->setMinimumHeight(400);
  return tv;
}

QLabel* MainWidget::createresultLabel() {
  auto res = new QLabel(tr("<center>Drag your DXF file here</center>"));
  res->setWordWrap(true);
  res->setMinimumHeight(100);
  res->setMinimumWidth(400);
  res->setTextInteractionFlags(Qt::TextSelectableByMouse |
                               Qt::TextSelectableByKeyboard);
  return res;
}
