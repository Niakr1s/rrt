#ifndef DXFLABEL_H
#define DXFLABEL_H

#include <QFileInfo>
#include <QLabel>
#include <QRect>
#include <QString>
#include <QVector>
#include "spatial.h"
#include "typedefs.h"

class DXFLabel : public QLabel {
  Q_OBJECT

 public:
  DXFLabel(QWidget* parent = nullptr);

  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  void setDefaultText();

  QString dxfFilePath() const;
  std::shared_ptr<rrt::Spatial> spatial() const;

 signals:
  void dxfClosed();
  void newXMLs(QVector<QFileInfo>, bool fromDB = false);
  void newDXFSpatial(std::shared_ptr<rrt::Spatial>);

 public slots:
  void onEndProcessingDXFSignal(std::shared_ptr<DXFResult> res);
  void openDXF(QFileInfo fi);

 private:
  QRect closeButtonRect_;
  QString dxfFilePath_;
  std::shared_ptr<rrt::Spatial> spatial_ = nullptr;

 private:
  void connectAll();
  void closeDXF();
};

#endif  // DXFLABEL_H
