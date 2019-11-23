#include "xmltreebuttons.h"

#include <QHBoxLayout>
#include <QPushButton>

XMLTreeButtons::XMLTreeButtons(QWidget* parent) : QWidget(parent) {
  btnExpand_ = makeDefaultButton(QIcon(":/icons/expand.svg"), tr(""),
                                 tr("Expand/Collapse"));
  btnExpand_->setCheckable(true);

  comboBox_ = makeComboBox();

  btnCopyNewline_ = makeDefaultButton(QIcon(":/icons/newline.png"), tr(""),
                                      tr("Copy with newline separator"), true);
  btnCopySemicolon_ =
      makeDefaultButton(QIcon(":/icons/semicolon.png"), tr(""),
                        tr("Copy with semicolon separator"), true);

  auto hbox = new QHBoxLayout();
  hbox->addWidget(btnExpand_);
  hbox->addStretch();
  hbox->addWidget(comboBox_);
  hbox->addWidget(btnCopySemicolon_);
  hbox->addWidget(btnCopyNewline_);

  hbox->setMargin(0);

  setLayout(hbox);
}

QPushButton* XMLTreeButtons::btnExpand() const {
  return btnExpand_;
}

QPushButton* XMLTreeButtons::btnCopySemicolon() const {
  return btnCopySemicolon_;
}

QPushButton* XMLTreeButtons::btnCopyNewline() const {
  return btnCopyNewline_;
}

QComboBox* XMLTreeButtons::comboBox() const {
  return comboBox_;
}

void XMLTreeButtons::onDXFClose() {
  comboBox_->hide();
  btnCopySemicolon_->hide();
  btnCopyNewline_->hide();
}

void XMLTreeButtons::onEndProcessingDXF(int) {
  // TODO propagate combobox
  btnCopySemicolon_->show();
  btnCopyNewline_->show();
}

QPushButton* XMLTreeButtons::makeDefaultButton(QIcon icon,
                                               QString str,
                                               QString tooltip,
                                               bool hide) {
  auto res = new QPushButton(icon, str);
  res->setToolTip(tooltip);
  res->setFlat(true);
  if (hide) {
    res->hide();
  }
  return res;
}

QComboBox* XMLTreeButtons::makeComboBox() {
  auto res = new QComboBox();
  res->hide();
  return res;
}
