#include "dxflabel.h"

#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QUrl>
#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include "dxf.h"
#include "rrtzip.h"

namespace bf = boost::filesystem;

DXFLabel::DXFLabel(QWidget* parent) : QLabel(parent) {
  setAcceptDrops(true);
  setDefaultText();
  setAlignment(Qt::AlignCenter);
  setWordWrap(true);
  setMinimumHeight(100);
  setMinimumWidth(400);
  setTextInteractionFlags(Qt::TextSelectableByMouse |
                          Qt::TextSelectableByKeyboard);

  connectAll();
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
    emit dxfClosed();
    closeDXF();
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
  QFileInfo dxfFile;
  bool dxfFileFound = false;
  for (auto& url : event->mimeData()->urls()) {
    if (url.isLocalFile()) {
      QFileInfo fi(url.toLocalFile());
      if (!dxfFileFound && fi.suffix().toLower() == "dxf") {
        dxfFileFound = true;
        dxfFile = fi;
      } else if (fi.suffix().toLower() == "xml") {
        xmlFiles.push_back(fi);
      } else if (fi.suffix().toLower() == "zip") {
        QString zipPathQstr = fi.filePath();
        rrt::Zip zip(bf::path(zipPathQstr.toStdWString()));
        rrt::Zip::paths_t paths;
        zip.extractFromZip(".xml", paths);
        for (auto& zipxml : paths) {
          QString zipqstr = QString::fromStdWString(zipxml.wstring());
          QFileInfo zipfi(zipqstr);
          xmlFiles.push_back(zipfi);
        }
      }
    }
  }
  if (!xmlFiles.empty()) {
    emit newXMLs(xmlFiles);
  } else if (dxfFileFound) {
    openDXF(dxfFile);
  }
}

void DXFLabel::setDefaultText() {
  setText(
      QString("<div>%1</div><div>%2</div><div>%3</div><div>%4</div>")
          .arg(tr("Drag your DXF or XMLs/ZIPs here."))
          .arg(tr("DXF will be checked in all top window XMLs."))
          .arg(tr("All blocks in DXF will be ignored. Everything in %1 layer "
                  "will be ignored. Any non-english layer will be ignored.")
                   .arg(QString(rrt::DXF::IGNORED)))
          .arg(tr("XMLs will be copied in %1/data.").arg(QDir::currentPath())));
}

void DXFLabel::closeDXF() {
  dxfFilePath_.clear();
  spatial_ = nullptr;
  setDefaultText();
}

void DXFLabel::openDXF(QFileInfo fi) {
  setDisabled(true);
  bf::path path = bf::path(fi.filePath().toStdWString());
  rrt::DXF dxf;
  try {
    dxf.fileImport(path);
    BOOST_LOG_TRIVIAL(info) << "Succesfully imported " << path.filename();
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error)
        << "Couldn't import " << path.filename() << ", reason: " << e.what();
    setEnabled(true);
    QMessageBox::critical(this, tr("Error"),
                          tr("Can't parse dxf file %1").arg(fi.fileName()));
    return;
  }
  dxfFilePath_ = fi.filePath();
  spatial_ = dxf.spatial();
  emit newDXFSpatial(spatial_);
  QString newText;
  newText = QString("<div>%1: \"%2\"</div>")
                .arg(tr("Current file"))
                .arg(dxfFilePath_);
  setText(newText);
}

void DXFLabel::onEndProcessingDXFSignal(std::shared_ptr<DXFResult> res) {
  int sz = 0;
  for (auto& k : *res) {
    sz += k.size();
  }
  setDisabled(false);
  setText(text() +
          QString(tr("<div>Got results:<b><font color=red>%1</font></b>. You "
                     "can copy them in clipboard "
                     "via buttons in top-right corner of treeview.</div>"))
              .arg(sz));
}

void DXFLabel::connectAll() {}

std::shared_ptr<rrt::Spatial> DXFLabel::spatial() const {
  return spatial_;
}

QString DXFLabel::dxfFilePath() const {
  return dxfFilePath_;
}
