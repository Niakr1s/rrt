#include "mainwidget.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
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

  connect(dxfLabel_, &DXFLabel::newDXFSpatialSignal, treeView_,
          &XMLTreeView::onNewDXFSpatial);

  connect(dxfLabel_, &DXFLabel::dxfCloseSignal, treeView_,
          &XMLTreeView::onDxfClose);

  connect(dxfLabel_, &DXFLabel::newXMLFilesSignal, treeView_,
          &XMLTreeView::onNewXMLFiles);

  connect(treeView_, &XMLTreeView::errXMLsSignal, this, &MainWidget::onErrXMLs);
  connect(dxfLabel_, &DXFLabel::errDXFSignal, this, &MainWidget::onErrDXF);

  connect(treeViewBtns_->btnExpand(), &QPushButton::clicked, treeView_,
          &XMLTreeView::expandAll);
  connect(treeViewBtns_->btnCollapse(), &QPushButton::clicked, treeView_,
          &XMLTreeView::collapseAll);
}

void MainWidget::onErrXMLs(QVector<QString> errXMlPaths) {
  QMessageBox* errXmlMessageBox = new QMessageBox(this);
  errXmlMessageBox->setMinimumWidth(600);
  errXmlMessageBox->setWindowTitle(tr("Couldn't parse some XMLs"));
  errXmlMessageBox->setText(tr("Invalid file names are listed below"));
  errXmlMessageBox->setIcon(QMessageBox::Warning);
  errXmlMessageBox->setDetailedText(VecStr(errXMlPaths).sepByNewLine());
  errXmlMessageBox->show();
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
