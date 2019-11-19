#include "dxflabel.h"

#include <QDebug>
#include <QErrorMessage>
#include <QFileInfo>
#include <QList>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QUrl>
#include "dxf.h"

DXFLabel::DXFLabel(QWidget* parent) : QLabel(parent) {
  setAcceptDrops(true);
  setDefaultText();
  setAlignment(Qt::AlignCenter);
  setWordWrap(true);
  setMinimumHeight(100);
  setMinimumWidth(400);
  setTextInteractionFlags(Qt::TextSelectableByMouse |
                          Qt::TextSelectableByKeyboard);

  connect(this, &DXFLabel::dxfCloseSignal, this, &DXFLabel::onDxfClose);
  connect(this, &DXFLabel::newDXFFileSignal, this, &DXFLabel::onNewDXFFile);
}

void DXFLabel::paintEvent(QPaintEvent* event) {
  QLabel::paintEvent(event);

  if (!dxfFilePath_.isEmpty()) {
    QPainter draw(this);
    closeButtonRect_ = QRect(rect().right() - 20, rect().top(), 20, 20);
    draw.drawImage(closeButtonRect_, QImage(":/icons/close.svg"));
  }
}

void DXFLabel::mousePressEvent(QMouseEvent* event) {
  if (!dxfFilePath_.isEmpty() && closeButtonRect_.contains(event->pos())) {
    emit dxfCloseSignal();
    event->accept();
  }
  QLabel::mousePressEvent(event);
}

void DXFLabel::dragEnterEvent(QDragEnterEvent* event) {
  if (event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void DXFLabel::dropEvent(QDropEvent* event) {
  for (auto& url : event->mimeData()->urls()) {
    if (url.isLocalFile()) {
      QFileInfo fi(url.toLocalFile());
      if (fi.suffix().toLower() == "dxf" || fi.suffix().toLower() == "dwg") {
        emit newDXFFileSignal(fi);
        return;
      }
    }
  }
}

void DXFLabel::setDefaultText() {
  setText(tr("Drag your DXF file here"));
}

void DXFLabel::onDxfClose() {
  dxfFilePath_.clear();
  spatial_ = nullptr;
  setDefaultText();
}

void DXFLabel::onNewDXFFile(const QFileInfo& fi) {
  try {
    rrt::DXF dxf;
    dxfFilePath_ = fi.filePath();
    dxf.fileImport(dxfFilePath_.toStdString());
    spatial_ = dxf.spatial();
    emit newDXFSpatialSignal(spatial_);
    QString newText;
    newText = QString("<div>%1: \"%2\"</div>")
                  .arg(tr("Current file"))
                  .arg(dxfFilePath_);
    if (spatial_->empty()) {
      newText += QString("<br><font color=\"Red\">%1</font>")
                     .arg(tr("Warninig: This file is empty"));
    }
    setText(newText);
  } catch (std::exception& e) {
    auto errMsg = new QErrorMessage(this);
    errMsg->showMessage(e.what());
    errMsg->show();
  }
}

std::shared_ptr<rrt::Spatial> DXFLabel::spatial() const {
  return spatial_;
}

QString DXFLabel::dxfFilePath() const {
  return dxfFilePath_;
}
