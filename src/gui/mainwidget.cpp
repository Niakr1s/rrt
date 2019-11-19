#include "mainwidget.h"

#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout();
  treeView_ = createTreeView();
  dxfLabel_ = createDXFLabel();

  vbox->addWidget(treeView_);
  vbox->addWidget(dxfLabel_);

  setLayout(vbox);
}

QTreeView* MainWidget::createTreeView() {
  QTreeView* tv = new QTreeView();

  XMLTreeModel* model = new XMLTreeModel();

  tv->setModel(model);
  tv->setSortingEnabled(true);
  tv->setEditTriggers(QTableView::NoEditTriggers);
  tv->setMinimumHeight(600);

  auto delegate = new XMLTreeDelegate();
  tv->setItemDelegate(delegate);

  return tv;
}

DXFLabel* MainWidget::createDXFLabel() {
  auto res = new DXFLabel();
  res->setWordWrap(true);
  res->setMinimumHeight(100);
  res->setMinimumWidth(400);
  res->setTextInteractionFlags(Qt::TextSelectableByMouse |
                               Qt::TextSelectableByKeyboard);
  return res;
}
