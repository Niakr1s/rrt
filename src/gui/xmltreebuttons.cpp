#include "xmltreebuttons.h"

#include <QHBoxLayout>
#include <QPushButton>

XMLTreeButtons::XMLTreeButtons(QWidget* parent) : QWidget(parent) {
  btnExpand_ = new QPushButton(QIcon(":/icons/expand.svg"), tr(""));
  btnCollapse_ = new QPushButton(QIcon(":/icons/collapse.svg"), tr(""));

  btnExpand_->setToolTip(tr("Expand"));
  btnCollapse_->setToolTip(tr("Collapse"));
  btnExpand_->setFlat(true);
  btnCollapse_->setFlat(true);

  auto hbox = new QHBoxLayout();
  hbox->addWidget(btnExpand_);
  hbox->addWidget(btnCollapse_);
  hbox->addStretch();
  hbox->setMargin(0);

  setLayout(hbox);
}

QPushButton* XMLTreeButtons::btnExpand() const {
  return btnExpand_;
}

QPushButton* XMLTreeButtons::btnCollapse() const {
  return btnCollapse_;
}
