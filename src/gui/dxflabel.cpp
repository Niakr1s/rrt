#include "dxflabel.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QUrl>
#include <boost/log/trivial.hpp>
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
  QVector<QFileInfo> xmlFiles;
  for (auto& url : event->mimeData()->urls()) {
    if (url.isLocalFile()) {
      QFileInfo fi(url.toLocalFile());
      if (fi.suffix().toLower() == "dxf") {
        emit newDXFFileSignal(fi);
        return;
      } else if (fi.suffix().toLower() == "xml") {
        xmlFiles.push_back(fi);
      }
    }
  }
  if (!xmlFiles.empty()) {
    emit newXMLFilesSignal(xmlFiles);
  }
}

void DXFLabel::setDefaultText() {
  setText(
      QString("<div>%1</div><div>%2</div><div>%3</div>")
          .arg(tr("Drag your DXF or XMLs here."))
          .arg(tr("DXF will be checked in all top window XMLs."))
          .arg(tr("XMLs will be copied in %1/data.").arg(QDir::currentPath())));
}

void DXFLabel::onDxfClose() {
  dxfFilePath_.clear();
  spatial_ = nullptr;
  setDefaultText();
}

void DXFLabel::onNewDXFFile(const QFileInfo& fi) {
  rrt::DXF dxf;
  try {
    dxf.fileImport(fi.filePath().toStdString());
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << "DXFLabel::onNewDXFFile: " << e.what();
    return;
  }
  dxfFilePath_ = fi.filePath();
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
}

std::shared_ptr<rrt::Spatial> DXFLabel::spatial() const {
  return spatial_;
}

QString DXFLabel::dxfFilePath() const {
  return dxfFilePath_;
}
