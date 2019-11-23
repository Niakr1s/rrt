#include "mainwidget.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include <boost/log/trivial.hpp>
#include "vecstr.h"
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

void MainWidget::onEndProcessingXMLs(QVector<QString> errXMlPaths) {
  if (!errXMlPaths.empty()) {
    QMessageBox* errXmlMessageBox = new QMessageBox(this);
    errXmlMessageBox->setMinimumWidth(600);
    errXmlMessageBox->setWindowTitle(tr("Couldn't parse some XMLs"));
    errXmlMessageBox->setText(tr("Invalid file names are listed below"));
    errXmlMessageBox->setIcon(QMessageBox::Warning);
    errXmlMessageBox->setDetailedText(
        VecStr<QString>(errXMlPaths).sepByNewLine());
    errXmlMessageBox->show();
  }
}

void MainWidget::onErrDXF(QString errDXFPath) {
  QMessageBox* errXmlMessageBox = new QMessageBox(this);
  errXmlMessageBox->setMinimumWidth(600);
  errXmlMessageBox->setWindowTitle(tr("Couldn't parse DXF"));
  errXmlMessageBox->setText(tr("Invalid file name is listed below"));
  errXmlMessageBox->setIcon(QMessageBox::Critical);
  errXmlMessageBox->setDetailedText(errDXFPath);
  errXmlMessageBox->show();
}

XMLTreeView* MainWidget::treeView() const {
  return treeView_;
}

DXFLabel* MainWidget::dxfLabel() const {
  return dxfLabel_;
}

void MainWidget::connectAll() {
  connect(dxfLabel_, &DXFLabel::newDXFSpatialSignal, treeView_,
          &XMLTreeView::onNewDXFSpatial);

  connect(dxfLabel_, &DXFLabel::dxfCloseSignal, treeView_,
          &XMLTreeView::onDxfClose);
  connect(dxfLabel_, &DXFLabel::dxfCloseSignal, treeViewBtns_,
          &XMLTreeButtons::onDXFClose);

  connect(dxfLabel_, &DXFLabel::newXMLFilesSignal, treeView_,
          &XMLTreeView::onNewXMLFiles);

  connect(treeView_, &XMLTreeView::endProcessingXMLsSignal, this,
          &MainWidget::onEndProcessingXMLs, Qt::QueuedConnection);

  connect(dxfLabel_, &DXFLabel::errDXFSignal, this, &MainWidget::onErrDXF);

  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::onExpandButtonToggled);
  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::onExpandButtonToggled);

  connect(treeView_, &XMLTreeView::endProcessingDXFSignal, dxfLabel_,
          &DXFLabel::onEndProcessingDXFSignal, Qt::QueuedConnection);
  connect(treeView_, &XMLTreeView::endProcessingDXFSignal, treeViewBtns_,
          &XMLTreeButtons::onEndProcessingDXF);
}
