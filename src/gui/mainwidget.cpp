#include "mainwidget.h"

#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout();
  treeView_ = new XMLTreeView();
  dxfLabel_ = new DXFLabel();
  vbox->addWidget(treeView_);
  vbox->addWidget(dxfLabel_);
  setLayout(vbox);

  connect(dxfLabel_, &DXFLabel::newDXFSpatialSignal, treeView_,
          &XMLTreeView::onNewDXFSpatial);

  connect(dxfLabel_, &DXFLabel::dxfCloseSignal, treeView_,
          &XMLTreeView::onDxfClose);
}
