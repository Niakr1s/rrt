#include "xmltreebuttons.h"

#include <QHBoxLayout>
#include <QPushButton>

XMLTreeButtons::XMLTreeButtons(QWidget* parent) : QWidget(parent) {
  btnExpand_ =
      makeDefaultButton(QIcon(":/icons/expand.svg"), tr(""), tr("Expand"));
  btnCollapse_ =
      makeDefaultButton(QIcon(":/icons/collapse.svg"), tr(""), tr("Collapse"));
  btnCopyNewline_ = makeDefaultButton(QIcon(":/icons/newline.png"), tr(""),
                                      tr("Copy with newline separator"));
  btnCopySemicolon_ = makeDefaultButton(QIcon(":/icons/semicolon.png"), tr(""),
                                        tr("Copy with semicolon separator"));

  auto hbox = new QHBoxLayout();
  hbox->addWidget(btnExpand_);
  hbox->addWidget(btnCollapse_);
  hbox->addStretch();
  hbox->addWidget(btnCopySemicolon_);
  hbox->addWidget(btnCopyNewline_);

  hbox->setMargin(0);

  setLayout(hbox);
}

QPushButton* XMLTreeButtons::btnExpand() const {
  return btnExpand_;
}

QPushButton* XMLTreeButtons::btnCollapse() const {
  return btnCollapse_;
}

QPushButton* XMLTreeButtons::btnCopySemicolon() const {
  return btnCopySemicolon_;
}

QPushButton* XMLTreeButtons::btnCopyNewline() const {
  return btnCopyNewline_;
}

QPushButton* XMLTreeButtons::makeDefaultButton(QIcon icon,
                                               QString str,
                                               QString tooltip) {
  auto res = new QPushButton(icon, str);
  res->setToolTip(tooltip);
  res->setFlat(true);
  return res;
}
