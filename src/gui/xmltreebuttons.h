#ifndef XMLTREEBUTTONS_H
#define XMLTREEBUTTONS_H

#include <QComboBox>
#include <QFileInfo>
#include <QHash>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <memory>
#include "typedefs.h"

class XMLTreeButtons : public QWidget {
  Q_OBJECT

 public:
  XMLTreeButtons(QWidget* parent = nullptr);

  QPushButton* btnExpand() const;
  QPushButton* btnCopySemicolon() const;
  QPushButton* btnCopyNewline() const;

  QComboBox* comboBox() const;

 public slots:
  void onDXFClose();
  void onEndProcessingDXF(std::shared_ptr<DXFResult> res);

  void onBtnCopySemicolonClicked();
  void onBtnCopyNewlineClicked();

 private:
  QPushButton* btnExpand_;

  QComboBox* comboBox_;
  const QString ALL_STRING = tr("All");

  QPushButton* btnCopySemicolon_;
  QPushButton* btnCopyNewline_;

  std::shared_ptr<DXFResult> result_;

 private:
  static QPushButton* makeDefaultButton(QIcon icon,
                                        QString str,
                                        QString tooltip,
                                        bool hide = false);

  static QComboBox* makeComboBox();

  void resToClipboard(QString key, QString sep);
  void propagateComboBox();
};

#endif  // XMLTREEBUTTONS_H
