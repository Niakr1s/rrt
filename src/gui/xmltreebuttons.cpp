#include "xmltreebuttons.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include "typedefs.h"

XMLTreeButtons::XMLTreeButtons(QWidget* parent)
    : QWidget(parent), result_(nullptr) {
  btnExpand_ = makeDefaultButton(QIcon(":/icons/expand.svg"), tr(""),
                                 tr("Expand/Collapse"));
  btnExpand_->setCheckable(true);

  comboBox_ = makeComboBox();

  btnCopyNewline_ = makeDefaultButton(QIcon(":/icons/newline.png"), tr(""),
                                      tr("Copy with newline separator"), true);
  btnCopySemicolon_ =
      makeDefaultButton(QIcon(":/icons/semicolon.png"), tr(""),
                        tr("Copy with semicolon separator"), true);

  connectAll();

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

void XMLTreeButtons::onEndProcessingDXF(std::shared_ptr<DXFResult> res) {
  result_ = res;
  propagateComboBox();
  comboBox_->show();
  btnCopySemicolon_->show();
  btnCopyNewline_->show();
}

void XMLTreeButtons::onBtnCopySemicolonClicked() {
  resToClipboard(comboBox_->currentText(), ";");
}

void XMLTreeButtons::onBtnCopyNewlineClicked() {
  resToClipboard(comboBox_->currentText(), "\n");
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

void XMLTreeButtons::resToClipboard(QString key, QString sep) {
  if (result_ == nullptr) {
    return;
  }
  QString toClipboard;
  if (key == ALL_STRING) {
    QStringList tmp;
    for (auto& k : result_->keys()) {
      tmp.push_back((*result_)[k].join(sep));
    }
    toClipboard = tmp.join(sep);
  } else {
    toClipboard = (*result_)[key].join(sep);
  }
  QGuiApplication::clipboard()->setText(toClipboard);
}

void XMLTreeButtons::propagateComboBox() {
  comboBox_->clear();
  comboBox_->addItem(ALL_STRING);
  if (result_->empty()) {
    return;
  }
  for (auto& key : result_->keys()) {
    comboBox_->addItem(key);
  }
}

void XMLTreeButtons::connectAll() {
  connect(btnCopySemicolon_, &QPushButton::clicked, this,
          &XMLTreeButtons::onBtnCopySemicolonClicked);
  connect(btnCopyNewline_, &QPushButton::clicked, this,
          &XMLTreeButtons::onBtnCopyNewlineClicked);
}
