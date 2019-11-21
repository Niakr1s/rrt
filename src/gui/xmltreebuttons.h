#ifndef XMLTREEBUTTONS_H
#define XMLTREEBUTTONS_H

#include <QPushButton>
#include <QWidget>

class XMLTreeButtons : public QWidget {
  Q_OBJECT

 public:
  XMLTreeButtons(QWidget* parent = nullptr);

  QPushButton* btnExpand() const;
  QPushButton* btnCollapse() const;
  QPushButton* btnCopySemicolon() const;
  QPushButton* btnCopyNewline() const;

 private:
  QPushButton* btnExpand_;
  QPushButton* btnCollapse_;
  QPushButton* btnCopySemicolon_;
  QPushButton* btnCopyNewline_;

 private:
  static QPushButton* makeDefaultButton(QIcon icon,
                                        QString str,
                                        QString tooltip);
};

#endif  // XMLTREEBUTTONS_H
