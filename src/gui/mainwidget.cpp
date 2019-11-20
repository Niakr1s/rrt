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
  treeView_ = new XMLTreeView();
  dxfLabel_ = new DXFLabel();
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
