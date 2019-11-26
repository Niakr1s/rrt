#include "mainwidget.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include <boost/log/trivial.hpp>
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout();
  treeViewBtns_ = new XMLTreeButtons();

  treeView_ = new XMLTreeView();
  dxfLabel_ = new DXFLabel();
  vbox->addWidget(treeViewBtns_);
  vbox->addWidget(treeView_);
  vbox->addWidget(dxfLabel_);
  setLayout(vbox);

  connectAll();
}

void MainWidget::onEndProcessingXMLs(QStringList errXMLPaths) {
  BOOST_LOG_TRIVIAL(debug) << "MainWidget::onEndProcessingXMLs: errors = "
                           << errXMLPaths.size();
  if (!errXMLPaths.empty()) {
    QMessageBox* errXmlMessageBox = new QMessageBox(this);
    errXmlMessageBox->setMinimumWidth(600);
    errXmlMessageBox->setWindowTitle(tr("Couldn't parse some XMLs"));
    errXmlMessageBox->setText(tr("Invalid file names are listed below"));
    errXmlMessageBox->setIcon(QMessageBox::Warning);
    errXmlMessageBox->setDetailedText(errXMLPaths.join("\n"));
    errXmlMessageBox->show();
  }
}

XMLTreeView* MainWidget::treeView() const {
  return treeView_;
}

DXFLabel* MainWidget::dxfLabel() const {
  return dxfLabel_;
}

void MainWidget::connectAll() {
  connect(dxfLabel_, &DXFLabel::newDXFSpatial, treeView_->xmlModel(),
          &XMLTreeModel::getIntersections);

  connect(dxfLabel_, &DXFLabel::dxfClosed, treeView_,
          &XMLTreeView::disableIntersectionsFiltering);
  connect(dxfLabel_, &DXFLabel::dxfClosed, treeViewBtns_,
          &XMLTreeButtons::hideCopyElements);

  connect(dxfLabel_, &DXFLabel::newXMLs, treeView_->xmlModel(),
          &XMLTreeModel::appendXMLs);

  connect(treeView_->xmlModel(), &XMLTreeModel::endProcessingXMLsSignal, this,
          &MainWidget::onEndProcessingXMLs, Qt::QueuedConnection);

  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::onExpandButtonToggled);
  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::onExpandButtonToggled);

  connect(treeView_->xmlModel(), &XMLTreeModel::gotIntersections, dxfLabel_,
          &DXFLabel::onEndProcessingDXFSignal, Qt::QueuedConnection);
  connect(treeView_->xmlModel(), &XMLTreeModel::gotIntersections, treeViewBtns_,
          &XMLTreeButtons::activateCopyElements);
}
