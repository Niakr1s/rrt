#ifndef XMLTREEBUTTONS_H
#define XMLTREEBUTTONS_H

#include <QComboBox>
#include <QFileInfo>
#include <QPushButton>
#include <QWidget>

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
  void onEndProcessingDXF(int);

 private:
  QPushButton* btnExpand_;

  QComboBox* comboBox_;
  QPushButton* btnCopySemicolon_;
  QPushButton* btnCopyNewline_;

 private:
  static QPushButton* makeDefaultButton(QIcon icon,
                                        QString str,
                                        QString tooltip,
                                        bool hide = false);

  QComboBox* makeComboBox();
};

#endif  // XMLTREEBUTTONS_H
