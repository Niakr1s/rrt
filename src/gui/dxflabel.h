#ifndef DXFLABEL_H
#define DXFLABEL_H

#include <QFileInfo>
#include <QLabel>
#include <QRect>
#include "spatial.h"

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
  void dxfCloseSignal();
  void newFileSignal(QFileInfo);

 public slots:
  void onDxfClose();
  void onNewFile(const QFileInfo& fi);

 private:
  QRect closeButtonRect_;
  QString dxfFilePath_;
  std::shared_ptr<rrt::Spatial> spatial_ = nullptr;
};

#endif  // DXFLABEL_H
